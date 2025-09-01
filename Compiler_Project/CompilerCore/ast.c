#include "ast.h"
#include "parsetree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// إنشاء عقدة AST جديدة
ASTNode* createASTNode(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    
    node->type = type;
    node->line = 0;
    node->column = 0;
    node->data.program = NULL;
    node->children = NULL;
    node->children_count = 0;
    node->max_children = 0;
    
    return node;
}

// إنشاء عقدة برنامج
ASTNode* createProgramNode(const wchar_t* name) {
    ASTNode* node = createASTNode(AST_NODE_PROGRAM);
    if (!node) {
        return NULL;
    }
    
    node->data.program = (ProgramNode*)malloc(sizeof(ProgramNode));
    if (!node->data.program) {
        free(node);
        return NULL;
    }
    
    if (name) {
        size_t len = wcslen(name);
        node->data.program->name = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
        if (node->data.program->name) {
            wcscpy(node->data.program->name, name);
        }
    } else {
        node->data.program->name = NULL;
    }
    
    node->data.program->block = NULL;
    
    return node;
}

// إنشاء عقدة كتلة
ASTNode* createBlockNode() {
    ASTNode* node = createASTNode(AST_NODE_BLOCK);
    if (!node) {
        return NULL;
    }
    
    node->data.block = (BlockNode*)malloc(sizeof(BlockNode));
    if (!node->data.block) {
        free(node);
        return NULL;
    }
    
    node->data.block->definitions = NULL;
    node->data.block->statements = NULL;
    node->data.block->definitions_count = 0;
    node->data.block->statements_count = 0;
    
    return node;
}

// إنشاء عقدة عبارة
ASTNode* createStatementNode(StatementType stmt_type) {
    ASTNode* node = createASTNode(AST_NODE_STATEMENT);
    if (!node) {
        return NULL;
    }
    
    node->data.statement = (StatementNode*)malloc(sizeof(StatementNode));
    if (!node->data.statement) {
        free(node);
        return NULL;
    }
    
    node->data.statement->type = stmt_type;
    node->data.statement->data.assignment = NULL;
    node->data.statement->data.conditional = NULL;
    node->data.statement->data.loop = NULL;
    node->data.statement->data.procedure_call = NULL;
    
    return node;
}

// إنشاء عقدة تعبير
ASTNode* createExpressionNode(ExpressionType expr_type) {
    ASTNode* node = createASTNode(AST_NODE_EXPRESSION);
    if (!node) {
        return NULL;
    }
    
    node->data.expression = (ExpressionNode*)malloc(sizeof(ExpressionNode));
    if (!node->data.expression) {
        free(node);
        return NULL;
    }
    
    node->data.expression->type = expr_type;
    node->data.expression->data.binary = NULL;
    node->data.expression->data.unary = NULL;
    node->data.expression->data.literal = NULL;
    node->data.expression->data.identifier = NULL;
    
    return node;
}

// إنشاء عقدة نوع
ASTNode* createTypeNode(DataType data_type) {
    ASTNode* node = createASTNode(AST_NODE_TYPE);
    if (!node) {
        return NULL;
    }
    
    node->data.type = (TypeNode*)malloc(sizeof(TypeNode));
    if (!node->data.type) {
        free(node);
        return NULL;
    }
    
    node->data.type->base_type = data_type;
    node->data.type->array_size = 0;
    node->data.type->fields = NULL;
    node->data.type->fields_count = 0;
    
    return node;
}

// إنشاء عقدة تصريح
ASTNode* createDeclarationNode(DeclarationType decl_type) {
    ASTNode* node = createASTNode(AST_NODE_DECLARATION);
    if (!node) {
        return NULL;
    }
    
    node->data.declaration = (DeclarationNode*)malloc(sizeof(DeclarationNode));
    if (!node->data.declaration) {
        free(node);
        return NULL;
    }
    
    node->data.declaration->type = decl_type;
    node->data.declaration->name = NULL;
    node->data.declaration->data_type = NULL;
    node->data.declaration->value = NULL;
    node->data.declaration->parameters = NULL;
    node->data.declaration->parameters_count = 0;
    
    return node;
}

// إضافة طفل إلى عقدة
int addChild(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) {
        return 0;
    }
    
    if (parent->children_count >= parent->max_children) {
        int new_size = parent->max_children == 0 ? 4 : parent->max_children * 2;
        ASTNode** new_children = (ASTNode**)realloc(parent->children, new_size * sizeof(ASTNode*));
        if (!new_children) {
            return 0;
        }
        parent->children = new_children;
        parent->max_children = new_size;
    }
    
    parent->children[parent->children_count] = child;
    parent->children_count++;
    
    return 1;
}

