#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <assert.h>
#include "../CompilerCore/parsetree.h"

// Test framework functions
static int test_count = 0;
static int test_passed = 0;
static int test_failed = 0;

void test_start(const char* test_name) {
    printf("Running test: %s\n", test_name);
    test_count++;
}

void test_pass(const char* test_name) {
    printf("✓ PASS: %s\n", test_name);
    test_passed++;
}

void test_fail(const char* test_name, const char* reason) {
    printf("✗ FAIL: %s - %s\n", test_name, reason);
    test_failed++;
}

void test_summary() {
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", test_count);
    printf("Passed: %d\n", test_passed);
    printf("Failed: %d\n", test_failed);
    printf("Success rate: %.1f%%\n", (float)test_passed / test_count * 100);
}

// Test parse tree node creation
void test_create_parse_tree_node() {
    test_start("Parse Tree Node Creation");
    
    // Test creating different types of nodes
    ParseTreeNode* program_node = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block_node = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* stmt_node = createParseTreeNode(ASSIGNMENT_STATEMENT_NODE, L"=");
    ParseTreeNode* expr_node = createParseTreeNode(ARITHMETIC_EXPRESSION_NODE, L"+");
    ParseTreeNode* id_node = createParseTreeNode(IDENTIFIER_NODE, L"متغير");
    ParseTreeNode* num_node = createParseTreeNode(NUMBER_NODE, L"5");
    
    if (program_node && block_node && stmt_node && expr_node && id_node && num_node) {
        test_pass("Parse Tree Node Creation");
        
        // Check node properties
        assert(program_node->type == PROGRAM_NODE);
        assert(wcscmp(program_node->value, L"برنامج") == 0);
        assert(program_node->children_count == 0);
        assert(program_node->children == NULL);
        assert(program_node->line_number == 0);
        assert(program_node->column_number == 0);
        
        assert(block_node->type == BLOCK_NODE);
        assert(wcscmp(block_node->value, L"كتلة") == 0);
        
        assert(stmt_node->type == ASSIGNMENT_STATEMENT_NODE);
        assert(wcscmp(stmt_node->value, L"=") == 0);
        
        assert(expr_node->type == ARITHMETIC_EXPRESSION_NODE);
        assert(wcscmp(expr_node->value, L"+") == 0);
        
        assert(id_node->type == IDENTIFIER_NODE);
        assert(wcscmp(id_node->value, L"متغير") == 0);
        
        assert(num_node->type == NUMBER_NODE);
        assert(wcscmp(num_node->value, L"5") == 0);
        
        // Cleanup
        freeParseTreeNode(program_node);
        freeParseTreeNode(block_node);
        freeParseTreeNode(stmt_node);
        freeParseTreeNode(expr_node);
        freeParseTreeNode(id_node);
        freeParseTreeNode(num_node);
        
    } else {
        test_fail("Parse Tree Node Creation", "Failed to create parse tree nodes");
    }
}

// Test parse tree node with children
void test_create_parse_tree_node_with_children() {
    test_start("Parse Tree Node with Children");
    
    // Create parent node
    ParseTreeNode* parent = createParseTreeNode(ARITHMETIC_EXPRESSION_NODE, L"+");
    
    // Create child nodes
    ParseTreeNode* left_child = createParseTreeNode(NUMBER_NODE, L"3");
    ParseTreeNode* right_child = createParseTreeNode(NUMBER_NODE, L"5");
    
    if (parent && left_child && right_child) {
        // Add children to parent
        addChild(parent, left_child);
        addChild(parent, right_child);
        
        test_pass("Parse Tree Node with Children");
        
        // Check children
        assert(parent->children_count == 2);
        assert(parent->children[0] == left_child);
        assert(parent->children[1] == right_child);
        assert(left_child->parent == parent);
        assert(right_child->parent == parent);
        
        // Cleanup
        freeParseTreeNode(parent);
        
    } else {
        test_fail("Parse Tree Node with Children", "Failed to create parse tree nodes");
    }
}

// Test parse tree creation
void test_create_parse_tree() {
    test_start("Parse Tree Creation");
    
    ParseTree* tree = createParseTree();
    
    if (tree) {
        test_pass("Parse Tree Creation");
        assert(tree->root == NULL);
        assert(tree->node_count == 0);
        assert(tree->depth == 0);
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Parse Tree Creation", "Failed to create parse tree");
    }
}

