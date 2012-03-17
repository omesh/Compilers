/* 
 * File:   Expressiontable.h
 * Author: omesh
 *
 * Created on 26 January, 2012, 12:19 PM
 */

#ifndef EXPRESSIONTABLE_H
#define	EXPRESSIONTABLE_H
#define DEBUG_TYPE "printTables"
#include "llvm/Support/Debug.h"
#include "IncHeaders.h"

using namespace llvm;

typedef std::map<Instruction*, int> ExpressionHashTable;
typedef std::pair<Instruction*, int> ExpressionHashTablePair;

void insertExpressionHashTable(Instruction*,int ValNum);

void addCommutatively(Instruction*,int ValNum);

bool existsExpressionHashTable(Instruction* I);
int getValNumExpressionTable(Instruction* I);
Instruction* getInstructionExpressionTable(Instruction* I);
void removeExpressionHavingOperand(Value* V);

void deleteExpressionTable();
void printHashTable();


#endif	/* EXPRESSIONTABLE_H */

