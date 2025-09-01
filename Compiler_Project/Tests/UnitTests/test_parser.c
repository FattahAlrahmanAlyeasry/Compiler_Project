#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../parser.h"
#include "../parsetree.h"

// Test function declarations
void test_initParser();
void test_parseProgram();
void test_parseBlock();
void test_parseDefinitionsPart();
void test_parseStatementsList();
void test_parseStatement();
void test_parseExpression();
void test_parseTreeConstruction();
void test_right_to_left_display();
void test_error_handling();

int main() {
    printf("=== Testing Parser ===\n\n");
    
    test_initParser();
    test_parseProgram();
    test_parseBlock();
    test_parseDefinitionsPart();
    test_parseStatementsList();
    test_parseStatement();
    test_parseExpression();
    test_parseTreeConstruction();
    test_right_to_left_display();
    test_error_handling();
    
    printf("\n=== All Parser Tests Passed! ===\n");
    return 0;
}

void test_initParser() {
    printf("Testing initParser...\n");
    
    Parser parser;
    int result = initParser(&parser);
    
    assert(result == 1);
    assert(parser.lexer != NULL);
    assert(parser.current_token.type == TOKEN_UNKNOWN);
    assert(parser.error_count == 0);
    
    printf("✓ initParser test passed\n");
}

void test_parseProgram() {
    printf("Testing parseProgram...\n");
    
    Parser parser;
    initParser(&parser);
    
    // Test simple program: برنامج [متغير صحيح] : اطبع 42 ؛
    const wchar_t* source = L"برنامج [متغير صحيح] : اطبع 42 ؛";
    setSource(&parser.lexer, source);
    
    ParseTree* tree = parseProgram(&parser);
    
    assert(tree != NULL);
    assert(tree->root != NULL);
    assert(tree->root->type == NODE_PROGRAM);
    assert(tree->root->children_count > 0);
    
    freeParseTree(tree);
    printf("✓ parseProgram test passed\n");
}

void test_parseBlock() {
    printf("Testing parseBlock...\n");
    
    Parser parser;
    initParser(&parser);
    
    const wchar_t* source = L"[متغير صحيح] : اطبع 42 ؛";
    setSource(&parser.lexer, source);
    
    ParseTreeNode* block = parseBlock(&parser);
    
    assert(block != NULL);
    assert(block->type == NODE_BLOCK);
    assert(block->children_count >= 2); // definitions + statements
    
    freeParseTreeNode(block);
    printf("✓ parseBlock test passed\n");
}

void test_parseDefinitionsPart() {
    printf("Testing parseDefinitionsPart...\n");
    
    Parser parser;
    initParser(&parser);
    
    const wchar_t* source = L"متغير صحيح متغير حقيقي";
    setSource(&parser.lexer, source);
    
    ParseTreeNode* definitions = parseDefinitionsPart(&parser);
    
    assert(definitions != NULL);
    assert(definitions->type == NODE_DEFINITIONS_PART);
    assert(definitions->children_count >= 1);
    
    freeParseTreeNode(definitions);
    printf("✓ parseDefinitionsPart test passed\n");
}

void test_parseStatementsList() {
    printf("Testing parseStatementsList...\n");
    
    Parser parser;
    initParser(&parser);
    
    const wchar_t* source = L"اطبع 42 ؛ اقرأ متغير ؛";
    setSource(&parser.lexer, source);
    
    ParseTreeNode* statements = parseStatementsList(&parser);
    
    assert(statements != NULL);
    assert(statements->type == NODE_STATEMENTS_LIST);
    assert(statements->children_count >= 1);
    
    freeParseTreeNode(statements);
    printf("✓ parseStatementsList test passed\n");
}

void test_parseStatement() {
    printf("Testing parseStatement...\n");
    
    Parser parser;
    initParser(&parser);
    
    // Test print statement
    const wchar_t* source = L"اطبع 42 ؛";
    setSource(&parser.lexer, source);
    
    ParseTreeNode* statement = parseStatement(&parser);
    
    assert(statement != NULL);
    assert(statement->type == NODE_PRINT_STATEMENT);
    
    freeParseTreeNode(statement);
    
    // Test assignment statement
    source = L"متغير = 42 ؛";
    setSource(&parser.lexer, source);
    
    statement = parseStatement(&parser);
    
    assert(statement != NULL);
    assert(statement->type == NODE_ASSIGNMENT_STATEMENT);
    
    freeParseTreeNode(statement);
    
    printf("✓ parseStatement test passed\n");
}

void test_parseExpression() {
    printf("Testing parseExpression...\n");
    
    Parser parser;
    initParser(&parser);
    
    // Test arithmetic expression
    const wchar_t* source = L"42 + 58 * 2";
    setSource(&parser.lexer, source);
    
    ParseTreeNode* expression = parseExpression(&parser);
    
    assert(expression != NULL);
    assert(expression->type == NODE_ARITHMETIC_EXPRESSION);
    
    freeParseTreeNode(expression);
    
    // Test logical expression
    source = L"صحيح && خطأ";
    setSource(&parser.lexer, source);
    
    expression = parseExpression(&parser);
    
    assert(expression != NULL);
    assert(expression->type == NODE_LOGICAL_EXPRESSION);
    
    freeParseTreeNode(expression);
    
    printf("✓ parseExpression test passed\n");
}

void test_parseTreeConstruction() {
    printf("Testing parse tree construction...\n");
    
    Parser parser;
    initParser(&parser);
    
    const wchar_t* source = L"برنامج [متغير صحيح] : اطبع 42 ؛";
    setSource(&parser.lexer, source);
    
    ParseTree* tree = parseProgram(&parser);
    
    assert(tree != NULL);
    assert(tree->root != NULL);
    
    // Test tree structure
    ParseTreeNode* root = tree->root;
    assert(root->type == NODE_PROGRAM);
    assert(root->children_count > 0);
    
    // Test block node
    ParseTreeNode* block = root->children[0];
    assert(block->type == NODE_BLOCK);
    
    // Test definitions part
    ParseTreeNode* definitions = block->children[0];
    assert(definitions->type == NODE_DEFINITIONS_PART);
    
    // Test statements part
    ParseTreeNode* statements = block->children[1];
    assert(statements->type == NODE_STATEMENTS_LIST);
    
    freeParseTree(tree);
    printf("✓ Parse tree construction test passed\n");
}

void test_right_to_left_display() {
    printf("Testing right-to-left display...\n");
    
    Parser parser;
    initParser(&parser);
    
    const wchar_t* source = L"برنامج [متغير صحيح] : اطبع 42 ؛";
    setSource(&parser.lexer, source);
    
    ParseTree* tree = parseProgram(&parser);
    
    // Set right-to-left display
    setRightToLeft(tree->root, true);
    
    // Test that the tree is configured for RTL display
    assert(isRightToLeftNode(tree->root));
    
    freeParseTree(tree);
    printf("✓ Right-to-left display test passed\n");
}

void test_error_handling() {
    printf("Testing error handling...\n");
    
    Parser parser;
    initParser(&parser);
    
    // Test with invalid syntax
    const wchar_t* source = L"برنامج [متغير صحيح : اطبع 42 ؛"; // Missing closing bracket
    setSource(&parser.lexer, source);
    
    ParseTree* tree = parseProgram(&parser);
    
    // Should have errors
    assert(parser.error_count > 0);
    
    if (tree) {
        freeParseTree(tree);
    }
    
    printf("✓ Error handling test passed\n");
}
