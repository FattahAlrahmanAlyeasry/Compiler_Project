#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// تهيئة المحلل الدلالي
SemanticAnalyzer* initSemanticAnalyzer(ParseTreeNode* parse_tree) {
    SemanticAnalyzer* analyzer = (SemanticAnalyzer*)malloc(sizeof(SemanticAnalyzer));
    if (!analyzer) {
        return NULL;
    }
    
    analyzer->symbol_table = initSymbolTable();
    if (!analyzer->symbol_table) {
        free(analyzer);
        return NULL;
    }
    
    analyzer->parse_tree = parse_tree;
    analyzer->ast = NULL;
    analyzer->errors = (SemanticError*)malloc(100 * sizeof(SemanticError));
    analyzer->error_count = 0;
    analyzer->max_errors = 100;
    analyzer->current_scope_level = 0;
    
    return analyzer;
}

// تحرير ذاكرة المحلل الدلالي
void freeSemanticAnalyzer(SemanticAnalyzer* analyzer) {
    if (!analyzer) {
        return;
    }
    
    if (analyzer->symbol_table) {
        freeSymbolTable(analyzer->symbol_table);
    }
    
    if (analyzer->ast) {
        freeAST(analyzer->ast);
    }
    
    if (analyzer->errors) {
        for (int i = 0; i < analyzer->error_count; i++) {
            if (analyzer->errors[i].message) {
                free(analyzer->errors[i].message);
            }
            if (analyzer->errors[i].symbol_name) {
                free(analyzer->errors[i].symbol_name);
            }
        }
        free(analyzer->errors);
    }
    
    free(analyzer);
}

// تحليل البرنامج بالكامل
int analyzeProgram(SemanticAnalyzer* analyzer) {
    if (!analyzer || !analyzer->parse_tree) {
        return 0;
    }
    
    // البحث عن عقدة البرنامج
    ParseTreeNode* program_node = findNodeByType(analyzer->parse_tree, PARSE_NODE_PROGRAM);
    if (!program_node) {
        semanticError(analyzer, SEMANTIC_ERROR_INVALID_OPERATION, 1, 1, "عقدة البرنامج غير موجودة", NULL);
        return 0;
    }
    
    // البحث عن عقدة الكتلة
    ParseTreeNode* block_node = NULL;
    for (int i = 0; i < program_node->children_count; i++) {
        if (program_node->children[i]->type == PARSE_NODE_BLOCK) {
            block_node = program_node->children[i];
            break;
        }
    }
    
    if (!block_node) {
        semanticError(analyzer, SEMANTIC_ERROR_INVALID_OPERATION, 1, 1, "كتلة البرنامج غير موجودة", NULL);
        return 0;
    }
    
    // تحليل الكتلة
    return analyzeBlock(analyzer, block_node);
}

// تحليل الكتلة
int analyzeBlock(SemanticAnalyzer* analyzer, ParseTreeNode* block_node) {
    if (!analyzer || !block_node) {
        return 0;
    }
    
    int success = 1;
    
    // البحث عن جزء التعريفات
    ParseTreeNode* definitions_node = NULL;
    for (int i = 0; i < block_node->children_count; i++) {
        if (block_node->children[i]->type == PARSE_NODE_DEFINITIONS_PART) {
            definitions_node = block_node->children[i];
            break;
        }
    }
    
    if (definitions_node) {
        success &= analyzeDefinitionsPart(analyzer, definitions_node);
    }
    
    // البحث عن قائمة العبارات
    ParseTreeNode* statements_node = NULL;
    for (int i = 0; i < block_node->children_count; i++) {
        if (block_node->children[i]->type == PARSE_NODE_STATEMENTS_LIST) {
            statements_node = block_node->children[i];
            break;
        }
    }
    
    if (statements_node) {
        success &= analyzeStatementsList(analyzer, statements_node);
    }
    
    return success;
}

