#define __OLIVE_HEADER_INCLUDED__
# line 1 "x86.brg"
    /*
       FILE: sample4.brg
  
       Copyright (c) 1997 Princeton University

       All rights reserved.

       This software is to be used for non-commercial purposes only,
       unless authorized permission to do otherwise is obtained.  
       For more information, contact spam@ee.princeton.edu
     */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "llvm/Instruction.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <string.h>

    enum {
        ADDI = 309, ADDRLP = 295, ASGNI = 53,
        CNSTI = 128, CVCI = 85, I0I = 661, INDIRC = 67, STORE = 28, MEM = 111, ADD = 8, SUB = 9, AND = 10, OR = 11, XOR = 12, MUL = 13, LOAD = 27,
        MEM8 = 112, MEM16 = 113, MEM32 = 114, MEM64 = 115, GLOB8 = 116, GLOB16 = 117, GLOB32 = 118, GLOB64 = 119, TRUNC8=120, TRUNC16=121, TRUNC32=122, TRUNC64=123,SEXT8=124, SEXT16=125, SEXT32=126,  
        SEXT64=127, RET8=129, RET16=130, RET32=131, RET64=132, ARG8=133, ARG16=134, ARG32=135, ARG64=136,OFFSET=137, GEP=138
    };

    char *word_length[] = {
        /* 0 */ "Zero Sized.",
        /* 1 */ "movc",
        /* 2 */ "movb",
        /* 3 */ "3 bytes cannot happen",
        /* 4 */ "movl",
        /* 5 */ "5 bit move ",
        /* 6 */ "6 bit move ",
        /* 7 */ "7 bit move ",
        /* 8 */ "movq",
    };

    FILE* fp;

    typedef struct tree {
        int op;
        struct tree * kids[2];
        int val;
        const char* name;
        llvm::Instruction* inst;

        struct {
            struct burm_state *state;
        } x;
    } *NODEPTR, *Tree;
#define GET_KIDS(p)	((p)->kids)
#define PANIC printf
#define STATE_LABEL(p) ((p)->x.state)
#define SET_STATE(p,s) (p)->x.state=(s)
#define DEFAULT_COST	break
#define NO_ACTION(x)

    typedef struct COST {
        int cost;
    } COST;
#define COST_LESS(a,b) ((a).cost < (b).cost)

    static COST COST_INFINITY = {32767};
    static COST COST_ZERO = {0};

    /*static char* burm_string = "FOO";*/
    static int _ern = 0;

    int OP_LABEL(NODEPTR p) {
        switch (p->op) {
            case CNSTI: if (p->val == 0) return 661 /* I0I */;
            default: return p->op;
        }
    }


    static void burm_trace(NODEPTR, int, int, int);
#define BURP 0
#define ADDI 1
#define ADDRLP 2
#define ASGNI 3
#define STORE 4
#define ADD 5
#define SUB 6
#define AND 7
#define OR 8
#define XOR 9
#define MUL 10
#define LOAD 11
#define CNSTI 12
#define CVCI 13
#define I0I 14
#define INDIRC 15
#define MEM8 16
#define MEM16 17
#define MEM32 18
#define MEM64 19
#define GLOB8 20
#define GLOB16 21
#define GLOB32 22
#define GLOB64 23
#define TRUNC8 24
#define TRUNC16 25
#define TRUNC32 26
#define TRUNC64 27
#define SEXT8 28
#define SEXT16 29
#define SEXT32 30
#define SEXT64 31
#define RET8 32
#define RET16 33
#define RET32 34
#define RET64 35
#define ARG8 36
#define ARG16 37
#define ARG32 38
#define ARG64 39
#define OFFSET 40
#define GEP 41
#define SHL 42
#define LSHR 43
#define ASHR 44
#define UDIV 45

struct burm_state {
  int op;
  NODEPTR node;
  struct burm_state **kids;
  COST cost[11];
  struct {
    unsigned burm_stmt:5;
    unsigned burm_ireg:3;
    unsigned burm_ireg8:5;
    unsigned burm_ireg16:5;
    unsigned burm_ireg32:5;
    unsigned burm_ireg64:5;
    unsigned burm_imem8:2;
    unsigned burm_imem16:2;
    unsigned burm_imem32:2;
    unsigned burm_imem64:2;
  } rule;
};

#ifndef ALLOC
#define ALLOC(n) malloc(n)
#endif

#ifndef burm_assert
#define burm_assert(x,y) if (!(x)) {  y; abort(); }
#endif

#define burm_stmt_NT 1
#define burm_ireg_NT 2
#define burm_ireg8_NT 3
#define burm_ireg16_NT 4
#define burm_ireg32_NT 5
#define burm_ireg64_NT 6
#define burm_imem8_NT 7
#define burm_imem16_NT 8
#define burm_imem32_NT 9
#define burm_imem64_NT 10
extern int burm_max_nt;
int burm_max_nt = 10;

char *burm_ntname[] = {
  0,
  "stmt",
  "ireg",
  "ireg8",
  "ireg16",
  "ireg32",
  "ireg64",
  "imem8",
  "imem16",
  "imem32",
  "imem64",
  0
};

static short burm_nts_0[] = { burm_ireg8_NT, 0 };
static short burm_nts_1[] = { burm_ireg16_NT, 0 };
static short burm_nts_2[] = { burm_ireg32_NT, 0 };
static short burm_nts_3[] = { burm_ireg64_NT, 0 };
static short burm_nts_4[] = { 0 };
static short burm_nts_5[] = { burm_ireg64_NT, burm_ireg8_NT, 0 };
static short burm_nts_6[] = { burm_ireg64_NT, burm_ireg16_NT, 0 };
static short burm_nts_7[] = { burm_ireg64_NT, burm_ireg32_NT, 0 };
static short burm_nts_8[] = { burm_ireg64_NT, burm_ireg64_NT, 0 };
static short burm_nts_9[] = { burm_imem8_NT, burm_ireg8_NT, 0 };
static short burm_nts_10[] = { burm_imem16_NT, burm_ireg16_NT, 0 };
static short burm_nts_11[] = { burm_imem32_NT, burm_ireg32_NT, 0 };
static short burm_nts_12[] = { burm_imem64_NT, burm_ireg64_NT, 0 };
static short burm_nts_13[] = { burm_ireg8_NT, burm_ireg8_NT, 0 };
static short burm_nts_14[] = { burm_ireg16_NT, burm_ireg16_NT, 0 };
static short burm_nts_15[] = { burm_ireg32_NT, burm_ireg32_NT, 0 };
static short burm_nts_16[] = { burm_ireg8_NT, burm_imem8_NT, 0 };
static short burm_nts_17[] = { burm_ireg16_NT, burm_imem16_NT, 0 };
static short burm_nts_18[] = { burm_ireg32_NT, burm_imem32_NT, 0 };
static short burm_nts_19[] = { burm_ireg64_NT, burm_imem64_NT, 0 };
static short burm_nts_20[] = { burm_ireg_NT, 0 };

short *burm_nts[] = {
  burm_nts_0,  /* 0 */
  burm_nts_1,  /* 1 */
  burm_nts_2,  /* 2 */
  burm_nts_3,  /* 3 */
  burm_nts_4,  /* 4 */
  burm_nts_4,  /* 5 */
  burm_nts_4,  /* 6 */
  burm_nts_4,  /* 7 */
  burm_nts_3,  /* 8 */
  burm_nts_4,  /* 9 */
  burm_nts_4,  /* 10 */
  burm_nts_4,  /* 11 */
  burm_nts_4,  /* 12 */
  burm_nts_5,  /* 13 */
  burm_nts_6,  /* 14 */
  burm_nts_7,  /* 15 */
  burm_nts_8,  /* 16 */
  burm_nts_0,  /* 17 */
  burm_nts_1,  /* 18 */
  burm_nts_2,  /* 19 */
  burm_nts_3,  /* 20 */
  burm_nts_3,  /* 21 */
  burm_nts_9,  /* 22 */
  burm_nts_10,  /* 23 */
  burm_nts_11,  /* 24 */
  burm_nts_12,  /* 25 */
  burm_nts_4,  /* 26 */
  burm_nts_4,  /* 27 */
  burm_nts_4,  /* 28 */
  burm_nts_4,  /* 29 */
  burm_nts_13,  /* 30 */
  burm_nts_14,  /* 31 */
  burm_nts_15,  /* 32 */
  burm_nts_8,  /* 33 */
  burm_nts_16,  /* 34 */
  burm_nts_17,  /* 35 */
  burm_nts_18,  /* 36 */
  burm_nts_19,  /* 37 */
  burm_nts_0,  /* 38 */
  burm_nts_1,  /* 39 */
  burm_nts_2,  /* 40 */
  burm_nts_3,  /* 41 */
  burm_nts_13,  /* 42 */
  burm_nts_14,  /* 43 */
  burm_nts_15,  /* 44 */
  burm_nts_8,  /* 45 */
  burm_nts_16,  /* 46 */
  burm_nts_17,  /* 47 */
  burm_nts_18,  /* 48 */
  burm_nts_19,  /* 49 */
  burm_nts_0,  /* 50 */
  burm_nts_1,  /* 51 */
  burm_nts_2,  /* 52 */
  burm_nts_3,  /* 53 */
  burm_nts_13,  /* 54 */
  burm_nts_14,  /* 55 */
  burm_nts_15,  /* 56 */
  burm_nts_8,  /* 57 */
  burm_nts_16,  /* 58 */
  burm_nts_17,  /* 59 */
  burm_nts_18,  /* 60 */
  burm_nts_19,  /* 61 */
  burm_nts_0,  /* 62 */
  burm_nts_1,  /* 63 */
  burm_nts_2,  /* 64 */
  burm_nts_3,  /* 65 */
  burm_nts_13,  /* 66 */
  burm_nts_14,  /* 67 */
  burm_nts_15,  /* 68 */
  burm_nts_8,  /* 69 */
  burm_nts_16,  /* 70 */
  burm_nts_17,  /* 71 */
  burm_nts_18,  /* 72 */
  burm_nts_19,  /* 73 */
  burm_nts_0,  /* 74 */
  burm_nts_1,  /* 75 */
  burm_nts_2,  /* 76 */
  burm_nts_3,  /* 77 */
  burm_nts_13,  /* 78 */
  burm_nts_14,  /* 79 */
  burm_nts_15,  /* 80 */
  burm_nts_8,  /* 81 */
  burm_nts_16,  /* 82 */
  burm_nts_17,  /* 83 */
  burm_nts_18,  /* 84 */
  burm_nts_19,  /* 85 */
  burm_nts_0,  /* 86 */
  burm_nts_1,  /* 87 */
  burm_nts_2,  /* 88 */
  burm_nts_3,  /* 89 */
  burm_nts_13,  /* 90 */
  burm_nts_14,  /* 91 */
  burm_nts_15,  /* 92 */
  burm_nts_8,  /* 93 */
  burm_nts_16,  /* 94 */
  burm_nts_17,  /* 95 */
  burm_nts_18,  /* 96 */
  burm_nts_19,  /* 97 */
  burm_nts_0,  /* 98 */
  burm_nts_1,  /* 99 */
  burm_nts_2,  /* 100 */
  burm_nts_3,  /* 101 */
  burm_nts_13,  /* 102 */
  burm_nts_14,  /* 103 */
  burm_nts_15,  /* 104 */
  burm_nts_8,  /* 105 */
  burm_nts_16,  /* 106 */
  burm_nts_17,  /* 107 */
  burm_nts_18,  /* 108 */
  burm_nts_19,  /* 109 */
  burm_nts_0,  /* 110 */
  burm_nts_1,  /* 111 */
  burm_nts_2,  /* 112 */
  burm_nts_3,  /* 113 */
  burm_nts_13,  /* 114 */
  burm_nts_14,  /* 115 */
  burm_nts_15,  /* 116 */
  burm_nts_8,  /* 117 */
  burm_nts_16,  /* 118 */
  burm_nts_17,  /* 119 */
  burm_nts_18,  /* 120 */
  burm_nts_19,  /* 121 */
  burm_nts_0,  /* 122 */
  burm_nts_1,  /* 123 */
  burm_nts_2,  /* 124 */
  burm_nts_3,  /* 125 */
  burm_nts_20,  /* 126 */
  burm_nts_20,  /* 127 */
  burm_nts_20,  /* 128 */
  burm_nts_20,  /* 129 */
  burm_nts_4,  /* 130 */
  burm_nts_4,  /* 131 */
  burm_nts_4,  /* 132 */
  burm_nts_4,  /* 133 */
  burm_nts_20,  /* 134 */
  burm_nts_20,  /* 135 */
  burm_nts_20,  /* 136 */
  burm_nts_20,  /* 137 */
  burm_nts_4,  /* 138 */
  burm_nts_4,  /* 139 */
  burm_nts_4,  /* 140 */
  burm_nts_4,  /* 141 */
  burm_nts_4,  /* 142 */
  burm_nts_4,  /* 143 */
  burm_nts_4,  /* 144 */
  burm_nts_4,  /* 145 */
  burm_nts_4,  /* 146 */
  burm_nts_4,  /* 147 */
  burm_nts_4,  /* 148 */
  burm_nts_4,  /* 149 */
  burm_nts_3,  /* 150 */
  burm_nts_4,  /* 151 */
  burm_nts_4,  /* 152 */
  burm_nts_4,  /* 153 */
  burm_nts_4,  /* 154 */
  burm_nts_0,  /* 155 */
  burm_nts_1,  /* 156 */
  burm_nts_2,  /* 157 */
  burm_nts_3,  /* 158 */
};

char burm_arity[] = {
  0,  /* 0=BURP */
  0,  /* 1=ADDI */
  0,  /* 2=ADDRLP */
  0,  /* 3=ASGNI */
  2,  /* 4=STORE */
  2,  /* 5=ADD */
  2,  /* 6=SUB */
  0,  /* 7=AND */
  2,  /* 8=OR */
  2,  /* 9=XOR */
  2,  /* 10=MUL */
  1,  /* 11=LOAD */
  0,  /* 12=CNSTI */
  0,  /* 13=CVCI */
  0,  /* 14=I0I */
  0,  /* 15=INDIRC */
  0,  /* 16=MEM8 */
  0,  /* 17=MEM16 */
  0,  /* 18=MEM32 */
  0,  /* 19=MEM64 */
  0,  /* 20=GLOB8 */
  0,  /* 21=GLOB16 */
  0,  /* 22=GLOB32 */
  0,  /* 23=GLOB64 */
  1,  /* 24=TRUNC8 */
  1,  /* 25=TRUNC16 */
  1,  /* 26=TRUNC32 */
  1,  /* 27=TRUNC64 */
  1,  /* 28=SEXT8 */
  1,  /* 29=SEXT16 */
  1,  /* 30=SEXT32 */
  1,  /* 31=SEXT64 */
  0,  /* 32=RET8 */
  0,  /* 33=RET16 */
  0,  /* 34=RET32 */
  0,  /* 35=RET64 */
  0,  /* 36=ARG8 */
  0,  /* 37=ARG16 */
  0,  /* 38=ARG32 */
  0,  /* 39=ARG64 */
  0,  /* 40=OFFSET */
  0,  /* 41=GEP */
  2,  /* 42=SHL */
  2,  /* 43=LSHR */
  2,  /* 44=ASHR */
  0,  /* 45=UDIV */
};

