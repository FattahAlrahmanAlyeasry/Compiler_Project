#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// أنواع عقد شجرة الإعراب
typedef enum {
    NODE_PROGRAM,
    NODE_BLOCK,
    NODE_DEFINITIONS_PART,
    NODE_CONSTANTS_DEFINITION,
    NODE_TYPES_DEFINITION,
    NODE_VARIABLES_DEFINITION,
    NODE_PROCEDURES_DEFINITION,
    NODE_STATEMENTS_LIST,
    NODE_STATEMENT,
    NODE_ASSIGNMENT,
    NODE_INPUT,
    NODE_OUTPUT,
    NODE_IF,
    NODE_ELSE,
    NODE_WHILE,
    NODE_REPEAT,
    NODE_FOR,
    NODE_PROCEDURE_CALL,
    NODE_EXPRESSION,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_IDENTIFIER,
    NODE_LITERAL,
    NODE_TYPE,
    NODE_DECLARATION,
    NODE_PARAMETER_LIST,
    NODE_ARGUMENT_LIST
} ParseTreeNodeType;

// عقدة شجرة الإعراب
typedef struct ParseTreeNode {
    ParseTreeNodeType type;
    Token* token;
    struct ParseTreeNode* parent;
    struct ParseTreeNode** children;
    int children_count;
    int children_capacity;
} ParseTreeNode;

// هيكل المحلل النحوي
typedef struct {
    Lexer* lexer;
    Token* current_token;
    ParseTreeNode* parse_tree;
    int error_count;
} Parser;

// دوال المحلل النحوي
Parser* initParser(Lexer* lexer);
void freeParser(Parser* parser);
ParseTreeNode* parse(Parser* parser);
int parseProgram(Parser* parser, ParseTreeNode* parent);
int parseBlock(Parser* parser, ParseTreeNode* parent);
int parseDefinitionsPart(Parser* parser, ParseTreeNode* parent);
int parseStatementsList(Parser* parser, ParseTreeNode* parent);
int parseStatement(Parser* parser, ParseTreeNode* parent);
int parseExpression(Parser* parser, ParseTreeNode* parent);
int parseType(Parser* parser, ParseTreeNode* parent);
int parseDeclaration(Parser* parser, ParseTreeNode* parent);

// دوال بناء شجرة الإعراب
ParseTreeNode* createParseTreeNode(ParseTreeNodeType type, Token* token);
void addChild(ParseTreeNode* parent, ParseTreeNode* child);
void freeParseTree(ParseTreeNode* root);

// دوال مساعدة
void parserError(Parser* parser, const wchar_t* message);
int matchToken(Parser* parser, TokenType expected_type);
int isStatementStart(TokenType type);
int isExpressionStart(TokenType type);
int isTypeStart(TokenType type);

// دوال عرض شجرة الإعراب
void printParseTree(ParseTreeNode* root, int depth);
void exportParseTreeToDot(ParseTreeNode* root, const char* filename);

#endif // PARSER_H