// تحليل جزء التعريفات
int analyzeDefinitionsPart(SemanticAnalyzer* analyzer, ParseTreeNode* def_node) {
    if (!analyzer || !def_node) {
        return 0;
    }
    
    int success = 1;
    
    for (int i = 0; i < def_node->children_count; i++) {
        ParseTreeNode* child = def_node->children[i];
        
        switch (child->type) {
            case PARSE_NODE_CONSTANTS_DEFINITION:
                success &= analyzeConstantsDefinition(analyzer, child);
                break;
            case PARSE_NODE_TYPES_DEFINITION:
                success &= analyzeTypesDefinition(analyzer, child);
                break;
            case PARSE_NODE_VARIABLES_DEFINITION:
                success &= analyzeVariablesDefinition(analyzer, child);
                break;
            case PARSE_NODE_PROCEDURES_DEFINITION:
                success &= analyzeProceduresDefinition(analyzer, child);
                break;
            default:
                break;
        }
    }
    
    return success;
}

// تحليل قائمة العبارات
int analyzeStatementsList(SemanticAnalyzer* analyzer, ParseTreeNode* stmt_node) {
    if (!analyzer || !stmt_node) {
        return 0;
    }
    
    int success = 1;
    
    for (int i = 0; i < stmt_node->children_count; i++) {
        ParseTreeNode* child = stmt_node->children[i];
        success &= analyzeStatement(analyzer, child);
    }
    
    return success;
}

// تحليل العبارة
int analyzeStatement(SemanticAnalyzer* analyzer, ParseTreeNode* stmt_node) {
    if (!analyzer || !stmt_node) {
        return 0;
    }
    
    switch (stmt_node->type) {
        case PARSE_NODE_ASSIGNMENT_STATEMENT:
            return analyzeAssignmentStatement(analyzer, stmt_node);
        case PARSE_NODE_IO_STATEMENT:
            return analyzeIOStatement(analyzer, stmt_node);
        case PARSE_NODE_CONDITIONAL_STATEMENT:
            return analyzeConditionalStatement(analyzer, stmt_node);
        case PARSE_NODE_LOOP_STATEMENT:
            return analyzeLoopStatement(analyzer, stmt_node);
        case PARSE_NODE_PROCEDURE_CALL:
            return analyzeProcedureCall(analyzer, stmt_node);
        case PARSE_NODE_COMPOUND_STATEMENT:
            return analyzeCompoundStatement(analyzer, stmt_node);
        case PARSE_NODE_EMPTY_STATEMENT:
            return 1; // العبارة الفارغة صحيحة دائماً
        default:
            semanticError(analyzer, SEMANTIC_ERROR_INVALID_OPERATION, stmt_node->line, stmt_node->column, 
                         "نوع عبارة غير معروف", NULL);
            return 0;
    }
}

// تحليل عبارة الإسناد
int analyzeAssignmentStatement(SemanticAnalyzer* analyzer, ParseTreeNode* assign_node) {
    if (!analyzer || !assign_node || assign_node->children_count < 2) {
        return 0;
    }
    
    ParseTreeNode* target = assign_node->children[0];
    ParseTreeNode* expression = assign_node->children[1];
    
    // التحقق من أن الهدف هو معرف
    if (target->type != PARSE_NODE_IDENTIFIER) {
        semanticError(analyzer, SEMANTIC_ERROR_INVALID_ASSIGNMENT, target->line, target->column,
                     "الهدف يجب أن يكون معرف", NULL);
        return 0;
    }
    
    // البحث عن المتغير في جدول الرموز
    SymbolEntry* symbol = lookupSymbol(analyzer->symbol_table, target->value);
    if (!symbol) {
        semanticError(analyzer, SEMANTIC_ERROR_UNDECLARED_SYMBOL, target->line, target->column,
                     "المتغير غير معرف", target->value);
        return 0;
    }
    
    // تحليل التعبير
    int expr_success = analyzeExpression(analyzer, expression);
    if (!expr_success) {
        return 0;
    }
    
    // التحقق من توافق الأنواع
    DataType target_type = symbol->data_type;
    DataType expr_type = getNodeDataType(expression);
    
    if (!checkAssignmentCompatibility(target_type, expr_type)) {
        semanticError(analyzer, SEMANTIC_ERROR_TYPE_MISMATCH, assign_node->line, assign_node->column,
                     "عدم توافق الأنواع في الإسناد", target->value);
        return 0;
    }
    
    return 1;
}

