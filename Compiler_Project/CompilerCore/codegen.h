#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symboltable.h"

// أنواع التسجيلات في MIPS
typedef enum {
    REG_ZERO,    // $zero - دائماً صفر
    REG_AT,      // $at - مؤقت للتجميع
    REG_V0,      // $v0 - قيمة الإرجاع
    REG_V1,      // $v1 - قيمة إرجاع إضافية
    REG_A0,      // $a0 - معامل 1
    REG_A1,      // $a1 - معامل 2
    REG_A2,      // $a2 - معامل 3
    REG_A3,      // $a3 - معامل 4
    REG_T0,      // $t0 - مؤقت
    REG_T1,      // $t1 - مؤقت
    REG_T2,      // $t2 - مؤقت
    REG_T3,      // $t3 - مؤقت
    REG_T4,      // $t4 - مؤقت
    REG_T5,      // $t5 - مؤقت
    REG_T6,      // $t6 - مؤقت
    REG_T7,      // $t7 - مؤقت
    REG_S0,      // $s0 - محفوظ
    REG_S1,      // $s1 - محفوظ
    REG_S2,      // $s2 - محفوظ
    REG_S3,      // $s3 - محفوظ
    REG_S4,      // $s4 - محفوظ
    REG_S5,      // $s5 - محفوظ
    REG_S6,      // $s6 - محفوظ
    REG_S7,      // $s7 - محفوظ
    REG_T8,      // $t8 - مؤقت
    REG_T9,      // $t9 - مؤقت
    REG_K0,      // $k0 - مؤقت للنواة
    REG_K1,      // $k1 - مؤقت للنواة
    REG_GP,      // $gp - مؤشر عام
    REG_SP,      // $sp - مؤشر المكدس
    REG_FP,      // $fp - مؤشر الإطار
    REG_RA       // $ra - عنوان الإرجاع
} MIPSRegister;

// أنواع التعليمات MIPS
typedef enum {
    INSTR_ADD,      // add
    INSTR_ADDI,     // addi
    INSTR_ADDU,     // addu
    INSTR_ADDIU,    // addiu
    INSTR_SUB,      // sub
    INSTR_SUBU,     // subu
    INSTR_MULT,     // mult
    INSTR_DIV,      // div
    INSTR_MFLO,     // mflo
    INSTR_MFHI,     // mfhi
    INSTR_AND,      // and
    INSTR_ORI,      // ori
    INSTR_XOR,      // xor
    INSTR_SLL,      // sll
    INSTR_SRL,      // srl
    INSTR_SLT,      // slt
    INSTR_SLTI,     // slti
    INSTR_BEQ,      // beq
    INSTR_BNE,      // bne
    INSTR_BLT,      // blt
    INSTR_BLE,      // ble
    INSTR_BGT,      // bgt
    INSTR_BGE,      // bge
    INSTR_J,        // j
    INSTR_JAL,      // jal
    INSTR_JR,       // jr
    INSTR_LW,       // lw
    INSTR_SW,       // sw
    INSTR_LI,       // li
    INSTR_MOVE,     // move
    INSTR_SYSCALL,  // syscall
    INSTR_LABEL     // تسمية
} MIPSInstructionType;

// تعليمة MIPS
typedef struct {
    MIPSInstructionType type;
    MIPSRegister rd;      // سجل الوجهة
    MIPSRegister rs;      // سجل المصدر 1
    MIPSRegister rt;      // سجل المصدر 2
    int immediate;        // قيمة فورية
    wchar_t* label;       // تسمية
    wchar_t* comment;     // تعليق
} MIPSInstruction;

// مولد الكود MIPS
typedef struct {
    MIPSInstruction* instructions;
    int instruction_count;
    int instruction_capacity;
    SymbolTable* symbol_table;
    int current_label;
    int stack_offset;
    int data_section_size;
    wchar_t** string_literals;
    int string_count;
    int string_capacity;
} CodeGenerator;

// دوال مولد الكود
CodeGenerator* initCodeGenerator(SymbolTable* symbol_table);
void freeCodeGenerator(CodeGenerator* generator);
int generateCode(CodeGenerator* generator, ASTNode* ast);
void emit(CodeGenerator* generator, MIPSInstruction instruction);
void emitLabel(CodeGenerator* generator, wchar_t* label);
void emitComment(CodeGenerator* generator, wchar_t* comment);

// دوال توليد الكود لأنواع معينة من العقد
int generateProgram(CodeGenerator* generator, ASTNode* node);
int generateBlock(CodeGenerator* generator, ASTNode* node);
int generateStatement(CodeGenerator* generator, ASTNode* node);
int generateExpression(CodeGenerator* generator, ASTNode* node);
int generateDeclaration(CodeGenerator* generator, ASTNode* node);

// دوال إدارة التسجيلات
MIPSRegister allocateRegister(CodeGenerator* generator);
void freeRegister(CodeGenerator* generator, MIPSRegister reg);
int isRegisterFree(CodeGenerator* generator, MIPSRegister reg);
void saveRegisters(CodeGenerator* generator);
void restoreRegisters(CodeGenerator* generator);

// دوال توليد التعليمات
MIPSInstruction createRTypeInstruction(MIPSInstructionType type, MIPSRegister rd, MIPSRegister rs, MIPSRegister rt);
MIPSInstruction createITypeInstruction(MIPSInstructionType type, MIPSRegister rs, MIPSRegister rt, int immediate);
MIPSInstruction createJTypeInstruction(MIPSInstructionType type, wchar_t* label);
MIPSInstruction createLoadInstruction(MIPSRegister rt, int offset, MIPSRegister base);
MIPSInstruction createStoreInstruction(MIPSRegister rt, int offset, MIPSRegister base);

// دوال إدارة الذاكرة
int allocateStackSpace(CodeGenerator* generator, int size);
void deallocateStackSpace(CodeGenerator* generator, int size);
int getVariableOffset(CodeGenerator* generator, wchar_t* variable_name);
int getArrayElementOffset(CodeGenerator* generator, wchar_t* array_name, int index);

// دوال توليد البيانات
void generateDataSection(CodeGenerator* generator);
void addStringLiteral(CodeGenerator* generator, wchar_t* string);
void generateVariableDeclarations(CodeGenerator* generator);

// دوال مساعدة
wchar_t* getRegisterName(MIPSRegister reg);
wchar_t* getInstructionName(MIPSInstructionType type);
void printMIPSCode(CodeGenerator* generator);
int saveMIPSCodeToFile(CodeGenerator* generator, const char* filename);
void codeGenError(CodeGenerator* generator, const wchar_t* message);

// دوال توليد التعليمات الخاصة
int generateArithmeticOperation(CodeGenerator* generator, ASTNode* node);
int generateComparisonOperation(CodeGenerator* generator, ASTNode* node);
int generateLogicalOperation(CodeGenerator* generator, ASTNode* node);
int generateControlFlow(CodeGenerator* generator, ASTNode* node);
int generateFunctionCall(CodeGenerator* generator, ASTNode* node);

#endif // CODEGEN_H

