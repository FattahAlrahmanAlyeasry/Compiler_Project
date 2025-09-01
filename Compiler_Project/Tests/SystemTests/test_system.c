#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../CompilerCore/lexer.h"
#include "../CompilerCore/parser.h"
#include "../CompilerCore/semantic.h"
#include "../CompilerCore/codegen.h"
#include "../CompilerCore/ast.h"
#include "../CompilerCore/parsetree.h"
#include "../CompilerCore/symboltable.h"

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

// Helper function to create a simple Arabic program
wchar_t* createSimpleArabicProgram() {
    return L"برنامج\n"
           L"[\n"
           L"  ثوابت\n"
           L"    ثابت صحيح = 10 ؛\n"
           L"  متغيرات\n"
           L"    متغير صحيح ؛\n"
           L"  إجراءات\n"
           L"    إجراء مرحبا (معامل صحيح) ؛\n"
           L"      اطبع \"مرحبا بالعالم\" ؛\n"
           L"    ؛\n"
           L"]\n"
           L":\n"
           L"  متغير = ثابت ؛\n"
           L"  اطبع متغير ؛\n"
           L"  استدعاء مرحبا (متغير) ؛\n"
           L"؛\n";
}

// Helper function to create a program with control structures
wchar_t* createControlStructuresProgram() {
    return L"برنامج\n"
           L"[\n"
           L"  متغيرات\n"
           L"    عداد صحيح ؛\n"
           L"    مجموع صحيح ؛\n"
           L"]\n"
           L":\n"
           L"  عداد = 1 ؛\n"
           L"  مجموع = 0 ؛\n"
           L"  طالما عداد <= 10\n"
           L"    مجموع = مجموع + عداد ؛\n"
           L"    عداد = عداد + 1 ؛\n"
           L"  ؛\n"
           L"  اطبع مجموع ؛\n"
           L"  إذا مجموع > 50\n"
           L"    اطبع \"المجموع كبير\" ؛\n"
           L"  وإلا\n"
           L"    اطبع \"المجموع صغير\" ؛\n"
           L"  ؛\n"
           L"؛\n";
}

// Helper function to create a program with arrays and records
wchar_t* createDataStructuresProgram() {
    return L"برنامج\n"
           L"[\n"
           L"  أنواع\n"
           L"    سجل طالب\n"
           L"      اسم نص ؛\n"
           L"      عمر صحيح ؛\n"
           L"      درجة حقيقي ؛\n"
           L"    ؛\n"
           L"  متغيرات\n"
           L"    قائمة صحيح [10] ؛\n"
           L"    طلاب طالب [5] ؛\n"
           L"    عداد صحيح ؛\n"
           L"]\n"
           L":\n"
           L"  عداد = 0 ؛\n"
           L"  طالما عداد < 10\n"
           L"    قائمة [عداد] = عداد * 2 ؛\n"
           L"    عداد = عداد + 1 ؛\n"
           L"  ؛\n"
           L"  طلاب [0].اسم = \"أحمد\" ؛\n"
           L"  طلاب [0].عمر = 20 ؛\n"
           L"  طلاب [0].درجة = 95.5 ؛\n"
           L"  اطبع قائمة [5] ؛\n"
           L"  اطبع طلاب [0].اسم ؛\n"
           L"؛\n";
}

// Helper function to create a program with arithmetic expressions
wchar_t* createArithmeticProgram() {
    return L"برنامج\n"
           L"[\n"
           L"  متغيرات\n"
           L"    أ صحيح ؛\n"
           L"    ب صحيح ؛\n"
           L"    ج صحيح ؛\n"
           L"    د حقيقي ؛\n"
           L"]\n"
           L":\n"
           L"  أ = 10 ؛\n"
           L"  ب = 5 ؛\n"
           L"  ج = (أ + ب) * 2 ؛\n"
           L"  د = أ / ب ؛\n"
           L"  اطبع ج ؛\n"
           L"  اطبع د ؛\n"
           L"  إذا أ > ب && ج > 20\n"
           L"    اطبع \"الشرط صحيح\" ؛\n"
           L"  ؛\n"
           L"؛\n";
}

