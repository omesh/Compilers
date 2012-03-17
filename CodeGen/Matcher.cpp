#include "llvm/Pass.h"
#include "llvm/Module.h"
#include "llvm/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Instructions.h"
#include "llvm/Support/Casting.h"
#include "sample4.h"
#include "Matcher.h"
#include "string"
#include "sstream"
#include "SymbolTable.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/CFG.h"
#include <stdio.h>
using namespace llvm;

//Map table Mapping Alloca Instructions to entries in Symbol Table
typedef std::map<Instruction*, int> MapTable;
MapTable mapTable;
typedef std::pair<Instruction*, int> mapTableEntry;


// Symbol Table and LivenessSet
extern SymbolTable symbolTable;
extern liveSet livenessSet;

std::string FunctionName;

void alloca_match(Instruction* I); // Populate Symbol Table with a new Entry

// generate Corresponding Trees
Tree store_tree(Instruction* I);
Tree sext_tree(Instruction* I);
Tree load_tree(Instruction* I);
Tree ret_tree(Instruction* I);
Tree gep_tree(Instruction* I);
Tree call_tree(Instruction* I);
Tree binaryop_tree(Instruction* I);
Tree load_tree(Instruction* I);
Tree getTree(Instruction* I);
void cmp_tree(Instruction* I);
//Code to print contents of a tree

void printTree(Tree t) {
//        errs() << t->op << "\n";
//        errs() << "value  -> " << t->val << "\n";
//        if (GET_KIDS(t)[0] == 0 && GET_KIDS(t)[1] == 0) {
//            return;
//        } else if (GET_KIDS(t)[1] == 0) {
//            errs() << "  ";
//            printTree(GET_KIDS(t)[0]);
//        } else {
//            errs() << "Operand 1 \n";
//            errs() << "  ";
//            printTree(GET_KIDS(t)[0]);
//            errs() << "Operand 2 \n";
//            errs() << "  ";
//            printTree(GET_KIDS(t)[1]);
//        }
}


int getArraySize(const ArrayType* arr) {
    errs() << "Called get Array Size on: " << *arr << "\n";
    const Type* elemType = arr->getElementType();
    if (elemType->isIntegerTy(32)) {
        return arr->getNumElements()*4;
    }
    if (elemType->isIntegerTy(16)) {
        return arr->getNumElements()*2;
    }
    if (elemType->isIntegerTy(8)) {
        return arr->getNumElements()*1;
    }
    if (elemType->isIntegerTy(64)) {
        return arr->getNumElements()*8;
    } else {
        return arr->getNumElements() * getArraySize(dyn_cast<ArrayType > (arr->getElementType()));
    }
}

int getStructSize(const StructType* S) {
    int size = 0;
    for (int i = 0; i < S->getNumElements(); i++) {
        if (isa<ArrayType > (S->getContainedType(i))) {
            size = size + getArraySize(dyn_cast<ArrayType > (S->getContainedType(i)));
        } else if (isa<StructType > (S->getContainedType(i))) {
            size = size + getStructSize(dyn_cast<StructType > (S->getContainedType(i)));
        } else {
            size = size + (S->getContainedType(i)->getScalarSizeInBits() / 8);
        }
    }
    return size;
}

int getGolbalArrayOffset(const ArrayType* arr, const GEPOperator* gep, int i = 2) {
    if (!dyn_cast<ArrayType > (arr->getElementType())) {
        int off = dyn_cast<ConstantInt > (gep->getOperand(i))->getSExtValue() * (arr->getElementType()->getScalarSizeInBits() / 8);
        errs() << " \n Returning Off: " << off;
        errs() << " \n i is : " << i;
        return off;
    }
    int off = getArraySize(dyn_cast<ArrayType > (arr->getElementType())) * dyn_cast<ConstantInt > (gep->getOperand(i))->getSExtValue() + (getGolbalArrayOffset(dyn_cast<ArrayType > (arr->getElementType()), gep, i + 1));
    errs() << " \n Returning Off: " << off;
    errs() << " \n i is : " << i;
    return off;
}


int getStructOffset(const GEPOperator* gep) {
    errs() << "\n getStructOffset Called On " << *gep;
    const GEPOperator* gep2 = dyn_cast<llvm::GEPOperator > (gep->getOperand(0));
    errs() << "\n getStructOffset Called On ";
    const Type* type = dyn_cast<PointerType > (gep->getPointerOperandType())->getElementType();
    errs() << "\n getStructOffset Called On " << *type << "\n";
    if (!gep2) {
        const StructType* arr = dyn_cast<StructType > (type);
        errs() << " fdlfkld " << arr;
        int len = getStructSize(arr);
        return ((len) * dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue()) -getOffset(mapTable.find(dyn_cast<Instruction > (gep->getOperand(0)))->second);
    }
    if (dyn_cast<StructType > (type)) {
        const StructType* arr = dyn_cast<StructType > (type);
        int len = getStructSize(arr);
        return (len)*dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue() + getStructOffset(gep2);
    } else {
        return -((type->getScalarSizeInBits() / 8) * dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue() + getStructOffset(gep2));
    }

}

Tree getGepTree(const GEPOperator* gep){
    errs() << " Called Get GEP TREE On " << *gep;
    const GEPOperator* gep2 = dyn_cast<llvm::GEPOperator > (gep->getOperand(0));
    const Type* type = dyn_cast<PointerType > (gep->getType())->getElementType();
    if (!gep2) {
        errs() << "\n not a ptr type \n";
        errs() << "\n" << *gep << "\n";
        const ArrayType* arr = dyn_cast<ArrayType > (type);
        int len = getArraySize(arr);
        Tree t2;
        if(dyn_cast<llvm::ConstantInt > (gep->getOperand(2))){
            t2 = tree(CNSTI,0,0);
            t2->val = dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue();
        }
        else{
            t2 = getTree(dyn_cast<Instruction>(gep->getOperand(2)));
        }
        Tree t1 = tree(OFFSET,0,0);
        t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (gep->getOperand(0)))->second);
        Tree t = tree(GEP,t1,t2);        
        printTree(t);
        errs() << "\n";
        return t;
    }
    if (dyn_cast<ArrayType > (type)) {
        const ArrayType* arr = dyn_cast<ArrayType > (type);
        int len = getArraySize(arr);
        Tree t1 = getGepTree(gep2);
        Tree t2;
        if(dyn_cast<llvm::ConstantInt > (gep->getOperand(2))){
            t2 = tree(CNSTI,0,0);
            t2->val = dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue();
        }
        else{
            t2 = getTree(dyn_cast<Instruction>(gep->getOperand(2)));
            
        }
        Tree t = tree(GEP,t1,t2); // ireg + val*ireg
        t->val = len;
        return t;
    } else {
        Tree t1 = getGepTree(gep2);
        Tree t2;
        if(dyn_cast<llvm::ConstantInt > (gep->getOperand(2))){
            t2 = tree(CNSTI,0,0);
            t2->val = (type->getScalarSizeInBits() / 8);
        }
        else{
            t2 = getTree(dyn_cast<Instruction>(gep->getOperand(2)));
        }
        Tree t = tree(GEP,t1,t2);
        return t;
    }
}


int getArrayOffset(const GEPOperator* gep) {
    //    errs() << "\n In get arrayoffset \n";
    errs() << " Called Get Array Offset On " << *gep;
    const GEPOperator* gep2 = dyn_cast<llvm::GEPOperator > (gep->getOperand(0));
    const Type* type = dyn_cast<PointerType > (gep->getType())->getElementType();
    errs() << " \n primitive type" << type->getPrimitiveSizeInBits();
    errs() << "\n" << type;
    errs() << "\n Num Elements " << *dyn_cast<PointerType > (gep->getType())->getElementType() << "\n";
    if (!gep2) {
        errs() << "\n not a ptr type \n";
        errs() << "\n" << *gep << "\n";
        const ArrayType* arr = dyn_cast<ArrayType > (type);
        //        int numElements = arr->getNumElements();
        //        errs() <<  "\n REACHED BASE "<< getArraySize(arr) << "\n";
        int len = getArraySize(arr);
        return ((len) * dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue()) -getOffset(mapTable.find(dyn_cast<Instruction > (gep->getOperand(0)))->second);
    }
    if (dyn_cast<ArrayType > (type)) {
        const ArrayType* arr = dyn_cast<ArrayType > (type);
        //        int numElements = arr->getNumElements();
        int len = getArraySize(arr);
        //        errs() << "\n Array Size " << len; 
        return (len)*dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue() + getArrayOffset(gep2);
    } else {
        //        errs() << "\n Scalar Size \n " << type->getScalarSizeInBits();
        return -((type->getScalarSizeInBits() / 8) * dyn_cast<llvm::ConstantInt > (gep->getOperand(2))->getSExtValue() + getArrayOffset(gep2));
    }
}

std::vector<long> globalArrValues;