// تحليل عبارة الإدخال/الإخراج
int analyzeIOStatement(SemanticAnalyzer* analyzer, ParseTreeNode* io_node) {
    if (!analyzer || !io_node || io_node->children_count < 1) {
        return 0;
    }
    
    ParseTreeNode* target = io_node->children[0];
    
    // التحقق من أن الهدف هو معرف
    if (target->type != PARSE_NODE_IDENTIFIER) {
        semanticError(analyzer, SEMANTIC_ERROR_INVALID_OPERATION, target->line, target->column,
                     "هدف الإدخال/الإخراج يجب أن يكون معرف", NULL);
        return 0;
    }
    
    // البحث عن المتغير في جدول الرموز
    SymbolEntry* symbol = lookupSymbol(analyzer->symbol_table, target->value);
    if (!symbol) {
        semanticError(analyzer, SEMANTIC_ERROR_UNDECLARED_SYMBOL, target->line, target->column,
                     "المتغير غير معرف", target->value);
        return 0;
    }
    
    return 1;
}

// تحليل العبارة الشرطية
int analyzeConditionalStatement(SemanticAnalyzer* analyzer, ParseTreeNode* cond_node) {
    if (!analyzer || !cond_node || cond_node->children_count < 2) {
        return 0;
    }
    
    ParseTreeNode* condition = cond_node->children[0];
    ParseTreeNode* then_part = cond_node->children[1];
    ParseTreeNode* else_part = (cond_node->children_count > 2) ? cond_node->children[2] : NULL;
    
    // تحليل الشرط
    int cond_success = analyzeExpression(analyzer, condition);
    if (!cond_success) {
        return 0;
    }
    
    // التحقق من أن الشرط من نوع منطقي
    DataType cond_type = getNodeDataType(condition);
    if (cond_type != TYPE_BOOL) {
        semanticError(analyzer, SEMANTIC_ERROR_TYPE_MISMATCH, condition->line, condition->column,
                     "الشرط يجب أن يكون من نوع منطقي", NULL);
        return 0;
    }
    
    // تحليل الجزء الأول
    int then_success = analyzeStatement(analyzer, then_part);
    if (!then_success) {
        return 0;
    }
    
    // تحليل الجزء الثاني (إن وجد)
    if (else_part) {
        int else_success = analyzeStatement(analyzer, else_part);
        if (!else_success) {
            return 0;
        }
    }
    
    return 1;
}

// تحليل عبارة الحلقة
int analyzeLoopStatement(SemanticAnalyzer* analyzer, ParseTreeNode* loop_node) {
    if (!analyzer || !loop_node || loop_node->children_count < 2) {
        return 0;
    }
    
    ParseTreeNode* condition = loop_node->children[0];
    ParseTreeNode* body = loop_node->children[1];
    
    // تحليل الشرط
    int cond_success = analyzeExpression(analyzer, condition);
    if (!cond_success) {
        return 0;
    }
    
    // التحقق من أن الشرط من نوع منطقي
    DataType cond_type = getNodeDataType(condition);
    if (cond_type != TYPE_BOOL) {
        semanticError(analyzer, SEMANTIC_ERROR_TYPE_MISMATCH, condition->line, condition->column,
                     "شرط الحلقة يجب أن يكون من نوع منطقي", NULL);
        return 0;
    }
    
    // تحليل جسم الحلقة
    int body_success = analyzeStatement(analyzer, body);
    if (!body_success) {
        return 0;
    }
    
    return 1;
}