// Test parse tree building
void test_build_parse_tree() {
    test_start("Parse Tree Building");
    
    // Create a simple parse tree structure
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* stmt = createParseTreeNode(ASSIGNMENT_STATEMENT_NODE, L"=");
    
    if (root && block && stmt) {
        // Build structure
        addChild(root, block);
        addChild(block, stmt);
        
        // Create parse tree
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 3;
        tree->depth = 2;
        
        test_pass("Parse Tree Building");
        
        // Verify structure
        assert(tree->root == root);
        assert(tree->root->children_count == 1);
        assert(tree->root->children[0] == block);
        assert(block->children_count == 1);
        assert(block->children[0] == stmt);
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Parse Tree Building", "Failed to create parse tree nodes");
    }
}

// Test parse tree node addition
void test_add_child() {
    test_start("Add Child to Parse Tree Node");
    
    ParseTreeNode* parent = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* child1 = createParseTreeNode(STATEMENT_NODE, L"عبارة1");
    ParseTreeNode* child2 = createParseTreeNode(STATEMENT_NODE, L"عبارة2");
    
    if (parent && child1 && child2) {
        // Add children
        addChild(parent, child1);
        addChild(parent, child2);
        
        test_pass("Add Child to Parse Tree Node");
        
        // Check children
        assert(parent->children_count == 2);
        assert(parent->children[0] == child1);
        assert(parent->children[1] == child2);
        assert(child1->parent == parent);
        assert(child2->parent == parent);
        
        // Check child indices
        assert(child1->child_index == 0);
        assert(child2->child_index == 1);
        
        // Cleanup
        freeParseTreeNode(parent);
        
    } else {
        test_fail("Add Child to Parse Tree Node", "Failed to create parse tree nodes");
    }
}

// Test parse tree node removal
void test_remove_child() {
    test_start("Remove Child from Parse Tree Node");
    
    ParseTreeNode* parent = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* child1 = createParseTreeNode(STATEMENT_NODE, L"عبارة1");
    ParseTreeNode* child2 = createParseTreeNode(STATEMENT_NODE, L"عبارة2");
    ParseTreeNode* child3 = createParseTreeNode(STATEMENT_NODE, L"عبارة3");
    
    if (parent && child1 && child2 && child3) {
        // Add children
        addChild(parent, child1);
        addChild(parent, child2);
        addChild(parent, child3);
        
        assert(parent->children_count == 3);
        
        // Remove middle child
        removeChild(parent, 1);
        
        test_pass("Remove Child from Parse Tree Node");
        
        // Check remaining children
        assert(parent->children_count == 2);
        assert(parent->children[0] == child1);
        assert(parent->children[1] == child3);
        assert(child1->child_index == 0);
        assert(child3->child_index == 1);
        
        // Cleanup
        freeParseTreeNode(parent);
        
    } else {
        test_fail("Remove Child from Parse Tree Node", "Failed to create parse tree nodes");
    }
}

// Test parse tree node insertion
void test_insert_child() {
    test_start("Insert Child at Position");
    
    ParseTreeNode* parent = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* child1 = createParseTreeNode(STATEMENT_NODE, L"عبارة1");
    ParseTreeNode* child2 = createParseTreeNode(STATEMENT_NODE, L"عبارة2");
    ParseTreeNode* new_child = createParseTreeNode(STATEMENT_NODE, L"عبارة جديدة");
    
    if (parent && child1 && child2 && new_child) {
        // Add initial children
        addChild(parent, child1);
        addChild(parent, child2);
        
        // Insert new child at position 1
        insertChild(parent, new_child, 1);
        
        test_pass("Insert Child at Position");
        
        // Check children order
        assert(parent->children_count == 3);
        assert(parent->children[0] == child1);
        assert(parent->children[1] == new_child);
        assert(parent->children[2] == child2);
        
        // Check child indices
        assert(child1->child_index == 0);
        assert(new_child->child_index == 1);
        assert(child2->child_index == 2);
        
        // Cleanup
        freeParseTreeNode(parent);
        
    } else {
        test_fail("Insert Child at Position", "Failed to create parse tree nodes");
    }
}