// بناء AST من شجرة الإعراب
ASTNode* buildAST(ParseTreeNode* parse_tree) {
    if (!parse_tree) {
        return NULL;
    }
    
    ASTNode* ast = NULL;
    
    switch (parse_tree->type) {
        case PARSE_NODE_PROGRAM:
            ast = createProgramNode(parse_tree->value);
            break;
        case PARSE_NODE_BLOCK:
            ast = createBlockNode();
            break;
        case PARSE_NODE_STATEMENTS_LIST:
            ast = createStatementNode(STATEMENT_COMPOUND);
            break;
        case PARSE_NODE_ASSIGNMENT_STATEMENT:
            ast = createStatementNode(STATEMENT_ASSIGNMENT);
            break;
        case PARSE_NODE_CONDITIONAL_STATEMENT:
            ast = createStatementNode(STATEMENT_CONDITIONAL);
            break;
        case PARSE_NODE_LOOP_STATEMENT:
            ast = createStatementNode(STATEMENT_LOOP);
            break;
        case PARSE_NODE_EXPRESSION:
            ast = createExpressionNode(EXPRESSION_BINARY);
            break;
        case PARSE_NODE_IDENTIFIER:
            ast = createExpressionNode(EXPRESSION_IDENTIFIER);
            break;
        case PARSE_NODE_LITERAL:
            ast = createExpressionNode(EXPRESSION_LITERAL);
            break;
        default:
            ast = createASTNode(AST_NODE_STATEMENT);
            break;
    }
    
    if (ast) {
        ast->line = parse_tree->line;
        ast->column = parse_tree->column;
        
        // بناء الأطفال
        for (int i = 0; i < parse_tree->children_count; i++) {
            ASTNode* child = buildAST(parse_tree->children[i]);
            if (child) {
                addChild(ast, child);
            }
        }
    }
    
    return ast;
}

// طباعة AST
void printAST(ASTNode* node) {
    if (!node) {
        return;
    }
    
    printASTNode(node, 0);
}

// طباعة AST إلى بفر
void printASTToBuffer(ASTNode* node, wchar_t* buffer, int buffer_size, int* pos) {
    if (!node || !buffer || !pos || *pos >= buffer_size) {
        return;
    }
    
    printASTNodeToBuffer(node, buffer, buffer_size, pos, 0);
}

// طباعة عقدة AST
void printASTNode(ASTNode* node, int depth) {
    if (!node) {
        return;
    }
    
    // طباعة المسافات
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    // طباعة نوع العقدة
    printf("AST Node: %s", getASTNodeTypeName(node->type));
    
    // طباعة معلومات إضافية حسب نوع العقدة
    switch (node->type) {
        case AST_NODE_PROGRAM:
            if (node->data.program && node->data.program->name) {
                printf(" - Name: %ls", node->data.program->name);
            }
            break;
        case AST_NODE_STATEMENT:
            if (node->data.statement) {
                printf(" - Statement Type: %s", getStatementTypeName(node->data.statement->type));
            }
            break;
        case AST_NODE_EXPRESSION:
            if (node->data.expression) {
                printf(" - Expression Type: %s", getExpressionTypeName(node->data.expression->type));
            }
            break;
        case AST_NODE_TYPE:
            if (node->data.type) {
                printf(" - Data Type: %s", getDataTypeName(node->data.type->base_type));
            }
            break;
        default:
            break;
    }
    
    printf(" (Line: %d, Column: %d)\n", node->line, line, node->column);
    
    // طباعة الأطفال
    for (int i = 0; i < node->children_count; i++) {
        printASTNode(node->children[i], depth + 1);
    }
}

// طباعة عقدة AST إلى بفر
static void printASTNodeToBuffer(ASTNode* node, wchar_t* buffer, int buffer_size, int* pos, int depth) {
    if (!node || !buffer || !pos || *pos >= buffer_size) {
        return;
    }
    
    // طباعة المسافات
    for (int i = 0; i < depth && *pos < buffer_size - 1; i++) {
        buffer[(*pos)++] = L' ';
        buffer[(*pos)++] = L' ';
    }
    
    // طباعة نوع العقدة
    const char* type_name = getASTNodeTypeName(node->type);
    for (int i = 0; type_name[i] && *pos < buffer_size - 1; i++) {
        buffer[(*pos)++] = type_name[i];
    }
    
    // طباعة معلومات إضافية حسب نوع العقدة
    switch (node->type) {
        case AST_NODE_PROGRAM:
            if (node->data.program && node->data.program->name) {
                if (*pos < buffer_size - 3) {
                    buffer[(*pos)++] = L' ';
                    buffer[(*pos)++] = L'-';
                    buffer[(*pos)++] = L' ';
                }
                
                for (int i = 0; node->data.program->name[i] && *pos < buffer_size - 1; i++) {
                    buffer[(*pos)++] = node->data.program->name[i];
                }
            }
            break;
        case AST_NODE_STATEMENT:
            if (node->data.statement) {
                if (*pos < buffer_size - 3) {
                    buffer[(*pos)++] = L' ';
                    buffer[(*pos)++] = L'-';
                    buffer[(*pos)++] = L' ';
                }
                
                const char* stmt_type = getStatementTypeName(node->data.statement->type);
                for (int i = 0; stmt_type[i] && *pos < buffer_size - 1; i++) {
                    buffer[(*pos)++] = stmt_type[i];
                }
            }
            break;
        case AST_NODE_EXPRESSION:
            if (node->data.expression) {
                if (*pos < buffer_size - 3) {
                    buffer[(*pos)++] = L' ';
                    buffer[(*pos)++] = L'-';
                    buffer[(*pos)++] = L' ';
                }
                
                const char* expr_type = getExpressionTypeName(node->data.expression->type);
                for (int i = 0; expr_type[i] && *pos < buffer_size - 1; i++) {
                    buffer[(*pos)++] = expr_type[i];
                }
            }
            break;
        case AST_NODE_TYPE:
            if (node->data.type) {
                if (*pos < buffer_size - 3) {
                    buffer[(*pos)++] = L' ';
                    buffer[(*pos)++] = L'-';
                    buffer[(*pos)++] = L' ';
                }
                
                const char* data_type = getDataTypeName(node->data.type->base_type);
                for (int i = 0; data_type[i] && *pos < buffer_size - 1; i++) {
                    buffer[(*pos)++] = data_type[i];
                }
            }
            break;
        default:
            break;
    }
    
    // طباعة الموقع
    if (node->line > 0 && *pos < buffer_size - 20) {
        swprintf(buffer + *pos, buffer_size - *pos, L" (Line: %d, Column: %d)", node->line, node->column);
        *pos += wcslen(buffer + *pos);
    }
    
    if (*pos < buffer_size - 1) {
        buffer[(*pos)++] = L'\n';
    }
    
    // طباعة الأطفال
    for (int i = 0; i < node->children_count; i++) {
        printASTNodeToBuffer(node->children[i], buffer, buffer_size, pos, depth + 1);
    }
}

