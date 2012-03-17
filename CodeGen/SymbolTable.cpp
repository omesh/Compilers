#include "SymbolTable.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Type.h"
#include "llvm/Instructions.h"
#include "sstream"
#include "algorithm"
#include <string.h>
SymbolTable symbolTable;
std::map<int,int> ArgumentOffset;
liveSet livenessSet;

using namespace llvm;

int getArgOffset(int i){
    return ArgumentOffset.find(i)->second;
}

const char* getArgReg(int i,int size){
    errs() << "i = " << i  << "size = " << size << "\n";
    const char* reg;
    if(i==0){
        reg = "7";
    }
    if(i==1){
        reg = "8";
    }
    if(i==2){
        reg = "9";
    }
    if(i==3){
        reg = "10";
    }
    if(i==4){
        reg = "11";
    }
    if(i==5){
        reg = "12";
    }
    std::string reg1 = getTypeReg(size,reg);
    errs() <<"here\n";
    return strdup(reg1.c_str());
}

bool addBeforeSet(llvm::Instruction* I, std::vector<llvm::Value*> liveVars) { // do we require to merge or replace is enough
    if ((livenessSet.find(I) != livenessSet.end())) {
        livenessSet.find(I)->second->beforeSet = liveVars;
        return true;
    } else {
        struct livenessInfo* lInfo = new livenessInfo;
        lInfo->beforeSet = liveVars;
        livenessSet.insert(liveSetPair(I, lInfo));
        return false;
    }
}

bool addAfterSet(llvm::Instruction* I, std::vector<llvm::Value*> liveVars) { // do we require to merge or replace is enough
    if (livenessSet.find(I) != livenessSet.end()) {
        livenessSet.find(I)->second->afterSet = liveVars;
        return true;
    } else {
        struct livenessInfo* lInfo = new livenessInfo;
        lInfo->afterSet = liveVars;
        livenessSet.insert(liveSetPair(I, lInfo));
        return false;
    }
}

void printLivenessInfo() {
    for (liveSet::iterator i = livenessSet.begin(), e = livenessSet.end(); i != e; i++) {
        errs() << "live before set";
        errs() << "\n {";
        for (std::vector<llvm::Value*>::iterator it = i->second->beforeSet.begin(), ie = i->second->beforeSet.end(); it != ie; it++) {

            errs() << **it;
        }
        errs() << " } \n";
        errs() << "\n Instruction " << *(i->first);
        errs() << "\n\n live after set";
        errs() << "\n {";
        for (std::vector<llvm::Value*>::iterator it = i->second->afterSet.begin(), ie = i->second->afterSet.end(); it != ie; it++) {
            errs() << **it;
        }
        errs() << " } \n\n";
    }
}

int getOffset(int i) {
    errs() << "\n Getting Offset for " << i << "\n";
    printSymbolTable();
    return symbolTable.find(i)->second->offset;
}

int getSize(int i) {
    return symbolTable.find(i)->second->size;
}

void printSymbolTable() {
    debug(errs() << "\n***** Printing contents of SymbolTable ");
    debug(errs() << " (Symbol    Offset  Size) \n");
    for (SymbolTable::iterator i = symbolTable.begin(), e = symbolTable.end(); i != e; i++) {
        debug(errs() << i->first << " ");
        debug(errs() << i->second->offset << " ");
        debug(errs() << i->second->size << "\n");
    }
    debug(errs() << "***** Finished Printing contents of SymbolTable \n");
}


std::map<std::string, std::vector<llvm::Value*> > registorDescriptor;
std::map<std::string, std::vector<llvm::Value*> > spilledRegs;
typedef std::map<std::string, std::vector<llvm::Value*> >::iterator RegistorDescriptorIterator;
std::map<llvm::Value*, std::vector<std::string> > memoryDescriptor;
std::map<llvm::Value*, std::vector<std::string> > spilledMems;
typedef std::map<llvm::Value*, std::vector<std::string> >::iterator MemoryDescriptorIterator;
std::vector<std::string> freeRegs;