// Test parse tree node replacement
void test_replace_child() {
    test_start("Replace Child in Parse Tree Node");
    
    ParseTreeNode* parent = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* old_child = createParseTreeNode(STATEMENT_NODE, L"عبارة قديمة");
    ParseTreeNode* new_child = createParseTreeNode(STATEMENT_NODE, L"عبارة جديدة");
    
    if (parent && old_child && new_child) {
        // Add old child
        addChild(parent, old_child);
        
        // Replace child
        replaceChild(parent, 0, new_child);
        
        test_pass("Replace Child in Parse Tree Node");
        
        // Check replacement
        assert(parent->children_count == 1);
        assert(parent->children[0] == new_child);
        assert(new_child->parent == parent);
        assert(new_child->child_index == 0);
        
        // Cleanup
        freeParseTreeNode(parent);
        freeParseTreeNode(old_child);
        
    } else {
        test_fail("Replace Child in Parse Tree Node", "Failed to create parse tree nodes");
    }
}

// Test parse tree node finding
void test_find_child() {
    test_start("Find Child in Parse Tree Node");
    
    ParseTreeNode* parent = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* child1 = createParseTreeNode(STATEMENT_NODE, L"عبارة1");
    ParseTreeNode* child2 = createParseTreeNode(STATEMENT_NODE, L"عبارة2");
    ParseTreeNode* child3 = createParseTreeNode(STATEMENT_NODE, L"عبارة3");
    
    if (parent && child1 && child2 && child3) {
        // Add children
        addChild(parent, child1);
        addChild(parent, child2);
        addChild(parent, child3);
        
        // Find children
        ParseTreeNode* found1 = findChild(parent, 0);
        ParseTreeNode* found2 = findChild(parent, 1);
        ParseTreeNode* found3 = findChild(parent, 2);
        ParseTreeNode* not_found = findChild(parent, 3);
        
        test_pass("Find Child in Parse Tree Node");
        
        // Check found children
        assert(found1 == child1);
        assert(found2 == child2);
        assert(found3 == child3);
        assert(not_found == NULL);
        
        // Cleanup
        freeParseTreeNode(parent);
        
    } else {
        test_fail("Find Child in Parse Tree Node", "Failed to create parse tree nodes");
    }
}

// Test parse tree node search
void test_search_nodes() {
    test_start("Search Parse Tree Nodes");
    
    // Create a parse tree structure
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* stmt1 = createParseTreeNode(ASSIGNMENT_STATEMENT_NODE, L"=");
    ParseTreeNode* stmt2 = createParseTreeNode(PRINT_STATEMENT_NODE, L"اطبع");
    ParseTreeNode* expr = createParseTreeNode(ARITHMETIC_EXPRESSION_NODE, L"+");
    
    if (root && block && stmt1 && stmt2 && expr) {
        addChild(root, block);
        addChild(block, stmt1);
        addChild(block, stmt2);
        addChild(stmt1, expr);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 5;
        
        // Search by type
        ParseTreeNode* found = findNodeByType(tree, ASSIGNMENT_STATEMENT_NODE);
        assert(found != NULL);
        assert(found->type == ASSIGNMENT_STATEMENT_NODE);
        
        // Search by value
        found = findNodeByValue(tree, L"اطبع");
        assert(found != NULL);
        assert(wcscmp(found->value, L"اطبع") == 0);
        
        // Search all nodes by type
        ParseTreeNode** stmt_nodes = findAllNodesByType(tree, STATEMENT_NODE);
        assert(stmt_nodes != NULL);
        
        test_pass("Search Parse Tree Nodes");
        
        // Cleanup
        if (stmt_nodes) free(stmt_nodes);
        freeParseTree(tree);
        
    } else {
        test_fail("Search Parse Tree Nodes", "Failed to create parse tree nodes");
    }
}

// Test parse tree validation
void test_validate_parse_tree() {
    test_start("Validate Parse Tree");
    
    // Create valid parse tree
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    
    if (root && block) {
        addChild(root, block);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 2;
        
        // Validate parse tree
        int result = validateParseTree(tree);
        
        if (result == 1) {
            test_pass("Validate Parse Tree");
            
            // Check validation details
            assert(validateParseTreeNode(root) == 1);
            assert(validateParseTreeNode(block) == 1);
            assert(checkNodeConsistency(root) == 1);
            
        } else {
            test_fail("Validate Parse Tree", "Parse tree validation failed");
        }
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Validate Parse Tree", "Failed to create parse tree nodes");
    }
}