char *burm_opname[] = {
  /* 0 */  "BURP",
  /* 1 */  "ADDI",
  /* 2 */  "ADDRLP",
  /* 3 */  "ASGNI",
  /* 4 */  "STORE",
  /* 5 */  "ADD",
  /* 6 */  "SUB",
  /* 7 */  "AND",
  /* 8 */  "OR",
  /* 9 */  "XOR",
  /* 10 */  "MUL",
  /* 11 */  "LOAD",
  /* 12 */  "CNSTI",
  /* 13 */  "CVCI",
  /* 14 */  "I0I",
  /* 15 */  "INDIRC",
  /* 16 */  "MEM8",
  /* 17 */  "MEM16",
  /* 18 */  "MEM32",
  /* 19 */  "MEM64",
  /* 20 */  "GLOB8",
  /* 21 */  "GLOB16",
  /* 22 */  "GLOB32",
  /* 23 */  "GLOB64",
  /* 24 */  "TRUNC8",
  /* 25 */  "TRUNC16",
  /* 26 */  "TRUNC32",
  /* 27 */  "TRUNC64",
  /* 28 */  "SEXT8",
  /* 29 */  "SEXT16",
  /* 30 */  "SEXT32",
  /* 31 */  "SEXT64",
  /* 32 */  "RET8",
  /* 33 */  "RET16",
  /* 34 */  "RET32",
  /* 35 */  "RET64",
  /* 36 */  "ARG8",
  /* 37 */  "ARG16",
  /* 38 */  "ARG32",
  /* 39 */  "ARG64",
  /* 40 */  "OFFSET",
  /* 41 */  "GEP",
  /* 42 */  "SHL",
  /* 43 */  "LSHR",
  /* 44 */  "ASHR",
  /* 45 */  "UDIV",
};


char *burm_string[] = {
  /* 0 */  "stmt: ireg8",
  /* 1 */  "stmt: ireg16",
  /* 2 */  "stmt: ireg32",
  /* 3 */  "stmt: ireg64",
  /* 4 */  "stmt: STORE(MEM8,CNSTI)",
  /* 5 */  "stmt: STORE(MEM16,CNSTI)",
  /* 6 */  "stmt: STORE(MEM32,CNSTI)",
  /* 7 */  "stmt: STORE(MEM64,CNSTI)",
  /* 8 */  "stmt: STORE(ireg64,CNSTI)",
  /* 9 */  "stmt: STORE(MEM8,ARG8)",
  /* 10 */  "stmt: STORE(MEM16,ARG16)",
  /* 11 */  "stmt: STORE(MEM32,ARG32)",
  /* 12 */  "stmt: STORE(MEM64,ARG64)",
  /* 13 */  "stmt: STORE(ireg64,ireg8)",
  /* 14 */  "stmt: STORE(ireg64,ireg16)",
  /* 15 */  "stmt: STORE(ireg64,ireg32)",
  /* 16 */  "stmt: STORE(ireg64,ireg64)",
  /* 17 */  "stmt: STORE(MEM8,ireg8)",
  /* 18 */  "stmt: STORE(MEM16,ireg16)",
  /* 19 */  "stmt: STORE(MEM32,ireg32)",
  /* 20 */  "stmt: STORE(MEM32,ireg64)",
  /* 21 */  "stmt: STORE(MEM64,ireg64)",
  /* 22 */  "stmt: STORE(imem8,ireg8)",
  /* 23 */  "stmt: STORE(imem16,ireg16)",
  /* 24 */  "stmt: STORE(imem32,ireg32)",
  /* 25 */  "stmt: STORE(imem64,ireg64)",
  /* 26 */  "stmt: STORE(GLOB8,CNSTI)",
  /* 27 */  "stmt: STORE(GLOB16,CNSTI)",
  /* 28 */  "stmt: STORE(GLOB32,CNSTI)",
  /* 29 */  "stmt: STORE(GLOB64,CNSTI)",
  /* 30 */  "ireg8: ADD(ireg8,ireg8)",
  /* 31 */  "ireg16: ADD(ireg16,ireg16)",
  /* 32 */  "ireg32: ADD(ireg32,ireg32)",
  /* 33 */  "ireg64: ADD(ireg64,ireg64)",
  /* 34 */  "ireg8: ADD(ireg8,imem8)",
  /* 35 */  "ireg16: ADD(ireg16,imem16)",
  /* 36 */  "ireg32: ADD(ireg32,imem32)",
  /* 37 */  "ireg64: ADD(ireg64,imem64)",
  /* 38 */  "ireg8: ADD(ireg8,CNSTI)",
  /* 39 */  "ireg16: ADD(ireg16,CNSTI)",
  /* 40 */  "ireg32: ADD(ireg32,CNSTI)",
  /* 41 */  "ireg64: ADD(ireg64,CNSTI)",
  /* 42 */  "ireg8: SUB(ireg8,ireg8)",
  /* 43 */  "ireg16: SUB(ireg16,ireg16)",
  /* 44 */  "ireg32: SUB(ireg32,ireg32)",
  /* 45 */  "ireg64: SUB(ireg64,ireg64)",
  /* 46 */  "ireg8: SUB(ireg8,imem8)",
  /* 47 */  "ireg16: SUB(ireg16,imem16)",
  /* 48 */  "ireg32: SUB(ireg32,imem32)",
  /* 49 */  "ireg64: SUB(ireg64,imem64)",
  /* 50 */  "ireg8: SUB(ireg8,CNSTI)",
  /* 51 */  "ireg16: SUB(ireg16,CNSTI)",
  /* 52 */  "ireg32: SUB(ireg32,CNSTI)",
  /* 53 */  "ireg64: SUB(ireg64,CNSTI)",
  /* 54 */  "ireg8: OR(ireg8,ireg8)",
  /* 55 */  "ireg16: OR(ireg16,ireg16)",
  /* 56 */  "ireg32: OR(ireg32,ireg32)",
  /* 57 */  "ireg64: OR(ireg64,ireg64)",
  /* 58 */  "ireg8: OR(ireg8,imem8)",
  /* 59 */  "ireg16: OR(ireg16,imem16)",
  /* 60 */  "ireg32: OR(ireg32,imem32)",
  /* 61 */  "ireg64: OR(ireg64,imem64)",
  /* 62 */  "ireg8: OR(ireg8,CNSTI)",
  /* 63 */  "ireg16: OR(ireg16,CNSTI)",
  /* 64 */  "ireg32: OR(ireg32,CNSTI)",
  /* 65 */  "ireg64: OR(ireg64,CNSTI)",
  /* 66 */  "ireg8: XOR(ireg8,ireg8)",
  /* 67 */  "ireg16: XOR(ireg16,ireg16)",
  /* 68 */  "ireg32: XOR(ireg32,ireg32)",
  /* 69 */  "ireg64: XOR(ireg64,ireg64)",
  /* 70 */  "ireg8: XOR(ireg8,imem8)",
  /* 71 */  "ireg16: XOR(ireg16,imem16)",
  /* 72 */  "ireg32: XOR(ireg32,imem32)",
  /* 73 */  "ireg64: XOR(ireg64,imem64)",
  /* 74 */  "ireg8: XOR(ireg8,CNSTI)",
  /* 75 */  "ireg16: XOR(ireg16,CNSTI)",
  /* 76 */  "ireg32: XOR(ireg32,CNSTI)",
  /* 77 */  "ireg64: XOR(ireg64,CNSTI)",
  /* 78 */  "ireg8: MUL(ireg8,ireg8)",
  /* 79 */  "ireg16: MUL(ireg16,ireg16)",
  /* 80 */  "ireg32: MUL(ireg32,ireg32)",
  /* 81 */  "ireg64: MUL(ireg64,ireg64)",
  /* 82 */  "ireg8: MUL(ireg8,imem8)",
  /* 83 */  "ireg16: MUL(ireg16,imem16)",
  /* 84 */  "ireg32: MUL(ireg32,imem32)",
  /* 85 */  "ireg64: MUL(ireg64,imem64)",
  /* 86 */  "ireg8: MUL(ireg8,CNSTI)",
  /* 87 */  "ireg16: MUL(ireg16,CNSTI)",
  /* 88 */  "ireg32: MUL(ireg32,CNSTI)",
  /* 89 */  "ireg64: MUL(ireg64,CNSTI)",
  /* 90 */  "ireg8: SHL(ireg8,ireg8)",
  /* 91 */  "ireg16: SHL(ireg16,ireg16)",
  /* 92 */  "ireg32: SHL(ireg32,ireg32)",
  /* 93 */  "ireg64: SHL(ireg64,ireg64)",
  /* 94 */  "ireg8: SHL(ireg8,imem8)",
  /* 95 */  "ireg16: SHL(ireg16,imem16)",
  /* 96 */  "ireg32: SHL(ireg32,imem32)",
  /* 97 */  "ireg64: SHL(ireg64,imem64)",
  /* 98 */  "ireg8: SHL(ireg8,CNSTI)",
  /* 99 */  "ireg16: SHL(ireg16,CNSTI)",
  /* 100 */  "ireg32: SHL(ireg32,CNSTI)",
  /* 101 */  "ireg64: SHL(ireg64,CNSTI)",
  /* 102 */  "ireg8: LSHR(ireg8,ireg8)",
  /* 103 */  "ireg16: LSHR(ireg16,ireg16)",
  /* 104 */  "ireg32: LSHR(ireg32,ireg32)",
  /* 105 */  "ireg64: LSHR(ireg64,ireg64)",
  /* 106 */  "ireg8: LSHR(ireg8,imem8)",
  /* 107 */  "ireg16: LSHR(ireg16,imem16)",
  /* 108 */  "ireg32: LSHR(ireg32,imem32)",
  /* 109 */  "ireg64: LSHR(ireg64,imem64)",
  /* 110 */  "ireg8: LSHR(ireg8,CNSTI)",
  /* 111 */  "ireg16: LSHR(ireg16,CNSTI)",
  /* 112 */  "ireg32: LSHR(ireg32,CNSTI)",
  /* 113 */  "ireg64: LSHR(ireg64,CNSTI)",
  /* 114 */  "ireg8: ASHR(ireg8,ireg8)",
  /* 115 */  "ireg16: ASHR(ireg16,ireg16)",
  /* 116 */  "ireg32: ASHR(ireg32,ireg32)",
  /* 117 */  "ireg64: ASHR(ireg64,ireg64)",
  /* 118 */  "ireg8: ASHR(ireg8,imem8)",
  /* 119 */  "ireg16: ASHR(ireg16,imem16)",
  /* 120 */  "ireg32: ASHR(ireg32,imem32)",
  /* 121 */  "ireg64: ASHR(ireg64,imem64)",
  /* 122 */  "ireg8: ASHR(ireg8,CNSTI)",
  /* 123 */  "ireg16: ASHR(ireg16,CNSTI)",
  /* 124 */  "ireg32: ASHR(ireg32,CNSTI)",
  /* 125 */  "ireg64: ASHR(ireg64,CNSTI)",
  /* 126 */  "ireg8: TRUNC8(ireg)",
  /* 127 */  "ireg16: TRUNC16(ireg)",
  /* 128 */  "ireg32: TRUNC32(ireg)",
  /* 129 */  "ireg64: TRUNC64(ireg)",
  /* 130 */  "ireg8: RET8",
  /* 131 */  "ireg16: RET16",
  /* 132 */  "ireg32: RET32",
  /* 133 */  "ireg64: RET64",
  /* 134 */  "ireg8: SEXT8(ireg)",
  /* 135 */  "ireg16: SEXT16(ireg)",
  /* 136 */  "ireg32: SEXT32(ireg)",
  /* 137 */  "ireg64: SEXT64(ireg)",
  /* 138 */  "imem8: LOAD(MEM8)",
  /* 139 */  "imem16: LOAD(MEM16)",
  /* 140 */  "imem32: LOAD(MEM32)",
  /* 141 */  "imem64: LOAD(MEM64)",
  /* 142 */  "ireg8: LOAD(MEM8)",
  /* 143 */  "ireg16: LOAD(MEM16)",
  /* 144 */  "ireg32: LOAD(MEM32)",
  /* 145 */  "ireg64: LOAD(MEM64)",
  /* 146 */  "ireg8: LOAD(GLOB8)",
  /* 147 */  "ireg16: LOAD(GLOB16)",
  /* 148 */  "ireg32: LOAD(GLOB32)",
  /* 149 */  "ireg64: LOAD(GLOB64)",
  /* 150 */  "ireg64: LOAD(ireg64)",
  /* 151 */  "imem8: MEM8",
  /* 152 */  "imem16: MEM16",
  /* 153 */  "imem32: MEM32",
  /* 154 */  "imem64: MEM64",
  /* 155 */  "ireg: ireg8",
  /* 156 */  "ireg: ireg16",
  /* 157 */  "ireg: ireg32",
  /* 158 */  "ireg: ireg64",
};


char *burm_files[] = {
"x86.brg",
};

int burm_file_numbers[] = {
  /* 0 */  0,
  /* 1 */  0,
  /* 2 */  0,
  /* 3 */  0,
  /* 4 */  0,
  /* 5 */  0,
  /* 6 */  0,
  /* 7 */  0,
  /* 8 */  0,
  /* 9 */  0,
  /* 10 */  0,
  /* 11 */  0,
  /* 12 */  0,
  /* 13 */  0,
  /* 14 */  0,
  /* 15 */  0,
  /* 16 */  0,
  /* 17 */  0,
  /* 18 */  0,
  /* 19 */  0,
  /* 20 */  0,
  /* 21 */  0,
  /* 22 */  0,
  /* 23 */  0,
  /* 24 */  0,
  /* 25 */  0,
  /* 26 */  0,
  /* 27 */  0,
  /* 28 */  0,
  /* 29 */  0,
  /* 30 */  0,
  /* 31 */  0,
  /* 32 */  0,
  /* 33 */  0,
  /* 34 */  0,
  /* 35 */  0,
  /* 36 */  0,
  /* 37 */  0,
  /* 38 */  0,
  /* 39 */  0,
  /* 40 */  0,
  /* 41 */  0,
  /* 42 */  0,
  /* 43 */  0,
  /* 44 */  0,
  /* 45 */  0,
  /* 46 */  0,
  /* 47 */  0,
  /* 48 */  0,
  /* 49 */  0,
  /* 50 */  0,
  /* 51 */  0,
  /* 52 */  0,
  /* 53 */  0,
  /* 54 */  0,
  /* 55 */  0,
  /* 56 */  0,
  /* 57 */  0,
  /* 58 */  0,
  /* 59 */  0,
  /* 60 */  0,
  /* 61 */  0,
  /* 62 */  0,
  /* 63 */  0,
  /* 64 */  0,
  /* 65 */  0,
  /* 66 */  0,
  /* 67 */  0,
  /* 68 */  0,
  /* 69 */  0,
  /* 70 */  0,
  /* 71 */  0,
  /* 72 */  0,
  /* 73 */  0,
  /* 74 */  0,
  /* 75 */  0,
  /* 76 */  0,
  /* 77 */  0,
  /* 78 */  0,
  /* 79 */  0,
  /* 80 */  0,
  /* 81 */  0,
  /* 82 */  0,
  /* 83 */  0,
  /* 84 */  0,
  /* 85 */  0,
  /* 86 */  0,
  /* 87 */  0,
  /* 88 */  0,
  /* 89 */  0,
  /* 90 */  0,
  /* 91 */  0,
  /* 92 */  0,
  /* 93 */  0,
  /* 94 */  0,
  /* 95 */  0,
  /* 96 */  0,
  /* 97 */  0,
  /* 98 */  0,
  /* 99 */  0,
  /* 100 */  0,
  /* 101 */  0,
  /* 102 */  0,
  /* 103 */  0,
  /* 104 */  0,
  /* 105 */  0,
  /* 106 */  0,
  /* 107 */  0,
  /* 108 */  0,
  /* 109 */  0,
  /* 110 */  0,
  /* 111 */  0,
  /* 112 */  0,
  /* 113 */  0,
  /* 114 */  0,
  /* 115 */  0,
  /* 116 */  0,
  /* 117 */  0,
  /* 118 */  0,
  /* 119 */  0,
  /* 120 */  0,
  /* 121 */  0,
  /* 122 */  0,
  /* 123 */  0,
  /* 124 */  0,
  /* 125 */  0,
  /* 126 */  0,
  /* 127 */  0,
  /* 128 */  0,
  /* 129 */  0,
  /* 130 */  0,
  /* 131 */  0,
  /* 132 */  0,
  /* 133 */  0,
  /* 134 */  0,
  /* 135 */  0,
  /* 136 */  0,
  /* 137 */  0,
  /* 138 */  0,
  /* 139 */  0,
  /* 140 */  0,
  /* 141 */  0,
  /* 142 */  0,
  /* 143 */  0,
  /* 144 */  0,
  /* 145 */  0,
  /* 146 */  0,
  /* 147 */  0,
  /* 148 */  0,
  /* 149 */  0,
  /* 150 */  0,
  /* 151 */  0,
  /* 152 */  0,
  /* 153 */  0,
  /* 154 */  0,
  /* 155 */  0,
  /* 156 */  0,
  /* 157 */  0,
  /* 158 */  0,
};