typedef std::vector<llvm::Value*>::iterator ValueVectorIterator;
typedef std::vector<std::string>::iterator RegsVectorIterator;

static int counter = 0;


std::map<std::string,std::string> Regs8bit;
std::map<std::string,std::string> Regs16bit;
std::map<std::string,std::string> Regs32bit;
std::map<std::string,std::string> Regs64bit;

int SpilledOffset;
void setSpilledOffset(int i){
    SpilledOffset=i;
}

void populateRegs() {
   // freeRegs.push_back("0");
    freeRegs.push_back("1");
    freeRegs.push_back("2");
    freeRegs.push_back("3");
    freeRegs.push_back("4");
    freeRegs.push_back("5");
    
    Regs8bit.insert(std::pair<std::string,std::string>("0","bl"));
    Regs8bit.insert(std::pair<std::string,std::string>("1","r10b"));
    Regs8bit.insert(std::pair<std::string,std::string>("2","r11b"));
    Regs8bit.insert(std::pair<std::string,std::string>("3","r12b"));
    Regs8bit.insert(std::pair<std::string,std::string>("4","r13b"));
    Regs8bit.insert(std::pair<std::string,std::string>("5","r14b"));
    
    Regs8bit.insert(std::pair<std::string,std::string>("6","ax"));//return reg
    Regs8bit.insert(std::pair<std::string,std::string>("7","dil"));
    Regs8bit.insert(std::pair<std::string,std::string>("8","sil"));
    Regs8bit.insert(std::pair<std::string,std::string>("9","dl"));
    Regs8bit.insert(std::pair<std::string,std::string>("10","cl"));
    Regs8bit.insert(std::pair<std::string,std::string>("11","r8b"));
    Regs8bit.insert(std::pair<std::string,std::string>("12","r9b"));
    
    Regs16bit.insert(std::pair<std::string,std::string>("0","bx"));
    Regs16bit.insert(std::pair<std::string,std::string>("1","r10w"));
    Regs16bit.insert(std::pair<std::string,std::string>("2","r11w"));
    Regs16bit.insert(std::pair<std::string,std::string>("3","r12w"));
    Regs16bit.insert(std::pair<std::string,std::string>("4","r13w"));
    Regs16bit.insert(std::pair<std::string,std::string>("5","r14w"));
    
    Regs16bit.insert(std::pair<std::string,std::string>("6","ax"));//return reg
    
    Regs16bit.insert(std::pair<std::string,std::string>("7","di"));// Args Regs
    Regs16bit.insert(std::pair<std::string,std::string>("8","si"));
    Regs16bit.insert(std::pair<std::string,std::string>("9","dx"));
    Regs16bit.insert(std::pair<std::string,std::string>("10","cx"));
    Regs16bit.insert(std::pair<std::string,std::string>("11","r8w"));
    Regs16bit.insert(std::pair<std::string,std::string>("12","r9w"));
    
    Regs32bit.insert(std::pair<std::string,std::string>("0","ebx"));
    Regs32bit.insert(std::pair<std::string,std::string>("1","r10d"));
    Regs32bit.insert(std::pair<std::string,std::string>("2","r11d"));
    Regs32bit.insert(std::pair<std::string,std::string>("3","r12d"));
    Regs32bit.insert(std::pair<std::string,std::string>("4","r13d"));
    Regs32bit.insert(std::pair<std::string,std::string>("5","r14d"));
    
    Regs32bit.insert(std::pair<std::string,std::string>("6","eax"));//return reg
    
    Regs32bit.insert(std::pair<std::string,std::string>("7","edi"));
    Regs32bit.insert(std::pair<std::string,std::string>("8","esi"));
    Regs32bit.insert(std::pair<std::string,std::string>("9","edx"));
    Regs32bit.insert(std::pair<std::string,std::string>("10","ecx"));
    Regs32bit.insert(std::pair<std::string,std::string>("11","r8d"));
    Regs32bit.insert(std::pair<std::string,std::string>("12","r9d"));
    
    
    Regs64bit.insert(std::pair<std::string,std::string>("0","rbx"));
    Regs64bit.insert(std::pair<std::string,std::string>("1","r10"));
    Regs64bit.insert(std::pair<std::string,std::string>("2","r11"));
    Regs64bit.insert(std::pair<std::string,std::string>("3","r12"));
    Regs64bit.insert(std::pair<std::string,std::string>("4","r13"));
    Regs64bit.insert(std::pair<std::string,std::string>("5","r14"));
    
    Regs64bit.insert(std::pair<std::string,std::string>("6","rax"));//return reg
    
    Regs64bit.insert(std::pair<std::string,std::string>("7","rdi"));
    Regs64bit.insert(std::pair<std::string,std::string>("8","rsi"));
    Regs64bit.insert(std::pair<std::string,std::string>("9","rdx"));
    Regs64bit.insert(std::pair<std::string,std::string>("10","rcx"));
    Regs64bit.insert(std::pair<std::string,std::string>("11","r8"));
    Regs64bit.insert(std::pair<std::string,std::string>("12","r9"));
    
    
}