// Test parse tree statistics
void test_parse_tree_statistics() {
    test_start("Parse Tree Statistics");
    
    // Create parse tree with multiple nodes
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* stmt1 = createParseTreeNode(ASSIGNMENT_STATEMENT_NODE, L"=");
    ParseTreeNode* stmt2 = createParseTreeNode(PRINT_STATEMENT_NODE, L"اطبع");
    ParseTreeNode* expr = createParseTreeNode(ARITHMETIC_EXPRESSION_NODE, L"+");
    
    if (root && block && stmt1 && stmt2 && expr) {
        addChild(root, block);
        addChild(block, stmt1);
        addChild(block, stmt2);
        addChild(stmt1, expr);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 5;
        
        test_pass("Parse Tree Statistics");
        
        // Check node count
        assert(getNodeCount(tree) == 5);
        assert(getNodeCountByType(tree, PROGRAM_NODE) == 1);
        assert(getNodeCountByType(tree, BLOCK_NODE) == 1);
        assert(getNodeCountByType(tree, STATEMENT_NODE) == 2);
        assert(getNodeCountByType(tree, EXPRESSION_NODE) == 1);
        
        // Check tree depth
        assert(getTreeDepth(tree) == 3);
        assert(getNodeDepth(root) == 0);
        assert(getNodeDepth(block) == 1);
        assert(getNodeDepth(stmt1) == 2);
        assert(getNodeDepth(expr) == 3);
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Parse Tree Statistics", "Failed to create parse tree nodes");
    }
}

// Test parse tree node properties
void test_parse_tree_node_properties() {
    test_start("Parse Tree Node Properties");
    
    ParseTreeNode* leaf = createParseTreeNode(NUMBER_NODE, L"42");
    ParseTreeNode* internal = createParseTreeNode(ARITHMETIC_EXPRESSION_NODE, L"+");
    ParseTreeNode* child = createParseTreeNode(NUMBER_NODE, L"5");
    
    if (leaf && internal && child) {
        addChild(internal, child);
        
        test_pass("Parse Tree Node Properties");
        
        // Check node types
        assert(isLeafNode(leaf) == 1);
        assert(isLeafNode(internal) == 0);
        assert(isInternalNode(leaf) == 0);
        assert(isInternalNode(internal) == 1);
        
        // Check right-to-left support
        assert(isRightToLeftNode(leaf) == 0); // Default
        setRightToLeft(leaf, 1);
        assert(isRightToLeftNode(leaf) == 1);
        
        // Cleanup
        freeParseTreeNode(internal);
        freeParseTreeNode(leaf);
        
    } else {
        test_fail("Parse Tree Node Properties", "Failed to create parse tree nodes");
    }
}

// Test parse tree printing
void test_print_parse_tree() {
    test_start("Print Parse Tree");
    
    // Create a simple parse tree
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* stmt = createParseTreeNode(ASSIGNMENT_STATEMENT_NODE, L"=");
    
    if (root && block && stmt) {
        addChild(root, block);
        addChild(block, stmt);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 3;
        
        // Test printing (this will output to console)
        int result = printParseTree(tree);
        
        if (result == 0) {
            test_pass("Print Parse Tree");
        } else {
            test_fail("Print Parse Tree", "Failed to print parse tree");
        }
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Print Parse Tree", "Failed to create parse tree nodes");
    }
}

// Test parse tree printing to buffer
void test_print_parse_tree_to_buffer() {
    test_start("Print Parse Tree to Buffer");
    
    // Create a simple parse tree
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* stmt = createParseTreeNode(ASSIGNMENT_STATEMENT_NODE, L"=");
    
    if (root && block && stmt) {
        addChild(root, block);
        addChild(block, stmt);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 3;
        
        // Test printing to buffer
        wchar_t buffer[1024];
        int pos = 0;
        int result = printParseTreeToBuffer(tree, buffer, 1024, &pos);
        
        if (result == 0 && pos > 0) {
            test_pass("Print Parse Tree to Buffer");
            
            // Check buffer content
            assert(wcslen(buffer) > 0);
            assert(wcsstr(buffer, L"برنامج") != NULL);
            assert(wcsstr(buffer, L"كتلة") != NULL);
            assert(wcsstr(buffer, L"=") != NULL);
            
        } else {
            test_fail("Print Parse Tree to Buffer", "Failed to print parse tree to buffer");
        }
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Print Parse Tree to Buffer", "Failed to create parse tree nodes");
    }
}

