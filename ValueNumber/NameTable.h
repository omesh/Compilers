/* 
 * File:   NameTable.h
 * Author: omesh
 *
 * Created on 26 January, 2012, 12:17 PM
 */

#ifndef NAMETABLE_H
#define	NAMETABLE_H
#define DEBUG_TYPE "printTables"
#include "llvm/Support/Debug.h"
#include "IncHeaders.h"

using namespace llvm;

struct nameTableEntry {
    std::vector<llvm::Value*> nameList;
    llvm::Value* constantValue;
    bool constFlag;
};


typedef std::map<int,nameTableEntry*> NameTable;
typedef std::vector<llvm::Value*> NameList;



void removeFromNameTableList(int ValNum, Value* V);
void insertNameTable(int , Value* , Value* =NULL, bool =false);
bool existsNameTable(int );
NameList* getNameListNameTable(int ValNum);
Value* getValueNameTable(int ValNum);
bool isConstantNametable(int ValNum);
nameTableEntry* getNameTableEntry(int ValNum);
void deleteNameTable();
void printNameTable();


#endif	/* NAMETABLE_H */

