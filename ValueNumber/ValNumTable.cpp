#include "ValNumTable.h"

ValNumTable valNumTable;

void updateValNum(Value* V,int ValNum){  // Remove Previous Value Number Mapping and Insert New One.
    if(valNumTable.find(V) != valNumTable.end())
        valNumTable.erase(valNumTable.find(V));
    valNumTable.insert(ValNumTablePair(V,ValNum));
}

void removeValNum(Value* V){       // Remove Previous Value Number Mapping.
    if(valNumTable.find(V) != valNumTable.end())
        valNumTable.erase(valNumTable.find(V));
}

int getValNum(Value* V){  // Return the Value Number. //change this to return NULL if ValNum not Present
    return valNumTable.find(V)->second;
}

void insertValNum(Value* V, int ValNum){ // Insert Value Number Without deleting previous one // Shoudn't be Used 
    valNumTable.insert(ValNumTablePair(V,ValNum));
}

bool existsValNum(Value* V){  // Check weather a Value Number Exists or not.
    return valNumTable.find(V) != valNumTable.end();
}

void deleteValNumTable(){
    valNumTable.clear();
}

void removeArrayRefs(Instruction* I){ // Remove all Array Memory Valnum Mappings for a particualar Array.
    std::vector<Value*> delRefs;
    for (ValNumTable::iterator i = valNumTable.begin(), e = valNumTable.end(); i != e; i++) {
        if(Instruction* begin = dyn_cast<Instruction>(i->first)){
            if(I->getOpcode() == begin->getOpcode()){
                if((I->getOperand(0) == (begin)->getOperand(0))){
                    delRefs.push_back(begin);
                }
            }
        }
    }
    for(std::vector<Value*>::iterator i = delRefs.begin(), e = delRefs.end() ; i != e ; i++){
        debug(errs() << "*** Removing From ValNum Tables ****" << *i << "\n");
        removeValNum(*i);
    }
}

void printValNumTable() {
    debug(errs() << "***** Printing contents of valNumTable  ");
    debug(errs() << " (Value    Value Number) \n");
    for (ValNumTable::iterator i = valNumTable.begin(), e = valNumTable.end(); i != e; i++) {
        debug(errs() << *(i->first) << " ");
        debug(errs() << i->second << "\n");
    }
    debug(errs() << "***** Finished Printing contents of valNumTable \n");
}