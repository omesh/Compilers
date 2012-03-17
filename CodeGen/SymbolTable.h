/* 
 * File:   SymbolTable.h
 * Author: veri
 *
 * Created on 12 February, 2012, 11:46 PM
 */

#ifndef SYMBOLTABLE_H
#define	SYMBOLTABLE_H
#include "llvm/Support/raw_ostream.h"
#include "llvm/Instruction.h"
#include "Matcher.h"
#include <map>
#include <string>
#include "algorithm"
#include <vector>
#define debug(x) x;

#define     lADD       Instruction::Add
#define     lFADD      Instruction::FAdd
#define     lSUB       Instruction::Sub
#define     lFSUB      Instruction::FSub
#define     lMUL       Instruction::Mul
#define     lFMUL      Instruction::FMul
#define     lUDIV      Instruction::UDiv
#define     lSDIV      Instruction::SDiv
#define     lFDIV      Instruction::FDiv
#define     lUREM      Instruction::URem
#define     lSREM      Instruction::SRem
#define     lFREM      Instruction::FRem
#define     lSHL       Instruction::Shl
#define     lLSHR      Instruction::LShr
#define     lASHR      Instruction::AShr
#define     lAND       Instruction::And
#define     lOR        Instruction::Or
#define     lXOR       Instruction::Xor
#define     lTRUNC     Instruction::Trunc
#define     lZEXT      Instruction::ZExt
#define     lSEXT      Instruction::SExt
#define     lFPTOUI    Instruction::FPToUI
#define     lFPTOSI    Instruction::FPToSI
#define     lUITOFP    Instruction::UIToFP
#define     lSITOFP    Instruction::SIToFP
#define     lFPTRUNC   Instruction::FPTrunc
#define     lFPEXT     Instruction::FPExt
#define     lPTRTOINT  Instruction::PtrToInt
#define     lINTTOPTR  Instruction::IntToPtr
#define     lBITCAST   Instruction::BitCast
#define     lSTORE     Instruction::Store
#define     lLOAD      Instruction::Load
#define     lALLOCA    Instruction::Alloca
#define     lGEP       Instruction::GetElementPtr
#define     lSEXT      Instruction::SExt
#define     lCALL      Instruction::Call
#define     lTRUNC     Instruction::Trunc
#define     lRET       Instruction::Ret
#define     lCmp       Instruction::ICmp
#define     lBR        Instruction::Br
#define     lSwitch    Instruction::Switch
#define     lCNSTI     21
#define     lMEM       111

struct variableInfo {
    int offset;
    int size;
    std::vector<std::string>  Regs;
};

struct livenessInfo{
    std::vector<llvm::Value*> beforeSet;
    std::vector<llvm::Value*> afterSet;
};



typedef std::map<int, variableInfo*> SymbolTable;
typedef std::pair<int, variableInfo*> SymbolTableEntry;

typedef std::map<llvm::Value*,livenessInfo*> liveSet;
typedef std::pair<llvm::Value*,livenessInfo*> liveSetPair;

bool addBeforeSet(llvm::Instruction*,std::vector<llvm::Value*>);
bool addAfterSet(llvm::Instruction*,std::vector<llvm::Value*>);
void printLivenessInfo();


void printSymbolTable();
int getSize(int i);
int getOffset(int i);
int getArgOffset(int);
const char* getArgReg(int i,int size);
const char* getTypeRegInst(unsigned i,llvm::Value* I);
std::string getReg_alloca(llvm::Instruction* I);
std::string getTypeReg(unsigned, std::string);
const char* getReg8(llvm::Value* I);
const char* getReg16(llvm::Value* I);
const char* getReg32(llvm::Value* I);
const char* getReg64(llvm::Value* I);
std::string getReg(llvm::Instruction*);
std::string getReg_trunc(llvm::Instruction* I);
std::string getReg_sext(llvm::Instruction* I);
std::string getReg_load(llvm::Instruction* I);
std::string getReg_binaryop(llvm::Instruction* I);
void populateRegs();
void remove_Regs(llvm::Value* V);
void empty_Regs(llvm::Value* V);
bool reg_live(std::string reg, llvm::Value* V);
void destroy_Reg(std::string reg);
void printMemoryDescriptor();
void printRegisterDescriptor();
bool inReg(llvm::Value* I);
std::string select_reg_spill();
bool addMemoryDescriptor(llvm::Value*, std::string);
bool addRegisterDescriptor(std::string, llvm::Value*);
void setSpilledOffset(int);
#endif	/* SYMBOLTABLE_H */