void getGlobalArrayElements(const ConstantArray* arr) {

    if (!dyn_cast<ArrayType > (arr->getType()->getElementType())) {
        for (int j = 0; j < arr->getType()->getNumElements(); j++) {
            globalArrValues.push_back(dyn_cast<llvm::ConstantInt > (arr->getOperand(j))->getSExtValue());
        }
    } else {
        for (int j = 0; j < arr->getType()->getNumElements(); j++) {
            getGlobalArrayElements(dyn_cast<ConstantArray > (arr->getOperand(j)));
        }
    }
}

int counter = 0;
int offset = 0;
extern std::map<int, int> ArgumentOffset;

void switch_tree(Instruction* BI) {
    std::stringstream os;
    Tree t = getTree(dyn_cast<Instruction > (BI->getOperand(0)));
    gen(t);
    const char* reg = getTypeRegInst(dyn_cast<Instruction > (BI->getOperand(0))->getType()->getScalarSizeInBits(), dyn_cast<Instruction > (BI->getOperand(0)));
    SwitchInst* I = dyn_cast<SwitchInst > (BI);
    for (int i = 1; i < I->getNumCases(); i++) {
        os << "  cmp $" << dyn_cast<ConstantInt > (I->getCaseValue(i))->getSExtValue() << ", %" << reg << "\n";
        os << "  je ." << I->getSuccessor(i)->getNameStr() << "_" << FunctionName << "\n";
    }
    os << "  jmp ." << I->getSuccessor(0)->getNameStr() << "_" << FunctionName << "\n";

    fp = fopen("CodeGen.asm", "a");
    fprintf(fp, strdup(os.str().c_str()));
    fclose(fp);

    //    errs() << "\n SWITCH  " << *(I->getOperand(0));
    //    errs() << "\n SWITCH  " << (I->getNumCases());
    //    errs() << "\n SWITCH  " << *(I->getSuccessor(0));
    //    errs() << "\n SWITCH  " << *(I->getCaseValue(2));
}

void br_tree(Instruction* BI) {

    std::stringstream os;
    if (!dyn_cast<BranchInst > (BI)->isConditional()) {
        os << "  jmp ." << BI->getOperand(0)->getNameStr() << "_" << FunctionName << "\n";
    } else {
        ICmpInst* I = dyn_cast<ICmpInst > (BI->getOperand(0));
        Tree t = getTree(dyn_cast<Instruction > (I->getOperand(0)));
        gen(t);
        const char* reg1 = getTypeRegInst(I->getOperand(0)->getType()->getScalarSizeInBits(), dyn_cast<Instruction > (I->getOperand(0)));
        const char* reg2;
        if (dyn_cast<ConstantInt > (dyn_cast<ICmpInst > (I)->getOperand(1))) {
            errs() << "\n CMP OPERAND 2 is a constant \n";
            std::stringstream ss;
            ss << "$" << dyn_cast<ConstantInt > (dyn_cast<ICmpInst > (I)->getOperand(1))->getSExtValue();
            reg2 = strdup(ss.str().c_str());
            os << "  cmp " << ss.str() << ", %" << reg1;
        } else {
            t = getTree(dyn_cast<Instruction > (I->getOperand(1)));
            gen(t);
            reg2 = getTypeRegInst(I->getOperand(1)->getType()->getScalarSizeInBits(), dyn_cast<Instruction > (I->getOperand(1)));
            os << "  cmp %" << reg2 << ", %" << reg1;
        }
        
        std::string label = dyn_cast<BranchInst>(BI)->getOperand(1)->getNameStr();
        errs() << "\n OUT \n";
        const char* jptype;
        if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_EQ) {
            os << "\n  jne .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_NE) {
            os << "\n  je .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_SGE) {
            os << "\n  jl .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_SGT) {
            os << "\n  jle .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_SLE) {
            os << "\n  jg .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_SLT) {
            os << "\n  jge .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_UGE) {
            os << "\n  jb .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_UGT) {
            os << "\n  jbe .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_ULE) {
            os << "\n  ja .";

        } else if (dyn_cast<ICmpInst > (I)->getPredicate() == ICmpInst::ICMP_ULT) {
            os << "\n  jae .";

        }
        os << label << "_" << FunctionName << "\n";
        os << "\n jmp ." << dyn_cast<BranchInst>(BI)->getOperand(2)->getNameStr() << "_" << FunctionName << "\n";
    }

    fp = fopen("CodeGen.asm", "a");
    fprintf(fp, strdup(os.str().c_str()));
    fclose(fp);
}


namespace {

    struct Matcher : public ModulePass {
        static char ID;

        Matcher() : ModulePass(ID) {
        }

        virtual bool runOnFunction(Function & F) {
            if (F.isDeclaration()) {
                return false;
            }
            FunctionName = F.getNameStr();
            offset = 0;
            int j = F.getArgumentList().size() - 1, argoffset = 16; // 8 for return address + 8 for pushed rbp
            for (Function::arg_iterator e = F.getArgumentList().end(), i = F.getArgumentList().begin(); i != e; e--) {
                ArgumentOffset.insert(std::pair<int, int>(j, argoffset));
                argoffset = argoffset + (i->getType()->getScalarSizeInBits() / 8);
                j--;
            }
            //Do liveness analysis
            std::vector<llvm::Value*> liveVars;
            std::map<llvm::BasicBlock*, std::vector<llvm::Value*> > blockGens; //stores Basic Block Gen

            User::op_iterator OI, OE;
            for (Function::iterator e = F.begin(), i = F.end(); i != e;) {
                --i;
                BasicBlock* b = dyn_cast<BasicBlock > (&*i);
                errs() << "\n Processing " << *b;

                if (blockGens.find(b) != blockGens.end()) { //populate with gen stored by successors of basic block
                    liveVars = blockGens.find(b)->second;
                }

                for (BasicBlock::iterator BE = i->begin(), BI = i->end(); BI != BE;) {
                    --BI;
                    errs() << "\n Insruction Processing " << *BI << "\n";
                    addAfterSet(BI, liveVars);

                    for (OI = BI->op_begin(), OE = BI->op_end(); OI != OE; ++OI) {

                        Value *val = *OI;
                        if (isa<Instruction > (val) || isa<Argument > (val)) {
                            if (std::find(liveVars.begin(), liveVars.end(), val) == liveVars.end()) {
                                liveVars.push_back(val);
                            }
                        }

                    }

                    std::vector<llvm::Value*>::iterator i;
                    if ((i = std::find(liveVars.begin(), liveVars.end(), BI)) != liveVars.end()) {
                        errs() << "Erasing " << **i << "   " << *BI << "\n";
                        liveVars.erase(i);

                    }

                    if (BI->getOpcode() == lSTORE) { //Store Should Destroy Alloca
                        if ((i = std::find(liveVars.begin(), liveVars.end(), BI->getOperand(1))) != liveVars.end()) {
                            errs() << "Erasing " << **i << "   " << *BI << "\n";
                            liveVars.erase(i);

                        }
                    }

                    addBeforeSet(BI, liveVars);
                }


                for (pred_iterator pi = llvm::pred_begin(b), pe = llvm::pred_end(b); pi != pe; pi++) { // store Gen to be added to predecessors
                    BasicBlock* B = dyn_cast<BasicBlock > (*pi);
                    blockGens.insert(std::pair<llvm::BasicBlock*, std::vector<llvm::Value*> >(B, liveVars));
                }

            }

            printLivenessInfo();
            //************************ Liveness Finished ***********************************//

            //Generate Code
            // funtion peremble
            fp = fopen("CodeGen.asm", "a");
            fprintf(fp, "  .text\n");
            fprintf(fp, ".globl %s \n", F.getNameStr().c_str());
            fprintf(fp, "  .type %s, @function\n", F.getNameStr().c_str());
            fprintf(fp, "%s :\n", F.getNameStr().c_str());
            fprintf(fp, "  pushq %%rbp \n");
            fprintf(fp, "  movq %rsp, %rbp \n");
            int LocalsSize = 0;
            
            for (Function::iterator b = F.begin(), be = F.end(); b != be; ++b) {
                for (BasicBlock::iterator i = b->begin(), ie = b->end(); i != ie; ++i) {
                    if (AllocaInst * AI = dyn_cast<AllocaInst > (&*i)) {
                        if (isa<ArrayType > (AI->getAllocatedType())) {
                            const ArrayType* arrType = dyn_cast<ArrayType > (AI->getAllocatedType());
                            //                            const Type* elemType = arrType->getElementType();
                            errs() << " Array Size: " << getArraySize(arrType);
                            LocalsSize = LocalsSize + getArraySize(arrType); //arrType->getNumElements() * elemType->getScalarSizeInBits();
                        } else if (isa<PointerType > (AI->getAllocatedType())) {
                            //                            errs() << *AI;
                            LocalsSize = LocalsSize + (8);
                        } else {
                            LocalsSize = LocalsSize + (AI->getAllocatedType()->getScalarSizeInBits() / 8);
                        }
                    }
                }
            }
            fprintf(fp, "  subq $%d, %rsp \n", LocalsSize);
            fclose(fp);

            setSpilledOffset(LocalsSize);
            
            for (Function::iterator i = F.begin(), e = F.end(); i != e; i++) {
                runOnBasicBlock(*i);
            }
            //function exit;
            fp = fopen("CodeGen.asm", "a");
            fprintf(fp, "  leave\n");
            fprintf(fp, "  ret\n");
            fclose(fp);
            return false;
        }