const char* getReg8(llvm::Value* I){
    return strdup(Regs8bit.find(getReg(dyn_cast<llvm::Instruction>(I)))->second.c_str());
}

const char* getReg16(llvm::Value* I){
    return strdup(Regs16bit.find(getReg(dyn_cast<llvm::Instruction>(I)))->second.c_str());
}

const char* getReg32(llvm::Value* I){
    return strdup(Regs32bit.find(getReg(dyn_cast<llvm::Instruction>(I)))->second.c_str());
}

const char* getReg64(llvm::Value* I){
    return strdup(Regs64bit.find(getReg(dyn_cast<llvm::Instruction>(I)))->second.c_str());
}

const char* getTypeRegInst(unsigned i,llvm::Value* I){
    errs() << "isa " << isa<Instruction>(I);
    std::string regi = getTypeReg(i, getReg(dyn_cast<llvm::Instruction>(I)));
    errs() << "\n GOT REG " << regi <<"\n"; 
    return strdup(regi.c_str());
}

std::string getTypeReg(unsigned i, std::string reg){
    errs() << "Allocating Reg for type " << reg << "\n";
    if(i == 8){
        return Regs8bit.find(reg)->second;
    }
    else if(i == 16){
        return Regs16bit.find(reg)->second;
    }
    else if(i == 32){
        return Regs32bit.find(reg)->second;
    }
    else if(i == 64){
//        errs() << " lllll " << Regs64bit.find(reg)->second << "\n";
        return Regs64bit.find(reg)->second;
    }
    else{
        errs() << "\n None for Type of Reg to Allocate Matched."<< i << " \n" ;
        return Regs32bit.find(reg)->second;
    }
    return "";
}
std::string getReg(llvm::Instruction* I) {
    std::stringstream ss;
    errs() << " Get Register Called on " << *I << "\n";
    //    ss << "R" << counter++;
    int InsOpcode = I->getOpcode();
    switch (InsOpcode) {
            //                    case lSTORE:
            ////                        store_nv(BI);
            //                        break;
        case lLOAD:// why if - else??
            errs() << "Matched load reg\n";
            if(isa<AllocaInst>(I->getOperand(0))){
                ss << getReg_load(I); //getTypeReg(dyn_cast<llvm::AllocaInst > (I->getOperand(0))->getAllocatedType()->getScalarSizeInBits(),getReg_load(I)); //getReg_load(I);
            }
            else{
                ss << getReg_load(I); //getTypeReg(I->getOperand(0)->getType()->getScalarSizeInBits(),getReg_load(I)); // how to get size of bits for global values?????
            }
            break;
        case lADD:
        case lASHR:
        case lLSHR:
        case lSHL:
        case lUDIV:
        case lSDIV:
        case lUREM:
        case lSREM:
            //                    case lFADD:
        case lSUB:
            //                    case lFSUB:
        case lMUL:
            //                    case lFMUL:
            //                    case lUDIV:
            //                    case lSDIV:
            //                    case lFDIV:
            //                    case lUREM:
            //                    case lSREM:
            //                    case lFREM:
            //                    case lSHL:
            //                    case lLSHR:
            //                    case lASHR:
            //                    case lAND:
            //                    case lOR:
            //                    case lXOR: 
            
            ss << getReg_binaryop(I);//getTypeReg(dyn_cast<Instruction>(I->getOperand(0))->getType()->getScalarSizeInBits(),getReg_binaryop(I));  //getReg_binaryop(I);
            break;
            //                    case lALLOCA:
            ////                        alloca_nv(BI);
            //                        break;
            //                    case lGEP:
            ////                        gep_nv(BI);
            //                        break;
            //                    case lSEXT:
            ////                        sext_nv(BI);
            //                        break;
            //                    case lCALL:
            ////                        call_nv(BI);
            //                        break;
        case lTRUNC:
            ss << getReg_trunc(I);
            break;
        case lALLOCA:
            ss << getReg_alloca(I);
            break;
        case lCALL:
            ss << "6";
            break;
        case lSEXT:
            ss << getReg_sext(I);
            break;
        default:
            break;
    }
    std::string a = ss.str();

    errs() << "\n Returning Reg " << a.c_str() << "\n";
    return std::string(a);
//    return strdup(a.c_str());
}

