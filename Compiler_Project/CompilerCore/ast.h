#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "parser.h"

// أنواع عقد شجرة البنية المجردة
typedef enum {
    AST_PROGRAM,
    AST_BLOCK,
    AST_CONSTANT_DECLARATION,
    AST_TYPE_DECLARATION,
    AST_VARIABLE_DECLARATION,
    AST_PROCEDURE_DECLARATION,
    AST_ASSIGNMENT_STATEMENT,
    AST_INPUT_STATEMENT,
    AST_OUTPUT_STATEMENT,
    AST_IF_STATEMENT,
    AST_WHILE_STATEMENT,
    AST_REPEAT_STATEMENT,
    AST_FOR_STATEMENT,
    AST_PROCEDURE_CALL_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_BREAK_STATEMENT,
    AST_CONTINUE_STATEMENT,
    AST_BINARY_EXPRESSION,
    AST_UNARY_EXPRESSION,
    AST_IDENTIFIER_EXPRESSION,
    AST_LITERAL_EXPRESSION,
    AST_ARRAY_ACCESS_EXPRESSION,
    AST_RECORD_ACCESS_EXPRESSION,
    AST_FUNCTION_CALL_EXPRESSION,
    AST_TYPE_CAST_EXPRESSION,
    AST_ARRAY_TYPE,
    AST_RECORD_TYPE,
    AST_BASIC_TYPE,
    AST_PARAMETER,
    AST_ARGUMENT
} ASTNodeType;

// أنواع البيانات الأساسية
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_RECORD,
    TYPE_VOID
} DataType;

// عقدة شجرة البنية المجردة
typedef struct ASTNode {
    ASTNodeType type;
    DataType data_type;
    Token* token;
    struct ASTNode* parent;
    struct ASTNode** children;
    int children_count;
    int children_capacity;
    
    // بيانات خاصة بكل نوع من العقد
    union {
        // للثوابت
        struct {
            wchar_t* name;
            wchar_t* value;
        } constant;
        
        // للمتغيرات
        struct {
            wchar_t* name;
            int array_size;
            wchar_t** field_names;
            int field_count;
        } variable;
        
        // للإجراءات
        struct {
            wchar_t* name;
            int parameter_count;
            wchar_t** parameter_names;
            DataType* parameter_types;
        } procedure;
        
        // للعبارات الشرطية
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_block;
            struct ASTNode* else_block;
        } conditional;
        
        // للعبارات التكرارية
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
            struct ASTNode* initialization;
            struct ASTNode* increment;
        } loop;
        
        // للتعبيرات الثنائية
        struct {
            TokenType operator;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary;
        
        // للتعبيرات الأحادية
        struct {
            TokenType operator;
            struct ASTNode* operand;
        } unary;
        
        // للمعرفات
        struct {
            wchar_t* name;
            int scope_level;
        } identifier;
        
        // للثوابت
        struct {
            wchar_t* value;
            union {
                int int_value;
                float float_value;
                wchar_t char_value;
                int bool_value;
                wchar_t* string_value;
            } data;
        } literal;
    } data;
} ASTNode;

// دوال شجرة البنية المجردة
ASTNode* createASTNode(ASTNodeType type);
void addChild(ASTNode* parent, ASTNode* child);
ASTNode* buildAST(ParseTreeNode* parse_tree);
void freeAST(ASTNode* root);
void printAST(ASTNode* root);
void printASTNode(ASTNode* root, int depth);
void printASTToBuffer(ASTNode* node, wchar_t* buffer, int buffer_size, int* pos);

// دوال بناء أنواع معينة من العقد
ASTNode* createProgramNode();
ASTNode* createBlockNode();
ASTNode* createConstantDeclarationNode(wchar_t* name, wchar_t* value, DataType type);
ASTNode* createVariableDeclarationNode(wchar_t* name, DataType type, int array_size);
ASTNode* createProcedureDeclarationNode(wchar_t* name, ASTNode** parameters, int param_count);
ASTNode* createAssignmentNode(wchar_t* name, ASTNode* expression);
ASTNode* createInputNode(wchar_t* variable_name);
ASTNode* createOutputNode(ASTNode* expression);
ASTNode* createIfNode(ASTNode* condition, ASTNode* then_block, ASTNode* else_block);
ASTNode* createWhileNode(ASTNode* condition, ASTNode* body);
ASTNode* createRepeatNode(ASTNode* body, ASTNode* condition);
ASTNode* createForNode(ASTNode* initialization, ASTNode* condition, ASTNode* increment, ASTNode* body);
ASTNode* createBinaryExpressionNode(TokenType operator, ASTNode* left, ASTNode* right);
ASTNode* createUnaryExpressionNode(TokenType operator, ASTNode* operand);
ASTNode* createIdentifierNode(wchar_t* name);
ASTNode* createLiteralNode(wchar_t* value, DataType type);

// دوال مساعدة
DataType getDataTypeFromToken(TokenType type);
wchar_t* getTypeName(DataType type);
int isNumericType(DataType type);
int isCompatibleType(DataType type1, DataType type2);
DataType getResultType(TokenType operator, DataType left_type, DataType right_type);

#endif // AST_H