// Helper function to create a program with procedures
wchar_t* createProceduresProgram() {
    return L"برنامج\n"
           L"[\n"
           L"  متغيرات\n"
           L"    ناتج صحيح ؛\n"
           L"  إجراءات\n"
           L"    إجراء جمع (أ صحيح، ب صحيح) ؛\n"
           L"      ناتج = أ + ب ؛\n"
           L"      اطبع ناتج ؛\n"
           L"    ؛\n"
           L"    إجراء ضرب (أ صحيح، ب صحيح) ؛\n"
           L"      ناتج = أ * ب ؛\n"
           L"      اطبع ناتج ؛\n"
           L"    ؛\n"
           L"]\n"
           L":\n"
           L"  استدعاء جمع (10، 20) ؛\n"
           L"  استدعاء ضرب (5، 6) ؛\n"
           L"؛\n";
}

// Test complete compilation pipeline with simple program
void test_simple_program_compilation() {
    test_start("Simple Program Compilation");
    
    wchar_t* source_code = createSimpleArabicProgram();
    
    // Initialize all components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Lexical analysis
        Token token;
        int token_count = 0;
        while (getNextToken(&lexer, &token) == 0 && token.type != END_OF_FILE) {
            token_count++;
        }
        
        // Reset lexer for parsing
        resetLexer(&lexer);
        
        // Syntactic analysis
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                int code_gen_result = generateCode(&code_generator, ast);
                
                if (code_gen_result == 0) {
                    test_pass("Simple Program Compilation");
                    
                    // Check results
                    assert(token_count > 0);
                    assert(parse_tree->node_count > 0);
                    assert(ast->node_count > 0);
                    assert(code_generator.instruction_count > 0);
                    
                } else {
                    test_fail("Simple Program Compilation", "Code generation failed");
                }
                
                freeAST(ast);
            } else {
                test_fail("Simple Program Compilation", "Semantic analysis failed");
            }
            
            freeParseTree(parse_tree);
        } else {
            test_fail("Simple Program Compilation", "Parsing failed");
        }
        
    } else {
        test_fail("Simple Program Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test compilation with control structures
void test_control_structures_compilation() {
    test_start("Control Structures Compilation");
    
    wchar_t* source_code = createControlStructuresProgram();
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Lexical analysis
        Token token;
        int token_count = 0;
        while (getNextToken(&lexer, &token) == 0 && token.type != END_OF_FILE) {
            token_count++;
        }
        
        // Reset lexer for parsing
        resetLexer(&lexer);
        
        // Syntactic analysis
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                int code_gen_result = generateCode(&code_generator, ast);
                
                if (code_gen_result == 0) {
                    test_pass("Control Structures Compilation");
                    
                    // Check for control structure instructions
                    int has_loop = 0;
                    int has_conditional = 0;
                    int has_branch = 0;
                    
                    for (int i = 0; i < code_generator.instruction_count; i++) {
                        if (strstr(code_generator.instructions[i], "j") != NULL) has_branch = 1;
                        if (strstr(code_generator.instructions[i], "b") != NULL) has_branch = 1;
                    }
                    
                    assert(has_branch);
                    
                } else {
                    test_fail("Control Structures Compilation", "Code generation failed");
                }
                
                freeAST(ast);
            } else {
                test_fail("Control Structures Compilation", "Semantic analysis failed");
            }
            
            freeParseTree(parse_tree);
        } else {
            test_fail("Control Structures Compilation", "Parsing failed");
        }
        
    } else {
        test_fail("Control Structures Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test compilation with data structures
void test_data_structures_compilation() {
    test_start("Data Structures Compilation");
    
    wchar_t* source_code = createDataStructuresProgram();
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Lexical analysis
        Token token;
        int token_count = 0;
        while (getNextToken(&lexer, &token) == 0 && token.type != END_OF_FILE) {
            token_count++;
        }
        
        // Reset lexer for parsing
        resetLexer(&lexer);
        
        // Syntactic analysis
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                int code_gen_result = generateCode(&code_generator, ast);
                
                if (code_gen_result == 0) {
                    test_pass("Data Structures Compilation");
                    
                    // Check for array and record handling
                    int has_memory_ops = 0;
                    int has_offset_calc = 0;
                    
                    for (int i = 0; i < code_generator.instruction_count; i++) {
                        if (strstr(code_generator.instructions[i], "lw") != NULL || 
                            strstr(code_generator.instructions[i], "sw") != NULL) has_memory_ops = 1;
                        if (strstr(code_generator.instructions[i], "sll") != NULL || 
                            strstr(code_generator.instructions[i], "add") != NULL) has_offset_calc = 1;
                    }
                    
                    assert(has_memory_ops);
                    
                } else {
                    test_fail("Data Structures Compilation", "Code generation failed");
                }
                
                freeAST(ast);
            } else {
                test_fail("Data Structures Compilation", "Semantic analysis failed");
            }
            
            freeParseTree(parse_tree);
        } else {
            test_fail("Data Structures Compilation", "Parsing failed");
        }
        
    } else {
        test_fail("Data Structures Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test compilation with arithmetic expressions
void test_arithmetic_expressions_compilation() {
    test_start("Arithmetic Expressions Compilation");
    
    wchar_t* source_code = createArithmeticProgram();
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Lexical analysis
        Token token;
        int token_count = 0;
        while (getNextToken(&lexer, &token) == 0 && token.type != END_OF_FILE) {
            token_count++;
        }
        
        // Reset lexer for parsing
        resetLexer(&lexer);
        
        // Syntactic analysis
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                int code_gen_result = generateCode(&code_generator, ast);
                
                if (code_gen_result == 0) {
                    test_pass("Arithmetic Expressions Compilation");
                    
                    // Check for arithmetic instructions
                    int has_arithmetic = 0;
                    int has_logical = 0;
                    
                    for (int i = 0; i < code_generator.instruction_count; i++) {
                        if (strstr(code_generator.instructions[i], "add") != NULL || 
                            strstr(code_generator.instructions[i], "sub") != NULL ||
                            strstr(code_generator.instructions[i], "mult") != NULL) has_arithmetic = 1;
                        if (strstr(code_generator.instructions[i], "and") != NULL || 
                            strstr(code_generator.instructions[i], "or") != NULL) has_logical = 1;
                    }
                    
                    assert(has_arithmetic);
                    
                } else {
                    test_fail("Arithmetic Expressions Compilation", "Code generation failed");
                }
                
                freeAST(ast);
            } else {
                test_fail("Arithmetic Expressions Compilation", "Semantic analysis failed");
            }
            
            freeParseTree(parse_tree);
        } else {
            test_fail("Arithmetic Expressions Compilation", "Parsing failed");
        }
        
    } else {
        test_fail("Arithmetic Expressions Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test compilation with procedures
void test_procedures_compilation() {
    test_start("Procedures Compilation");
    
    wchar_t* source_code = createProceduresProgram();
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Lexical analysis
        Token token;
        int token_count = 0;
        while (getNextToken(&lexer, &token) == 0 && token.type != END_OF_FILE) {
            token_count++;
        }
        
        // Reset lexer for parsing
        resetLexer(&lexer);
        
        // Syntactic analysis
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                int code_gen_result = generateCode(&code_generator, ast);
                
                if (code_gen_result == 0) {
                    test_pass("Procedures Compilation");
                    
                    // Check for procedure call instructions
                    int has_procedure_calls = 0;
                    int has_labels = 0;
                    
                    for (int i = 0; i < code_generator.instruction_count; i++) {
                        if (strstr(code_generator.instructions[i], "jal") != NULL) has_procedure_calls = 1;
                        if (strstr(code_generator.instructions[i], ":") != NULL) has_labels = 1;
                    }
                    
                    assert(has_procedure_calls || has_labels);
                    
                } else {
                    test_fail("Procedures Compilation", "Code generation failed");
                }
                
                freeAST(ast);
            } else {
                test_fail("Procedures Compilation", "Semantic analysis failed");
            }
            
            freeParseTree(parse_tree);
        } else {
            test_fail("Procedures Compilation", "Parsing failed");
        }
        
    } else {
        test_fail("Procedures Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test error handling in compilation pipeline
void test_error_handling_compilation() {
    test_start("Error Handling Compilation");
    
    // Create program with syntax errors
    wchar_t* source_code = L"برنامج\n"
                           L"[\n"
                           L"  متغيرات\n"
                           L"    متغير صحيح ؛\n"
                           L"]\n"
                           L":\n"
                           L"  متغير = ؛\n"  // Missing expression
                           L"  اطبع ؛\n"     // Missing expression
                           L"؛\n";
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Lexical analysis should succeed
        Token token;
        int token_count = 0;
        while (getNextToken(&lexer, &token) == 0 && token.type != END_OF_FILE) {
            token_count++;
        }
        
        assert(token_count > 0);
        
        // Reset lexer for parsing
        resetLexer(&lexer);
        
        // Parsing should detect syntax errors
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        // Check for parser errors
        if (getParserErrorCount(&parser) > 0) {
            test_pass("Error Handling Compilation");
            
            // Verify error details
            ParserError* errors = getParserErrors(&parser);
            assert(errors != NULL);
            
        } else {
            test_fail("Error Handling Compilation", "Parser did not detect syntax errors");
        }
        
        if (parse_tree) {
            freeParseTree(parse_tree);
        }
        
    } else {
        test_fail("Error Handling Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test memory management in compilation pipeline
void test_memory_management_compilation() {
    test_start("Memory Management Compilation");
    
    wchar_t* source_code = createSimpleArabicProgram();
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Run compilation multiple times to test memory management
        for (int iteration = 0; iteration < 10; iteration++) {
            // Reset lexer
            resetLexer(&lexer);
            
            // Parse
            ParseTree* parse_tree = parse(&parser, &lexer);
            
            if (parse_tree && parse_tree->root) {
                // Semantic analysis
                AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
                
                if (ast && ast->root) {
                    // Code generation
                    generateCode(&code_generator, ast);
                    
                    // Cleanup iteration
                    freeAST(ast);
                }
                
                freeParseTree(parse_tree);
            }
        }
        
        test_pass("Memory Management Compilation");
        
    } else {
        test_fail("Memory Management Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test performance of compilation pipeline
void test_performance_compilation() {
    test_start("Performance Compilation");
    
    wchar_t* source_code = createSimpleArabicProgram();
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Measure compilation time
        clock_t start_time = clock();
        
        // Reset lexer
        resetLexer(&lexer);
        
        // Parse
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                generateCode(&code_generator, ast);
                
                clock_t end_time = clock();
                double compilation_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
                
                test_pass("Performance Compilation");
                
                // Check performance (should complete within reasonable time)
                assert(compilation_time < 1.0); // Less than 1 second
                
                freeAST(ast);
            }
            
            freeParseTree(parse_tree);
        }
        
    } else {
        test_fail("Performance Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test MIPS code generation quality
void test_mips_code_quality() {
    test_start("MIPS Code Quality");
    
    wchar_t* source_code = createArithmeticProgram();
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Reset lexer
        resetLexer(&lexer);
        
        // Parse
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                int code_gen_result = generateCode(&code_generator, ast);
                
                if (code_gen_result == 0) {
                    test_pass("MIPS Code Quality");
                    
                    // Check MIPS code structure
                    int has_data_section = 0;
                    int has_text_section = 0;
                    int has_main_label = 0;
                    int has_syscall = 0;
                    
                    for (int i = 0; i < code_generator.instruction_count; i++) {
                        if (strstr(code_generator.instructions[i], ".data") != NULL) has_data_section = 1;
                        if (strstr(code_generator.instructions[i], ".text") != NULL) has_text_section = 1;
                        if (strstr(code_generator.instructions[i], "main:") != NULL) has_main_label = 1;
                        if (strstr(code_generator.instructions[i], "syscall") != NULL) has_syscall = 1;
                    }
                    
                    assert(has_data_section);
                    assert(has_text_section);
                    assert(has_main_label);
                    assert(has_syscall);
                    
                    // Test MIPS code saving
                    int save_result = saveMIPSCodeToFile(&code_generator, "test_output.s");
                    assert(save_result == 0);
                    
                    // Clean up test file
                    remove("test_output.s");
                    
                } else {
                    test_fail("MIPS Code Quality", "Code generation failed");
                }
                
                freeAST(ast);
            }
            
            freeParseTree(parse_tree);
        }
        
    } else {
        test_fail("MIPS Code Quality", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Test Unicode support in compilation pipeline
void test_unicode_support_compilation() {
    test_start("Unicode Support Compilation");
    
    // Create program with complex Arabic text
    wchar_t* source_code = L"برنامج\n"
                           L"[\n"
                           L"  متغيرات\n"
                           L"    اسم_المتغير صحيح ؛\n"
                           L"    قيمة_المتغير حقيقي ؛\n"
                           L"]\n"
                           L":\n"
                           L"  اسم_المتغير = 42 ؛\n"
                           L"  قيمة_المتغير = 3.14159 ؛\n"
                           L"  اطبع \"مرحبا بالعالم العربي\" ؛\n"
                           L"  اطبع \"اسم المتغير: \" ؛\n"
                           L"  اطبع اسم_المتغير ؛\n"
                           L"؛\n";
    
    // Initialize components
    Lexer lexer;
    Parser parser;
    SemanticAnalyzer semantic_analyzer;
    CodeGenerator code_generator;
    
    int result1 = initLexer(&lexer, source_code);
    int result2 = initParser(&parser);
    int result3 = initSemanticAnalyzer(&semantic_analyzer);
    int result4 = initCodeGenerator(&code_generator);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        // Reset lexer
        resetLexer(&lexer);
        
        // Parse
        ParseTree* parse_tree = parse(&parser, &lexer);
        
        if (parse_tree && parse_tree->root) {
            // Semantic analysis
            AST* ast = analyzeProgram(&semantic_analyzer, parse_tree);
            
            if (ast && ast->root) {
                // Code generation
                int code_gen_result = generateCode(&code_generator, ast);
                
                if (code_gen_result == 0) {
                    test_pass("Unicode Support Compilation");
                    
                    // Check that Arabic identifiers were processed correctly
                    int has_arabic_strings = 0;
                    
                    for (int i = 0; i < code_generator.instruction_count; i++) {
                        if (strstr(code_generator.instructions[i], "مرحبا") != NULL || 
                            strstr(code_generator.instructions[i], "العالم") != NULL) {
                            has_arabic_strings = 1;
                            break;
                        }
                    }
                    
                    assert(has_arabic_strings);
                    
                } else {
                    test_fail("Unicode Support Compilation", "Code generation failed");
                }
                
                freeAST(ast);
            }
            
            freeParseTree(parse_tree);
        }
        
    } else {
        test_fail("Unicode Support Compilation", "Component initialization failed");
    }
    
    // Cleanup
    freeLexer(&lexer);
    freeParser(&parser);
    freeSemanticAnalyzer(&semantic_analyzer);
    freeCodeGenerator(&code_generator);
}

// Main test function
int main() {
    // Set locale for Unicode support
    setlocale(LC_ALL, "");
    
    printf("=== System Tests ===\n\n");
    
    // Run all system tests
    test_simple_program_compilation();
    test_control_structures_compilation();
    test_data_structures_compilation();
    test_arithmetic_expressions_compilation();
    test_procedures_compilation();
    test_error_handling_compilation();
    test_memory_management_compilation();
    test_performance_compilation();
    test_mips_code_quality();
    test_unicode_support_compilation();
    
    // Print test summary
    test_summary();
    
    return (test_failed == 0) ? 0 : 1;
}