std::string getReg_trunc(llvm::Instruction* I) {
    if(inReg(I)){
        return *(memoryDescriptor.find(I)->second.begin());
    }
    std::string allocReg = getReg(dyn_cast<Instruction>(I->getOperand(0)));
    addMemoryDescriptor(I, allocReg);
    addRegisterDescriptor(allocReg, I);
    return allocReg;
}

std::string getReg_sext(llvm::Instruction* I) {
    if(inReg(I)){
        return *(memoryDescriptor.find(I)->second.begin());
    }
    std::string allocReg = getReg(dyn_cast<Instruction>(I->getOperand(0)));
    addMemoryDescriptor(I, allocReg);
    addRegisterDescriptor(allocReg, I);
    return allocReg;
}

bool inReg(llvm::Value* I) { // how to make sure doesn't point to zero size vector.
    if ((memoryDescriptor.find(I) != memoryDescriptor.end())) {
        if (memoryDescriptor.find(I)->second.empty()) {
            memoryDescriptor.erase(memoryDescriptor.find(I));
        }
    }
    return (memoryDescriptor.find(I) != memoryDescriptor.end());
}

bool addMemoryDescriptor(llvm::Value* V, std::string reg) { // Should be merging into Regs
    if (memoryDescriptor.find(V) != memoryDescriptor.end()) {
        memoryDescriptor.find(V)->second.push_back(reg);
        return true;
    } else {
        std::vector<std::string> vec;
        vec.push_back(reg);
        memoryDescriptor.insert(std::pair<llvm::Value*, std::vector<std::string> >(V, vec));
        return false;
    }
    return false;
}

bool addRegisterDescriptor(std::string reg, llvm::Value* V) { // Should be merging into Instructions
    if (registorDescriptor.find(reg) != registorDescriptor.end()) {
        registorDescriptor.find(reg)->second.push_back(V);
        return true;
    } else {
        std::vector<llvm::Value*> vec;
        vec.push_back(V);
        registorDescriptor.insert(std::pair<std::string, std::vector<llvm::Value*> >(reg, vec));
        return false;
    }
    return false;
}

