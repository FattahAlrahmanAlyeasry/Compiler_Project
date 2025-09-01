#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../codegen.h"
#include "../ast.h"

// Test function declarations
void test_initCodeGenerator();
void test_generateCode();
void test_generateProgram();
void test_generateStatement();
void test_generateExpression();
void test_register_management();
void test_instruction_emission();
void test_error_handling();
void test_output_functions();

int main() {
    printf("=== Testing MIPS Code Generator ===\n\n");
    
    test_initCodeGenerator();
    test_generateCode();
    test_generateProgram();
    test_generateStatement();
    test_generateExpression();
    test_register_management();
    test_instruction_emission();
    test_error_handling();
    test_output_functions();
    
    printf("\n=== All MIPS Code Generator Tests Passed! ===\n");
    return 0;
}

void test_initCodeGenerator() {
    printf("Testing initCodeGenerator...\n");
    
    CodeGenerator generator;
    int result = initCodeGenerator(&generator);
    
    assert(result == 1);
    assert(generator.ast != NULL);
    assert(generator.instruction_count == 0);
    assert(generator.instructions != NULL);
    assert(generator.available_registers != NULL);
    assert(generator.next_register == 0);
    
    printf("✓ initCodeGenerator test passed\n");
}

void test_generateCode() {
    printf("Testing generateCode...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    // Create a simple AST for testing
    ASTNode* program_node = createASTNode(AST_NODE_PROGRAM, L"برنامج");
    ASTNode* block_node = createASTNode(AST_NODE_BLOCK, L"[]");
    ASTNode* print_node = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع");
    ASTNode* literal_node = createASTNode(AST_NODE_LITERAL, L"42");
    
    addChild(program_node, block_node);
    addChild(block_node, print_node);
    addChild(print_node, literal_node);
    
    generator.ast = program_node;
    
    int result = generateCode(&generator);
    
    assert(result == 1);
    assert(generator.instruction_count > 0);
    
    freeAST(program_node);
    printf("✓ generateCode test passed\n");
}

void test_generateProgram() {
    printf("Testing generateProgram...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    ASTNode* program_node = createASTNode(AST_NODE_PROGRAM, L"برنامج");
    ASTNode* block_node = createASTNode(AST_NODE_BLOCK, L"[]");
    
    addChild(program_node, block_node);
    
    int result = generateProgram(&generator, program_node);
    
    assert(result == 1);
    assert(generator.instruction_count > 0);
    
    // Check for main function
    MIPSInstruction* first_instruction = &generator.instructions[0];
    assert(first_instruction->type == MIPS_INSTRUCTION_LABEL);
    
    freeAST(program_node);
    printf("✓ generateProgram test passed\n");
}

void test_generateStatement() {
    printf("Testing generateStatement...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    // Test print statement
    ASTNode* print_node = createASTNode(AST_NODE_PRINT_STATEMENT, L"اطبع");
    ASTNode* literal_node = createASTNode(AST_NODE_LITERAL, L"42");
    
    addChild(print_node, literal_node);
    
    int result = generateStatement(&generator, print_node);
    
    assert(result == 1);
    assert(generator.instruction_count > 0);
    
    freeAST(print_node);
    
    // Test assignment statement
    ASTNode* assign_node = createASTNode(AST_NODE_ASSIGNMENT, L"=");
    ASTNode* var_node = createASTNode(AST_NODE_IDENTIFIER, L"متغير");
    ASTNode* value_node = createASTNode(AST_NODE_LITERAL, L"42");
    
    addChild(assign_node, var_node);
    addChild(assign_node, value_node);
    
    result = generateStatement(&generator, assign_node);
    
    assert(result == 1);
    
    freeAST(assign_node);
    printf("✓ generateStatement test passed\n");
}

void test_generateExpression() {
    printf("Testing generateExpression...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    // Test arithmetic expression
    ASTNode* expr_node = createASTNode(AST_NODE_ARITHMETIC_EXPRESSION, L"+");
    ASTNode* left_node = createASTNode(AST_NODE_LITERAL, L"5");
    ASTNode* right_node = createASTNode(AST_NODE_LITERAL, L"3");
    
    addChild(expr_node, left_node);
    addChild(expr_node, right_node);
    
    MIPSRegister result_reg = generateExpression(&generator, expr_node);
    
    assert(result_reg != MIPS_REGISTER_NONE);
    assert(generator.instruction_count > 0);
    
    freeAST(expr_node);
    printf("✓ generateExpression test passed\n");
}

void test_register_management() {
    printf("Testing register management...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    // Test register allocation
    MIPSRegister reg1 = allocateRegister(&generator);
    assert(reg1 != MIPS_REGISTER_NONE);
    
    MIPSRegister reg2 = allocateRegister(&generator);
    assert(reg2 != MIPS_REGISTER_NONE);
    assert(reg2 != reg1);
    
    // Test register freeing
    freeRegister(&generator, reg1);
    freeRegister(&generator, reg2);
    
    // Test that registers are available again
    MIPSRegister reg3 = allocateRegister(&generator);
    assert(reg3 != MIPS_REGISTER_NONE);
    
    printf("✓ Register management test passed\n");
}

void test_instruction_emission() {
    printf("Testing instruction emission...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    // Test emitting different types of instructions
    emit(&generator, MIPS_INSTRUCTION_LI, MIPS_REGISTER_T0, MIPS_REGISTER_NONE, MIPS_REGISTER_NONE, 42);
    emit(&generator, MIPS_INSTRUCTION_ADD, MIPS_REGISTER_T1, MIPS_REGISTER_T0, MIPS_REGISTER_T0, 0);
    emit(&generator, MIPS_INSTRUCTION_MOVE, MIPS_REGISTER_A0, MIPS_REGISTER_T1, MIPS_REGISTER_NONE, 0);
    emit(&generator, MIPS_INSTRUCTION_SYSCALL, MIPS_REGISTER_NONE, MIPS_REGISTER_NONE, MIPS_REGISTER_NONE, 0);
    
    assert(generator.instruction_count == 4);
    
    // Check instruction types
    assert(generator.instructions[0].type == MIPS_INSTRUCTION_LI);
    assert(generator.instructions[1].type == MIPS_INSTRUCTION_ADD);
    assert(generator.instructions[2].type == MIPS_INSTRUCTION_MOVE);
    assert(generator.instructions[3].type == MIPS_INSTRUCTION_SYSCALL);
    
    printf("✓ Instruction emission test passed\n");
}

void test_error_handling() {
    printf("Testing error handling...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    // Test error reporting
    codeGenError(&generator, L"Test error message");
    
    assert(generator.error_count > 0);
    
    // Test getting error count
    int error_count = getCodeGenErrorCount(&generator);
    assert(error_count > 0);
    
    printf("✓ Error handling test passed\n");
}

void test_output_functions() {
    printf("Testing output functions...\n");
    
    CodeGenerator generator;
    initCodeGenerator(&generator);
    
    // Add some instructions
    emit(&generator, MIPS_INSTRUCTION_LI, MIPS_REGISTER_T0, MIPS_REGISTER_NONE, MIPS_REGISTER_NONE, 42);
    emit(&generator, MIPS_INSTRUCTION_MOVE, MIPS_REGISTER_A0, MIPS_REGISTER_T0, MIPS_REGISTER_NONE, 0);
    emit(&generator, MIPS_INSTRUCTION_SYSCALL, MIPS_REGISTER_NONE, MIPS_REGISTER_NONE, MIPS_REGISTER_NONE, 0);
    
    // Test printing MIPS code
    printf("Generated MIPS code:\n");
    printMIPSCode(&generator);
    
    // Test saving to file
    int result = saveMIPSCodeToFile(&generator, L"test_output.s");
    assert(result == 1);
    
    printf("✓ Output functions test passed\n");
}