        std::map<std::string, std::string> typeMappings;

        void initializeMappings() {
            typeMappings.insert(std::pair<std::string, std::string > ("i32*", "int"));
            typeMappings.insert(std::pair<std::string, std::string > ("i64*", "int"));
            typeMappings.insert(std::pair<std::string, std::string > ("i16*", "short"));
            typeMappings.insert(std::pair<std::string, std::string > ("i8*", "byte"));
        }

        const char* getTypeName(int numBits) {
            std::string str;
            if (numBits == 8) {
                str = ".byte";
            } else if (numBits == 16) {
                str = ".long";
            } else if (numBits == 32) {
                str = ".long";
            } else if (numBits == 64) {
                str = ".quad";
            } else {
                errs() << " ***** NO MATCH DURING GET TYPE NAME ******";
                str = ".long";
            }
            return strdup(str.c_str());
        }

        virtual bool runOnModule(Module & M) {
            errs() << "  .file \"t1.c\" \n";
            initializeMappings();
            populateRegs();
            fp = fopen("CodeGen.asm", "a");
            fprintf(fp, "  .data\n");
            for (Module::global_iterator i = M.global_begin(), e = M.global_end(); i != e; i++) {

                if (!isa<ArrayType > (i->getOperand(0)->getType())) {
                    fprintf(fp, ".globl %s \n", i->getNameStr().c_str());
                    fprintf(fp, "  .type %s, @object \n", i->getNameStr().c_str());
                    fprintf(fp, "%s:\n", i->getNameStr().c_str());
                    fprintf(fp, "  %s %d\n", getTypeName(i->getOperand(0)->getType()->getScalarSizeInBits()), (dyn_cast<llvm::ConstantInt>) (i->getOperand(0))->getSExtValue());
                } else {
                    const ArrayType* arrType = dyn_cast<ArrayType > (i->getOperand(0)->getType());

                    //                    std::string h = arrType->getElementType()->getDescription();
                    //                    errs() << "Num elements " << arrType->getNumElements() << "\n";

                    if (dyn_cast<ConstantArray > (i->getOperand(0))->isCString()) { // For arrays of type char*

                        fprintf(fp, "  .section  .rodata\n");
                        fprintf(fp, "%s:\n", i->getNameStr().c_str());
                        std::string str = dyn_cast<ConstantArray > (i->getOperand(0))->getAsString();
                        fprintf(fp, "  .string \"%s\"\n", str.c_str());

                    } else {
                        errs() << "here\n";
                        int numElems = arrType->getNumElements();
                        fprintf(fp, ".globl %s \n", i->getNameStr().c_str());
                        fprintf(fp, "  .type %s, @object \n", i->getNameStr().c_str());
                        fprintf(fp, "%s:\n", i->getNameStr().c_str());
                        errs() << i->getNameStr() << "\n";
                        // For arrays of type i32
                        const Type* a = arrType;
                        while (dyn_cast<ArrayType > (a)) {
                            a = dyn_cast<ArrayType > (a)->getElementType();
                        }
                        int numBits = a->getScalarSizeInBits();
                        errs() << numBits << "\n";
                        globalArrValues.clear();
                        getGlobalArrayElements(dyn_cast<ConstantArray > (i->getOperand(0)));

                        //                        SmallVectorImpl<Constant*> list(10);
                        //                        dyn_cast<ConstantArray > (i->getOperand(0))->getVectorElements(list);

                        for (std::vector<long>::iterator i = globalArrValues.begin(), e = globalArrValues.end(); i != e; i++) {
                            fprintf(fp, "  %s %d\n", getTypeName(numBits), *i);
                            errs() << " " << *i << " \n";
                        }
                        //                        errs() << *(dyn_cast<ConstantArray > (i->getOperand(0))->getOperand(0));
                        //                        if ((dyn_cast<ArrayType > (i->getOperand(0)->getType())->getContainedType(0)->getDescription()) == "i32") {
                        //                        for (int j = 0; j < numElems; j++) {
                        //                            fprintf(fp, "  %s %d\n", getTypeName(numBits), (dyn_cast<llvm::ConstantInt>) (dyn_cast<ConstantArray > (i->getOperand(0))->getOperand(j))->getSExtValue());
                        //                        }
                        //                        }

                    }
                    //                    errs() << "String Type " << h << "\n";
                }

            }
            fclose(fp);
            for (Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI) {

                runOnFunction(*MI);
            }

            return false;
        }

