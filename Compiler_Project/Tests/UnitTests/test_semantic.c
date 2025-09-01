#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../semantic.h"
#include "../ast.h"
#include "../symboltable.h"

// Test function declarations
void test_initSemanticAnalyzer();
void test_analyzeProgram();
void test_analyzeBlock();
void test_analyzeStatement();
void test_analyzeExpression();
void test_type_checking();
void test_symbol_table_integration();
void test_ast_building();
void test_error_handling();

int main() {
    printf("=== Testing Semantic Analyzer ===\n\n");
    
    test_initSemanticAnalyzer();
    test_analyzeProgram();
    test_analyzeBlock();
    test_analyzeStatement();
    test_analyzeExpression();
    test_type_checking();
    test_symbol_table_integration();
    test_ast_building();
    test_error_handling();
    
    printf("\n=== All Semantic Analyzer Tests Passed! ===\n");
    return 0;
}

void test_initSemanticAnalyzer() {
    printf("Testing initSemanticAnalyzer...\n");
    
    SemanticAnalyzer analyzer;
    int result = initSemanticAnalyzer(&analyzer);
    
    assert(result == 1);
    assert(analyzer.symbol_table != NULL);
    assert(analyzer.current_scope_level == 0);
    assert(analyzer.error_count == 0);
    assert(analyzer.ast != NULL);
    
    printf("✓ initSemanticAnalyzer test passed\n");
}

