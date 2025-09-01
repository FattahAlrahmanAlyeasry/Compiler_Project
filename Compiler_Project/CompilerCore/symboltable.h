#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "ast.h"

// أنواع الرموز
typedef enum {
    SYMBOL_CONSTANT,
    SYMBOL_VARIABLE,
    SYMBOL_PROCEDURE,
    SYMBOL_TYPE,
    SYMBOL_PARAMETER
} SymbolType;

// دخول في جدول الرموز
typedef struct SymbolEntry {
    wchar_t* name;
    SymbolType type;
    DataType data_type;
    int scope_level;
    int line_number;
    int column_number;
    
    // بيانات خاصة بكل نوع من الرموز
    union {
        // للثوابت
        struct {
            wchar_t* value;
        } constant;
        
        // للمتغيرات
        struct {
            int array_size;
            int offset;
            wchar_t** field_names;
            DataType* field_types;
            int field_count;
        } variable;
        
        // للإجراءات
        struct {
            int parameter_count;
            wchar_t** parameter_names;
            DataType* parameter_types;
            int local_variable_count;
            int total_size;
        } procedure;
        
        // للأنواع
        struct {
            int size;
            wchar_t** field_names;
            DataType* field_types;
            int field_count;
        } type_info;
        
        // للمعاملات
        struct {
            int parameter_index;
            int offset;
        } parameter;
    } data;
    
    struct SymbolEntry* next; // للتعامل مع التصادمات
} SymbolEntry;

// نطاق
typedef struct Scope {
    int level;
    wchar_t* name;
    SymbolEntry** symbols;
    int symbol_count;
    int symbol_capacity;
    struct Scope* parent;
    struct Scope* children;
    struct Scope* next_sibling;
} Scope;

// جدول الرموز
typedef struct {
    Scope* global_scope;
    Scope* current_scope;
    int current_level;
    int error_count;
} SymbolTable;

// دوال جدول الرموز
SymbolTable* initSymbolTable();
void freeSymbolTable(SymbolTable* table);
int enterScope(SymbolTable* table, wchar_t* scope_name);
int exitScope(SymbolTable* table);
int addSymbol(SymbolTable* table, SymbolEntry* symbol);
SymbolEntry* lookupSymbol(SymbolTable* table, wchar_t* name);
SymbolEntry* lookupSymbolInScope(SymbolTable* table, wchar_t* name, int scope_level);
void symbolTableError(SymbolTable* table, const wchar_t* message, int line, int column);

// دوال إنشاء الرموز
SymbolEntry* createConstantSymbol(wchar_t* name, wchar_t* value, DataType type, int line, int col);
SymbolEntry* createVariableSymbol(wchar_t* name, DataType type, int array_size, int line, int col);
SymbolEntry* createProcedureSymbol(wchar_t* name, int line, int col);
SymbolEntry* createTypeSymbol(wchar_t* name, DataType type, int line, int col);
SymbolEntry* createParameterSymbol(wchar_t* name, DataType type, int index, int line, int col);

// دوال مساعدة
void freeSymbolEntry(SymbolEntry* symbol);
void printSymbolTable(SymbolTable* table);
void printScope(Scope* scope, int depth);
int getSymbolSize(SymbolEntry* symbol);
int calculateTypeSize(DataType type, int array_size);

// دوال إدارة النطاقات
Scope* createScope(int level, wchar_t* name);
void addScopeChild(Scope* parent, Scope* child);
void freeScope(Scope* scope);
int getScopeLevel(SymbolTable* table);

// دوال التحقق من صحة الرموز
int checkSymbolRedeclaration(SymbolTable* table, wchar_t* name);
int checkSymbolUsage(SymbolTable* table, wchar_t* name);
int checkTypeCompatibility(SymbolTable* table, DataType type1, DataType type2);
int checkArrayBounds(SymbolTable* table, wchar_t* array_name, int index);

#endif // SYMBOLTABLE_H