// Test parse tree node printing to buffer
void test_print_parse_tree_node_to_buffer() {
    test_start("Print Parse Tree Node to Buffer");
    
    ParseTreeNode* node = createParseTreeNode(ARITHMETIC_EXPRESSION_NODE, L"+");
    
    if (node) {
        // Test printing node to buffer
        wchar_t buffer[256];
        int pos = 0;
        int result = printParseTreeNodeToBuffer(node, buffer, 256, &pos);
        
        if (result == 0 && pos > 0) {
            test_pass("Print Parse Tree Node to Buffer");
            
            // Check buffer content
            assert(wcslen(buffer) > 0);
            assert(wcsstr(buffer, L"+") != NULL);
            
        } else {
            test_fail("Print Parse Tree Node to Buffer", "Failed to print node to buffer");
        }
        
        // Cleanup
        freeParseTreeNode(node);
        
    } else {
        test_fail("Print Parse Tree Node to Buffer", "Failed to create parse tree node");
    }
}

// Test parse tree export to DOT
void test_export_parse_tree_to_dot() {
    test_start("Export Parse Tree to DOT");
    
    // Create a simple parse tree
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    ParseTreeNode* stmt = createParseTreeNode(ASSIGNMENT_STATEMENT_NODE, L"=");
    
    if (root && block && stmt) {
        addChild(root, block);
        addChild(block, stmt);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 3;
        
        // Test export to DOT
        int result = exportParseTreeToDot(tree, "test_parse_tree.dot");
        
        if (result == 0) {
            test_pass("Export Parse Tree to DOT");
            
            // Verify file was created
            FILE* file = fopen("test_parse_tree.dot", "r");
            if (file != NULL) {
                char line[256];
                int line_count = 0;
                while (fgets(line, sizeof(line), file) && line_count < 10) {
                    line_count++;
                }
                fclose(file);
                
                assert(line_count > 0);
                
                // Clean up temporary file
                remove("test_parse_tree.dot");
                
            } else {
                test_fail("Export Parse Tree to DOT", "Failed to verify exported file");
            }
            
        } else {
            test_fail("Export Parse Tree to DOT", "Failed to export parse tree to DOT");
        }
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Export Parse Tree to DOT", "Failed to create parse tree nodes");
    }
}

// Test parse tree export to XML
void test_export_parse_tree_to_xml() {
    test_start("Export Parse Tree to XML");
    
    // Create a simple parse tree
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    
    if (root && block) {
        addChild(root, block);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 2;
        
        // Test export to XML
        int result = exportParseTreeToXML(tree, "test_parse_tree.xml");
        
        if (result == 0) {
            test_pass("Export Parse Tree to XML");
            
            // Verify file was created
            FILE* file = fopen("test_parse_tree.xml", "r");
            if (file != NULL) {
                char line[256];
                int line_count = 0;
                while (fgets(line, sizeof(line), file) && line_count < 10) {
                    line_count++;
                }
                fclose(file);
                
                assert(line_count > 0);
                
                // Clean up temporary file
                remove("test_parse_tree.xml");
                
            } else {
                test_fail("Export Parse Tree to XML", "Failed to verify exported file");
            }
            
        } else {
            test_fail("Export Parse Tree to XML", "Failed to export parse tree to XML");
        }
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Export Parse Tree to XML", "Failed to create parse tree nodes");
    }
}

// Test parse tree export to JSON
void test_export_parse_tree_to_json() {
    test_start("Export Parse Tree to JSON");
    
    // Create a simple parse tree
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    
    if (root && block) {
        addChild(root, block);
        
        ParseTree* tree = createParseTree();
        tree->root = root;
        tree->node_count = 2;
        
        // Test export to JSON
        int result = exportParseTreeToJSON(tree, "test_parse_tree.json");
        
        if (result == 0) {
            test_pass("Export Parse Tree to JSON");
            
            // Verify file was created
            FILE* file = fopen("test_parse_tree.json", "r");
            if (file != NULL) {
                char line[256];
                int line_count = 0;
                while (fgets(line, sizeof(line), file) && line_count < 10) {
                    line_count++;
                }
                fclose(file);
                
                assert(line_count > 0);
                
                // Clean up temporary file
                remove("test_parse_tree.json");
                
            } else {
                test_fail("Export Parse Tree to JSON", "Failed to verify exported file");
            }
            
        } else {
            test_fail("Export Parse Tree to JSON", "Failed to export parse tree to JSON");
        }
        
        // Cleanup
        freeParseTree(tree);
        
    } else {
        test_fail("Export Parse Tree to JSON", "Failed to create parse tree nodes");
    }
}