// تحليل استدعاء الإجراء
int analyzeProcedureCall(SemanticAnalyzer* analyzer, ParseTreeNode* call_node) {
    if (!analyzer || !call_node || call_node->children_count < 1) {
        return 0;
    }
    
    ParseTreeNode* proc_name = call_node->children[0];
    
    // التحقق من أن اسم الإجراء هو معرف
    if (proc_name->type != PARSE_NODE_IDENTIFIER) {
        semanticError(analyzer, SEMANTIC_ERROR_INVALID_FUNCTION_CALL, proc_name->line, proc_name->column,
                     "اسم الإجراء يجب أن يكون معرف", NULL);
        return 0;
    }
    
    // البحث عن الإجراء في جدول الرموز
    SymbolEntry* symbol = lookupSymbol(analyzer->symbol_table, proc_name->value);
    if (!symbol) {
        semanticError(analyzer, SEMANTIC_ERROR_UNDECLARED_SYMBOL, proc_name->line, proc_name->column,
                     "الإجراء غير معرف", proc_name->value);
        return 0;
    }
    
    // التحقق من أن الرمز هو إجراء
    if (symbol->type != SYMBOL_PROCEDURE) {
        semanticError(analyzer, SEMANTIC_ERROR_INVALID_FUNCTION_CALL, proc_name->line, proc_name->column,
                     "الرمز ليس إجراء", proc_name->value);
        return 0;
    }
    
    return 1;
}

// تحليل العبارة المركبة
int analyzeCompoundStatement(SemanticAnalyzer* analyzer, ParseTreeNode* compound_node) {
    if (!analyzer || !compound_node) {
        return 0;
    }
    
    int success = 1;
    
    for (int i = 0; i < compound_node->children_count; i++) {
        ParseTreeNode* child = compound_node->children[i];
        success &= analyzeStatement(analyzer, child);
    }
    
    return success;
}

// تحليل التعبير
int analyzeExpression(SemanticAnalyzer* analyzer, ParseTreeNode* expr_node) {
    if (!analyzer || !expr_node) {
        return 0;
    }
    
    switch (expr_node->type) {
        case PARSE_NODE_ARITHMETIC_EXPRESSION:
            return analyzeArithmeticExpression(analyzer, expr_node);
        case PARSE_NODE_RELATIONAL_EXPRESSION:
            return analyzeRelationalExpression(analyzer, expr_node);
        case PARSE_NODE_LOGICAL_EXPRESSION:
            return analyzeLogicalExpression(analyzer, expr_node);
        case PARSE_NODE_IDENTIFIER:
            return analyzeIdentifier(analyzer, expr_node);
        case PARSE_NODE_LITERAL:
            return analyzeLiteral(analyzer, expr_node);
        default:
            semanticError(analyzer, SEMANTIC_ERROR_INVALID_OPERATION, expr_node->line, expr_node->column,
                         "نوع تعبير غير معروف", NULL);
            return 0;
    }
}

// تحليل التعبير الحسابي
int analyzeArithmeticExpression(SemanticAnalyzer* analyzer, ParseTreeNode* arith_node) {
    if (!analyzer || !arith_node || arith_node->children_count < 2) {
        return 0;
    }
    
    ParseTreeNode* left = arith_node->children[0];
    ParseTreeNode* right = arith_node->children[1];
    
    // تحليل الطرف الأيسر
    int left_success = analyzeExpression(analyzer, left);
    if (!left_success) {
        return 0;
    }
    
    // تحليل الطرف الأيمن
    int right_success = analyzeExpression(analyzer, right);
    if (!right_success) {
        return 0;
    }
    
    // التحقق من توافق الأنواع
    DataType left_type = getNodeDataType(left);
    DataType right_type = getNodeDataType(right);
    
    if (!checkArithmeticOperation(left_type, right_type, "عملية حسابية")) {
        semanticError(analyzer, SEMANTIC_ERROR_TYPE_MISMATCH, arith_node->line, arith_node->column,
                     "عدم توافق الأنواع في العملية الحسابية", NULL);
        return 0;
    }
    
    return 1;
}

