/* 
 * File:   ValueNumbering.h
 * Author: omesh
 *
 * Created on 26 January, 2012, 12:39 PM
 */

#ifndef VALUENUMBERING_H
#define	VALUENUMBERING_H

#include "IncHeaders.h"
#include "llvm/Instruction.h"
#include "llvm/Instruction.def"
#include "llvm/Constant.h"

#define      ADD       Instruction::Add
#define      FADD      Instruction::FAdd
#define      SUB       Instruction::Sub
#define      FSUB      Instruction::FSub
#define      MUL       Instruction::Mul
#define      FMUL      Instruction::FMul
#define      UDIV      Instruction::UDiv
#define      SDIV      Instruction::SDiv
#define      FDIV      Instruction::FDiv
#define      UREM      Instruction::URem
#define      SREM      Instruction::SRem
#define      FREM      Instruction::FRem
#define      SHL       Instruction::Shl
#define      LSHR      Instruction::LShr
#define      ASHR      Instruction::AShr
#define      AND       Instruction::And
#define      OR        Instruction::Or
#define      XOR       Instruction::Xor
#define      TRUNC     Instruction::Trunc
#define      ZEXT      Instruction::ZExt
#define      SEXT      Instruction::SExt
#define      FPTOUI    Instruction::FPToUI
#define      FPTOSI    Instruction::FPToSI
#define      UITOFP    Instruction::UIToFP
#define      SITOFP    Instruction::SIToFP
#define      FPTRUNC   Instruction::FPTrunc
#define      FPEXT     Instruction::FPExt
#define      PTRTOINT  Instruction::PtrToInt
#define      INTTOPTR  Instruction::IntToPtr
#define      BITCAST   Instruction::BitCast
#define      STORE     Instruction::Store
#define      LOAD      Instruction::Load
#define      ALLOCA    Instruction::Alloca
#define      GEP       Instruction::GetElementPtr
#define      SEXT      Instruction::SExt
#define      CALL      Instruction::Call

#endif	/* VALUENUMBERING_H */