// تحرير ذاكرة AST
void freeAST(ASTNode* node) {
    if (!node) {
        return;
    }
    
    // تحرير الأطفال أولاً
    for (int i = 0; i < node->children_count; i++) {
        freeAST(node->children[i]);
    }
    
    // تحرير البيانات حسب نوع العقدة
    switch (node->type) {
        case AST_NODE_PROGRAM:
            if (node->data.program) {
                if (node->data.program->name) {
                    free(node->data.program->name);
                }
                free(node->data.program);
            }
            break;
        case AST_NODE_BLOCK:
            if (node->data.block) {
                free(node->data.block);
            }
            break;
        case AST_NODE_STATEMENT:
            if (node->data.statement) {
                free(node->data.statement);
            }
            break;
        case AST_NODE_EXPRESSION:
            if (node->data.expression) {
                free(node->data.expression);
            }
            break;
        case AST_NODE_TYPE:
            if (node->data.type) {
                free(node->data.type);
            }
            break;
        case AST_NODE_DECLARATION:
            if (node->data.declaration) {
                if (node->data.declaration->name) {
                    free(node->data.declaration->name);
                }
                free(node->data.declaration);
            }
            break;
        default:
            break;
    }
    
    // تحرير مصفوفة الأطفال
    if (node->children) {
        free(node->children);
    }
    
    // تحرير العقدة نفسها
    free(node);
}

// دوال مساعدة للحصول على أسماء الأنواع
const char* getASTNodeTypeName(ASTNodeType type) {
    switch (type) {
        case AST_NODE_PROGRAM: return "PROGRAM";
        case AST_NODE_BLOCK: return "BLOCK";
        case AST_NODE_STATEMENT: return "STATEMENT";
        case AST_NODE_EXPRESSION: return "EXPRESSION";
        case AST_NODE_TYPE: return "TYPE";
        case AST_NODE_DECLARATION: return "DECLARATION";
        default: return "UNKNOWN";
    }
}

const char* getStatementTypeName(StatementType type) {
    switch (type) {
        case STATEMENT_ASSIGNMENT: return "ASSIGNMENT";
        case STATEMENT_CONDITIONAL: return "CONDITIONAL";
        case STATEMENT_LOOP: return "LOOP";
        case STATEMENT_PROCEDURE_CALL: return "PROCEDURE_CALL";
        case STATEMENT_COMPOUND: return "COMPOUND";
        case STATEMENT_EMPTY: return "EMPTY";
        default: return "UNKNOWN";
    }
}

const char* getExpressionTypeName(ExpressionType type) {
    switch (type) {
        case EXPRESSION_BINARY: return "BINARY";
        case EXPRESSION_UNARY: return "UNARY";
        case EXPRESSION_LITERAL: return "LITERAL";
        case EXPRESSION_IDENTIFIER: return "IDENTIFIER";
        case EXPRESSION_FUNCTION_CALL: return "FUNCTION_CALL";
        case EXPRESSION_ARRAY_ACCESS: return "ARRAY_ACCESS";
        case EXPRESSION_RECORD_ACCESS: return "RECORD_ACCESS";
        default: return "UNKNOWN";
    }
}

const char* getDataTypeName(DataType type) {
    switch (type) {
        case TYPE_INT: return "INT";
        case TYPE_FLOAT: return "FLOAT";
        case TYPE_CHAR: return "CHAR";
        case TYPE_BOOL: return "BOOL";
        case TYPE_STRING: return "STRING";
        case TYPE_LIST: return "LIST";
        case TYPE_RECORD: return "RECORD";
        case TYPE_VOID: return "VOID";
        default: return "UNKNOWN";
    }
}
