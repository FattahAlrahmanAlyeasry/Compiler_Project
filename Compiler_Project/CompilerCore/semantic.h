#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symboltable.h"
#include "parsetree.h"

// أنواع الأخطاء الدلالية
typedef enum {
    SEMANTIC_ERROR_UNDECLARED_SYMBOL,
    SEMANTIC_ERROR_REDECLARED_SYMBOL,
    SEMANTIC_ERROR_TYPE_MISMATCH,
    SEMANTIC_ERROR_INVALID_OPERATION,
    SEMANTIC_ERROR_INVALID_ASSIGNMENT,
    SEMANTIC_ERROR_INVALID_FUNCTION_CALL,
    SEMANTIC_ERROR_INVALID_ARRAY_ACCESS,
    SEMANTIC_ERROR_INVALID_RECORD_ACCESS,
    SEMANTIC_ERROR_INVALID_LOOP_CONTROL,
    SEMANTIC_ERROR_INVALID_RETURN_TYPE
} SemanticErrorType;

// هيكل الخطأ الدلالي
typedef struct {
    SemanticErrorType type;
    int line;
    int column;
    char* message;
    char* symbol_name;
} SemanticError;

// المحلل الدلالي
typedef struct {
    SymbolTable* symbol_table;
    ASTNode* ast;
    ParseTreeNode* parse_tree;
    SemanticError* errors;
    int error_count;
    int max_errors;
    int current_scope_level;
} SemanticAnalyzer;

// دوال تهيئة وتحرير الذاكرة
SemanticAnalyzer* initSemanticAnalyzer(ParseTreeNode* parse_tree);
void freeSemanticAnalyzer(SemanticAnalyzer* analyzer);

// دوال التحليل الدلالي الرئيسية
int analyzeProgram(SemanticAnalyzer* analyzer);
int analyzeBlock(SemanticAnalyzer* analyzer, ParseTreeNode* block_node);
int analyzeDefinitionsPart(SemanticAnalyzer* analyzer, ParseTreeNode* def_node);
int analyzeStatementsList(SemanticAnalyzer* analyzer, ParseTreeNode* stmt_node);

// دوال تحليل التعريفات
int analyzeConstantsDefinition(SemanticAnalyzer* analyzer, ParseTreeNode* const_node);
int analyzeTypesDefinition(SemanticAnalyzer* analyzer, ParseTreeNode* type_node);
int analyzeVariablesDefinition(SemanticAnalyzer* analyzer, ParseTreeNode* var_node);
int analyzeProceduresDefinition(SemanticAnalyzer* analyzer, ParseTreeNode* proc_node);

// دوال تحليل العبارات
int analyzeStatement(SemanticAnalyzer* analyzer, ParseTreeNode* stmt_node);
int analyzeAssignmentStatement(SemanticAnalyzer* analyzer, ParseTreeNode* assign_node);
int analyzeIOStatement(SemanticAnalyzer* analyzer, ParseTreeNode* io_node);
int analyzeConditionalStatement(SemanticAnalyzer* analyzer, ParseTreeNode* cond_node);
int analyzeLoopStatement(SemanticAnalyzer* analyzer, ParseTreeNode* loop_node);
int analyzeProcedureCall(SemanticAnalyzer* analyzer, ParseTreeNode* call_node);
int analyzeCompoundStatement(SemanticAnalyzer* analyzer, ParseTreeNode* compound_node);

// دوال تحليل التعبيرات
int analyzeExpression(SemanticAnalyzer* analyzer, ParseTreeNode* expr_node);
int analyzeArithmeticExpression(SemanticAnalyzer* analyzer, ParseTreeNode* arith_node);
int analyzeRelationalExpression(SemanticAnalyzer* analyzer, ParseTreeNode* rel_node);
int analyzeLogicalExpression(SemanticAnalyzer* analyzer, ParseTreeNode* logic_node);
int analyzeIdentifier(SemanticAnalyzer* analyzer, ParseTreeNode* id_node);
int analyzeLiteral(SemanticAnalyzer* analyzer, ParseTreeNode* lit_node);

// دوال فحص الأنواع
int checkTypeCompatibility(DataType type1, DataType type2, const char* operation);
int checkAssignmentCompatibility(DataType target_type, DataType source_type);
int checkArithmeticOperation(DataType type1, DataType type2, const char* operation);
int checkRelationalOperation(DataType type1, DataType type2);
int checkLogicalOperation(DataType type1, DataType type2);

// دوال إدارة الأخطاء
void semanticError(SemanticAnalyzer* analyzer, SemanticErrorType type, int line, int column, const char* message, const char* symbol_name);
void addSemanticError(SemanticAnalyzer* analyzer, SemanticError error);
int getSemanticErrorCount(SemanticAnalyzer* analyzer);
SemanticError* getSemanticErrors(SemanticAnalyzer* analyzer);

// دوال بناء AST
ASTNode* buildASTFromParseTree(SemanticAnalyzer* analyzer);
ASTNode* buildASTNode(SemanticAnalyzer* analyzer, ParseTreeNode* parse_node);
ASTNode* buildProgramNode(SemanticAnalyzer* analyzer, ParseTreeNode* program_node);
ASTNode* buildBlockNode(SemanticAnalyzer* analyzer, ParseTreeNode* block_node);
ASTNode* buildStatementNode(SemanticAnalyzer* analyzer, ParseTreeNode* stmt_node);
ASTNode* buildExpressionNode(SemanticAnalyzer* analyzer, ParseTreeNode* expr_node);
ASTNode* buildDeclarationNode(SemanticAnalyzer* analyzer, ParseTreeNode* decl_node);

// دوال مساعدة
DataType getNodeDataType(ParseTreeNode* node);
int isConstantExpression(ParseTreeNode* expr_node);
int isLValue(ParseTreeNode* node);
int validateArrayAccess(SemanticAnalyzer* analyzer, ParseTreeNode* array_node);
int validateRecordAccess(SemanticAnalyzer* analyzer, ParseTreeNode* record_node);

#endif // SEMANTIC_H