// تحليل التعبير العلائقي
int analyzeRelationalExpression(SemanticAnalyzer* analyzer, ParseTreeNode* rel_node) {
    if (!analyzer || !rel_node || rel_node->children_count < 2) {
        return 0;
    }
    
    ParseTreeNode* left = rel_node->children[0];
    ParseTreeNode* right = rel_node->children[1];
    
    // تحليل الطرف الأيسر
    int left_success = analyzeExpression(analyzer, left);
    if (!left_success) {
        return 0;
    }
    
    // تحليل الطرف الأيمن
    int right_success = analyzeExpression(analyzer, right);
    if (!right_success) {
        return 0;
    }
    
    // التحقق من توافق الأنواع
    DataType left_type = getNodeDataType(left);
    DataType right_type = getNodeDataType(right);
    
    if (!checkRelationalOperation(left_type, right_type)) {
        semanticError(analyzer, SEMANTIC_ERROR_TYPE_MISMATCH, rel_node->line, rel_node->column,
                     "عدم توافق الأنواع في العملية العلائقية", NULL);
        return 0;
    }
    
    return 1;
}

// تحليل التعبير المنطقي
int analyzeLogicalExpression(SemanticAnalyzer* analyzer, ParseTreeNode* logic_node) {
    if (!analyzer || !logic_node || logic_node->children_count < 2) {
        return 0;
    }
    
    ParseTreeNode* left = logic_node->children[0];
    ParseTreeNode* right = logic_node->children[1];
    
    // تحليل الطرف الأيسر
    int left_success = analyzeExpression(analyzer, left);
    if (!left_success) {
        return 0;
    }
    
    // تحليل الطرف الأيمن
    int right_success = analyzeExpression(analyzer, right);
    if (!right_success) {
        return 0;
    }
    
    // التحقق من توافق الأنواع
    DataType left_type = getNodeDataType(left);
    DataType right_type = getNodeDataType(right);
    
    if (!checkLogicalOperation(left_type, right_type)) {
        semanticError(analyzer, SEMANTIC_ERROR_TYPE_MISMATCH, logic_node->line, logic_node->column,
                     "عدم توافق الأنواع في العملية المنطقية", NULL);
        return 0;
    }
    
    return 1;
}

// تحليل المعرف
int analyzeIdentifier(SemanticAnalyzer* analyzer, ParseTreeNode* id_node) {
    if (!analyzer || !id_node) {
        return 0;
    }
    
    // البحث عن الرمز في جدول الرموز
    SymbolEntry* symbol = lookupSymbol(analyzer->symbol_table, id_node->value);
    if (!symbol) {
        semanticError(analyzer, SEMANTIC_ERROR_UNDECLARED_SYMBOL, id_node->line, id_node->column,
                     "المعرف غير معرف", id_node->value);
        return 0;
    }
    
    return 1;
}

// تحليل الثابت
int analyzeLiteral(SemanticAnalyzer* analyzer, ParseTreeNode* lit_node) {
    if (!analyzer || !lit_node) {
        return 0;
    }
    
    // الثوابت صحيحة دائماً
    return 1;
}

// فحص توافق الأنواع
int checkTypeCompatibility(DataType type1, DataType type2, const char* operation) {
    // الأنواع المتطابقة متوافقة دائماً
    if (type1 == type2) {
        return 1;
    }
    
    // التحويلات الآمنة
    if (type1 == TYPE_INT && type2 == TYPE_FLOAT) {
        return 1; // int -> float
    }
    
    if (type1 == TYPE_CHAR && type2 == TYPE_INT) {
        return 1; // char -> int
    }
    
    return 0;
}