bool none_live_before(llvm::Value* V, std::vector<llvm::Value*> vec) {
    for (std::vector<llvm::Value*>::iterator it = vec.begin(), ie = vec.end(); it != ie; it++) {

        ValueVectorIterator Lbegin = (livenessSet.find(V)->second->beforeSet.begin()); //Should use beforeSet or AfterSet
        ValueVectorIterator Lend = (livenessSet.find(V)->second->beforeSet.end());
        ValueVectorIterator i;

        if ((i = std::find(Lbegin, Lend, *it)) != Lend) {
            return false;
            errs() << "\n none_live_before returning false \n";
        }
    }
    errs() << "\n none_live_before returning true \n";
    return true;
}
bool none_live_after(llvm::Value* V, std::vector<llvm::Value*> vec) {
    for (std::vector<llvm::Value*>::iterator it = vec.begin(), ie = vec.end(); it != ie; it++) {

        ValueVectorIterator Lbegin = (livenessSet.find(V)->second->afterSet.begin()); //Should use beforeSet or AfterSet
        ValueVectorIterator Lend = (livenessSet.find(V)->second->afterSet.end());
        ValueVectorIterator i;

        if ((i = std::find(Lbegin, Lend, *it)) != Lend) {
            return false;
            errs() << "\n none_live_after returning false \n";
        }
    }
    errs() << "\n none_live_after returning true \n";
    return true;
}
void remove_Regs(llvm::Value* V) { // Something got Stored to a memory location
    llvm::Value* V1 = dyn_cast<llvm::Instruction > (V)->getOperand(1);
    errs() << "\n Remove Regs Called on " << *V1 << "\n";
    for (RegistorDescriptorIterator b = registorDescriptor.begin(), e = registorDescriptor.end(); b != e; b++) {
        std::vector<llvm::Value*>::iterator i = std::find(b->second.begin(), b->second.end(), V1);
        if (i != b->second.end()) {
            b->second.erase(i);
        }
    }
    if(memoryDescriptor.find(V1) != memoryDescriptor.end()){
        memoryDescriptor.erase(memoryDescriptor.find(V1));
    }
    empty_Regs(V);
}

void empty_Regs(llvm::Value* V) { // Add Regs to Free Pool
    std::vector<std::string> fre;
    for (RegistorDescriptorIterator b = registorDescriptor.begin(), e = registorDescriptor.end(); b != e; b++) {
        if (none_live_before(V, b->second)) {
            //remove memory Descriptors and Remove Register Descriptors
            freeRegs.push_back(b->first);
            fre.push_back(b->first);
        }
    }
    for (RegsVectorIterator it = fre.begin(), ie = fre.end(); it != ie; it++) {
        destroy_Reg(*it);
        //registorDescriptor.erase(registorDescriptor.find(*it)); //remove memory Descriptors and Remove Register Descriptors
    }

}

void destroy_Reg(std::string reg) {
    errs() << "\n Destroying Register \n";
    std::vector<MemoryDescriptorIterator> mi;
    for (ValueVectorIterator it = registorDescriptor.find(reg)->second.begin(), ie = registorDescriptor.find(reg)->second.end(); it != ie; it++) {
        RegsVectorIterator i = std::find(memoryDescriptor.find(*it)->second.begin(), memoryDescriptor.find(*it)->second.end(), reg);
        errs() << "\n Obtained Iterator \n" << *i << "\n";
        memoryDescriptor.find(*it)->second.erase(i);
        if (memoryDescriptor.find(*it)->second.empty()) {
            mi.push_back(memoryDescriptor.find(*it));
        }
    }
    for (std::vector<MemoryDescriptorIterator>::iterator b = mi.begin(), e = mi.end(); b != e; b++) {
        memoryDescriptor.erase(*b);
    }
    registorDescriptor.erase(registorDescriptor.find(reg));
//    freeRegs.push_back(reg);
}

