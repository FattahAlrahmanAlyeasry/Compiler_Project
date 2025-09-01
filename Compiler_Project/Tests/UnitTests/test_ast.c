#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../ast.h"

// Test function declarations
void test_createASTNode();
void test_buildAST();
void test_freeAST();
void test_addChild();
void test_removeChild();
void test_insertChild();
void test_replaceChild();
void test_findChild();
void test_node_properties();
void test_tree_traversal();
void test_validation();

int main() {
    printf("=== Testing Abstract Syntax Tree (AST) ===\n\n");
    
    test_createASTNode();
    test_buildAST();
    test_freeAST();
    test_addChild();
    test_removeChild();
    test_insertChild();
    test_replaceChild();
    test_findChild();
    test_node_properties();
    test_tree_traversal();
    test_validation();
    
    printf("\n=== All AST Tests Passed! ===\n");
    return 0;
}

void test_createASTNode() {
    printf("Testing createASTNode...\n");
    
    ASTNode* node = createASTNode(AST_NODE_PROGRAM, L"برنامج");
    
    assert(node != NULL);
    assert(node->type == AST_NODE_PROGRAM);
    assert(wcscmp(node->value, L"برنامج") == 0);
    assert(node->children_count == 0);
    assert(node->children == NULL);
    assert(node->data_type == DATA_TYPE_UNKNOWN);
    
    freeASTNode(node);
    printf("✓ createASTNode test passed\n");
}

void test_buildAST() {
    printf("Testing buildAST...\n");
    
    ASTNode* root = createASTNode(AST_NODE_PROGRAM, L"برنامج");
    ASTNode* block = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* var_def = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    
    addChild(root, block);
    addChild(block, var_def);
    
    AST* ast = buildAST(root);
    
    assert(ast != NULL);
    assert(ast->root == root);
    assert(ast->node_count == 3);
    
    freeAST(ast);
    printf("✓ buildAST test passed\n");
}

void test_freeAST() {
    printf("Testing freeAST...\n");
    
    ASTNode* root = createASTNode(AST_NODE_PROGRAM, L"برنامج");
    ASTNode* block = createASTNode(AST_NODE_BLOCK, L"[]");
    
    addChild(root, block);
    
    AST* ast = buildAST(root);
    
    // This should not crash
    freeAST(ast);
    
    printf("✓ freeAST test passed\n");
}

void test_addChild() {
    printf("Testing addChild...\n");
    
    ASTNode* parent = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* child1 = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ASTNode* child2 = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع 42");
    
    addChild(parent, child1);
    addChild(parent, child2);
    
    assert(parent->children_count == 2);
    assert(parent->children[0] == child1);
    assert(parent->children[1] == child2);
    
    freeASTNode(parent);
    printf("✓ addChild test passed\n");
}

void test_removeChild() {
    printf("Testing removeChild...\n");
    
    ASTNode* parent = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* child1 = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ASTNode* child2 = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع 42");
    
    addChild(parent, child1);
    addChild(parent, child2);
    
    removeChild(parent, 0);
    
    assert(parent->children_count == 1);
    assert(parent->children[0] == child2);
    
    freeASTNode(parent);
    printf("✓ removeChild test passed\n");
}

void test_insertChild() {
    printf("Testing insertChild...\n");
    
    ASTNode* parent = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* child1 = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ASTNode* child2 = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع 42");
    ASTNode* new_child = createASTNode(AST_NODE_READ_STATEMENT, L"اقرأ متغير");
    
    addChild(parent, child1);
    addChild(parent, child2);
    
    insertChild(parent, 1, new_child);
    
    assert(parent->children_count == 3);
    assert(parent->children[0] == child1);
    assert(parent->children[1] == new_child);
    assert(parent->children[2] == child2);
    
    freeASTNode(parent);
    printf("✓ insertChild test passed\n");
}

void test_replaceChild() {
    printf("Testing replaceChild...\n");
    
    ASTNode* parent = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* child1 = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ASTNode* child2 = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع 42");
    ASTNode* replacement = createASTNode(AST_NODE_READ_STATEMENT, L"اقرأ متغير");
    
    addChild(parent, child1);
    addChild(parent, child2);
    
    replaceChild(parent, 0, replacement);
    
    assert(parent->children[0] == replacement);
    
    freeASTNode(parent);
    printf("✓ replaceChild test passed\n");
}

void test_findChild() {
    printf("Testing findChild...\n");
    
    ASTNode* parent = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* child1 = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ASTNode* child2 = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع 42");
    
    addChild(parent, child1);
    addChild(parent, child2);
    
    ASTNode* found = findChild(parent, AST_NODE_PRINT_STATEMENT);
    assert(found == child2);
    
    found = findChild(parent, AST_NODE_VARIABLE_DEFINITION);
    assert(found == child1);
    
    freeASTNode(parent);
    printf("✓ findChild test passed\n");
}

void test_node_properties() {
    printf("Testing node properties...\n");
    
    ASTNode* node = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    
    // Test data type
    setNodeDataType(node, DATA_TYPE_INTEGER);
    assert(getNodeDataType(node) == DATA_TYPE_INTEGER);
    
    // Test extra data
    int extra_data = 42;
    setNodeExtraData(node, &extra_data);
    int* retrieved = (int*)getNodeExtraData(node);
    assert(*retrieved == 42);
    
    // Test validation
    assert(validateASTNode(node) == 1);
    
    freeASTNode(node);
    printf("✓ node properties test passed\n");
}

void test_tree_traversal() {
    printf("Testing tree traversal...\n");
    
    ASTNode* root = createASTNode(AST_NODE_PROGRAM, L"برنامج");
    ASTNode* block = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* var_def = createASTNode(AST_NODE_VARIABLE_DEFINITION, L"متغير صحيح");
    ASTNode* statement = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع 42");
    
    addChild(root, block);
    addChild(block, var_def);
    addChild(block, statement);
    
    // Test tree statistics
    assert(getNodeCount(root) == 4);
    assert(getTreeDepth(root) == 3);
    assert(getNodeDepth(var_def) == 2);
    
    // Test node types
    assert(isLeafNode(var_def) == 1);
    assert(isInternalNode(block) == 1);
    
    freeASTNode(root);
    printf("✓ tree traversal test passed\n");
}

void test_validation() {
    printf("Testing validation...\n");
    
    ASTNode* root = createASTNode(AST_NODE_PROGRAM, L"برنامج");
    ASTNode* block = createASTNode(AST_NODE_BLOCK, L"[]");
    
    addChild(root, block);
    
    // Test tree validation
    assert(validateASTNode(root) == 1);
    assert(checkNodeConsistency(root) == 1);
    
    // Test invalid node
    ASTNode* invalid_node = createASTNode(AST_NODE_UNKNOWN, L"");
    assert(validateASTNode(invalid_node) == 0);
    
    freeASTNode(root);
    freeASTNode(invalid_node);
    printf("✓ validation test passed\n");
}