// Test parse tree memory management
void test_parse_tree_memory_management() {
    test_start("Parse Tree Memory Management");
    
    // Create large parse tree structure
    ParseTreeNode* root = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    ParseTreeNode* block = createParseTreeNode(BLOCK_NODE, L"كتلة");
    
    if (root && block) {
        // Add many children to test memory management
        for (int i = 0; i < 100; i++) {
            wchar_t name[32];
            swprintf(name, 32, L"عبارة%d", i);
            ParseTreeNode* child = createParseTreeNode(STATEMENT_NODE, name);
            if (child) {
                addChild(block, child);
            }
        }
        
        addChild(root, block);
        
        test_pass("Parse Tree Memory Management");
        
        // Check memory allocation
        assert(block->children_count == 100);
        assert(block->children_capacity >= 100);
        
        // Test resizing
        resizeChildrenArray(block, 200);
        assert(block->children_capacity >= 200);
        
        // Test compacting
        compactChildrenArray(block);
        assert(block->children_capacity >= 100);
        
        // Cleanup
        freeParseTreeNode(root);
        
    } else {
        test_fail("Parse Tree Memory Management", "Failed to create parse tree nodes");
    }
}

// Test parse tree error handling
void test_parse_tree_error_handling() {
    test_start("Parse Tree Error Handling");
    
    // Test invalid operations
    ParseTreeNode* node = createParseTreeNode(PROGRAM_NODE, L"برنامج");
    
    if (node) {
        test_pass("Parse Tree Error Handling");
        
        // Test invalid child index
        ParseTreeNode* child = findChild(node, -1);
        assert(child == NULL);
        
        child = findChild(node, 100);
        assert(child == NULL);
        
        // Test invalid removal
        int result = removeChild(node, 0);
        assert(result != 0); // Should fail
        
        // Test invalid insertion
        ParseTreeNode* new_child = createParseTreeNode(STATEMENT_NODE, L"عبارة");
        if (new_child) {
            result = insertChild(node, new_child, -1);
            assert(result != 0); // Should fail
            
            result = insertChild(node, new_child, 100);
            assert(result != 0); // Should fail
            
            freeParseTreeNode(new_child);
        }
        
        // Cleanup
        freeParseTreeNode(node);
        
    } else {
        test_fail("Parse Tree Error Handling", "Failed to create parse tree node");
    }
}

// Test parse tree extra data
void test_parse_tree_extra_data() {
    test_start("Parse Tree Extra Data");
    
    ParseTreeNode* node = createParseTreeNode(IDENTIFIER_NODE, L"متغير");
    
    if (node) {
        // Set extra data
        setNodeExtraData(node, L"نوع", L"صحيح");
        setNodeExtraData(node, L"قيمة", L"10");
        
        test_pass("Parse Tree Extra Data");
        
        // Get extra data
        wchar_t* type = getNodeExtraData(node, L"نوع");
        wchar_t* value = getNodeExtraData(node, L"قيمة");
        wchar_t* not_found = getNodeExtraData(node, L"غير موجود");
        
        assert(type && wcscmp(type, L"صحيح") == 0);
        assert(value && wcscmp(value, L"10") == 0);
        assert(not_found == NULL);
        
        // Cleanup
        freeParseTreeNode(node);
        
    } else {
        test_fail("Parse Tree Extra Data", "Failed to create parse tree node");
    }
}

// Main test function
int main() {
    // Set locale for Unicode support
    setlocale(LC_ALL, "");
    
    printf("=== Parse Tree Unit Tests ===\n\n");
    
    // Run all tests
    test_create_parse_tree_node();
    test_create_parse_tree_node_with_children();
    test_create_parse_tree();
    test_build_parse_tree();
    test_add_child();
    test_remove_child();
    test_insert_child();
    test_replace_child();
    test_find_child();
    test_search_nodes();
    test_validate_parse_tree();
    test_parse_tree_statistics();
    test_parse_tree_node_properties();
    test_print_parse_tree();
    test_print_parse_tree_to_buffer();
    test_print_parse_tree_node_to_buffer();
    test_export_parse_tree_to_dot();
    test_export_parse_tree_to_xml();
    test_export_parse_tree_to_json();
    test_parse_tree_memory_management();
    test_parse_tree_error_handling();
    test_parse_tree_extra_data();
    
    // Print test summary
    test_summary();
    
    return (test_failed == 0) ? 0 : 1;
}