void destroySpilledReg(std::string reg) {
    errs() << "\n Destroying Register \n";
    std::vector<MemoryDescriptorIterator> mi;
    for (ValueVectorIterator it = registorDescriptor.find(reg)->second.begin(), ie = registorDescriptor.find(reg)->second.end(); it != ie; it++) {
        RegsVectorIterator i = std::find(memoryDescriptor.find(*it)->second.begin(), memoryDescriptor.find(*it)->second.end(), reg);
        errs() << "\n Obtained Iterator \n" << *i << "\n";
        memoryDescriptor.find(*it)->second.erase(i);
        if (memoryDescriptor.find(*it)->second.empty()) {
            mi.push_back(memoryDescriptor.find(*it));
        }
    }
    for (std::vector<MemoryDescriptorIterator>::iterator b = mi.begin(), e = mi.end(); b != e; b++) {
        memoryDescriptor.erase(*b);
    }
    registorDescriptor.erase(registorDescriptor.find(reg));
    freeRegs.push_back(reg);
//    freeRegs.push_back(reg);
}

std::string getReg_alloca(llvm::Instruction* I){
    if (inReg(I)){
        return *(memoryDescriptor.find(I)->second.begin());
    }
    else if (!freeRegs.empty()) {
        std::string allocReg = freeRegs.back();
            freeRegs.pop_back();
            addMemoryDescriptor(I, allocReg);
            addRegisterDescriptor(allocReg, I);
            printMemoryDescriptor();
            printRegisterDescriptor();
            return allocReg;
    }
    else {
            errs() << "\n Panic : No Register is Free...Need to Spill Registers \n";
//            std::string registerToSpill = select_reg_spill();
//            spilledRegs.insert(std::pair<std::string,std::vector<llvm::Value*> >(reg,registorDescriptor.find(registerToSpill)->second));
        }
}

std::string getReg_load(llvm::Instruction* I) {
    if(inReg(I)){
        return *(memoryDescriptor.find(I)->second.begin());
    }
    if (inReg(I->getOperand(0))) {
        //return reg;
        std::string allocReg = memoryDescriptor.find(I->getOperand(0))->second.at(0);
        addMemoryDescriptor(I, allocReg);
        addRegisterDescriptor(allocReg, I);
        printMemoryDescriptor();
        printRegisterDescriptor();
        return allocReg;
    } else {
        //assign regs and update mem and reg tables
        empty_Regs(I); //Use liveness Info to Empty Regs;
        // Some Register is Free
        if (!freeRegs.empty()) {
            std::string allocReg = freeRegs.back();
            freeRegs.pop_back();
            addMemoryDescriptor(I->getOperand(0), allocReg);
            addMemoryDescriptor(I, allocReg);
            addRegisterDescriptor(allocReg, I->getOperand(0));
            addRegisterDescriptor(allocReg, I);
            printMemoryDescriptor();
            printRegisterDescriptor();
            return allocReg;
        }//No Register is Free Panic :(
        else {
            errs() << "\n Panic : No Register is Free...Need to Spill Registers \n";
            //            std::string registerToSpill = select_reg_spill();
            //            spilledRegs.insert(std::pair<std::string,std::vector<llvm::Value*> >(reg,registorDescriptor.find(registerToSpill)->second));
        }
    }

}

void printMemoryDescriptor() {
    errs() << "\n Printing Memory Descriptor \n";
    for (MemoryDescriptorIterator it = memoryDescriptor.begin(), ie = memoryDescriptor.end(); it != ie; it++) {
        errs() << *(it->first) << "----> ";
        for (RegsVectorIterator i = it->second.begin(), e = it->second.end(); i != e; i++) {
            errs() << *i << " ";
        }
        errs() << "\n";
    }
}