int burm_line_numbers[] = {
  /* 0 */  100,
  /* 1 */  113,
  /* 2 */  126,
  /* 3 */  139,
  /* 4 */  152,
  /* 5 */  164,
  /* 6 */  176,
  /* 7 */  188,
  /* 8 */  200,
  /* 9 */  215,
  /* 10 */  235,
  /* 11 */  255,
  /* 12 */  275,
  /* 13 */  295,
  /* 14 */  311,
  /* 15 */  327,
  /* 16 */  343,
  /* 17 */  359,
  /* 18 */  376,
  /* 19 */  393,
  /* 20 */  410,
  /* 21 */  427,
  /* 22 */  444,
  /* 23 */  462,
  /* 24 */  480,
  /* 25 */  498,
  /* 26 */  516,
  /* 27 */  534,
  /* 28 */  552,
  /* 29 */  570,
  /* 30 */  587,
  /* 31 */  612,
  /* 32 */  637,
  /* 33 */  662,
  /* 34 */  689,
  /* 35 */  711,
  /* 36 */  733,
  /* 37 */  755,
  /* 38 */  777,
  /* 39 */  798,
  /* 40 */  819,
  /* 41 */  840,
  /* 42 */  860,
  /* 43 */  885,
  /* 44 */  910,
  /* 45 */  935,
  /* 46 */  962,
  /* 47 */  984,
  /* 48 */  1006,
  /* 49 */  1028,
  /* 50 */  1050,
  /* 51 */  1071,
  /* 52 */  1092,
  /* 53 */  1113,
  /* 54 */  1133,
  /* 55 */  1158,
  /* 56 */  1183,
  /* 57 */  1208,
  /* 58 */  1235,
  /* 59 */  1257,
  /* 60 */  1279,
  /* 61 */  1301,
  /* 62 */  1323,
  /* 63 */  1344,
  /* 64 */  1365,
  /* 65 */  1386,
  /* 66 */  1406,
  /* 67 */  1431,
  /* 68 */  1456,
  /* 69 */  1481,
  /* 70 */  1508,
  /* 71 */  1530,
  /* 72 */  1552,
  /* 73 */  1574,
  /* 74 */  1596,
  /* 75 */  1617,
  /* 76 */  1638,
  /* 77 */  1659,
  /* 78 */  1679,
  /* 79 */  1704,
  /* 80 */  1729,
  /* 81 */  1754,
  /* 82 */  1781,
  /* 83 */  1803,
  /* 84 */  1825,
  /* 85 */  1847,
  /* 86 */  1869,
  /* 87 */  1890,
  /* 88 */  1911,
  /* 89 */  1932,
  /* 90 */  1953,
  /* 91 */  1978,
  /* 92 */  2003,
  /* 93 */  2028,
  /* 94 */  2055,
  /* 95 */  2077,
  /* 96 */  2099,
  /* 97 */  2121,
  /* 98 */  2143,
  /* 99 */  2164,
  /* 100 */  2185,
  /* 101 */  2206,
  /* 102 */  2226,
  /* 103 */  2251,
  /* 104 */  2276,
  /* 105 */  2301,
  /* 106 */  2328,
  /* 107 */  2350,
  /* 108 */  2372,
  /* 109 */  2394,
  /* 110 */  2416,
  /* 111 */  2437,
  /* 112 */  2458,
  /* 113 */  2479,
  /* 114 */  2499,
  /* 115 */  2524,
  /* 116 */  2549,
  /* 117 */  2574,
  /* 118 */  2601,
  /* 119 */  2623,
  /* 120 */  2645,
  /* 121 */  2667,
  /* 122 */  2689,
  /* 123 */  2710,
  /* 124 */  2731,
  /* 125 */  2752,
  /* 126 */  2773,
  /* 127 */  2784,
  /* 128 */  2795,
  /* 129 */  2806,
  /* 130 */  2818,
  /* 131 */  2828,
  /* 132 */  2838,
  /* 133 */  2848,
  /* 134 */  2859,
  /* 135 */  2897,
  /* 136 */  2935,
  /* 137 */  2973,
  /* 138 */  3012,
  /* 139 */  3024,
  /* 140 */  3036,
  /* 141 */  3048,
  /* 142 */  3060,
  /* 143 */  3077,
  /* 144 */  3094,
  /* 145 */  3111,
  /* 146 */  3128,
  /* 147 */  3150,
  /* 148 */  3172,
  /* 149 */  3194,
  /* 150 */  3216,
  /* 151 */  3235,
  /* 152 */  3244,
  /* 153 */  3253,
  /* 154 */  3262,
  /* 155 */  3271,
  /* 156 */  3283,
  /* 157 */  3295,
  /* 158 */  3307,
};
static short burm_decode_stmt[] = {
   -1,
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
  13,
  14,
  15,
  16,
  17,
  18,
  19,
  20,
  21,
  22,
  23,
  24,
  25,
  26,
  27,
  28,
  29,
};

static short burm_decode_ireg[] = {
   -1,
  155,
  156,
  157,
  158,
};

static short burm_decode_ireg8[] = {
   -1,
  30,
  34,
  38,
  42,
  46,
  50,
  54,
  58,
  62,
  66,
  70,
  74,
  78,
  82,
  86,
  90,
  94,
  98,
  102,
  106,
  110,
  114,
  118,
  122,
  126,
  130,
  134,
  142,
  146,
};

static short burm_decode_ireg16[] = {
   -1,
  31,
  35,
  39,
  43,
  47,
  51,
  55,
  59,
  63,
  67,
  71,
  75,
  79,
  83,
  87,
  91,
  95,
  99,
  103,
  107,
  111,
  115,
  119,
  123,
  127,
  131,
  135,
  143,
  147,
};

static short burm_decode_ireg32[] = {
   -1,
  32,
  36,
  40,
  44,
  48,
  52,
  56,
  60,
  64,
  68,
  72,
  76,
  80,
  84,
  88,
  92,
  96,
  100,
  104,
  108,
  112,
  116,
  120,
  124,
  128,
  132,
  136,
  144,
  148,
};

static short burm_decode_ireg64[] = {
   -1,
  33,
  37,
  41,
  45,
  49,
  53,
  57,
  61,
  65,
  69,
  73,
  77,
  81,
  85,
  89,
  93,
  97,
  101,
  105,
  109,
  113,
  117,
  121,
  125,
  129,
  133,
  137,
  145,
  149,
  150,
};

static short burm_decode_imem8[] = {
   -1,
  138,
  151,
};

static short burm_decode_imem16[] = {
   -1,
  139,
  152,
};

static short burm_decode_imem32[] = {
   -1,
  140,
  153,
};

static short burm_decode_imem64[] = {
   -1,
  141,
  154,
};

int burm_rule(struct burm_state *state, int goalnt) {
  burm_assert(goalnt >= 1 && goalnt <= 10,
        PANIC("Bad goal nonterminal %d in burm_rule\n", goalnt));
  if (!state)
    return 0;
  switch (goalnt) {
  case burm_stmt_NT:  return burm_decode_stmt[((struct burm_state *)state)->rule.burm_stmt];
  case burm_ireg_NT:  return burm_decode_ireg[((struct burm_state *)state)->rule.burm_ireg];
  case burm_ireg8_NT:  return burm_decode_ireg8[((struct burm_state *)state)->rule.burm_ireg8];
  case burm_ireg16_NT:  return burm_decode_ireg16[((struct burm_state *)state)->rule.burm_ireg16];
  case burm_ireg32_NT:  return burm_decode_ireg32[((struct burm_state *)state)->rule.burm_ireg32];
  case burm_ireg64_NT:  return burm_decode_ireg64[((struct burm_state *)state)->rule.burm_ireg64];
  case burm_imem8_NT:  return burm_decode_imem8[((struct burm_state *)state)->rule.burm_imem8];
  case burm_imem16_NT:  return burm_decode_imem16[((struct burm_state *)state)->rule.burm_imem16];
  case burm_imem32_NT:  return burm_decode_imem32[((struct burm_state *)state)->rule.burm_imem32];
  case burm_imem64_NT:  return burm_decode_imem64[((struct burm_state *)state)->rule.burm_imem64];
  default:
    burm_assert(0, PANIC("Bad goal nonterminal %d in burm_rule\n", goalnt));
  }
  return 0;
}


struct burm_action {
  int nt;
  struct burm_state* st;
};

#ifndef RULE
#define RULE(n,s) \
    (act = (burm_action*) malloc(sizeof(struct burm_action)),act->nt=n,act->st=s,act)
#endif