        virtual bool runOnBasicBlock(BasicBlock & BB) {
            fp = fopen("CodeGen.asm", "a");
            fprintf(fp, "\n\n.%s_%s:\n", strdup(BB.getNameStr().c_str()), strdup(FunctionName.c_str()));
            fclose(fp);

            for (BasicBlock::iterator BI = BB.begin(), BE = BB.end(); BI != BE; ++BI) {
                int InsOpcode = BI->getOpcode();
                Tree t;
                if (lALLOCA == InsOpcode) {
                    alloca_match(BI);
                }
                if (lSwitch == InsOpcode) {
                    switch_tree(BI);
                }
                if (lRET == InsOpcode) {
                    fp = fopen("CodeGen.asm", "a");
                    errs() << "Here1\n";
                    std::string reg1 = getTypeReg(BI->getOperand(0)->getType()->getScalarSizeInBits(), std::string("6"));
                    errs() << "Here\n";
                    const char* reg2 = getTypeRegInst(BI->getOperand(0)->getType()->getScalarSizeInBits(), BI->getOperand(0));

                    fprintf(fp, "  mov %%%s, %%%s \n", reg2, strdup(reg1.c_str()));
                    errs() << "DONE";
                    fclose(fp);
                }
                if (lBR == InsOpcode) {

                    br_tree(BI);

                }
                if (lSTORE == InsOpcode) {
                    if (dyn_cast<AllocaInst > (BI->getOperand(0)) && dyn_cast<AllocaInst > (BI->getOperand(1))) {
                        errs() << "HAHAHA YOU JOKER \n";
                        errs() << *(BI->getOperand(0));
                        fp = fopen("CodeGen.asm", "a");
                        //                                fprintf(fp,"  leaq -%d(%%rbp), %%%s\n",getOffset(mapTable.find(dyn_cast<Instruction > (BI->getOperand(0)))->second),getReg64(BI->getOperand(1)));
                        fprintf(fp, "  leaq -%d(%%rbp), %%rbx\n", getOffset(mapTable.find(dyn_cast<Instruction > (BI->getOperand(0)))->second));
                        fprintf(fp, "  movq %%rbx, -%d(%%rbp)\n", getOffset(mapTable.find(dyn_cast<Instruction > (BI->getOperand(1)))->second));
                        fclose(fp);
                        continue;
                    }
                    if (isa<AllocaInst > (BI->getOperand(1))) {

                        errs() << "\n In Store to Alloca\n";

                        if (isa<llvm::ConstantInt > (BI->getOperand(0))) {

                            errs() << "\n In Constant Store to Alloca\n";

                            // Creater a Constant Subtree, val field stores constant value.
                            Tree t2 = tree(CNSTI, 0, 0);
                            t2->val = dyn_cast<llvm::ConstantInt > (BI->getOperand(0))->getSExtValue();
                            t2->inst = dyn_cast<Instruction > (BI->getOperand(0));

                            //Create a MEM Subtree, val field stores the pointer to entry in symbol table.//change val field stores offset
                            Tree t1;
                            AllocaInst* allocaInst = dyn_cast<AllocaInst > (BI->getOperand(1));
                            int numBits = allocaInst->getAllocatedType()->getScalarSizeInBits();

                            if (numBits == 8) {
                                t1 = tree(MEM8, 0, 0);
                            } else if (numBits == 16) {
                                t1 = tree(MEM16, 0, 0);
                            } else if (numBits == 32) {
                                t1 = tree(MEM32, 0, 0);
                            } else if (numBits == 64) {
                                t1 = tree(MEM64, 0, 0);
                            } else {
                                errs() << "\n************ NO MATCH IN STORE FOR MEM TYPE ************\n";
                                t1 = tree(MEM32, 0, 0);
                            }

                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (BI->getOperand(1)))->second); //Offset to Memory Location
                            t1->inst = dyn_cast<Instruction > (BI->getOperand(1));

                            // Set up tree for Store.
                            t = tree(STORE, t1, t2);
                            t->inst = BI;


                        } else {// can be a expression or a parameter ??????????????
                            errs() << "\n In Not Constant Store to Alloca\n";
                            errs() << *BI << "\n";
                            errs() << BI->getOperand(0)->getType()->getScalarSizeInBits();
                            Tree t2;
                            if (isa<Argument > (BI->getOperand(0))) {
                                errs() << "Argument Number ";
                                errs() << dyn_cast<Argument > (BI->getOperand(0))->getArgNo();

                                int numBits = dyn_cast<Argument > (BI->getOperand(0))->getType()->getScalarSizeInBits();

                                if (numBits == 8) {
                                    t2 = tree(ARG8, 0, 0);
                                } else if (numBits == 16) {
                                    t2 = tree(ARG16, 0, 0);
                                } else if (numBits == 32) {
                                    t2 = tree(ARG32, 0, 0);
                                } else if (numBits == 64) {
                                    t2 = tree(ARG64, 0, 0);
                                } else {
                                    errs() << "\n************ NO MATCH IN STORE FOR ARG TYPE ************\n";
                                    t2 = tree(ARG32, 0, 0);
                                }
                                t2->val = dyn_cast<Argument > (BI->getOperand(0))->getArgNo();
                            } else {
                                t2 = getTree(dyn_cast<Instruction > (BI->getOperand(0)));
                                t2->inst = dyn_cast<Instruction > (BI->getOperand(0));
                            }


                            Tree t1;
                            AllocaInst* allocaInst = dyn_cast<AllocaInst > (BI->getOperand(1));
                            int numBits = allocaInst->getAllocatedType()->getScalarSizeInBits();

                            if (numBits == 8) {
                                t1 = tree(MEM8, 0, 0);
                            } else if (numBits == 16) {
                                t1 = tree(MEM16, 0, 0);
                            } else if (numBits == 32) {
                                t1 = tree(MEM32, 0, 0);
                            } else if (numBits == 64) {
                                t1 = tree(MEM64, 0, 0);
                            } else {
                                errs() << "\n************ NO MATCH IN STORE FOR MEM TYPE ************\n";
                                t1 = tree(MEM32, 0, 0);
                            }

                            t1->inst = dyn_cast<Instruction > (BI->getOperand(1));
                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (BI->getOperand(1)))->second); //ptr to Memory Location in Symbol table

                            t = tree(STORE, t1, t2);
                            t->inst = BI;
                        }


                    } else {
                        Tree t2;
                        if (isa<llvm::ConstantInt > (BI->getOperand(0))) { //  hack storing store instruction instead of global value.

                            errs() << "\n In Constant Store to Not Alloca\n";

                            // Creater a Constant Subtree, val field stores constant value.
                            t2 = tree(CNSTI, 0, 0);
                            t2->val = ((dyn_cast<llvm::ConstantInt>) (BI->getOperand(0)))->getSExtValue();
                            t2->inst = dyn_cast<Instruction > (BI->getOperand(0));


                        } else {
                            t2 = getTree(dyn_cast<Instruction > (BI->getOperand(0)));
                            t2->inst = dyn_cast<Instruction > (BI->getOperand(0));

                        }

                        //Create a MEM Subtree, val field stores the pointer to entry in symbol table.
                        Tree t1;
                        //                            t1->val = mapTable.find(dyn_cast<Instruction > (BI->getOperand(1)))->second; //ptr to Memory Location in Symbol table


                        if (isa<GEPOperator > (BI->getOperand(1))) {
                            errs() << "\n Store operand is GEP \n";
                            if (isa<GlobalValue > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(0))) {

                                errs() << "\n In Constant Store to GLOBAL ARRAY\n";
                                const Type* arr = dyn_cast<PointerType > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(0)->getType())->getElementType();
                                if (dyn_cast<ArrayType > (arr)) {
                                    errs() << "Array Type";
                                    int offset = getGolbalArrayOffset(dyn_cast<ArrayType > (arr), dyn_cast<GEPOperator > (BI->getOperand(1)));
                                    errs() << "\n getGolbalArrayOffset returned:" << offset << "\n";
                                    if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(32)) {
                                        errs() << "\n gep type int \n";
                                        t1 = tree(GLOB32, 0, 0);
                                        t1->val = offset;

                                    } else if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(64)) {
                                        errs() << "\n gep type 64 \n";
                                        t1 = tree(GLOB64, 0, 0);
                                        t1->val = offset;

                                    } else if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(16)) {
                                        errs() << "\n gep type 16 \n";
                                        t1 = tree(GLOB16, 0, 0);
                                        t1->val = offset;

                                    } else if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(8)) {
                                        errs() << "\n gep type 8 \n";
                                        t1 = tree(GLOB8, 0, 0);
                                        t1->val = offset;

                                    } else {
                                        errs() << "\n NO GLOB TYPE MATCH \n";
                                        t1 = tree(GLOB32, 0, 0);
                                        t1->val = offset;
                                    }

                                } else {
                                    errs() << "Not array type";
                                    if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(32)) {
                                        errs() << "\n gep type int \n";
                                        t1 = tree(GLOB32, 0, 0);
                                        t1->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue() * 4;

                                    } else if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(64)) {
                                        errs() << "\n gep type 64 \n";
                                        t1 = tree(GLOB64, 0, 0);
                                        t1->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue() * 8;

                                    } else if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(16)) {
                                        errs() << "\n gep type 16 \n";
                                        t1 = tree(GLOB16, 0, 0);
                                        t1->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue() * 2;

                                    } else if (dyn_cast<PointerType > (BI->getOperand(1)->getType())->getElementType()->isIntegerTy(8)) {
                                        errs() << "\n gep type 8 \n";
                                        t1 = tree(GLOB8, 0, 0);
                                        t1->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue();

                                    } else {
                                        errs() << "\n NO GLOB TYPE MATCH \n";
                                        t1 = tree(GLOB32, 0, 0);
                                        t1->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue() * 4;
                                    }
                                }


                                t1->name = strdup((dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(0))->getNameStr().c_str());

                            } else { // store to a Local Array.
                                //                                    t1 = tree(MEM, 0, 0);
                                errs() << "\n Store to a local Array \n";
                                GEPOperator* gepptr = dyn_cast<GEPOperator > (BI->getOperand(1));
                                if (!dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(0))) {
                                    if (dyn_cast<StructType > (dyn_cast<PointerType > (gepptr->getOperand(0)->getType())->getElementType())) {
                                        errs() << "\n STORE TO A STRUCT TYPE \n";
                                        if (BI->getOperand(0)->getType()->isIntegerTy(32)) {
                                            errs() << "\n  Is Integer type \n";
                                            t1 = tree(MEM32, 0, 0);

                                        } else if (BI->getOperand(0)->getType()->isIntegerTy(16)) {
                                            errs() << "\n  Is short type \n";
                                            t1 = tree(MEM16, 0, 0);

                                        } else if (BI->getOperand(0)->getType()->isIntegerTy(8)) {
                                            errs() << "\n  Is char type \n";
                                            t1 = tree(MEM8, 0, 0);

                                        } else if (BI->getOperand(0)->getType()->isIntegerTy(64)) {
                                            t1 = tree(MEM64, 0, 0);
                                            errs() << "\n  Is long type \n";

                                        } else {
                                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                                            t1 = tree(MEM32, 0, 0);
                                        }
                                        t1->val = getStructOffset(dyn_cast<GEPOperator > (BI->getOperand(1)));
                                        errs() << " \n Get Struct Offset returned " << t1->val;
                                    } else {
                                        errs() << "\n MultiDimensional Array\n";
                                        const Type* type = dyn_cast<PointerType > (dyn_cast<GEPOperator > (BI->getOperand(1))->getType())->getElementType();
                                        if (type->isIntegerTy(32)) {
                                            errs() << "\n  Is Integer type \n";
                                            t1 = tree(MEM32, 0, 0);

                                        } else if (type->isIntegerTy(16)) {
                                            errs() << "\n  Is short type \n";
                                            t1 = tree(MEM16, 0, 0);

                                        } else if (type->isIntegerTy(8)) {
                                            errs() << "\n  Is char type \n";
                                            t1 = tree(MEM8, 0, 0);

                                        } else if (type->isIntegerTy(64)) {
                                            t1 = tree(MEM64, 0, 0);
                                            errs() << "\n  Is long type \n";

                                        } else {
                                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                                            t1 = tree(MEM32, 0, 0);
                                        }
                                        t1->val = getArrayOffset(dyn_cast<GEPOperator > (BI->getOperand(1)));
                                        errs() << " \n Get Array Offset returned " << t1->val;
                                        
                                        
//                                        t1 = getGepTree(dyn_cast<GEPOperator > (BI->getOperand(1)));
                                    }
                                } else {
                                    errs() << *BI;
                                    if (dyn_cast<StructType > (dyn_cast<PointerType > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(0)->getType())->getElementType())) {
                                        const StructType* arrType = dyn_cast<StructType > (dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(0))->getAllocatedType());
                                        int offset = 0;
                                        for (int i = 0; i < arrType->getNumElements(); i++) {
                                            const Type* type = arrType->getContainedType(i);
                                            if (dyn_cast<ArrayType > (type)) {
                                                offset = offset + getArraySize(dyn_cast<ArrayType > (type));
                                            } else if (dyn_cast<StructType > (type)) {
                                                offset = offset + getStructSize(dyn_cast<StructType > (type));
                                            } else {
                                                offset = offset + (type->getScalarSizeInBits() / 8);
                                            }
                                        }
                                        if (BI->getOperand(0)->getType()->isIntegerTy(32)) {
                                            errs() << "\n  Is Integer type \n";
                                            t1 = tree(MEM32, 0, 0);
                                            errs() << *BI->getOperand(1);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - offset;
                                            errs() << "\nhere\n";

                                        } else if (BI->getOperand(0)->getType()->isIntegerTy(16)) {
                                            errs() << "\n  Is short type \n";
                                            t1 = tree(MEM16, 0, 0);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - offset;

                                        } else if (BI->getOperand(0)->getType()->isIntegerTy(8)) {
                                            errs() << "\n  Is char type \n";
                                            t1 = tree(MEM8, 0, 0);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - offset;

                                        } else if (BI->getOperand(0)->getType()->isIntegerTy(64)) {
                                            t1 = tree(MEM64, 0, 0);
                                            errs() << "\n  Is long type \n";

                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - offset;
                                        } else {
                                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                                            t1 = tree(MEM32, 0, 0);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - offset;
                                        }
                                    } else {
                                        const ArrayType* arrType = dyn_cast<ArrayType > (dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(0))->getAllocatedType());
                                        const Type* elemType = arrType->getElementType();
                                        if (elemType->isIntegerTy(32)) {
                                            errs() << "\n  Is Integer type \n";
                                            t1 = tree(MEM32, 0, 0);
                                            errs() << *BI->getOperand(1);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue()*4);
                                            errs() << "\nhere\n";

                                        } else if (elemType->isIntegerTy(16)) {
                                            errs() << "\n  Is short type \n";
                                            t1 = tree(MEM16, 0, 0);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue()*4);

                                        } else if (elemType->isIntegerTy(8)) {
                                            errs() << "\n  Is char type \n";
                                            t1 = tree(MEM8, 0, 0);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue()*4);

                                        } else if (elemType->isIntegerTy(64)) {
                                            t1 = tree(MEM64, 0, 0);
                                            errs() << "\n  Is long type \n";

                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue()*4);
                                        } else {
                                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                                            t1 = tree(MEM32, 0, 0);
                                            t1->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (BI->getOperand(1))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (BI->getOperand(1))->getOperand(2))->getSExtValue()*4);
                                        }
                                    }
                                }
                            }
                            t1->inst = BI;
                        } else { // Should be a Global Constant.

                            if (isa<GlobalValue > (BI->getOperand(1))) {
                                errs() << "\nIs a Global Constant\n";
                                int numBits = dyn_cast<GlobalValue > (BI->getOperand(1))->getOperand(0)->getType()->getScalarSizeInBits();

                                if (numBits == 8) {
                                    errs() << "\n  Is global byte type \n";
                                    t1 = tree(GLOB8, 0, 0);
                                } else if (numBits == 16) {
                                    errs() << "\n  Is global short type \n";
                                    t1 = tree(GLOB16, 0, 0);
                                } else if (numBits == 32) {
                                    errs() << "\n  Is global int type \n";
                                    t1 = tree(GLOB32, 0, 0);
                                } else if (numBits == 64) {
                                    errs() << "\n  Is global quad type \n";
                                    t1 = tree(GLOB64, 0, 0);
                                } else {
                                    errs() << "\n************ NO MATCH IN STORE FOR GLOB TYPE ************\n";
                                    t1 = tree(GLOB32, 0, 0);
                                }

                                t1->name = strdup(BI->getOperand(1)->getNameStr().c_str());
                                t1->val = 0;
                                t1->inst = BI;
                            } else {
                                errs() << "\n Store to an Instruction \n";
                                t1 = getTree(dyn_cast<Instruction > (BI->getOperand(1)));
                                t1->inst = dyn_cast<Instruction > (BI->getOperand(1));

                            }

                        }


                        // Set up tree for Store.
                        t = tree(STORE, t1, t2);
                        t->inst = BI;
                    }
                    printTree(t);
                    gen(t);
                    // Need to Improve Tree printing
                }
                if (lCALL == InsOpcode) {
                    call_tree(BI);
                }

            }
            errs() << BB;

            return false;
        }

    };
}