void printRegisterDescriptor() {
    errs() << "\n Printing Register Descriptor \n";
    for (RegistorDescriptorIterator it = registorDescriptor.begin(), ie = registorDescriptor.end(); it != ie; it++) {
        errs() << it->first << "----> ";
        for (ValueVectorIterator i = it->second.begin(), e = it->second.end(); i != e; i++) {
            errs() << **i << " ";
        }
        errs() << "\n";
    }
}
std::map<llvm::Value*, int> spilledTable;
std::stringstream os;

bool isSpilled(llvm::Value* I) {
    if (spilledTable.find(I) != spilledTable.end()) {
        return true;
    }
    return false;
}

std::string select_reg_spill(llvm::Value* I) {
    std::vector<llvm::Value*> live = livenessSet.find(I)->second->beforeSet;
    std::sort(live.begin(), live.end());
    std::reverse(live.begin(), live.end());
    for (std::vector<llvm::Value*>::iterator i = live.begin(), e = live.end(); i != e; i++) {
        if (inReg(I) && !isSpilled(I)) {
            return memoryDescriptor.find(I)->second.back();
        }
    }
    return "1";
}

std::string spillSomeReg(llvm::Value* I) {
    std::string regToSpill = select_reg_spill(I);

    for (ValueVectorIterator i = registorDescriptor.find(regToSpill)->second.begin(), e = registorDescriptor.find(regToSpill)->second.end(); i != e; i++) {
        if (isa<LoadInst > (*i) || dyn_cast<AllocaInst > (*i)) {
            continue;
        } else {
            os << "  subq $8, %rsp\n";
            os << " movq " << getTypeReg(64, regToSpill) << ", (%rsp)";

            SpilledOffset = SpilledOffset + 8;
            spilledTable.insert(std::pair<llvm::Value*, int>((*i), SpilledOffset));
        }
        destroy_Reg(regToSpill);
    }
    errs() << "\n";

    return regToSpill;
}

std::string loadSpilled(llvm::Value* I) {
    if (freeRegs.empty()) {
        spillSomeReg(I);
    }
    std::string allocReg = freeRegs.back();
    freeRegs.pop_back();
    os << "  movq -" << spilledTable.find(I)->second << "(%rbp), " << getTypeReg(64, allocReg) << "\n";
    addMemoryDescriptor(I, allocReg);
    addRegisterDescriptor(allocReg, I);
    return allocReg;
}

std::string getReg_binaryop(llvm::Instruction* I) {
    if (inReg(I)) {
        return *(memoryDescriptor.find(I)->second.begin());
    }
        //    else if(isSpilled(I)){
        //        return loadSpilled(I);
        //    }
    else if (inReg(I->getOperand(0))) {
        errs() << "\n Register is in \n";
        for (RegsVectorIterator it = memoryDescriptor.find(I->getOperand(0))->second.begin(), ie = memoryDescriptor.find(I->getOperand(0))->second.end(); it != ie; it++) {
            if (!reg_live(*it, I)) {
                errs() << "\n ********* REg allocalted is *******" << (*it) << "\n";
                std::string allocReg = (*it);
                destroy_Reg(*it); // Dangerous destroying  while iterating

                freeRegs.pop_back();
                addMemoryDescriptor(I, allocReg);
                addRegisterDescriptor(allocReg, I);
                printMemoryDescriptor();
                printRegisterDescriptor();
                return allocReg;
            }
        }
    }
    empty_Regs(I);
    if (!freeRegs.empty()) {
        std::string allocReg = freeRegs.back();
        freeRegs.pop_back();
        addMemoryDescriptor(I, allocReg);
        addRegisterDescriptor(allocReg, I);
        printMemoryDescriptor();
        printRegisterDescriptor();
        return allocReg;
    } else {
        errs() << "Panic Cannot find Reg for binary ops. Need to Spill";
        return "";
    }
    return "";
}

bool reg_live(std::string reg, llvm::Value* V) {
    errs() << "\n Checking if " << reg << " is live\n" << !none_live_after(V, registorDescriptor.find(reg)->second);
    return !none_live_after(V, registorDescriptor.find(reg)->second);
}