int burm_cost_code(COST *_c, int _ern,struct burm_state *_s)
{
  NODEPTR *_children;
  struct burm_action *act;
  switch(_ern){
  default:
    DEFAULT_COST;
  case 0:
{

# line 97 "x86.brg"

    (*_c).cost = _s->cost[burm_ireg8_NT].cost;

}
  break;
  case 1:
{

# line 110 "x86.brg"

    (*_c).cost = _s->cost[burm_ireg16_NT].cost;

}
  break;
  case 2:
{

# line 123 "x86.brg"

    (*_c).cost = _s->cost[burm_ireg32_NT].cost;

}
  break;
  case 3:
{

# line 136 "x86.brg"

    (*_c).cost = _s->cost[burm_ireg64_NT].cost;

}
  break;
  case 4:
{

# line 149 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 5:
{

# line 161 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 6:
{

# line 173 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 7:
{

# line 185 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 8:
{

# line 197 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 9:
{

# line 212 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 10:
{

# line 232 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 11:
{

# line 252 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 12:
{

# line 272 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 13:
{

# line 292 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 14:
{

# line 308 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 15:
{

# line 324 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 16:
{

# line 340 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 17:
{

# line 356 "x86.brg"

    (*_c).cost = _s->kids[1]->cost[burm_ireg8_NT].cost + 2;

}
  break;
  case 18:
{

# line 373 "x86.brg"

    (*_c).cost = _s->kids[1]->cost[burm_ireg16_NT].cost + 2;

}
  break;
  case 19:
{

# line 390 "x86.brg"

    (*_c).cost = _s->kids[1]->cost[burm_ireg32_NT].cost + 2;

}
  break;
  case 20:
{

# line 407 "x86.brg"

    (*_c).cost = _s->kids[1]->cost[burm_ireg64_NT].cost + 2;//pointers produced such match in a way type checked 

}
  break;
  case 21:
{

# line 424 "x86.brg"

    (*_c).cost = _s->kids[1]->cost[burm_ireg64_NT].cost + 2;

}
  break;
  case 22:
{

# line 441 "x86.brg"

    (*_c).cost = _s->kids[0]->cost[burm_imem8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;

}
  break;
  case 23:
{

# line 459 "x86.brg"

    (*_c).cost = _s->kids[0]->cost[burm_imem16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;

}
  break;
  case 24:
{

# line 477 "x86.brg"

    (*_c).cost = _s->kids[0]->cost[burm_imem32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;

}
  break;
  case 25:
{

# line 495 "x86.brg"

    (*_c).cost = _s->kids[0]->cost[burm_imem64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;

}
  break;
  case 26:
{

# line 513 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 27:
{

# line 531 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 28:
{

# line 549 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 29:
{

# line 567 "x86.brg"

    (*_c).cost = 2;

}
  break;
  case 30:
{

# line 585 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 31:
{

# line 610 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 32:
{

# line 635 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 33:
{

# line 660 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 34:
{

# line 686 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 35:
{

# line 708 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 36:
{

# line 730 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 37:
{

# line 752 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 38:
{

# line 774 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 39:
{

# line 795 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 40:
{

# line 816 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 41:
{

# line 837 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 42:
{

# line 858 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 43:
{

# line 883 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 44:
{

# line 908 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 45:
{

# line 933 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 46:
{

# line 959 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 47:
{

# line 981 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 48:
{

# line 1003 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 49:
{

# line 1025 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 50:
{

# line 1047 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 51:
{

# line 1068 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 52:
{

# line 1089 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 53:
{

# line 1110 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 54:
{

# line 1131 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 55:
{

# line 1156 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 56:
{

# line 1181 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 57:
{

# line 1206 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 58:
{

# line 1232 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 59:
{

# line 1254 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 60:
{

# line 1276 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 61:
{

# line 1298 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 62:
{

# line 1320 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 63:
{

# line 1341 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 64:
{

# line 1362 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 65:
{

# line 1383 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 66:
{

# line 1404 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 67:
{

# line 1429 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 68:
{

# line 1454 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 69:
{

# line 1479 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 70:
{

# line 1505 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 71:
{

# line 1527 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 72:
{

# line 1549 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 73:
{

# line 1571 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 74:
{

# line 1593 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 75:
{

# line 1614 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 76:
{

# line 1635 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 77:
{

# line 1656 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 78:
{

# line 1677 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 79:
{

# line 1702 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 80:
{

# line 1727 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 81:
{

# line 1752 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 82:
{

# line 1778 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 83:
{

# line 1800 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 84:
{

# line 1822 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 85:
{

# line 1844 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 86:
{

# line 1866 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 87:
{

# line 1887 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 88:
{

# line 1908 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 89:
{

# line 1929 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 90:
{

# line 1951 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 91:
{

# line 1976 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 92:
{

# line 2001 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 93:
{

# line 2026 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 94:
{

# line 2052 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 95:
{

# line 2074 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 96:
{

# line 2096 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 97:
{

# line 2118 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 98:
{

# line 2140 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 99:
{

# line 2161 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 100:
{

# line 2182 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 101:
{

# line 2203 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 102:
{

# line 2224 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 103:
{

# line 2249 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 104:
{

# line 2274 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 105:
{

# line 2299 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 106:
{

# line 2325 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 107:
{

# line 2347 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 108:
{

# line 2369 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 109:
{

# line 2391 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 110:
{

# line 2413 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 111:
{

# line 2434 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 112:
{

# line 2455 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 113:
{

# line 2476 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 114:
{

# line 2497 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg8_NT].cost + _s->kids[1]->cost[burm_ireg8_NT].cost + 2;
}
  break;
  case 115:
{

# line 2522 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg16_NT].cost + _s->kids[1]->cost[burm_ireg16_NT].cost + 2;
}
  break;
  case 116:
{

# line 2547 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg32_NT].cost + _s->kids[1]->cost[burm_ireg32_NT].cost + 2;
}
  break;
  case 117:
{

# line 2572 "x86.brg"

    (*_c).cost = 2; //_s->kids[0]->cost[burm_ireg64_NT].cost + _s->kids[1]->cost[burm_ireg64_NT].cost + 2;
}
  break;
  case 118:
{

# line 2598 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost+_s->kids[1]->cost[burm_imem8_NT].cost + 2;
    
}
  break;
  case 119:
{

# line 2620 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost+_s->kids[1]->cost[burm_imem16_NT].cost + 2;
    
}
  break;
  case 120:
{

# line 2642 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost+_s->kids[1]->cost[burm_imem32_NT].cost + 2;
    
}
  break;
  case 121:
{

# line 2664 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost+_s->kids[1]->cost[burm_imem64_NT].cost + 2;
    
}
  break;
  case 122:
{

# line 2686 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg8_NT].cost + 2;
    
}
  break;
  case 123:
{

# line 2707 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg16_NT].cost + 2;
    
}
  break;
  case 124:
{

# line 2728 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg32_NT].cost + 2;
    
}
  break;
  case 125:
{

# line 2749 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg64_NT].cost + 2;
    
}
  break;
  case 126:
{

# line 2770 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 127:
{

# line 2781 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 128:
{

# line 2792 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 129:
{

# line 2803 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 130:
{

# line 2815 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 131:
{

# line 2825 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 132:
{

# line 2835 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 133:
{

# line 2845 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 134:
{

# line 2856 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 135:
{

# line 2894 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 136:
{

# line 2932 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 137:
{

# line 2970 "x86.brg"

        (*_c).cost = _s->kids[0]->cost[burm_ireg_NT].cost + 2;
    
}
  break;
  case 138:
{

# line 3009 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 139:
{

# line 3021 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 140:
{

# line 3033 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 141:
{

# line 3045 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 142:
{

# line 3057 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 143:
{

# line 3074 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 144:
{

# line 3091 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 145:
{

# line 3108 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 146:
{

# line 3125 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 147:
{

# line 3147 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 148:
{

# line 3169 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 149:
{

# line 3191 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 150:
{

# line 3213 "x86.brg"

        (*_c).cost = 1;
    
}
  break;
  case 151:
{

# line 3232 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 152:
{

# line 3241 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 153:
{

# line 3250 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 154:
{

# line 3259 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 155:
{

# line 3268 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 156:
{

# line 3280 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 157:
{

# line 3292 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  case 158:
{

# line 3304 "x86.brg"

        (*_c).cost = 0;
    
}
  break;
  }
  return 1;
}

# line 85 "x86.brg"
void stmt_action(struct burm_state *_s, 
# line 85 "x86.brg"
int indent);

# line 86 "x86.brg"
const char* ireg_action(struct burm_state *_s, 
# line 86 "x86.brg"
int indent);

# line 87 "x86.brg"
const char* ireg8_action(struct burm_state *_s, 
# line 87 "x86.brg"
int indent);

# line 88 "x86.brg"
const char* ireg16_action(struct burm_state *_s, 
# line 88 "x86.brg"
int indent);

# line 89 "x86.brg"
const char* ireg32_action(struct burm_state *_s, 
# line 89 "x86.brg"
int indent);

# line 90 "x86.brg"
const char* ireg64_action(struct burm_state *_s, 
# line 90 "x86.brg"
int indent);

# line 91 "x86.brg"
int imem8_action(struct burm_state *_s, 
# line 91 "x86.brg"
int indent);

# line 92 "x86.brg"
int imem16_action(struct burm_state *_s, 
# line 92 "x86.brg"
int indent);

# line 93 "x86.brg"
int imem32_action(struct burm_state *_s, 
# line 93 "x86.brg"
int indent);

# line 94 "x86.brg"
int imem64_action(struct burm_state *_s, 
# line 94 "x86.brg"
int indent);


#include <stdarg.h>

void burm_exec(struct burm_state *state, int nterm, ...) 
{
  va_list(ap);
  va_start(ap,nterm);

  burm_assert(nterm >= 1 && nterm <= 10,
        PANIC("Bad nonterminal %d in $exec\n", nterm));

  if (state)
    switch (nterm) {
    case burm_stmt_NT:
      stmt_action(state,va_arg(ap,int));
      break;
    case burm_ireg_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_ireg8_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_ireg16_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_ireg32_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_ireg64_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_imem8_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_imem16_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_imem32_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    case burm_imem64_NT:
      PANIC("$exec cannot take non-void functions as arguments\n");
      break;
    default:
      PANIC("Bad nonterminal %d in $exec\n", nterm);
      break;
    }
  else
    PANIC("Bad state for $exec in nonterminal %d \n",nterm);
  va_end(ap);
}

#define EXEC(s,n,a) ( \
  (n == burm_stmt_NT)? burm_exec(s,n,a): \
  PANIC("Bad nonterminal %d in $exec\n", n))

struct burm_state *burm_immed(struct burm_state *s,int n);
#ifndef NO_ACTION
#define NO_ACTION assert(0)
#endif

# line 85 "x86.brg"
void stmt_action(struct burm_state *_s, 
# line 85 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_stmt[_s->rule.burm_stmt];
  NODEPTR *_children;
  if(_s->rule.burm_stmt==0)
    NO_ACTION(stmt);
  switch(_ern){
  case 0:
{

# line 100 "x86.brg"

    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    fp = fopen("CodeGen.asm", "a");
    ireg8_action(_s,indent + 1);
    fclose(fp);

}
  break;
  case 1:
{

# line 113 "x86.brg"

    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    fp = fopen("CodeGen.asm", "a");
    ireg16_action(_s,indent + 1);
    fclose(fp);

}
  break;
  case 2:
{

# line 126 "x86.brg"

    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    fp = fopen("CodeGen.asm", "a");
    ireg32_action(_s,indent + 1);
    fclose(fp);

}
  break;
  case 3:
{

# line 139 "x86.brg"

    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    fp = fopen("CodeGen.asm", "a");
    ireg64_action(_s,indent + 1);
    fclose(fp);

}
  break;
  case 4:
{

# line 152 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    fprintf(fp, "  movb $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    fprintf(stderr, "  movb $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 5:
{

# line 164 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    fprintf(fp, "  movw $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    fprintf(stderr, "  movw $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 6:
{

# line 176 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    fprintf(fp, "  movl $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    fprintf(stderr, "  movl $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 7:
{

# line 188 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    fprintf(fp, "  movq $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    fprintf(stderr, "  movq $%d , -%d(%%rbp) \n", _s->kids[1]->node->val, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 8:
{

# line 200 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg64_action(_s->kids[0],indent + 1);
    fprintf(fp, "  movl $%d , (%%%s) \n", _s->kids[1]->node->val,getReg64(_s->kids[0]->node->inst));
    fprintf(stderr, "  movl $%d , (%%%s) \n", _s->kids[1]->node->val, getReg64(_s->kids[0]->node->inst));
    fclose(fp);

}
  break;
  case 9:
{

# line 215 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    const char* Reg1;
    if(_s->kids[1]->node->val < 6){
      Reg1 = getArgReg(_s->kids[1]->node->val,8);
      fprintf(fp, "  movb %%%s , -%d(%%rbp) \n", Reg1, _s->kids[0]->node->val);
    }
    else{
      fprintf(fp, "  movb %d(%%rbp) , %%bl \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(fp, "  movb %%bl , -%d(%%rbp) \n", _s->kids[0]->node->val);
      fprintf(stderr, "  movb %d(%%rbp) , %%bl \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(stderr, "  movb %%bl , -%d(%%rbp) \n", _s->kids[0]->node->val);
    }
    fclose(fp);

}
  break;
  case 10:
{

# line 235 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    const char* Reg1;
    if(_s->kids[1]->node->val < 6){
      Reg1 = getArgReg(_s->kids[1]->node->val,16);
      fprintf(fp, "  movw %%%s , -%d(%%rbp) \n", Reg1, _s->kids[0]->node->val);
    }
    else{
      fprintf(fp, "  movw %d(%%rbp) , %%bx \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(fp, "  movw %%bx , -%d(%%rbp) \n", _s->kids[0]->node->val);
      fprintf(stderr, "  movw %d(%%rbp) , %%bx \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(stderr, "  movw %%bx , -%d(%%rbp) \n", _s->kids[0]->node->val);
    }
    fclose(fp);

}
  break;
  case 11:
{

# line 255 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    const char* Reg1;
    if(_s->kids[1]->node->val < 6){
      Reg1 = getArgReg(_s->kids[1]->node->val,32);
      fprintf(fp, "  movl %%%s , -%d(%%rbp) \n", Reg1, _s->kids[0]->node->val);
    }
    else{
      fprintf(fp, "  movl %d(%%rbp) , %%ebx \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(fp, "  movl %%ebx , -%d(%%rbp) \n", _s->kids[0]->node->val);
      fprintf(stderr, "  movl %d(%%rbp) , %%ebx \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(stderr, "  movl %%ebx , -%d(%%rbp) \n", _s->kids[0]->node->val);
    }
    fclose(fp);

}
  break;
  case 12:
{

# line 275 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    const char* Reg1;
    if(_s->kids[1]->node->val < 6){
      Reg1 = getArgReg(_s->kids[1]->node->val,64);
      fprintf(fp, "  movq %%%s , -%d(%%rbp) \n", Reg1, _s->kids[0]->node->val);
    }
    else{
      fprintf(fp, "  movq %d(%%rbp) , %%bl \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(fp, "  movq %%rbx , -%d(%%rbp) \n", _s->kids[0]->node->val);
      fprintf(stderr, "  movq %d(%%rbp) , %%bl \n", getArgOffset(_s->kids[1]->node->val));
      fprintf(stderr, "  movq %%rbx , -%d(%%rbp) \n", _s->kids[0]->node->val);
    }
    fclose(fp);

}
  break;
  case 13:
{

# line 295 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg64_action(_s->kids[0],indent + 1);
    ireg8_action(_s->kids[1],indent + 1);
    fprintf(fp, "  movb $%%%s , (%%%s) \n", getReg8(_s->kids[1]->node->inst),getReg64(_s->kids[0]->node->inst));
    fprintf(stderr, "  movb $%%%s , (%%%s) \n", getReg8(_s->kids[1]->node->inst), getReg64(_s->kids[0]->node->inst));
    fclose(fp);

}
  break;
  case 14:
{

# line 311 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg64_action(_s->kids[0],indent + 1);
    ireg16_action(_s->kids[1],indent + 1);
    fprintf(fp, "  movw $%%%s , (%%%s) \n", getReg16(_s->kids[1]->node->inst),getReg64(_s->kids[0]->node->inst));
    fprintf(stderr, "  movw $%%%s , (%%%s) \n", getReg16(_s->kids[1]->node->inst), getReg64(_s->kids[0]->node->inst));
    fclose(fp);

}
  break;
  case 15:
{

# line 327 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg64_action(_s->kids[0],indent + 1);
    ireg32_action(_s->kids[1],indent + 1);
    fprintf(fp, "  movd $%%%s , (%%%s) \n", getReg32(_s->kids[1]->node->inst),getReg64(_s->kids[0]->node->inst));
    fprintf(stderr, "  movd $%%%s , (%%%s) \n", getReg32(_s->kids[1]->node->inst), getReg64(_s->kids[0]->node->inst));
    fclose(fp);

}
  break;
  case 16:
{

# line 343 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg64_action(_s->kids[0],indent + 1);
    ireg64_action(_s->kids[1],indent + 1);
    fprintf(fp, "  movq $%%%s , (%%%s) \n", getReg64(_s->kids[1]->node->inst),getReg64(_s->kids[0]->node->inst));
    fprintf(stderr, "  movq $%%%s , (%%%s) \n", getReg64(_s->kids[1]->node->inst), getReg64(_s->kids[0]->node->inst));
    fclose(fp);

}
  break;
  case 17:
{

# line 359 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg8_action(_s->kids[1],indent + 1);
    const char* regist = getReg8(_s->kids[1]->node->inst);
    fprintf(fp, "  movb  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movb  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 18:
{

# line 376 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg16_action(_s->kids[1],indent + 1);
    const char* regist = getReg16(_s->kids[1]->node->inst);
    fprintf(fp, "  movw  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movw  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 19:
{

# line 393 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg32_action(_s->kids[1],indent + 1);
    const char* regist = getReg32(_s->kids[1]->node->inst);
    fprintf(fp, "  movl  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movl  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 20:
{

# line 410 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg64_action(_s->kids[1],indent + 1);
    const char* regist = getReg32(_s->kids[1]->node->inst);
    fprintf(fp, "  movl  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movl  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 21:
{

# line 427 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    ireg64_action(_s->kids[1],indent + 1);
    const char* regist = getReg64(_s->kids[1]->node->inst);
    fprintf(fp, "  movq  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movq  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 22:
{

# line 444 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    imem8_action(_s->kids[0],indent + 1);
    ireg8_action(_s->kids[1],indent + 1);
    const char* regist = getReg8(_s->kids[1]->node->inst);
    fprintf(fp, "  movb  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movb  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 23:
{

# line 462 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    imem16_action(_s->kids[0],indent + 1);
    ireg16_action(_s->kids[1],indent + 1);
    const char* regist = getReg16(_s->kids[1]->node->inst);
    fprintf(fp, "  movw  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movw  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 24:
{

# line 480 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    imem32_action(_s->kids[0],indent + 1);
    ireg32_action(_s->kids[1],indent + 1);
    const char* regist = getReg32(_s->kids[1]->node->inst);
    fprintf(fp, "  movl  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movl  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 25:
{

# line 498 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    int i;
    for (i = 0; i < indent; i++)
        fprintf(stderr, " ");
    imem64_action(_s->kids[0],indent + 1);
    ireg64_action(_s->kids[1],indent + 1);
    const char* regist = getReg64(_s->kids[1]->node->inst);
    fprintf(fp, "  movq  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    fprintf(stderr, "  movq  %%%s, -%d(%%rbp) \n", regist, _s->kids[0]->node->val);
    remove_Regs(_s->node->inst);
    fclose(fp);

}
  break;
  case 26:
{

# line 516 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    if (_s->kids[0]->node->val == 0) {
        fprintf(fp, "  movb  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
        fprintf(stderr, "  movb  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
    } else {
        fprintf(fp, "  movb  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
        fprintf(stderr, "  movb  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
    }
    remove_Regs(_s->node->inst);
    fprintf(stderr, "\n Finished const global store \n");
    fclose(fp);

}
  break;
  case 27:
{

# line 534 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    if (_s->kids[0]->node->val == 0) {
        fprintf(fp, "  movw  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
        fprintf(stderr, "  movw  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
    } else {
        fprintf(fp, "  movw  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
        fprintf(stderr, "  movw  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
    }
    remove_Regs(_s->node->inst);
    fprintf(stderr, "\n Finished const global store \n");
    fclose(fp);

}
  break;
  case 28:
{

# line 552 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    if (_s->kids[0]->node->val == 0) {
        fprintf(fp, "  movl  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
        fprintf(stderr, "  movl  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
    } else {
        fprintf(fp, "  movl  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
        fprintf(stderr, "  movl  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
    }
    remove_Regs(_s->node->inst);
    fprintf(stderr, "\n Finished const global store \n");
    fclose(fp);

}
  break;
  case 29:
{

# line 570 "x86.brg"

    fp = fopen("CodeGen.asm", "a");
    if (_s->kids[0]->node->val == 0) {
        fprintf(fp, "  movq  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
        fprintf(stderr, "  movq  $%d, %s(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name);
    } else {
        fprintf(fp, "  movq  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
        fprintf(stderr, "  movq  $%d, %s+%d(%%rip) \n", _s->kids[1]->node->val, _s->kids[0]->node->name, _s->kids[0]->node->val);
    }
    remove_Regs(_s->node->inst);
    fprintf(stderr, "\n Finished const global store \n");
    fclose(fp);

}
  break;
  }
}

# line 86 "x86.brg"
const char* ireg_action(struct burm_state *_s, 
# line 86 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_ireg[_s->rule.burm_ireg];
  NODEPTR *_children;
  if(_s->rule.burm_ireg==0)
    NO_ACTION(ireg);
  switch(_ern){
  case 155:
{

# line 3271 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s,indent + 1);
        return "";
    
}
  break;
  case 156:
{

# line 3283 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s,indent + 1);
        return "";
    
}
  break;
  case 157:
{

# line 3295 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s,indent + 1);
        return "";
    
}
  break;
  case 158:
{

# line 3307 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s,indent + 1);
        return "";
    
}
  break;
  }
}

# line 87 "x86.brg"
const char* ireg8_action(struct burm_state *_s, 
# line 87 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_ireg8[_s->rule.burm_ireg8];
  NODEPTR *_children;
  if(_s->rule.burm_ireg8==0)
    NO_ACTION(ireg8);
  switch(_ern){
  case 30:
{

# line 587 "x86.brg"

        fprintf(stderr, "Matched ADD reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  addb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  addb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 34:
{

# line 689 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  addb -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  addb -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 38:
{

# line 777 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "addb %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "addb %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 42:
{

# line 860 "x86.brg"

        fprintf(stderr, "Matched SUB reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  subb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  subb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 46:
{

# line 962 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  subb -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  subb -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 50:
{

# line 1050 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "subb $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "subb $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 54:
{

# line 1133 "x86.brg"

        fprintf(stderr, "Matched OR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  orb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  orb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 58:
{

# line 1235 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  orb -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  orb -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 62:
{

# line 1323 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "orb %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "orb %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 66:
{

# line 1406 "x86.brg"

        fprintf(stderr, "Matched XOR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  xorb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  xorb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 70:
{

# line 1508 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memxor = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  xorb -%d(%%rbp), %%%s \n", memxor, regist2);
        fprintf(stderr, "  xorb -%d(%%rbp), %%%s \n", memxor, regist2);
        return regist2;
    
}
  break;
  case 74:
{

# line 1596 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "xorb %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "xorb %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 78:
{

# line 1679 "x86.brg"

        fprintf(stderr, "Matched MUL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  imulb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  imulb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 82:
{

# line 1781 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memimul = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  imulb -%d(%%rbp), %%%s \n", memimul, regist2);
        fprintf(stderr, "  imulb -%d(%%rbp), %%%s \n", memimul, regist2);
        return regist2;
    
}
  break;
  case 86:
{

# line 1869 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "imulb $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "imulb $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 90:
{

# line 1953 "x86.brg"

        fprintf(stderr, "Matched SHL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shlb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shlb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 94:
{

# line 2055 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memshl = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shlb -%d(%%rbp), %%%s \n", memshl, regist2);
        fprintf(stderr, "  shlb -%d(%%rbp), %%%s \n", memshl, regist2);
        return regist2;
    
}
  break;
  case 98:
{

# line 2143 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shlb $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shlb $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 102:
{

# line 2226 "x86.brg"

        fprintf(stderr, "Matched LSHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shrb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shrb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 106:
{

# line 2328 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memshr = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shrb -%d(%%rbp), %%%s \n", memshr, regist2);
        fprintf(stderr, "  shrb -%d(%%rbp), %%%s \n", memshr, regist2);
        return regist2;
    
}
  break;
  case 110:
{

# line 2416 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shrb %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shrb %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 114:
{

# line 2499 "x86.brg"

        fprintf(stderr, "Matched ASHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        ireg8_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->kids[1]->node->inst);
        const char* regist3 = getReg8(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  sarb  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  sarb  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 118:
{

# line 2601 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movb  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movb  %%%s, %%%s \n", regist1, regist2);
        }
        int memsar = imem8_action(_s->kids[1],indent + 1);

        fprintf(fp, "  sarb -%d(%%rbp), %%%s \n", memsar, regist2);
        fprintf(stderr, "  sarb -%d(%%rbp), %%%s \n", memsar, regist2);
        return regist2;
    
}
  break;
  case 122:
{

# line 2689 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg8_action(_s->kids[0],indent + 1);
        const char* regist = getReg8(_s->kids[0]->node->inst);
        const char* regist2 = getReg8(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movb  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movb  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "sarb %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "sarb %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 126:
{

# line 2773 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
        return getReg8(_s->node->inst);
    
}
  break;
  case 130:
{

# line 2818 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        return getReg8(_s->node->inst);
    
}
  break;
  case 134:
{

# line 2859 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
	const char* reg1;
	const char* op;
        int source = _s->node->val;
        if(source == 8){
	  reg1 = getReg8(_s->node->inst->getOperand(0));
	  op = "movsbb";
	}
	else if(source == 16){
	  reg1 = getReg16(_s->node->inst->getOperand(0));
	  op = "movsbw";
	}
	else if(source == 32){
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movsbl";
	}
	else if(source == 64){
	  reg1 = getReg64(_s->node->inst->getOperand(0));
	  op = "movsbq";
	}
	else{
	  fprintf(stderr, "\n ************** NO MATCH FOR REG IN SEXT ************* \n");
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movsbl";
	}
        const char* reg2 = getReg8(_s->node->inst);
	fprintf(fp, "  %s %%%s, %%%s \n",op ,reg1, reg2);
        fprintf(stderr, "  %s %%%s, %%%s  \n",op ,reg1, reg2);
        return reg2;
    
}
  break;
  case 142:
{

# line 3060 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "LOAD mem32 to reg Matched \n");
        bool inRegister = inReg(_s->kids[0]->node->inst);
        const char* regist = getReg8(_s->node->inst);
        if (!inRegister) {
            fprintf(fp, "  movb  -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
            fprintf(stderr, "  movb -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
        }
        return regist;
    
}
  break;
  case 146:
{

# line 3128 "x86.brg"

        fprintf(stderr, "Matching Load GloB\n");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        bool inRegister = inReg(_s->kids[0]->node->inst->getOperand(0));
        const char* regist = getReg8(_s->node->inst);
        if (!inRegister) {
            if (_s->kids[0]->node->val == 0) {
                fprintf(fp, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->inst->getOperand(0)->getNameStr().c_str(), regist);
                fprintf(stderr, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->name, regist); //inst->getOperand(0)->getNameStr().c_str()             
            } else {
                fprintf(fp, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->inst->getOperand(0)->getNameStr().c_str(), _s->kids[0]->node->val, regist);
                fprintf(stderr, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->name, _s->kids[0]->node->val, regist); //inst->getOperand(0)->getNameStr().c_str()      
            }
        }
        return regist;
    
}
  break;
  }
}

# line 88 "x86.brg"
const char* ireg16_action(struct burm_state *_s, 
# line 88 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_ireg16[_s->rule.burm_ireg16];
  NODEPTR *_children;
  if(_s->rule.burm_ireg16==0)
    NO_ACTION(ireg16);
  switch(_ern){
  case 31:
{

# line 612 "x86.brg"

        fprintf(stderr, "Matched ADD reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  addw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  addw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 35:
{

# line 711 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  addw -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  addw -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 39:
{

# line 798 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "  addw $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "  addw $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 43:
{

# line 885 "x86.brg"

        fprintf(stderr, "Matched SUB reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  subw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  subw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 47:
{

# line 984 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  subw -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  subw -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 51:
{

# line 1071 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "subw $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "subw $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 55:
{

# line 1158 "x86.brg"

        fprintf(stderr, "Matched OR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  orw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  orw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 59:
{

# line 1257 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  orw -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  orw -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 63:
{

# line 1344 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "orw %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "orw %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 67:
{

# line 1431 "x86.brg"

        fprintf(stderr, "Matched XOR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  xorw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  xorw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 71:
{

# line 1530 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memxor = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  xorw -%d(%%rbp), %%%s \n", memxor, regist2);
        fprintf(stderr, "  xorw -%d(%%rbp), %%%s \n", memxor, regist2);
        return regist2;
    
}
  break;
  case 75:
{

# line 1617 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "  xorw %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "  xorw %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 79:
{

# line 1704 "x86.brg"

        fprintf(stderr, "Matched MUL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  imulw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  imulw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 83:
{

# line 1803 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memimul = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  imulw -%d(%%rbp), %%%s \n", memimul, regist2);
        fprintf(stderr, "  imulw -%d(%%rbp), %%%s \n", memimul, regist2);
        return regist2;
    
}
  break;
  case 87:
{

# line 1890 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "imulw $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "imulw $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 91:
{

# line 1978 "x86.brg"

        fprintf(stderr, "Matched SHL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shlw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shlw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 95:
{

# line 2077 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memshl = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shlw -%d(%%rbp), %%%s \n", memshl, regist2);
        fprintf(stderr, "  shlw -%d(%%rbp), %%%s \n", memshl, regist2);
        return regist2;
    
}
  break;
  case 99:
{

# line 2164 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shlw $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shlw $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 103:
{

# line 2251 "x86.brg"

        fprintf(stderr, "Matched LSHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shrw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shrw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 107:
{

# line 2350 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memshr = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shrw -%d(%%rbp), %%%s \n", memshr, regist2);
        fprintf(stderr, "  shrw -%d(%%rbp), %%%s \n", memshr, regist2);
        return regist2;
    
}
  break;
  case 111:
{

# line 2437 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shrw %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shrw %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 115:
{

# line 2524 "x86.brg"

        fprintf(stderr, "Matched ASHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        ireg16_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->kids[1]->node->inst);
        const char* regist3 = getReg16(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  sarw  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  sarw  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 119:
{

# line 2623 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movw  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movw  %%%s, %%%s \n", regist1, regist2);
        }
        int memsar = imem16_action(_s->kids[1],indent + 1);

        fprintf(fp, "  sarw -%d(%%rbp), %%%s \n", memsar, regist2);
        fprintf(stderr, "  sarw -%d(%%rbp), %%%s \n", memsar, regist2);
        return regist2;
    
}
  break;
  case 123:
{

# line 2710 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg16_action(_s->kids[0],indent + 1);
        const char* regist = getReg16(_s->kids[0]->node->inst);
        const char* regist2 = getReg16(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movw  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movw  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "sarw %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "sarw %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 127:
{

# line 2784 "x86.brg"

       int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
       return getReg16(_s->node->inst);
    
}
  break;
  case 131:
{

# line 2828 "x86.brg"

       int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
       return getReg16(_s->node->inst);
    
}
  break;
  case 135:
{

# line 2897 "x86.brg"

       int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
	const char* reg1;
	const char* op;
        int source = _s->node->val;
        if(source == 8){
	  reg1 = getReg8(_s->node->inst->getOperand(0));
	  op = "movsbw";
	}
	else if(source == 16){
	  reg1 = getReg16(_s->node->inst->getOperand(0));
	  op = "movsww";
	}
	else if(source == 32){
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movslw";
	}
	else if(source == 64){
	  reg1 = getReg64(_s->node->inst->getOperand(0));
	  op = "movsqw";
	}
	else{
	  fprintf(stderr, "\n ************** NO MATCH FOR REG IN SEXT ************* \n");
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movswl";
	}
        const char* reg2 = getReg16(_s->node->inst);
	fprintf(fp, "  %s %%%s, %%%s \n",op ,reg1, reg2);
        fprintf(stderr, "  %s %%%s, %%%s  \n",op ,reg1, reg2);
        return reg2;
    
}
  break;
  case 143:
{

# line 3077 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "LOAD mem32 to reg Matched \n");
        bool inRegister = inReg(_s->kids[0]->node->inst);
        const char* regist = getReg16(_s->node->inst);
        if (!inRegister) {
            fprintf(fp, "  movw  -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
            fprintf(stderr, "  movw -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
        }
        return regist;
    
}
  break;
  case 147:
{

# line 3150 "x86.brg"

        fprintf(stderr, "Matching Load GloB\n");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        bool inRegister = inReg(_s->kids[0]->node->inst->getOperand(0));
        const char* regist = getReg16(_s->node->inst);
        if (!inRegister) {
            if (_s->kids[0]->node->val == 0) {
                fprintf(fp, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->inst->getOperand(0)->getNameStr().c_str(), regist);
                fprintf(stderr, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->name, regist); //inst->getOperand(0)->getNameStr().c_str()             
            } else {
                fprintf(fp, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->inst->getOperand(0)->getNameStr().c_str(), _s->kids[0]->node->val, regist);
                fprintf(stderr, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->name, _s->kids[0]->node->val, regist); //inst->getOperand(0)->getNameStr().c_str()      
            }
        }
        return regist;
    
}
  break;
  }
}

# line 89 "x86.brg"
const char* ireg32_action(struct burm_state *_s, 
# line 89 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_ireg32[_s->rule.burm_ireg32];
  NODEPTR *_children;
  if(_s->rule.burm_ireg32==0)
    NO_ACTION(ireg32);
  switch(_ern){
  case 32:
{

# line 637 "x86.brg"

        fprintf(stderr, "Matched ADD reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  addl  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  addl  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 36:
{

# line 733 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  addl -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  addl -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 40:
{

# line 819 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "  addl $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "  addl $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 44:
{

# line 910 "x86.brg"

        fprintf(stderr, "Matched SUB reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  subl  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  subl  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 48:
{

# line 1006 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  subl -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  subl -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 52:
{

# line 1092 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "  subl $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "  subl $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 56:
{

# line 1183 "x86.brg"

        fprintf(stderr, "Matched OR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  orl  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  orl  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 60:
{

# line 1279 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  orl -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  orl -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 64:
{

# line 1365 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "orl %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "orl %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 68:
{

# line 1456 "x86.brg"

        fprintf(stderr, "Matched XOR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  xorl  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  xorl  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 72:
{

# line 1552 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memxor = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  xorl -%d(%%rbp), %%%s \n", memxor, regist2);
        fprintf(stderr, "  xorl -%d(%%rbp), %%%s \n", memxor, regist2);
        return regist2;
    
}
  break;
  case 76:
{

# line 1638 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "  xorl %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "  xorl %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 80:
{

# line 1729 "x86.brg"

        fprintf(stderr, "Matched MUL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  imull  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  imull  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 84:
{

# line 1825 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memimul = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  imull -%d(%%rbp), %%%s \n", memimul, regist2);
        fprintf(stderr, "  imull -%d(%%rbp), %%%s \n", memimul, regist2);
        return regist2;
    
}
  break;
  case 88:
{

# line 1911 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "imull $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "imull $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 92:
{

# line 2003 "x86.brg"

        fprintf(stderr, "Matched SHL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shll  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shll  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 96:
{

# line 2099 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memshl = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shll -%d(%%rbp), %%%s \n", memshl, regist2);
        fprintf(stderr, "  shll -%d(%%rbp), %%%s \n", memshl, regist2);
        return regist2;
    
}
  break;
  case 100:
{

# line 2185 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shll $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shll $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 104:
{

# line 2276 "x86.brg"

        fprintf(stderr, "Matched LSHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shrl  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shrl  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 108:
{

# line 2372 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memshr = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shrl -%d(%%rbp), %%%s \n", memshr, regist2);
        fprintf(stderr, "  shrl -%d(%%rbp), %%%s \n", memshr, regist2);
        return regist2;
    
}
  break;
  case 112:
{

# line 2458 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shrl $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shrl $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 116:
{

# line 2549 "x86.brg"

        fprintf(stderr, "Matched ASHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        ireg32_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->kids[1]->node->inst);
        const char* regist3 = getReg32(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  sarl  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  sarl  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 120:
{

# line 2645 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movl  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movl  %%%s, %%%s \n", regist1, regist2);
        }
        int memsar = imem32_action(_s->kids[1],indent + 1);

        fprintf(fp, "  sarl -%d(%%rbp), %%%s \n", memsar, regist2);
        fprintf(stderr, "  sarl -%d(%%rbp), %%%s \n", memsar, regist2);
        return regist2;
    
}
  break;
  case 124:
{

# line 2731 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg32_action(_s->kids[0],indent + 1);
        const char* regist = getReg32(_s->kids[0]->node->inst);
        const char* regist2 = getReg32(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movl  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movl  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "sarl $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "sarl $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 128:
{

# line 2795 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
       return getReg32(_s->node->inst);
    
}
  break;
  case 132:
{

# line 2838 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
       return getReg32(_s->node->inst);
    
}
  break;
  case 136:
{

# line 2935 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
	const char* reg1;
	const char* op;
        int source = _s->node->val;
        if(source == 8){
	  reg1 = getReg8(_s->node->inst->getOperand(0));
	  op = "movsbl";
	}
	else if(source == 16){
	  reg1 = getReg16(_s->node->inst->getOperand(0));
	  op = "movswl";
	}
	else if(source == 32){
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movsll";
	}
	else if(source == 64){
	  reg1 = getReg64(_s->node->inst->getOperand(0));
	  op = "movsql";
	}
	else{
	  fprintf(stderr, "\n ************** NO MATCH FOR REG IN SEXT ************* \n");
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movsll";
	}
        const char* reg2 = getReg32(_s->node->inst);
	fprintf(fp, "  %s %%%s, %%%s \n",op ,reg1, reg2);
        fprintf(stderr, "  %s %%%s, %%%s  \n",op ,reg1, reg2);
        return reg2;
    
}
  break;
  case 144:
{

# line 3094 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "LOAD mem32 to reg Matched \n");
        bool inRegister = inReg(_s->kids[0]->node->inst);
        const char* regist = getReg32(_s->node->inst);
        if (!inRegister) {
            fprintf(fp, "  movl  -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
            fprintf(stderr, "  movl -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
        }
        return regist;
    
}
  break;
  case 148:
{

# line 3172 "x86.brg"

        fprintf(stderr, "Matching Load GloB\n");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        bool inRegister = inReg(_s->kids[0]->node->inst->getOperand(0));
        const char* regist = getReg32(_s->node->inst);
        if (!inRegister) {
            if (_s->kids[0]->node->val == 0) {
                fprintf(fp, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->name, regist);
                fprintf(stderr, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->name, regist); //inst->getOperand(0)->getNameStr().c_str()             
            } else {
                fprintf(fp, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->name, _s->kids[0]->node->val, regist);
                fprintf(stderr, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->name, _s->kids[0]->node->val, regist); //inst->getOperand(0)->getNameStr().c_str()      
            }
        }
        return regist;
    
}
  break;
  }
}

# line 90 "x86.brg"
const char* ireg64_action(struct burm_state *_s, 
# line 90 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_ireg64[_s->rule.burm_ireg64];
  NODEPTR *_children;
  if(_s->rule.burm_ireg64==0)
    NO_ACTION(ireg64);
  switch(_ern){
  case 33:
{

# line 662 "x86.brg"

        fprintf(stderr, "Matched ADD reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  addq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  addq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 37:
{

# line 755 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  addq -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  addq -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 41:
{

# line 840 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "  addq $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "  addq $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 45:
{

# line 935 "x86.brg"

        fprintf(stderr, "Matched SUB reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  subq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  subq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 49:
{

# line 1028 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  subq -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  subq -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 53:
{

# line 1113 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "subq %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "subq %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 57:
{

# line 1208 "x86.brg"

        fprintf(stderr, "Matched OR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  orq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  orq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 61:
{

# line 1301 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memor = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  orq -%d(%%rbp), %%%s \n", memor, regist2);
        fprintf(stderr, "  orq -%d(%%rbp), %%%s \n", memor, regist2);
        return regist2;
    
}
  break;
  case 65:
{

# line 1386 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "orq %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "orq %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 69:
{

# line 1481 "x86.brg"

        fprintf(stderr, "Matched XOR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  xorq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  xorq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 73:
{

# line 1574 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memxor = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  xorq -%d(%%rbp), %%%s \n", memxor, regist2);
        fprintf(stderr, "  xorq -%d(%%rbp), %%%s \n", memxor, regist2);
        return regist2;
    
}
  break;
  case 77:
{

# line 1659 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "  xorq %d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "  xorq %d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 81:
{

# line 1754 "x86.brg"

        fprintf(stderr, "Matched MUL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  imulq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  imulq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 85:
{

# line 1847 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memimul = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  imulq -%d(%%rbp), %%%s \n", memimul, regist2);
        fprintf(stderr, "  imulq -%d(%%rbp), %%%s \n", memimul, regist2);
        return regist2;
    
}
  break;
  case 89:
{

# line 1932 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "imulq $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "imulq $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 93:
{

# line 2028 "x86.brg"

        fprintf(stderr, "Matched SHL reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shlq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shlq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 97:
{

# line 2121 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memshl = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shlq -%d(%%rbp), %%%s \n", memshl, regist2);
        fprintf(stderr, "  shlq -%d(%%rbp), %%%s \n", memshl, regist2);
        return regist2;
    
}
  break;
  case 101:
{

# line 2206 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shlq $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shlq $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 105:
{

# line 2301 "x86.brg"

        fprintf(stderr, "Matched LSHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  shrq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  shrq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 109:
{

# line 2394 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memshr = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  shrq -%d(%%rbp), %%%s \n", memshr, regist2);
        fprintf(stderr, "  shrq -%d(%%rbp), %%%s \n", memshr, regist2);
        return regist2;
    
}
  break;
  case 113:
{

# line 2479 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "shrq $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "shrq $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 117:
{

# line 2574 "x86.brg"

        fprintf(stderr, "Matched ASHR reg,reg");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        ireg64_action(_s->kids[1],indent + 1);

        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->kids[1]->node->inst);
        const char* regist3 = getReg64(_s->node->inst);

        if (strcmp(regist1, regist3)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist3);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist3);
        }

        fprintf(fp, "  sarq  %%%s, %%%s \n", regist2, regist3);
        fprintf(stderr, "  sarq  %%%s, %%%s \n", regist2, regist3);
        return regist3;
    
}
  break;
  case 121:
{

# line 2667 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist1 = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist1, regist2)) {
            fprintf(fp, "  movq  %%%s, %%%s \n", regist1, regist2);
            fprintf(stderr, "  movq  %%%s, %%%s \n", regist1, regist2);
        }
        int memsar = imem64_action(_s->kids[1],indent + 1);

        fprintf(fp, "  sarq -%d(%%rbp), %%%s \n", memsar, regist2);
        fprintf(stderr, "  sarq -%d(%%rbp), %%%s \n", memsar, regist2);
        return regist2;
    
}
  break;
  case 125:
{

# line 2752 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        const char* regist = getReg64(_s->kids[0]->node->inst);
        const char* regist2 = getReg64(_s->node->inst);
        if (strcmp(regist, regist2)) {
            fprintf(fp, "movq  %%%s, %%%s \n", regist, regist2);
            fprintf(stderr, "movq  %%%s, %%%s \n", regist, regist2);
        }

        fprintf(fp, "sarq $%d, %%%s \n", _s->kids[1]->node->val, regist2);
        fprintf(stderr, "sarq $%d, %%%s  \n", _s->kids[1]->node->val, regist2);
        return regist2;
    
}
  break;
  case 129:
{

# line 2806 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
       return getReg64(_s->node->inst);
    
}
  break;
  case 133:
{

# line 2848 "x86.brg"

	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
       return getReg64(_s->node->inst);
    
}
  break;
  case 137:
{

# line 2973 "x86.brg"
// will never happen just for debugging
	int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        ireg_action(_s->kids[0],indent + 1);
	const char* reg1;
	const char* op;
        int source = _s->node->val;
        if(source == 8){
	  reg1 = getReg8(_s->node->inst->getOperand(0));
	  op = "movsbq";
	}
	else if(source == 16){
	  reg1 = getReg16(_s->node->inst->getOperand(0));
	  op = "movswq";
	}
	else if(source == 32){
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movslq";
	}
	else if(source == 64){
	  reg1 = getReg64(_s->node->inst->getOperand(0));
	  op = "movsqq";
	}
	else{
	  fprintf(stderr, "\n ************** NO MATCH FOR REG IN SEXT ************* \n");
	  reg1 = getReg32(_s->node->inst->getOperand(0));
	  op = "movsql";
	}
        const char* reg2 = getReg64(_s->node->inst);
	fprintf(fp, "  %s %%%s, %%%s \n",op ,reg1, reg2);
        fprintf(stderr, "  %s %%%s, %%%s  \n",op ,reg1, reg2);
        return reg2;
    
}
  break;
  case 145:
{

# line 3111 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "LOAD mem32 to reg Matched \n");
        bool inRegister = inReg(_s->kids[0]->node->inst);
        const char* regist = getReg64(_s->node->inst);
        if (!inRegister) {
            fprintf(fp, "  movq  -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
            fprintf(stderr, "  movq -%d(%%rbp),%%%s \n", _s->kids[0]->node->val, regist);
        }
        return regist;
    
}
  break;
  case 149:
{

# line 3194 "x86.brg"

        fprintf(stderr, "Matching Load GloB\n");
        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        bool inRegister = inReg(_s->kids[0]->node->inst->getOperand(0));
        const char* regist = getReg64(_s->node->inst);
        if (!inRegister) {
            if (_s->kids[0]->node->val == 0) {
                fprintf(fp, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->name, regist);
                fprintf(stderr, "  mov %s(%%rip),%%%s \n", _s->kids[0]->node->name, regist); //inst->getOperand(0)->getNameStr().c_str()             
            } else {
                fprintf(fp, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->name, _s->kids[0]->node->val, regist);
                fprintf(stderr, "  mov %s+%d(%%rip),%%%s \n", _s->kids[0]->node->name, _s->kids[0]->node->val, regist); //inst->getOperand(0)->getNameStr().c_str()      
            }
        }
        return regist;
    
}
  break;
  case 150:
{

# line 3216 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
           fprintf(stderr, " ");
        ireg64_action(_s->kids[0],indent + 1);
        bool inRegister = inReg(_s->node->inst);
        const char* regist = getReg64(_s->node->inst);
    //    if (!inRegister) {
            fprintf(fp, "  movq  (%%%s),%%%s \n", getReg64(_s->kids[0]->node->inst), regist);
            fprintf(stderr, "  movq (%%%s),%%%s \n", getReg64(_s->kids[0]->node->inst), regist);
    //    }
        return regist;
    
}
  break;
  }
}

# line 91 "x86.brg"
int imem8_action(struct burm_state *_s, 
# line 91 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_imem8[_s->rule.burm_imem8];
  NODEPTR *_children;
  if(_s->rule.burm_imem8==0)
    NO_ACTION(imem8);
  switch(_ern){
  case 138:
{

# line 3012 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "Load mem to mem Matched \n", burm_string[_ern]);                                                
        return _s->kids[0]->node->val;
    
}
  break;
  case 151:
{

# line 3235 "x86.brg"

        //    fprintf(stderr, "Matched MEM \n");                                                                               
        return _s->node->val;
    
}
  break;
  }
}

# line 92 "x86.brg"
int imem16_action(struct burm_state *_s, 
# line 92 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_imem16[_s->rule.burm_imem16];
  NODEPTR *_children;
  if(_s->rule.burm_imem16==0)
    NO_ACTION(imem16);
  switch(_ern){
  case 139:
{

# line 3024 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "Load mem to mem Matched \n", burm_string[_ern]);                                                
        return _s->kids[0]->node->val;
    
}
  break;
  case 152:
{

# line 3244 "x86.brg"

        //    fprintf(stderr, "Matched MEM \n");                                                                               
        return _s->node->val;
    
}
  break;
  }
}

# line 93 "x86.brg"
int imem32_action(struct burm_state *_s, 
# line 93 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_imem32[_s->rule.burm_imem32];
  NODEPTR *_children;
  if(_s->rule.burm_imem32==0)
    NO_ACTION(imem32);
  switch(_ern){
  case 140:
{

# line 3036 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "Load mem to mem Matched \n", burm_string[_ern]);                                                
        return _s->kids[0]->node->val;
    
}
  break;
  case 153:
{

# line 3253 "x86.brg"

        //    fprintf(stderr, "Matched MEM \n");                                                                               
        return _s->node->val;
    
}
  break;
  }
}

# line 94 "x86.brg"
int imem64_action(struct burm_state *_s, 
# line 94 "x86.brg"
int indent)
{
  struct burm_state *_t;
  int _ern=burm_decode_imem64[_s->rule.burm_imem64];
  NODEPTR *_children;
  if(_s->rule.burm_imem64==0)
    NO_ACTION(imem64);
  switch(_ern){
  case 141:
{

# line 3048 "x86.brg"

        int i;
        for (i = 0; i < indent; i++)
            fprintf(stderr, " ");
        //    fprintf(stderr, "Load mem to mem Matched \n", burm_string[_ern]);                                                
        return _s->kids[0]->node->val;
    
}
  break;
  case 154:
{

# line 3262 "x86.brg"

        //    fprintf(stderr, "Matched MEM \n");                                                                               
        return _s->node->val;
    
}
  break;
  }
}
static void burm_closure_ireg8(struct burm_state *, COST);
static void burm_closure_ireg16(struct burm_state *, COST);
static void burm_closure_ireg32(struct burm_state *, COST);
static void burm_closure_ireg64(struct burm_state *, COST);

static void burm_closure_ireg8(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,155,s) && COST_LESS(c,s->cost[burm_ireg_NT])) {
    s->cost[burm_ireg_NT] = c ;
    s->rule.burm_ireg = 1;
  }
  if(burm_cost_code(&c,0,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
    s->cost[burm_stmt_NT] = c ;
    s->rule.burm_stmt = 1;
  }
}

static void burm_closure_ireg16(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,156,s) && COST_LESS(c,s->cost[burm_ireg_NT])) {
    s->cost[burm_ireg_NT] = c ;
    s->rule.burm_ireg = 2;
  }
  if(burm_cost_code(&c,1,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
    s->cost[burm_stmt_NT] = c ;
    s->rule.burm_stmt = 2;
  }
}

static void burm_closure_ireg32(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,157,s) && COST_LESS(c,s->cost[burm_ireg_NT])) {
    s->cost[burm_ireg_NT] = c ;
    s->rule.burm_ireg = 3;
  }
  if(burm_cost_code(&c,2,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
    s->cost[burm_stmt_NT] = c ;
    s->rule.burm_stmt = 3;
  }
}

static void burm_closure_ireg64(struct burm_state *s, COST c) {
  if(burm_cost_code(&c,158,s) && COST_LESS(c,s->cost[burm_ireg_NT])) {
    s->cost[burm_ireg_NT] = c ;
    s->rule.burm_ireg = 4;
  }
  if(burm_cost_code(&c,3,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
    s->cost[burm_stmt_NT] = c ;
    s->rule.burm_stmt = 4;
  }
}

struct burm_state *burm_alloc_state(NODEPTR u,int op,int arity)
{
  struct burm_state *p, **k;
  p = (struct burm_state *)ALLOC(sizeof *p);
  burm_assert(p, PANIC("1:ALLOC returned NULL in burm_alloc_state\n"));
  p->op = op;
  p->node = u;
  if(arity){
    k=(struct burm_state **)ALLOC(arity*sizeof (struct burm_state *));
    burm_assert(k, PANIC("2:ALLOC returned NULL in burm_alloc_state\n"));
    p->kids=k;
  }else
    p->kids=0;
  p->rule.burm_stmt =
  p->rule.burm_ireg =
  p->rule.burm_ireg8 =
  p->rule.burm_ireg16 =
  p->rule.burm_ireg32 =
  p->rule.burm_ireg64 =
  p->rule.burm_imem8 =
  p->rule.burm_imem16 =
  p->rule.burm_imem32 =
  p->rule.burm_imem64 =
    0;
  p->cost[1] =
  p->cost[2] =
  p->cost[3] =
  p->cost[4] =
  p->cost[5] =
  p->cost[6] =
  p->cost[7] =
  p->cost[8] =
  p->cost[9] =
  p->cost[10] =
    COST_INFINITY;
  return p;
}
struct burm_state *burm_label1(NODEPTR u) {
  int op, arity, i, immed_matched=0;
  COST c=COST_ZERO;
  struct burm_state *s,**k;
  NODEPTR *children;
  op=OP_LABEL(u);
  arity=burm_arity[op];
  switch(op){
  case 0:		/* BURP */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 1:		/* ADDI */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 2:		/* ADDRLP */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 3:		/* ASGNI */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 4:		/* STORE */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* stmt: STORE(GLOB64,CNSTI) */
      k[0]->op == 23 && 	/* GLOB64 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,29,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 30;
      }
    }
    if (   /* stmt: STORE(GLOB32,CNSTI) */
      k[0]->op == 22 && 	/* GLOB32 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,28,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 29;
      }
    }
    if (   /* stmt: STORE(GLOB16,CNSTI) */
      k[0]->op == 21 && 	/* GLOB16 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,27,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 28;
      }
    }
    if (   /* stmt: STORE(GLOB8,CNSTI) */
      k[0]->op == 20 && 	/* GLOB8 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,26,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 27;
      }
    }
    if (   /* stmt: STORE(imem64,ireg64) */
      k[0]->rule.burm_imem64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,25,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 26;
      }
    }
    if (   /* stmt: STORE(imem32,ireg32) */
      k[0]->rule.burm_imem32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,24,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 25;
      }
    }
    if (   /* stmt: STORE(imem16,ireg16) */
      k[0]->rule.burm_imem16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,23,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 24;
      }
    }
    if (   /* stmt: STORE(imem8,ireg8) */
      k[0]->rule.burm_imem8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,22,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 23;
      }
    }
    if (   /* stmt: STORE(MEM64,ireg64) */
      k[0]->op == 19 && 	/* MEM64 */
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,21,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 22;
      }
    }
    if (   /* stmt: STORE(MEM32,ireg64) */
      k[0]->op == 18 && 	/* MEM32 */
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,20,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 21;
      }
    }
    if (   /* stmt: STORE(MEM32,ireg32) */
      k[0]->op == 18 && 	/* MEM32 */
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,19,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 20;
      }
    }
    if (   /* stmt: STORE(MEM16,ireg16) */
      k[0]->op == 17 && 	/* MEM16 */
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,18,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 19;
      }
    }
    if (   /* stmt: STORE(MEM8,ireg8) */
      k[0]->op == 16 && 	/* MEM8 */
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,17,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 18;
      }
    }
    if (   /* stmt: STORE(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,16,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 17;
      }
    }
    if (   /* stmt: STORE(ireg64,ireg32) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,15,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 16;
      }
    }
    if (   /* stmt: STORE(ireg64,ireg16) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,14,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 15;
      }
    }
    if (   /* stmt: STORE(ireg64,ireg8) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,13,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 14;
      }
    }
    if (   /* stmt: STORE(MEM64,ARG64) */
      k[0]->op == 19 && 	/* MEM64 */
      k[1]->op == 39	/* ARG64 */
    ) {
      if(burm_cost_code(&c,12,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 13;
      }
    }
    if (   /* stmt: STORE(MEM32,ARG32) */
      k[0]->op == 18 && 	/* MEM32 */
      k[1]->op == 38	/* ARG32 */
    ) {
      if(burm_cost_code(&c,11,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 12;
      }
    }
    if (   /* stmt: STORE(MEM16,ARG16) */
      k[0]->op == 17 && 	/* MEM16 */
      k[1]->op == 37	/* ARG16 */
    ) {
      if(burm_cost_code(&c,10,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 11;
      }
    }
    if (   /* stmt: STORE(MEM8,ARG8) */
      k[0]->op == 16 && 	/* MEM8 */
      k[1]->op == 36	/* ARG8 */
    ) {
      if(burm_cost_code(&c,9,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 10;
      }
    }
    if (   /* stmt: STORE(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,8,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 9;
      }
    }
    if (   /* stmt: STORE(MEM64,CNSTI) */
      k[0]->op == 19 && 	/* MEM64 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,7,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 8;
      }
    }
    if (   /* stmt: STORE(MEM32,CNSTI) */
      k[0]->op == 18 && 	/* MEM32 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,6,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 7;
      }
    }
    if (   /* stmt: STORE(MEM16,CNSTI) */
      k[0]->op == 17 && 	/* MEM16 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,5,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 6;
      }
    }
    if (   /* stmt: STORE(MEM8,CNSTI) */
      k[0]->op == 16 && 	/* MEM8 */
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,4,s) && COST_LESS(c,s->cost[burm_stmt_NT])) {
        s->cost[burm_stmt_NT] = c ;
        s->rule.burm_stmt = 5;
      }
    }
    break;
  case 5:		/* ADD */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: ADD(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,41,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 3;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: ADD(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,40,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 3;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: ADD(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,39,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 3;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: ADD(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,38,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 3;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: ADD(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,37,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 2;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: ADD(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,36,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 2;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: ADD(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,35,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 2;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: ADD(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,34,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 2;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: ADD(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,33,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 1;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: ADD(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,32,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 1;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: ADD(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,31,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 1;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: ADD(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,30,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 1;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 6:		/* SUB */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: SUB(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,53,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 6;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: SUB(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,52,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 6;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: SUB(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,51,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 6;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: SUB(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,50,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 6;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: SUB(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,49,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 5;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: SUB(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,48,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 5;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: SUB(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,47,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 5;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: SUB(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,46,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 5;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: SUB(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,45,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 4;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: SUB(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,44,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 4;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: SUB(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,43,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 4;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: SUB(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,42,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 4;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 7:		/* AND */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 8:		/* OR */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: OR(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,65,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 9;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: OR(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,64,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 9;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: OR(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,63,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 9;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: OR(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,62,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 9;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: OR(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,61,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 8;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: OR(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,60,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 8;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: OR(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,59,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 8;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: OR(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,58,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 8;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: OR(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,57,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 7;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: OR(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,56,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 7;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: OR(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,55,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 7;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: OR(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,54,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 7;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 9:		/* XOR */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: XOR(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,77,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 12;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: XOR(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,76,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 12;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: XOR(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,75,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 12;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: XOR(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,74,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 12;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: XOR(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,73,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 11;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: XOR(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,72,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 11;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: XOR(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,71,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 11;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: XOR(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,70,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 11;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: XOR(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,69,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 10;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: XOR(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,68,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 10;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: XOR(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,67,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 10;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: XOR(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,66,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 10;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 10:		/* MUL */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: MUL(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,89,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 15;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: MUL(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,88,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 15;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: MUL(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,87,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 15;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: MUL(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,86,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 15;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: MUL(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,85,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 14;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: MUL(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,84,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 14;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: MUL(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,83,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 14;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: MUL(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,82,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 14;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: MUL(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,81,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 13;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: MUL(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,80,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 13;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: MUL(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,79,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 13;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: MUL(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,78,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 13;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 11:		/* LOAD */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: LOAD(ireg64) */
      k[0]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,150,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 30;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg64: LOAD(GLOB64) */
      k[0]->op == 23	/* GLOB64 */
    ) {
      if(burm_cost_code(&c,149,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 29;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: LOAD(GLOB32) */
      k[0]->op == 22	/* GLOB32 */
    ) {
      if(burm_cost_code(&c,148,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 29;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: LOAD(GLOB16) */
      k[0]->op == 21	/* GLOB16 */
    ) {
      if(burm_cost_code(&c,147,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 29;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: LOAD(GLOB8) */
      k[0]->op == 20	/* GLOB8 */
    ) {
      if(burm_cost_code(&c,146,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 29;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: LOAD(MEM64) */
      k[0]->op == 19	/* MEM64 */
    ) {
      if(burm_cost_code(&c,145,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 28;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: LOAD(MEM32) */
      k[0]->op == 18	/* MEM32 */
    ) {
      if(burm_cost_code(&c,144,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 28;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: LOAD(MEM16) */
      k[0]->op == 17	/* MEM16 */
    ) {
      if(burm_cost_code(&c,143,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 28;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: LOAD(MEM8) */
      k[0]->op == 16	/* MEM8 */
    ) {
      if(burm_cost_code(&c,142,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 28;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* imem64: LOAD(MEM64) */
      k[0]->op == 19	/* MEM64 */
    ) {
      if(burm_cost_code(&c,141,s) && COST_LESS(c,s->cost[burm_imem64_NT])) {
        s->cost[burm_imem64_NT] = c ;
        s->rule.burm_imem64 = 1;
      }
    }
    if (   /* imem32: LOAD(MEM32) */
      k[0]->op == 18	/* MEM32 */
    ) {
      if(burm_cost_code(&c,140,s) && COST_LESS(c,s->cost[burm_imem32_NT])) {
        s->cost[burm_imem32_NT] = c ;
        s->rule.burm_imem32 = 1;
      }
    }
    if (   /* imem16: LOAD(MEM16) */
      k[0]->op == 17	/* MEM16 */
    ) {
      if(burm_cost_code(&c,139,s) && COST_LESS(c,s->cost[burm_imem16_NT])) {
        s->cost[burm_imem16_NT] = c ;
        s->rule.burm_imem16 = 1;
      }
    }
    if (   /* imem8: LOAD(MEM8) */
      k[0]->op == 16	/* MEM8 */
    ) {
      if(burm_cost_code(&c,138,s) && COST_LESS(c,s->cost[burm_imem8_NT])) {
        s->cost[burm_imem8_NT] = c ;
        s->rule.burm_imem8 = 1;
      }
    }
    break;
  case 12:		/* CNSTI */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 13:		/* CVCI */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 14:		/* I0I */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 15:		/* INDIRC */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 16:		/* MEM8 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* imem8: MEM8 */
      if(burm_cost_code(&c,151,s) && COST_LESS(c,s->cost[burm_imem8_NT])) {
        s->cost[burm_imem8_NT] = c ;
        s->rule.burm_imem8 = 2;
      }
    }
    break;
  case 17:		/* MEM16 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* imem16: MEM16 */
      if(burm_cost_code(&c,152,s) && COST_LESS(c,s->cost[burm_imem16_NT])) {
        s->cost[burm_imem16_NT] = c ;
        s->rule.burm_imem16 = 2;
      }
    }
    break;
  case 18:		/* MEM32 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* imem32: MEM32 */
      if(burm_cost_code(&c,153,s) && COST_LESS(c,s->cost[burm_imem32_NT])) {
        s->cost[burm_imem32_NT] = c ;
        s->rule.burm_imem32 = 2;
      }
    }
    break;
  case 19:		/* MEM64 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* imem64: MEM64 */
      if(burm_cost_code(&c,154,s) && COST_LESS(c,s->cost[burm_imem64_NT])) {
        s->cost[burm_imem64_NT] = c ;
        s->rule.burm_imem64 = 2;
      }
    }
    break;
  case 20:		/* GLOB8 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 21:		/* GLOB16 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 22:		/* GLOB32 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 23:		/* GLOB64 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 24:		/* TRUNC8 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg8: TRUNC8(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,126,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 25;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 25:		/* TRUNC16 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg16: TRUNC16(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,127,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 25;
        burm_closure_ireg16(s, c );
      }
    }
    break;
  case 26:		/* TRUNC32 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg32: TRUNC32(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,128,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 25;
        burm_closure_ireg32(s, c );
      }
    }
    break;
  case 27:		/* TRUNC64 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: TRUNC64(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,129,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 25;
        burm_closure_ireg64(s, c );
      }
    }
    break;
  case 28:		/* SEXT8 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg8: SEXT8(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,134,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 27;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 29:		/* SEXT16 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg16: SEXT16(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,135,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 27;
        burm_closure_ireg16(s, c );
      }
    }
    break;
  case 30:		/* SEXT32 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg32: SEXT32(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,136,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 27;
        burm_closure_ireg32(s, c );
      }
    }
    break;
  case 31:		/* SEXT64 */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: SEXT64(ireg) */
      k[0]->rule.burm_ireg
    ) {
      if(burm_cost_code(&c,137,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 27;
        burm_closure_ireg64(s, c );
      }
    }
    break;
  case 32:		/* RET8 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* ireg8: RET8 */
      if(burm_cost_code(&c,130,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 26;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 33:		/* RET16 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* ireg16: RET16 */
      if(burm_cost_code(&c,131,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 26;
        burm_closure_ireg16(s, c );
      }
    }
    break;
  case 34:		/* RET32 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* ireg32: RET32 */
      if(burm_cost_code(&c,132,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 26;
        burm_closure_ireg32(s, c );
      }
    }
    break;
  case 35:		/* RET64 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    {  		/* ireg64: RET64 */
      if(burm_cost_code(&c,133,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 26;
        burm_closure_ireg64(s, c );
      }
    }
    break;
  case 36:		/* ARG8 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 37:		/* ARG16 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 38:		/* ARG32 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 39:		/* ARG64 */
#ifdef LEAF_TRAP
    if(s=LEAF_TRAP(u,op))
      return s;
#endif
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=0;
    break;
  case 40:		/* OFFSET */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 41:		/* GEP */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  case 42:		/* SHL */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: SHL(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,101,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 18;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: SHL(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,100,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 18;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: SHL(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,99,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 18;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: SHL(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,98,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 18;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: SHL(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,97,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 17;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: SHL(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,96,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 17;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: SHL(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,95,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 17;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: SHL(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,94,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 17;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: SHL(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,93,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 16;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: SHL(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,92,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 16;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: SHL(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,91,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 16;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: SHL(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,90,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 16;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 43:		/* LSHR */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: LSHR(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,113,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 21;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: LSHR(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,112,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 21;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: LSHR(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,111,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 21;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: LSHR(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,110,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 21;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: LSHR(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,109,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 20;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: LSHR(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,108,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 20;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: LSHR(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,107,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 20;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: LSHR(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,106,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 20;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: LSHR(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,105,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 19;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: LSHR(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,104,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 19;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: LSHR(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,103,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 19;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: LSHR(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,102,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 19;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 44:		/* ASHR */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    if (   /* ireg64: ASHR(ireg64,CNSTI) */
      k[0]->rule.burm_ireg64 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,125,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 24;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: ASHR(ireg32,CNSTI) */
      k[0]->rule.burm_ireg32 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,124,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 24;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: ASHR(ireg16,CNSTI) */
      k[0]->rule.burm_ireg16 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,123,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 24;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: ASHR(ireg8,CNSTI) */
      k[0]->rule.burm_ireg8 && 
      k[1]->op == 12	/* CNSTI */
    ) {
      if(burm_cost_code(&c,122,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 24;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: ASHR(ireg64,imem64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_imem64
    ) {
      if(burm_cost_code(&c,121,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 23;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: ASHR(ireg32,imem32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_imem32
    ) {
      if(burm_cost_code(&c,120,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 23;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: ASHR(ireg16,imem16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_imem16
    ) {
      if(burm_cost_code(&c,119,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 23;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: ASHR(ireg8,imem8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_imem8
    ) {
      if(burm_cost_code(&c,118,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 23;
        burm_closure_ireg8(s, c );
      }
    }
    if (   /* ireg64: ASHR(ireg64,ireg64) */
      k[0]->rule.burm_ireg64 && 
      k[1]->rule.burm_ireg64
    ) {
      if(burm_cost_code(&c,117,s) && COST_LESS(c,s->cost[burm_ireg64_NT])) {
        s->cost[burm_ireg64_NT] = c ;
        s->rule.burm_ireg64 = 22;
        burm_closure_ireg64(s, c );
      }
    }
    if (   /* ireg32: ASHR(ireg32,ireg32) */
      k[0]->rule.burm_ireg32 && 
      k[1]->rule.burm_ireg32
    ) {
      if(burm_cost_code(&c,116,s) && COST_LESS(c,s->cost[burm_ireg32_NT])) {
        s->cost[burm_ireg32_NT] = c ;
        s->rule.burm_ireg32 = 22;
        burm_closure_ireg32(s, c );
      }
    }
    if (   /* ireg16: ASHR(ireg16,ireg16) */
      k[0]->rule.burm_ireg16 && 
      k[1]->rule.burm_ireg16
    ) {
      if(burm_cost_code(&c,115,s) && COST_LESS(c,s->cost[burm_ireg16_NT])) {
        s->cost[burm_ireg16_NT] = c ;
        s->rule.burm_ireg16 = 22;
        burm_closure_ireg16(s, c );
      }
    }
    if (   /* ireg8: ASHR(ireg8,ireg8) */
      k[0]->rule.burm_ireg8 && 
      k[1]->rule.burm_ireg8
    ) {
      if(burm_cost_code(&c,114,s) && COST_LESS(c,s->cost[burm_ireg8_NT])) {
        s->cost[burm_ireg8_NT] = c ;
        s->rule.burm_ireg8 = 22;
        burm_closure_ireg8(s, c );
      }
    }
    break;
  case 45:		/* UDIV */
    s=burm_alloc_state(u,op,arity);
    SET_STATE(u,s);
    k=s->kids;
    children=GET_KIDS(u);
    for(i=0;i<arity;i++)
      k[i]=burm_label1(children[i]);
    break;
  default:
    burm_assert(0, PANIC("Bad operator %d in burm_state\n", op));
  }
  return s;
}

struct burm_state *burm_label(NODEPTR p) {
  burm_label1(p);
  return ((struct burm_state *)STATE_LABEL(p))->rule.burm_stmt ? STATE_LABEL(p) : 0;
}

void burm_free(struct burm_state *s)
{
  int i,arity=burm_arity[s->op];
  if(s->kids==0)
    free(s);
  else {
    for(i=0;i<arity;i++)
      burm_free(s->kids[i]);
    free(s->kids);
  }
}
struct burm_state *burm_immed(struct burm_state *s,int n)
{
  NODEPTR *children = GET_KIDS(s->node);
  if(s->kids[n])
    return s->kids[n];
  else
  return s->kids[n]=burm_label1(children[n]);
}
int burm_op_label(NODEPTR p) {
  burm_assert(p, PANIC("NULL tree in burm_op_label\n"));
  return OP_LABEL(p);
}

struct burm_state *burm_state_label(NODEPTR p) {
  burm_assert(p, PANIC("NULL tree in burm_state_label\n"));
  return STATE_LABEL(p);
}

NODEPTR burm_child(NODEPTR p, int index) {
  NODEPTR *kids;
  burm_assert(p, PANIC("NULL tree in burm_child\n"));
  kids=GET_KIDS(p);
  burm_assert((0<=index && index<burm_arity[OP_LABEL(p)]),
    PANIC("Bad index %d in burm_child\n", index));

  return kids[index];
}
NODEPTR *burm_kids(NODEPTR p, int eruleno, NODEPTR kids[]) {
  burm_assert(p, PANIC("NULL tree in burm_kids\n"));
  burm_assert(kids, PANIC("NULL kids in burm_kids\n"));
  switch (eruleno) {
  case 158: /* ireg: ireg64 */
  case 157: /* ireg: ireg32 */
  case 156: /* ireg: ireg16 */
  case 155: /* ireg: ireg8 */
  case 3: /* stmt: ireg64 */
  case 2: /* stmt: ireg32 */
  case 1: /* stmt: ireg16 */
  case 0: /* stmt: ireg8 */
    kids[0] = p;
    break;
  case 154: /* imem64: MEM64 */
  case 153: /* imem32: MEM32 */
  case 152: /* imem16: MEM16 */
  case 151: /* imem8: MEM8 */
  case 149: /* ireg64: LOAD(GLOB64) */
  case 148: /* ireg32: LOAD(GLOB32) */
  case 147: /* ireg16: LOAD(GLOB16) */
  case 146: /* ireg8: LOAD(GLOB8) */
  case 145: /* ireg64: LOAD(MEM64) */
  case 144: /* ireg32: LOAD(MEM32) */
  case 143: /* ireg16: LOAD(MEM16) */
  case 142: /* ireg8: LOAD(MEM8) */
  case 141: /* imem64: LOAD(MEM64) */
  case 140: /* imem32: LOAD(MEM32) */
  case 139: /* imem16: LOAD(MEM16) */
  case 138: /* imem8: LOAD(MEM8) */
  case 133: /* ireg64: RET64 */
  case 132: /* ireg32: RET32 */
  case 131: /* ireg16: RET16 */
  case 130: /* ireg8: RET8 */
  case 29: /* stmt: STORE(GLOB64,CNSTI) */
  case 28: /* stmt: STORE(GLOB32,CNSTI) */
  case 27: /* stmt: STORE(GLOB16,CNSTI) */
  case 26: /* stmt: STORE(GLOB8,CNSTI) */
  case 12: /* stmt: STORE(MEM64,ARG64) */
  case 11: /* stmt: STORE(MEM32,ARG32) */
  case 10: /* stmt: STORE(MEM16,ARG16) */
  case 9: /* stmt: STORE(MEM8,ARG8) */
  case 7: /* stmt: STORE(MEM64,CNSTI) */
  case 6: /* stmt: STORE(MEM32,CNSTI) */
  case 5: /* stmt: STORE(MEM16,CNSTI) */
  case 4: /* stmt: STORE(MEM8,CNSTI) */
    break;
  case 150: /* ireg64: LOAD(ireg64) */
  case 137: /* ireg64: SEXT64(ireg) */
  case 136: /* ireg32: SEXT32(ireg) */
  case 135: /* ireg16: SEXT16(ireg) */
  case 134: /* ireg8: SEXT8(ireg) */
  case 129: /* ireg64: TRUNC64(ireg) */
  case 128: /* ireg32: TRUNC32(ireg) */
  case 127: /* ireg16: TRUNC16(ireg) */
  case 126: /* ireg8: TRUNC8(ireg) */
  case 125: /* ireg64: ASHR(ireg64,CNSTI) */
  case 124: /* ireg32: ASHR(ireg32,CNSTI) */
  case 123: /* ireg16: ASHR(ireg16,CNSTI) */
  case 122: /* ireg8: ASHR(ireg8,CNSTI) */
  case 113: /* ireg64: LSHR(ireg64,CNSTI) */
  case 112: /* ireg32: LSHR(ireg32,CNSTI) */
  case 111: /* ireg16: LSHR(ireg16,CNSTI) */
  case 110: /* ireg8: LSHR(ireg8,CNSTI) */
  case 101: /* ireg64: SHL(ireg64,CNSTI) */
  case 100: /* ireg32: SHL(ireg32,CNSTI) */
  case 99: /* ireg16: SHL(ireg16,CNSTI) */
  case 98: /* ireg8: SHL(ireg8,CNSTI) */
  case 89: /* ireg64: MUL(ireg64,CNSTI) */
  case 88: /* ireg32: MUL(ireg32,CNSTI) */
  case 87: /* ireg16: MUL(ireg16,CNSTI) */
  case 86: /* ireg8: MUL(ireg8,CNSTI) */
  case 77: /* ireg64: XOR(ireg64,CNSTI) */
  case 76: /* ireg32: XOR(ireg32,CNSTI) */
  case 75: /* ireg16: XOR(ireg16,CNSTI) */
  case 74: /* ireg8: XOR(ireg8,CNSTI) */
  case 65: /* ireg64: OR(ireg64,CNSTI) */
  case 64: /* ireg32: OR(ireg32,CNSTI) */
  case 63: /* ireg16: OR(ireg16,CNSTI) */
  case 62: /* ireg8: OR(ireg8,CNSTI) */
  case 53: /* ireg64: SUB(ireg64,CNSTI) */
  case 52: /* ireg32: SUB(ireg32,CNSTI) */
  case 51: /* ireg16: SUB(ireg16,CNSTI) */
  case 50: /* ireg8: SUB(ireg8,CNSTI) */
  case 41: /* ireg64: ADD(ireg64,CNSTI) */
  case 40: /* ireg32: ADD(ireg32,CNSTI) */
  case 39: /* ireg16: ADD(ireg16,CNSTI) */
  case 38: /* ireg8: ADD(ireg8,CNSTI) */
  case 8: /* stmt: STORE(ireg64,CNSTI) */
    kids[0] = burm_child(p,0);
    break;
  case 121: /* ireg64: ASHR(ireg64,imem64) */
  case 120: /* ireg32: ASHR(ireg32,imem32) */
  case 119: /* ireg16: ASHR(ireg16,imem16) */
  case 118: /* ireg8: ASHR(ireg8,imem8) */
  case 117: /* ireg64: ASHR(ireg64,ireg64) */
  case 116: /* ireg32: ASHR(ireg32,ireg32) */
  case 115: /* ireg16: ASHR(ireg16,ireg16) */
  case 114: /* ireg8: ASHR(ireg8,ireg8) */
  case 109: /* ireg64: LSHR(ireg64,imem64) */
  case 108: /* ireg32: LSHR(ireg32,imem32) */
  case 107: /* ireg16: LSHR(ireg16,imem16) */
  case 106: /* ireg8: LSHR(ireg8,imem8) */
  case 105: /* ireg64: LSHR(ireg64,ireg64) */
  case 104: /* ireg32: LSHR(ireg32,ireg32) */
  case 103: /* ireg16: LSHR(ireg16,ireg16) */
  case 102: /* ireg8: LSHR(ireg8,ireg8) */
  case 97: /* ireg64: SHL(ireg64,imem64) */
  case 96: /* ireg32: SHL(ireg32,imem32) */
  case 95: /* ireg16: SHL(ireg16,imem16) */
  case 94: /* ireg8: SHL(ireg8,imem8) */
  case 93: /* ireg64: SHL(ireg64,ireg64) */
  case 92: /* ireg32: SHL(ireg32,ireg32) */
  case 91: /* ireg16: SHL(ireg16,ireg16) */
  case 90: /* ireg8: SHL(ireg8,ireg8) */
  case 85: /* ireg64: MUL(ireg64,imem64) */
  case 84: /* ireg32: MUL(ireg32,imem32) */
  case 83: /* ireg16: MUL(ireg16,imem16) */
  case 82: /* ireg8: MUL(ireg8,imem8) */
  case 81: /* ireg64: MUL(ireg64,ireg64) */
  case 80: /* ireg32: MUL(ireg32,ireg32) */
  case 79: /* ireg16: MUL(ireg16,ireg16) */
  case 78: /* ireg8: MUL(ireg8,ireg8) */
  case 73: /* ireg64: XOR(ireg64,imem64) */
  case 72: /* ireg32: XOR(ireg32,imem32) */
  case 71: /* ireg16: XOR(ireg16,imem16) */
  case 70: /* ireg8: XOR(ireg8,imem8) */
  case 69: /* ireg64: XOR(ireg64,ireg64) */
  case 68: /* ireg32: XOR(ireg32,ireg32) */
  case 67: /* ireg16: XOR(ireg16,ireg16) */
  case 66: /* ireg8: XOR(ireg8,ireg8) */
  case 61: /* ireg64: OR(ireg64,imem64) */
  case 60: /* ireg32: OR(ireg32,imem32) */
  case 59: /* ireg16: OR(ireg16,imem16) */
  case 58: /* ireg8: OR(ireg8,imem8) */
  case 57: /* ireg64: OR(ireg64,ireg64) */
  case 56: /* ireg32: OR(ireg32,ireg32) */
  case 55: /* ireg16: OR(ireg16,ireg16) */
  case 54: /* ireg8: OR(ireg8,ireg8) */
  case 49: /* ireg64: SUB(ireg64,imem64) */
  case 48: /* ireg32: SUB(ireg32,imem32) */
  case 47: /* ireg16: SUB(ireg16,imem16) */
  case 46: /* ireg8: SUB(ireg8,imem8) */
  case 45: /* ireg64: SUB(ireg64,ireg64) */
  case 44: /* ireg32: SUB(ireg32,ireg32) */
  case 43: /* ireg16: SUB(ireg16,ireg16) */
  case 42: /* ireg8: SUB(ireg8,ireg8) */
  case 37: /* ireg64: ADD(ireg64,imem64) */
  case 36: /* ireg32: ADD(ireg32,imem32) */
  case 35: /* ireg16: ADD(ireg16,imem16) */
  case 34: /* ireg8: ADD(ireg8,imem8) */
  case 33: /* ireg64: ADD(ireg64,ireg64) */
  case 32: /* ireg32: ADD(ireg32,ireg32) */
  case 31: /* ireg16: ADD(ireg16,ireg16) */
  case 30: /* ireg8: ADD(ireg8,ireg8) */
  case 25: /* stmt: STORE(imem64,ireg64) */
  case 24: /* stmt: STORE(imem32,ireg32) */
  case 23: /* stmt: STORE(imem16,ireg16) */
  case 22: /* stmt: STORE(imem8,ireg8) */
  case 16: /* stmt: STORE(ireg64,ireg64) */
  case 15: /* stmt: STORE(ireg64,ireg32) */
  case 14: /* stmt: STORE(ireg64,ireg16) */
  case 13: /* stmt: STORE(ireg64,ireg8) */
    kids[0] = burm_child(p,0);
    kids[1] = burm_child(p,1);
    break;
  case 21: /* stmt: STORE(MEM64,ireg64) */
  case 20: /* stmt: STORE(MEM32,ireg64) */
  case 19: /* stmt: STORE(MEM32,ireg32) */
  case 18: /* stmt: STORE(MEM16,ireg16) */
  case 17: /* stmt: STORE(MEM8,ireg8) */
    kids[0] = burm_child(p,1);
    break;
  default:
    burm_assert(0, PANIC("Bad external rule number %d in burm_kids\n", eruleno));
  }
  return kids;
}

void dumpCover(NODEPTR p, int goalnt, int indent)
{
  int eruleno = burm_rule(STATE_LABEL(p), goalnt);
  short *nts = burm_nts[eruleno];
  NODEPTR kids[10];
  int i;

  fprintf(stderr, "\t\t");
  for (i = 0; i < indent; i++)
    fprintf(stderr, " ");
  fprintf(stderr, "%s\n", burm_string[eruleno]);
  burm_kids(p, eruleno, kids);
  for (i = 0; nts[i]; i++)
    dumpCover(kids[i], nts[i], indent + 1);
}

# line 3314 "x86.brg"


    static int trace;

    /* burm_trace - print trace message for matching p; decrement trace */
    static void burm_trace(NODEPTR p, int eruleno, int cost, int bestcost) {
        if (trace < 0)
            fprintf(stderr, "0x%x matched %s = %d with cost %d vs. %d\n", p,
                burm_string[eruleno], eruleno, cost, bestcost);
        else if (trace > 0 && cost < bestcost) {
            --trace;
            fprintf(stderr, "0x%x matched %s = %d with cost %d\n", p,
                    burm_string[eruleno], eruleno, cost);
        }
    }

    static void gen(NODEPTR p) {
        if (burm_label(p) == 0)
            fprintf(stderr, "no cover\n");
        else {
            stmt_action(p->x.state, 0);
            dumpCover(p, 1, 0);
        }
    }

    static Tree tree(int op, Tree l, Tree r) {
        Tree t = (Tree) malloc(sizeof *t);

        t->op = op;
        t->kids[0] = l;
        t->kids[1] = r;
        t->val = 0;
        t->x.state = 0;
        return t;
    }

    //main(void) {
    //	Tree t;
    //
    //	if (getenv("Trace"))
    //		trace = atoi(getenv("Trace"));
    //	printf("i = c + 4;\n");
    //	t = tree(STORE,
    //        tree(MEM,0,0),
    //        (t = tree(CNSTI,0,0), t->val =4, t));
    ////  t = tree(ASGNI,
    ////		tree(ADDRLP, 0, 0),
    ////		tree(ADDI,
    ////			tree(CVCI, tree(INDIRC, tree(ADDRLP, 0, 0), 0), 0),
    ////			(t = tree(CNSTI, 0, 0), t->val = 4, t)
    ////		)
    ////	);
    //	gen(t);
    //	return 0;
    //}