void alloca_match(Instruction* I) { //Storing  Information In Symbolic table
    AllocaInst *AI = dyn_cast<llvm::AllocaInst > (I);

    if (!isa<ArrayType > (AI->getAllocatedType())) {
        if (isa<StructType > (AI->getAllocatedType())) {
             errs() << "\n ************ STRUCT TYPE **************** \n";
             errs() << *AI;
            const StructType* S = dyn_cast<StructType > (AI->getAllocatedType());
            counter++;
            mapTable.insert(mapTableEntry(I, counter));
            struct variableInfo* vInfo = new variableInfo;
            vInfo->size = getStructSize(S);
            offset = offset + (vInfo->size);
            vInfo->offset = offset;
            symbolTable.insert(SymbolTableEntry(counter, vInfo));

        } else {
            errs() << "\n ************Not Array Allocation**************** \n";
            errs() << *AI;
            counter++;
            mapTable.insert(mapTableEntry(I, counter));
            struct variableInfo* vInfo = new variableInfo;
            if (isa<PointerType > (AI->getAllocatedType())) {
                errs() << *AI;
                vInfo->size = (64) / 8;
            } else {
                vInfo->size = (AI->getAllocatedType()->getScalarSizeInBits()) / 8;
            }

            offset = offset + (vInfo->size);
            vInfo->offset = offset;

            symbolTable.insert(SymbolTableEntry(counter, vInfo));
        }
        printSymbolTable();
    } else {
        //code for array allocation
        errs() << "\n ************Array Allocation**************** \n";
        errs() << *AI;
        counter++;
        mapTable.insert(mapTableEntry(I, counter));
        struct variableInfo* vInfo = new variableInfo;
        const ArrayType* arrType = dyn_cast<ArrayType > (AI->getAllocatedType());
        const Type* elemType = arrType->getElementType();
        if (elemType->isIntegerTy(32)) {
            errs() << "\n  Is Integer type \n";
            errs() << "Size : " << 4 * arrType->getNumElements() << "\n";
            vInfo->size = 4 * arrType->getNumElements();
            offset = offset + (vInfo->size);
            vInfo->offset = offset;
        } else if (elemType->isIntegerTy(16)) {
            errs() << "\n  Is short type \n";
            vInfo->size = 2 * arrType->getNumElements();
            offset = offset + (vInfo->size);
            vInfo->offset = offset;
        } else if (elemType->isIntegerTy(8)) {
            errs() << "\n  Is char type \n";
            vInfo->size = 1 * arrType->getNumElements();
            offset = offset + (vInfo->size);
            vInfo->offset = offset;
        } else if (elemType->isIntegerTy(64)) {
            errs() << "\n  Is long type \n";
            vInfo->size = 8 * arrType->getNumElements();
            offset = offset + (vInfo->size);
            vInfo->offset = offset;
        } else {
            errs() << "\n Array Is Multidimensional \n" << getArraySize(arrType);
            vInfo->size = getArraySize(arrType);
            offset = offset + (vInfo->size);
            vInfo->offset = offset;
        }
        symbolTable.insert(SymbolTableEntry(counter, vInfo));
        printSymbolTable();
    }
}

Tree trunc_tree(Instruction* I) {
    int numBits = dyn_cast<TruncInst > (I)->getType()->getScalarSizeInBits();
    Tree t1 = getTree(dyn_cast<Instruction > (I->getOperand(0)));
    t1->inst = dyn_cast<Instruction > (I->getOperand(0));
    Tree t;
    if (numBits == 8) {
        t = tree(TRUNC8, t1, 0);
    } else if (numBits == 16) {
        t = tree(TRUNC16, t1, 0);
    } else if (numBits == 32) {
        t = tree(TRUNC32, t1, 0);
    } else if (numBits == 64) {
        t = tree(TRUNC64, t1, 0);
    } else {
        errs() << "\n************ NO MATCH IN TRUNC FOR MEM TYPE ************\n";
        t = tree(TRUNC32, 0, 0);
    }
    t->inst = I;
    return t;
}

Tree sext_tree(Instruction* I) { // Need To Implement
    int numBits = dyn_cast<SExtInst > (I)->getType()->getScalarSizeInBits();
    Tree t1 = getTree(dyn_cast<Instruction > (I->getOperand(0)));
    t1->inst = dyn_cast<Instruction > (I->getOperand(0));
    Tree t;
    if (numBits == 8) {
        t = tree(SEXT8, t1, 0);
    } else if (numBits == 16) {
        t = tree(SEXT16, t1, 0);
    } else if (numBits == 32) {
        t = tree(SEXT32, t1, 0);
    } else if (numBits == 64) {
        t = tree(SEXT64, t1, 0);
    } else {
        errs() << "\n************ NO MATCH IN SEXT FOR MEM TYPE ************\n";
        t = tree(SEXT32, t1, 0);
    }
    t->val = dyn_cast<SExtInst > (I)->getSrcTy()->getScalarSizeInBits();
    errs() << "***SIGN EXTENDING TO " << t->val << "********\n";
    t->inst = I;
    return t;
}

