#include "NameTable.h"


NameTable nameTable;

void removeFromNameTableList(int ValNum, Value* V){   // Given a ValNum removes Value From the NameList of that ValNum
    
    NameTable::iterator it = nameTable.find(ValNum);
    if(it != nameTable.end()){
        NameList::iterator i;
        if((i = std::find(it->second->nameList.begin(),it->second->nameList.end(),V)) != it->second->nameList.end()){
            debug(errs() << "Removing from nametablelist"<< ValNum <<"\n");
            it->second->nameList.erase(i);
            if(it->second->nameList.empty()){
                nameTable.erase(nameTable.find(ValNum));
            }
        }
    }
}

NameList* getNameListNameTable(int ValNum){  // Get the NameList For a ValNum
    return &(nameTable.find(ValNum)->second->nameList);
}

nameTableEntry* getNameTableEntry(int ValNum){ //Get the NameTable Entry For a ValNum
    return nameTable.find(ValNum)->second;
}


bool isConstantNametable(int ValNum){  // Check if a valnum is mapped to Constant.
    if(!existsNameTable(ValNum)) return false;
    return getNameTableEntry(ValNum)->constFlag;
}

Value* getValueNameTable(int ValNum){ // get the Value for a ValNum. Change to return Null if Not Present.
    return getNameTableEntry(ValNum)->constantValue;
}

bool existsNameTable(int ValNum){ // Check If ValNum exists in NameTable
    return nameTable.find(ValNum) != nameTable.end();
}

void insertNameTable(int ValNum, Value* V1, Value* V2, bool constFlag){ // Insert a New Mapping In NameTable
    if(nameTable.find(ValNum) != nameTable.end()){
        nameTable.find(ValNum)->second->nameList.push_back(V1);
    }
    else{
        struct nameTableEntry* ntE = new nameTableEntry;
        ntE->constFlag = constFlag;
        ntE->constantValue = V2;
        ntE->nameList.push_back(V1);
        nameTable.insert(std::pair<int, nameTableEntry*>(ValNum, ntE));
    }
}

void deleteNameTable(){
    nameTable.clear();
}

void printNameTable() {
    debug(errs() << "***** Printing contents of NameTable ");
    debug(errs() << " (ValueNumber  NameList  ConstantValue    ConstantFlag) \n");
    for (NameTable::iterator i = nameTable.begin(), e = nameTable.end(); i != e; i++) {
        debug(errs() << i->first << "  ");
        for (std::vector<llvm::Value*>::iterator iv = i->second->nameList.begin(), ev = i->second->nameList.end(); iv != ev; iv++) {
            debug(errs() << **(iv) << ",");
        }
        debug(errs() << " ");
        if (i->second->constFlag) {
            debug(errs() << (dyn_cast<llvm::ConstantInt>) (i->second->constantValue)->getSExtValue() << "  ");
        } else {
            debug(errs() << "-  ");
        }
        debug(errs() << i->second->constFlag << "\n");

    }
    debug(errs() << "***** Finished Printing contents of NameTable \n");
}