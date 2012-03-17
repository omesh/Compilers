/* 
 * File:   ValNumTable.h
 * Author: omesh
 *
 * Created on 26 January, 2012, 12:20 PM
 */

#ifndef VALNUMTABLE_H
#define	VALNUMTABLE_H
#define DEBUG_TYPE "printTables"
#include "llvm/Support/Debug.h"
#include "IncHeaders.h"
using namespace llvm;


typedef std::map<Value*, int> ValNumTable;


typedef std::pair<Value*, int> ValNumTablePair;

void updateValNum(Value*,int ValNum);

void removeValNum(Value*);
bool existsValNum(Value* V);
int getValNum(Value*);
void deleteValNumTable();
void insertValNum(Value*, int);
void removeArrayRefs(Instruction* I);
void printValNumTable() ;


#endif	/* VALNUMTABLE_H */