void test_analyzeProgram() {
    printf("Testing analyzeProgram...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    // Create a simple parse tree for testing
    ParseTreeNode* program_node = createParseTreeNode(NODE_PROGRAM, L"برنامج");
    ParseTreeNode* block_node = createParseTreeNode(NODE_BLOCK, L"[]");
    
    addChild(program_node, block_node);
    
    ParseTree* parse_tree = createParseTree();
    parse_tree->root = program_node;
    
    int result = analyzeProgram(&analyzer, parse_tree);
    
    assert(result == 1);
    assert(analyzer.ast != NULL);
    
    freeParseTree(parse_tree);
    printf("✓ analyzeProgram test passed\n");
}

void test_analyzeBlock() {
    printf("Testing analyzeBlock...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    ParseTreeNode* block_node = createParseTreeNode(NODE_BLOCK, L"[]");
    ParseTreeNode* var_def = createParseTreeNode(NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ParseTreeNode* statement = createParseTreeNode(NODE_PRINT_STATEMENT, L"اطبع 42");
    
    addChild(block_node, var_def);
    addChild(block_node, statement);
    
    ASTNode* ast_block = analyzeBlock(&analyzer, block_node);
    
    assert(ast_block != NULL);
    assert(ast_block->type == AST_NODE_BLOCK);
    
    freeParseTreeNode(block_node);
    freeAST(analyzer.ast);
    printf("✓ analyzeBlock test passed\n");
}

void test_analyzeStatement() {
    printf("Testing analyzeStatement...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    // Test assignment statement
    ParseTreeNode* assign_node = createParseTreeNode(NODE_ASSIGNMENT_STATEMENT, L"=");
    ParseTreeNode* var_node = createParseTreeNode(NODE_IDENTIFIER, L"متغير");
    ParseTreeNode* value_node = createParseTreeNode(NODE_LITERAL_INTEGER, L"42");
    
    addChild(assign_node, var_node);
    addChild(assign_node, value_node);
    
    ASTNode* ast_statement = analyzeStatement(&analyzer, assign_node);
    
    assert(ast_statement != NULL);
    assert(ast_statement->type == AST_NODE_ASSIGNMENT);
    
    freeParseTreeNode(assign_node);
    freeAST(analyzer.ast);
    printf("✓ analyzeStatement test passed\n");
}

void test_analyzeExpression() {
    printf("Testing analyzeExpression...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    // Test arithmetic expression
    ParseTreeNode* expr_node = createParseTreeNode(NODE_ARITHMETIC_EXPRESSION, L"+");
    ParseTreeNode* left_node = createParseTreeNode(NODE_LITERAL_INTEGER, L"5");
    ParseTreeNode* right_node = createParseTreeNode(NODE_LITERAL_INTEGER, L"3");
    
    addChild(expr_node, left_node);
    addChild(expr_node, right_node);
    
    ASTNode* ast_expr = analyzeExpression(&analyzer, expr_node);
    
    assert(ast_expr != NULL);
    assert(ast_expr->type == AST_NODE_ARITHMETIC_EXPRESSION);
    
    freeParseTreeNode(expr_node);
    freeAST(analyzer.ast);
    printf("✓ analyzeExpression test passed\n");
}

void test_type_checking() {
    printf("Testing type checking...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    // Test type compatibility
    DataType int_type = DATA_TYPE_INTEGER;
    DataType float_type = DATA_TYPE_FLOAT;
    
    int compatible = checkTypeCompatibility(int_type, float_type);
    assert(compatible == 1); // Integer can be assigned to float
    
    // Test assignment compatibility
    int assign_ok = checkAssignmentCompatibility(int_type, float_type);
    assert(assign_ok == 1);
    
    // Test arithmetic operations
    int arith_ok = checkArithmeticOperation(int_type, int_type);
    assert(arith_ok == 1);
    
    // Test relational operations
    int rel_ok = checkRelationalOperation(int_type, int_type);
    assert(rel_ok == 1);
    
    // Test logical operations
    int log_ok = checkLogicalOperation(DATA_TYPE_BOOLEAN, DATA_TYPE_BOOLEAN);
    assert(log_ok == 1);
    
    printf("✓ Type checking test passed\n");
}

void test_symbol_table_integration() {
    printf("Testing symbol table integration...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    // Enter a new scope
    enterScope(analyzer.symbol_table);
    
    // Add a symbol
    SymbolEntry symbol;
    symbol.name = L"متغير";
    symbol.type = SYMBOL_TYPE_VARIABLE;
    symbol.data_type = DATA_TYPE_INTEGER;
    symbol.scope_level = 1;
    
    int result = addSymbol(analyzer.symbol_table, &symbol);
    assert(result == 1);
    
    // Look up the symbol
    SymbolEntry* found = lookupSymbol(analyzer.symbol_table, L"متغير");
    assert(found != NULL);
    assert(wcscmp(found->name, L"متغير") == 0);
    assert(found->type == SYMBOL_TYPE_VARIABLE);
    
    // Exit scope
    exitScope(analyzer.symbol_table);
    
    printf("✓ Symbol table integration test passed\n");
}

void test_ast_building() {
    printf("Testing AST building...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    // Create a simple parse tree
    ParseTreeNode* program_node = createParseTreeNode(NODE_PROGRAM, L"برنامج");
    ParseTreeNode* block_node = createParseTreeNode(NODE_BLOCK, L"[]");
    ParseTreeNode* var_def = createParseTreeNode(NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ParseTreeNode* statement = createParseTreeNode(NODE_PRINT_STATEMENT, L"اطبع 42");
    
    addChild(block_node, var_def);
    addChild(block_node, statement);
    addChild(program_node, block_node);
    
    ParseTree* parse_tree = createParseTree();
    parse_tree->root = program_node;
    
    // Build AST from parse tree
    ASTNode* ast = buildASTFromParseTree(&analyzer, parse_tree);
    
    assert(ast != NULL);
    assert(ast->type == AST_NODE_PROGRAM);
    
    freeParseTree(parse_tree);
    freeAST(ast);
    printf("✓ AST building test passed\n");
}

void test_error_handling() {
    printf("Testing error handling...\n");
    
    SemanticAnalyzer analyzer;
    initSemanticAnalyzer(&analyzer);
    
    // Test semantic error reporting
    semanticError(&analyzer, SEMANTIC_ERROR_TYPE_MISMATCH, L"Type mismatch error", 1, 1);
    
    assert(analyzer.error_count > 0);
    
    // Test getting error count
    int error_count = getSemanticErrorCount(&analyzer);
    assert(error_count > 0);
    
    // Test getting error messages
    wchar_t buffer[1024];
    int result = getSemanticErrors(&analyzer, buffer, 1024);
    assert(result == 1);
    assert(wcslen(buffer) > 0);
    
    printf("✓ Error handling test passed\n");
}