Tree load_tree(Instruction* I) { //MEM should have Alloca and load should have I as inst, check how it affects code
    errs() << "\n Is a load \n" << *I;
    if (isa<AllocaInst > (I->getOperand(0))) {
        errs() << "\n Is a alloca \n";
        Tree t;
        AllocaInst* allocaInst = dyn_cast<AllocaInst > (I->getOperand(0));
        int numBits = allocaInst->getAllocatedType()->getScalarSizeInBits();

        if (numBits == 8) {
            t = tree(MEM8, 0, 0);
        } else if (numBits == 16) {
            t = tree(MEM16, 0, 0);
        } else if (numBits == 32) {
            t = tree(MEM32, 0, 0);
        } else if (numBits == 64) {
            t = tree(MEM64, 0, 0);
        } else {
            if (isa<PointerType > (allocaInst->getAllocatedType())) {
                t = tree(MEM64, 0, 0);
            } else {
                errs() << "\n************ NO MATCH IN STORE FOR MEM TYPE ************\n";
                t = tree(MEM32, 0, 0);
            }
        }

        t->val = getOffset(mapTable.find(dyn_cast<Instruction > (I->getOperand(0)))->second);
        t->inst = dyn_cast<Instruction > (I->getOperand(0));
        Tree t1 = tree(LOAD, t, 0);
        t1->inst = I;

        return t1;
    } else {
        Tree t;
        //        t->val = mapTable.find(dyn_cast<Instruction > (I->getOperand(0)))->second;

        if (isa<GEPOperator > (I->getOperand(0))) {
            errs() << "\n IN LOAD GLOB PTR TYPE \n";

            if (isa<GlobalValue > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0))) {

                const Type* arr = dyn_cast<PointerType > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0)->getType())->getElementType();
                if (dyn_cast<ArrayType > (arr)) {
                    errs() << "Array Type";
                    int offset = getGolbalArrayOffset(dyn_cast<ArrayType > (arr), dyn_cast<GEPOperator > (I->getOperand(0)));
                    errs() << "\n getGolbalArrayOffset returned:" << offset << "\n";
                    if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(32)) {
                        errs() << "\n gep type int \n";
                        t = tree(GLOB32, 0, 0);
                        t->val = offset;

                    } else if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(64)) {
                        errs() << "\n gep type 64 \n";
                        t = tree(GLOB64, 0, 0);
                        t->val = offset;

                    } else if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(16)) {
                        errs() << "\n gep type 16 \n";
                        t = tree(GLOB16, 0, 0);
                        t->val = offset;

                    } else if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(8)) {
                        errs() << "\n gep type 8 \n";
                        t = tree(GLOB8, 0, 0);
                        t->val = offset;

                    } else {
                        errs() << "\n NO GLOB TYPE MATCH \n";
                        t = tree(GLOB32, 0, 0);
                        t->val = offset;
                    }

                } else {

                    if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(32)) {
                        t = tree(GLOB32, 0, 0);
                        t->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(2))->getSExtValue() * 4;
                    } else if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(64)) {
                        t = tree(GLOB64, 0, 0);
                        t->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(2))->getSExtValue() * 8;
                    } else if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(16)) {
                        t = tree(GLOB16, 0, 0);
                        t->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(2))->getSExtValue() * 2;
                    } else if (dyn_cast<PointerType > (I->getOperand(0)->getType())->getElementType()->isIntegerTy(8)) {
                        t = tree(GLOB8, 0, 0);
                        t->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(2))->getSExtValue();
                    } else {
                        errs() << "\n Not MATCH FOR GLOB TYPE \n";
                        t = tree(GLOB32, 0, 0);
                        t->val = dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(2))->getSExtValue() * 4;
                    }
                }
                t->name = strdup((dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0))->getNameStr().c_str());

            } else {
                errs() << "Get Pointer not a global \n"; //handle local arrays.
                errs() << *I << "\n";
                //                errs() << isa<GEPOperator > (I->getOperand(0));
                //                errs() << dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0)) << "\n";
                if (!dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0))) {

                    GEPOperator* gepptr = dyn_cast<GEPOperator > (I->getOperand(0));
                    if (dyn_cast<StructType > (dyn_cast<PointerType > (gepptr->getOperand(0)->getType())->getElementType())) {
                        errs() << "\n STORE TO A STRUCT TYPE \n";
                        if (I->getOperand(0)->getType()->isIntegerTy(32)) {
                            errs() << "\n  Is Integer type \n";
                            t = tree(MEM32, 0, 0);

                        } else if (I->getOperand(0)->getType()->isIntegerTy(16)) {
                            errs() << "\n  Is short type \n";
                            t = tree(MEM16, 0, 0);

                        } else if (I->getOperand(0)->getType()->isIntegerTy(8)) {
                            errs() << "\n  Is char type \n";
                            t = tree(MEM8, 0, 0);

                        } else if (I->getOperand(0)->getType()->isIntegerTy(64)) {
                            t = tree(MEM64, 0, 0);
                            errs() << "\n  Is long type \n";

                        } else {
                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                            t = tree(MEM32, 0, 0);
                        }
                        t->val = getStructOffset(dyn_cast<GEPOperator > (I->getOperand(0)));
                        errs() << " \n Get Struct Offset returned " << t->val;
                    }

                    else {
                        errs() << "\n MultiDimensional Array in Load\n";
                        const Type* type = dyn_cast<PointerType > (dyn_cast<GEPOperator > (I->getOperand(0))->getType())->getElementType();
                        if (type->isIntegerTy(32)) {
                            errs() << "\n  Is Integer type \n";
                            t = tree(MEM32, 0, 0);

                        } else if (type->isIntegerTy(16)) {
                            errs() << "\n  Is short type \n";
                            t = tree(MEM16, 0, 0);

                        } else if (type->isIntegerTy(8)) {
                            errs() << "\n  Is char type \n";
                            t = tree(MEM8, 0, 0);

                        } else if (type->isIntegerTy(64)) {
                            t = tree(MEM64, 0, 0);
                            errs() << "\n  Is long type \n";

                        } else {
                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                            t = tree(MEM32, 0, 0);
                        }
                        t->val = getArrayOffset(dyn_cast<GEPOperator > (I->getOperand(0)));
                        errs() << " \n Get Array Offset returned " << t->val;
                        
                        
//                         t = getGepTree(dyn_cast<GEPOperator > (I->getOperand(0)));
//                         errs() << "\n PRINTING GEP TREE: \n";
//                         printTree(t);
                    }

                } else {

                    if (dyn_cast<StructType > (dyn_cast<PointerType > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0)->getType())->getElementType())) {
                        const StructType* arrType = dyn_cast<StructType > (dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0))->getAllocatedType());
                        int offset = 0;
                        for (int i = 0; i < arrType->getNumElements(); i++) {
                            const Type* type = arrType->getContainedType(i);
                            if (dyn_cast<ArrayType > (type)) {
                                offset = offset + getArraySize(dyn_cast<ArrayType > (type));
                            } else if (dyn_cast<StructType > (type)) {
                                offset = offset + getStructSize(dyn_cast<StructType > (type));
                            } else {
                                offset = offset + (type->getScalarSizeInBits() / 8);
                            }
                        }
                        if (I->getOperand(0)->getType()->isIntegerTy(32)) {
                            errs() << "\n  Is Integer type \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;
                            errs() << "\nhere\n";

                        } else if (I->getOperand(0)->getType()->isIntegerTy(16)) {
                            errs() << "\n  Is short type \n";
                            t = tree(MEM16, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;

                        } else if (I->getOperand(0)->getType()->isIntegerTy(8)) {
                            errs() << "\n  Is char type \n";
                            t = tree(MEM8, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;

                        } else if (I->getOperand(0)->getType()->isIntegerTy(64)) {
                            t = tree(MEM64, 0, 0);
                            errs() << "\n  Is long type \n";

                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;
                        } else {
                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;
                        }
                    }

                    else {
                        const ArrayType* arrType = dyn_cast<ArrayType > (dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0))->getAllocatedType());
                        const Type* elemType = arrType->getElementType();
                        if (elemType->isIntegerTy(32)) {
                            errs() << "\n  Is Integer type \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);

                            errs() << "\n t->val " << t->val << "\n";

                        } else if (elemType->isIntegerTy(16)) {
                            errs() << "\n  Is short type \n";
                            t = tree(MEM16, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);
                        } else if (elemType->isIntegerTy(8)) {
                            errs() << "\n  Is char type \n";
                            t = tree(MEM8, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);

                        } else if (elemType->isIntegerTy(64)) {
                            t = tree(MEM64, 0, 0);
                            errs() << "\n  Is long type \n";

                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);
                        } else {
                            errs() << "\n Not MATCH FOR MEM TYPE IN Load PTR \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);
                        }
                    }
                }
            }

        } else {
            if (isa<GlobalValue > (I->getOperand(0))) {
                int numBits = dyn_cast<GlobalValue > (I->getOperand(0))->getOperand(0)->getType()->getScalarSizeInBits();

                if (numBits == 8) {
                    t = tree(GLOB8, 0, 0);
                } else if (numBits == 16) {
                    t = tree(GLOB16, 0, 0);
                } else if (numBits == 32) {
                    t = tree(GLOB32, 0, 0);
                } else if (numBits == 64) {
                    t = tree(GLOB64, 0, 0);
                } else {
                    errs() << "\n************ NO MATCH IN LOAD FOR GLOB NON PTR TYPE ************\n";
                    t = tree(GLOB32, 0, 0);
                }
                t->name = strdup(I->getOperand(0)->getNameStr().c_str());
                t->val = 0;
            } else { //load from a reg.
                errs() << "\n Load from a Register \n";
                t = getTree(dyn_cast<Instruction > (I->getOperand(0)));
            }
        }

        t->inst = dyn_cast<Instruction > (I);
        Tree t1 = tree(LOAD, t, 0);
        t1->inst = I;

        return t1;
    }
}