// فحص توافق الإسناد
int checkAssignmentCompatibility(DataType target_type, DataType source_type) {
    return checkTypeCompatibility(target_type, source_type, "إسناد");
}

// فحص العمليات الحسابية
int checkArithmeticOperation(DataType type1, DataType type2, const char* operation) {
    // العمليات الحسابية تدعم الأنواع الرقمية
    if ((type1 == TYPE_INT || type1 == TYPE_FLOAT) && 
        (type2 == TYPE_INT || type2 == TYPE_FLOAT)) {
        return 1;
    }
    
    return 0;
}

// فحص العمليات العلائقية
int checkRelationalOperation(DataType type1, DataType type2) {
    // العمليات العلائقية تدعم الأنواع الرقمية
    if ((type1 == TYPE_INT || type1 == TYPE_FLOAT) && 
        (type2 == TYPE_INT || type2 == TYPE_FLOAT)) {
        return 1;
    }
    
    return 0;
}

// فحص العمليات المنطقية
int checkLogicalOperation(DataType type1, DataType type2) {
    // العمليات المنطقية تدعم الأنواع المنطقية فقط
    return (type1 == TYPE_BOOL && type2 == TYPE_BOOL);
}

// الإبلاغ عن خطأ دلالي
void semanticError(SemanticAnalyzer* analyzer, SemanticErrorType type, int line, int column, 
                   const char* message, const char* symbol_name) {
    if (!analyzer || analyzer->error_count >= analyzer->max_errors) {
        return;
    }
    
    SemanticError* error = &analyzer->errors[analyzer->error_count];
    error->type = type;
    error->line = line;
    error->column = column;
    
    if (message) {
        error->message = strdup(message);
    } else {
        error->message = NULL;
    }
    
    if (symbol_name) {
        error->symbol_name = strdup(symbol_name);
    } else {
        error->symbol_name = NULL;
    }
    
    analyzer->error_count++;
    
    // طباعة الخطأ
    printf("خطأ دلالي في السطر %d، العمود %d: %s\n", line, column, message ? message : "");
}

// الحصول على عدد الأخطاء الدلالية
int getSemanticErrorCount(SemanticAnalyzer* analyzer) {
    return analyzer ? analyzer->error_count : 0;
}

// الحصول على الأخطاء الدلالية
SemanticError* getSemanticErrors(SemanticAnalyzer* analyzer) {
    return analyzer ? analyzer->errors : NULL;
}

// الحصول على نوع البيانات من عقدة
DataType getNodeDataType(ParseTreeNode* node) {
    if (!node) {
        return TYPE_VOID;
    }
    
    switch (node->type) {
        case PARSE_NODE_LITERAL:
            // تحديد نوع الثابت من قيمته
            if (node->value) {
                if (wcschr(node->value, L'.')) {
                    return TYPE_FLOAT;
                } else if (wcscmp(node->value, L"صحيح") == 0 || 
                          wcscmp(node->value, L"خطأ") == 0) {
                    return TYPE_BOOL;
                } else if (node->value[0] == L'"' || node->value[0] == L'\'') {
                    return TYPE_STRING;
                } else {
                    return TYPE_INT;
                }
            }
            return TYPE_INT;
        case PARSE_NODE_IDENTIFIER:
            // يجب البحث في جدول الرموز
            return TYPE_VOID;
        default:
            return TYPE_VOID;
    }
}

// بناء AST من شجرة الإعراب
ASTNode* buildASTFromParseTree(SemanticAnalyzer* analyzer) {
    if (!analyzer || !analyzer->parse_tree) {
        return NULL;
    }
    
    analyzer->ast = buildAST(analyzer->parse_tree);
    return analyzer->ast;
}