Tree ret_tree(Instruction* I) {
    int numBits = dyn_cast<CallInst > (I)->getType()->getScalarSizeInBits();
    Tree t;
    if (numBits == 8) {
        t = tree(RET8, 0, 0);
    } else if (numBits == 16) {
        t = tree(RET16, 0, 0);
    } else if (numBits == 32) {
        t = tree(RET32, 0, 0);
    } else if (numBits == 64) {
        t = tree(RET64, 0, 0);
    } else {
        errs() << "\n************ NO MATCH IN RET TREE FOR SIZE OF RETURN VALUE ************\n";
        t = tree(RET32, 0, 0);
    }
    t->inst = I;
    return t;
}

Tree gep_tree(Instruction* I) { // Need to Implement this

}

int min(int a, int b) {
    return (a > b) ? b : a;
}
char *param_regs[] = {
    "edi",
    "esi",
    "edx",
    "ecx",
    "r8d",
    "r9d",
};





Tree call_tree(Instruction* I) {
    errs() << "\n Entered Call Instruction \n";


    int numArguments = dyn_cast<CallInst > (I)->getNumArgOperands(); //dyn_cast<Function>(I->getOperand(0))->getArgumentList().size();
    errs() << "numArguments: " << numArguments;

    // Build Dependencies
    for (int i = 0; i < numArguments; i++) {
        errs() << " OPerand  " << i << "\n";
        if (isa<AllocaInst > (dyn_cast<CallInst > (I)->getArgOperand(i))) {
            continue;
        }
        if (isa<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)) && !isa<ConstantInt > (dyn_cast<CallInst > (I)->getArgOperand(i))) {
            Tree t;
            t = getTree(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            printTree(t);
            gen(t);
        } else {
            errs() << " OPerand..  " << i << "\n"; //Is either a constant int or gep handled during loading args, for local gep may need to build tree.
        }
    }

    //caller saving stuff

    fp = fopen("CodeGen.asm", "a");
    //    Function::arg_iterator start = dyn_cast<CallInst>(I)->getCalledFunction()->getArgumentList().begin();
    errs() << "Minimunm is " << min(6, numArguments);
    fprintf(fp, "  push  %%r10 \n");
    fprintf(fp, "  push  %%r11 \n");
    fprintf(fp, "  push  %%r12 \n");
    fprintf(fp, "  push  %%r13 \n");
    fprintf(fp, "  push  %%r14 \n");


    for (int i = 0; i < min(6, numArguments); i++) {
        if (isa<AllocaInst > (dyn_cast<CallInst > (I)->getArgOperand(i))) {
            fprintf(fp, "  lea -%d(%%rbp), %%%s\n", getOffset(mapTable.find(dyn_cast<AllocaInst > (dyn_cast<CallInst > (I)->getArgOperand(i)))->second), getArgReg(i, 64));
            continue;
        }

        if (isa<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i))) {

            int numBits = dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i))->getType()->getScalarSizeInBits();
            const char* allocReg;
            if (numBits == 8) {
                allocReg = getReg8(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            } else if (numBits == 16) {
                allocReg = getReg16(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            } else if (numBits == 32) {
                allocReg = getReg32(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            } else if (numBits == 64) {
                allocReg = getReg64(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            } else {
                errs() << "\n************ NO MATCH IN CALL FOR GETREG ************\n";
                allocReg = getReg32(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            }

            fprintf(fp, "  mov  %%%s, %%%s\n", allocReg, getArgReg(i, numBits));
        } else if (isa<GEPOperator > (dyn_cast<CallInst > (I)->getArgOperand(i))) { //local array gep will be loaded first and then passed.
            fprintf(fp, "  mov  $%s, %%%s\n", dyn_cast<GEPOperator > (dyn_cast<CallInst > (I)->getArgOperand(i))->getOperand(0)->getNameStr().c_str(), getArgReg(i, 64));
        } else if (isa<ConstantInt > (dyn_cast<CallInst > (I)->getArgOperand(i))) {
            errs() << "Matched Inst\n";
            int size = (dyn_cast<CallInst > (I)->getArgOperand(i))->getType()->getScalarSizeInBits();
            fprintf(fp, "  mov $%d, %%%s\n", dyn_cast<llvm::ConstantInt > (dyn_cast<CallInst > (I)->getArgOperand(i))->getSExtValue(), getArgReg(i, size));
        } else {

        }
    }
    std::stringstream pusher;
    std::stringstream poper;
    for (int i = numArguments; i >= 6; i--) { //enter call setup
        if (isa<AllocaInst > (dyn_cast<CallInst > (I)->getArgOperand(i))) {
            fprintf(fp, "  lea -%d(%%rbp), %%rbx\n", getOffset(mapTable.find(dyn_cast<AllocaInst > (dyn_cast<CallInst > (I)->getArgOperand(i)))->second));
            pusher << "  push  %rbx" << "\n";
            poper << "  pop  %rbx" << "\n";
            continue;
        }
        if (isa<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i))) {

            //            int numBits = dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i))->getType()->getScalarSizeInBits();
            const char* allocReg;
            //            if (numBits == 8) {
            //                allocReg = getReg8(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            //                pusher << " subq $8, %%rsp";
            //                pusher << "  movw  %" << allocReg << ", " << "(%rsp)" << "\n";
            //                
            //                poper << "  pop  %" << allocReg << "\n";
            //            } else if (numBits == 16) {
            //                allocReg = getReg16(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            //                pusher << " subq $8, %%rsp";
            //                pusher << "  movd  %" << allocReg << ", " << "(%rsp)" << "\n";
            //                
            //            } else if (numBits == 32) {
            //                allocReg = getReg32(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            //                pusher << "  subq $8, %%rsp\n";
            //                pusher << "  movl  %" << allocReg << ", " << "(%rsp)" << "\n";
            //                
            //            } else if (numBits == 64) {
            //                allocReg = getReg64(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            //                pusher << "  push  %" << allocReg << "\n";
            //                poper << "  pop  %" << allocReg << "\n";
            //            } else {
            //                errs() << "\n************ NO MATCH IN CALL FOR GETREG ************\n";
            //                allocReg = getReg32(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            //            }

            allocReg = getReg64(dyn_cast<Instruction > (dyn_cast<CallInst > (I)->getArgOperand(i)));
            pusher << "  push  %" << allocReg << "\n";
            poper << "  pop  %" << allocReg << "\n";
            //            pusher << "  push  %" << allocReg << "\n";
            //            poper << "  pop  %" << allocReg << "\n";
            //            fprintf(fp, "  push  %%%s \n", allocReg);

        } else if (isa<GEPOperator > (dyn_cast<CallInst > (I)->getArgOperand(i))) { //local array gep will be loaded first and then passed.
            //            fprintf(fp, "  push  $%s \n", dyn_cast<GEPOperator > (dyn_cast<CallInst > (I)->getArgOperand(i))->getOperand(0)->getNameStr().c_str());
            pusher << "  pushq $" << (dyn_cast<GEPOperator > (dyn_cast<CallInst > (I)->getArgOperand(i))->getOperand(0)->getNameStr()) << "\n";
            poper << "  popq %%rbx" << "\n";
        } else if (isa<ConstantInt > (dyn_cast<CallInst > (I)->getArgOperand(i))) {
            errs() << "Matched Inst \n";
            int size = (dyn_cast<CallInst > (I)->getArgOperand(i))->getType()->getScalarSizeInBits();
            //            fprintf(fp, "  pushq $%d \n", dyn_cast<llvm::ConstantInt > (dyn_cast<CallInst > (I)->getArgOperand(i))->getSExtValue());
            pusher << "  pushq $" << dyn_cast<llvm::ConstantInt > (dyn_cast<CallInst > (I)->getArgOperand(i))->getSExtValue() << "\n";
            poper << "  popq %%rbx" << "\n";
        } else {

        }
    }
    fprintf(fp, strdup(pusher.str().c_str()));
    fprintf(fp, "  movl  $0, %%eax\n");
    std::stringstream os;
    
    std::string calledfunc = dyn_cast<CallInst >(I)->getCalledFunction()->getNameStr();
    
    os << "  call " << calledfunc  << "\n";
    fprintf(fp, strdup(os.str().c_str())); //"  call %s\n", dyn_cast<CallInst > (I)->getCalledFunction()->getNameStr().c_str());
    fprintf(fp, strdup(poper.str().c_str()));

    //caller restore stuff
    fprintf(fp, "  pop  %%r14 \n");
    fprintf(fp, "  pop  %%r13 \n");
    fprintf(fp, "  pop  %%r12 \n");
    fprintf(fp, "  pop  %%r11 \n");
    fprintf(fp, "  pop  %%r10 \n");

    fclose(fp);
}

Tree binaryop_tree(Instruction* I) {

    int op;
    int InsOpcode = I->getOpcode();

    switch (InsOpcode) {
        case lSTORE:
            op = STORE;
            break;
        case lLOAD:
            op = LOAD;
            break;
        case lADD:
            op = ADD;
            break;
        case lFADD: //not handling floating point
        case lSUB:
            op = SUB;
            break;
        case lFSUB:
        case lMUL:
            op = MUL;
            break;
        case lFMUL:
        case lUDIV:
            op = UDIV;
            break;
        case lSDIV: //TO BE HAndeled 
        case lFDIV:
        case lUREM:
        case lSREM:
        case lFREM:
        case lSHL:
            op = SHL;
            break;
        case lLSHR:
            op = LSHR;
            break;
        case lASHR:
            op = ASHR;
            break;
        case lAND:
            op = AND;
            break;
        case lOR:
            op = OR;
            break;
        case lXOR:
            op = XOR;
            break;
        case lGEP:
            break;
        case lSEXT:
            break;
            //        case CALL:
            //            call_match(BI);
            //            break;
        default:
            break;
    }
    Tree t;
    if (isa<llvm::ConstantInt > (I->getOperand(0)) && isa<llvm::ConstantInt > (I->getOperand(1))) {
        Tree t1 = tree(CNSTI, 0, 0);
        Tree t2 = tree(CNSTI, 0, 0);
        t1->inst = dyn_cast<Instruction > (I->getOperand(0)); // May need info about types at code generation time better to store it.
        t2->inst = dyn_cast<Instruction > (I->getOperand(1)); // May need info about types at code generation time better to store it.
        t1->val = dyn_cast<ConstantInt > (I->getOperand(0))->getSExtValue();
        t2->val = dyn_cast<ConstantInt > (I->getOperand(1))->getSExtValue();
        t = tree(op, t1, t2);
    } else if (isa<llvm::ConstantInt > (I->getOperand(0))) {
        if (I->isCommutative()) {
            Tree t2 = tree(CNSTI, 0, 0);
            t2->inst = dyn_cast<Instruction > (I->getOperand(0)); // May need info about types at code generation time better to store it.
            t2->val = dyn_cast<ConstantInt > (I->getOperand(0))->getSExtValue();
            t = tree(op, getTree(dyn_cast<Instruction > (I->getOperand(1))), t2);
        } else {
            Tree t1 = tree(CNSTI, 0, 0);
            t1->inst = dyn_cast<Instruction > (I->getOperand(0)); // May need info about types at code generation time better to store it.
            t1->val = dyn_cast<ConstantInt > (I->getOperand(0))->getSExtValue();
            t = tree(op, t1, getTree(dyn_cast<Instruction > (I->getOperand(1))));
        }
    } else if (isa<llvm::ConstantInt > (I->getOperand(1))) {

        Tree t2 = tree(CNSTI, 0, 0);
        t2->inst = dyn_cast<Instruction > (I->getOperand(1)); // May need info about types at code generation time better to store it.
        t2->val = dyn_cast<ConstantInt > (I->getOperand(1))->getSExtValue();
        t = tree(op, getTree(dyn_cast<Instruction > (I->getOperand(0))), t2);
    } else {
        t = tree(op, getTree(dyn_cast<Instruction > (I->getOperand(0))), getTree(dyn_cast<Instruction > (I->getOperand(1))));
    }
    t->inst = I;

    return t;
}

Tree bitcast_tree(Instruction* I){
    Tree t;
    if (dyn_cast<StructType > (dyn_cast<PointerType > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0)->getType())->getElementType())) {
                        const StructType* arrType = dyn_cast<StructType > (dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0))->getAllocatedType());
                        int offset = 0;
                        for (int i = 0; i < arrType->getNumElements(); i++) {
                            const Type* type = arrType->getContainedType(i);
                            if (dyn_cast<ArrayType > (type)) {
                                offset = offset + getArraySize(dyn_cast<ArrayType > (type));
                            } else if (dyn_cast<StructType > (type)) {
                                offset = offset + getStructSize(dyn_cast<StructType > (type));
                            } else {
                                offset = offset + (type->getScalarSizeInBits() / 8);
                            }
                        }
                        if (I->getOperand(0)->getType()->isIntegerTy(32)) {
                            errs() << "\n  Is Integer type \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;
                            errs() << "\nhere\n";

                        } else if (I->getOperand(0)->getType()->isIntegerTy(16)) {
                            errs() << "\n  Is short type \n";
                            t = tree(MEM16, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;

                        } else if (I->getOperand(0)->getType()->isIntegerTy(8)) {
                            errs() << "\n  Is char type \n";
                            t = tree(MEM8, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;

                        } else if (I->getOperand(0)->getType()->isIntegerTy(64)) {
                            t = tree(MEM64, 0, 0);
                            errs() << "\n  Is long type \n";

                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;
                        } else {
                            errs() << "\n Not MATCH FOR MEM TYPE \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - offset;
                        }
                    }

                    else {
                        const ArrayType* arrType = dyn_cast<ArrayType > (dyn_cast<llvm::AllocaInst > (dyn_cast<GEPOperator > (I->getOperand(0))->getOperand(0))->getAllocatedType());
                        const Type* elemType = arrType->getElementType();
                        if (elemType->isIntegerTy(32)) {
                            errs() << "\n  Is Integer type \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);

                            errs() << "\n t->val " << t->val << "\n";

                        } else if (elemType->isIntegerTy(16)) {
                            errs() << "\n  Is short type \n";
                            t = tree(MEM16, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);
                        } else if (elemType->isIntegerTy(8)) {
                            errs() << "\n  Is char type \n";
                            t = tree(MEM8, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);

                        } else if (elemType->isIntegerTy(64)) {
                            t = tree(MEM64, 0, 0);
                            errs() << "\n  Is long type \n";

                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);
                        } else {
                            errs() << "\n Not MATCH FOR MEM TYPE IN Load PTR \n";
                            t = tree(MEM32, 0, 0);
                            t->val = getOffset(mapTable.find(dyn_cast<Instruction > (dyn_cast<Instruction > (I->getOperand(0))->getOperand(0)))->second) - (dyn_cast<llvm::ConstantInt > (dyn_cast<GEPOperator > (dyn_cast<LoadInst > (I)->getOperand(0))->getOperand(2))->getSExtValue()*4);
                        }
                    }
    return t;
}

Tree getTree(Instruction* I) {
    int InsOpcode = I->getOpcode();
    switch (InsOpcode) {
        case lSTORE:
            return store_tree(I); //Should Never be Called
            break;
        case lLOAD:
            errs() << "\n Matched Load \n";
            return load_tree(I);
            break;
        case lTRUNC:
            errs() << "\n Trunc Matched \n";
            return trunc_tree(I);
            break;
        case lBITCAST:
            return bitcast_tree(I);
            break;
        case lADD:
        case lFADD:
        case lSUB:
        case lFSUB:
        case lMUL:
        case lFMUL:
        case lUDIV:
        case lSDIV:
        case lFDIV:
        case lUREM:
        case lSREM:
        case lFREM:
        case lSHL:
        case lLSHR:
        case lASHR:
        case lAND:
        case lOR:
        case lXOR:
            return binaryop_tree(I);
            break;
        case lGEP:
            return gep_tree(I);
            break;
        case lSEXT:
            errs() << "\n SEXT Matched \n";
            return sext_tree(I);
            break;
        case lCALL:
            return ret_tree(I);
            //        case CALL:
            //            call_match(BI);
            //            break;
        default:
            break;
    }
}

char Matcher::ID = 0;
static RegisterPass<Matcher> X("Matcher", "Matcher World Pass", false, false);



















