#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <wchar.h>
#include "../CompilerCore/lexer.h"
#include "../CompilerCore/parser.h"
#include "../CompilerCore/semantic.h"
#include "../CompilerCore/codegen.h"

// متغيرات عامة للاختبار
static int test_count = 0;
static int passed_count = 0;
static int failed_count = 0;

// دوال مساعدة للاختبار
void test_start(const char* test_name) {
    printf("اختبار الأداء: %s\n", test_name);
    test_count++;
}

void test_pass(const char* test_name) {
    printf("✓ نجح: %s\n", test_name);
    passed_count++;
}

void test_fail(const char* test_name, const char* reason) {
    printf("✗ فشل: %s - السبب: %s\n", test_name, reason);
    failed_count++;
}

void test_summary() {
    printf("\n=== ملخص اختبارات الأداء ===\n");
    printf("إجمالي الاختبارات: %d\n", test_count);
    printf("الاختبارات الناجحة: %d\n", passed_count);
    printf("الاختبارات الفاشلة: %d\n", failed_count);
    printf("نسبة النجاح: %.1f%%\n", (float)passed_count / test_count * 100);
}

// دالة لقياس الوقت
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// دالة لقياس استخدام الذاكرة (تقريبية)
size_t get_memory_usage() {
    // في Windows، يمكن استخدام GetProcessMemoryInfo
    // في Linux، يمكن قراءة /proc/self/status
    // هنا نستخدم تقريب بسيط
    return 0; // سيتم تحديثها لاحقاً
}

// إنشاء ملف اختبار بحجم معين
FILE* create_large_test_file(const char* filename, int size_kb) {
    FILE* file = fopen(filename, "w,ccs=UTF-8");
    if (!file) return NULL;
    
    // إنشاء برنامج كبير
    fwprintf(file, L"برنامج اختبار_كبير;\n");
    fwprintf(file, L"[\n");
    fwprintf(file, L"  ثوابت\n");
    
    // إضافة ثوابت كثيرة
    for (int i = 0; i < size_kb / 2; i++) {
        fwprintf(file, L"    ثابت_%d = %d;\n", i, i);
    }
    
    fwprintf(file, L"  أنواع\n");
    fwprintf(file, L"    نوع_مصفوفة = مصفوفة[100] من عدد صحيح;\n");
    
    fwprintf(file, L"  متغيرات\n");
    for (int i = 0; i < size_kb / 4; i++) {
        fwprintf(file, L"    متغير_%d : عدد صحيح;\n", i);
    }
    
    fwprintf(file, L"  إجراءات\n");
    fwprintf(file, L"    إجراء إجراء_كبير;\n");
    fwprintf(file, L"    بداية\n");
    
    // إضافة عبارات كثيرة
    for (int i = 0; i < size_kb / 2; i++) {
        fwprintf(file, L"      متغير_%d = ثابت_%d;\n", i % (size_kb / 4), i);
        fwprintf(file, L"      إذا متغير_%d > 50 إذن\n", i % (size_kb / 4));
        fwprintf(file, L"        اطبع متغير_%d;\n", i % (size_kb / 4));
        fwprintf(file, L"      نهاية;\n");
    }
    
    fwprintf(file, L"    نهاية;\n");
    fwprintf(file, L"  بداية\n");
    fwprintf(file, L"    إجراء_كبير;\n");
    fwprintf(file, L"  نهاية\n");
    fwprintf(file, L"].");
    
    fclose(file);
    return fopen(filename, "r,ccs=UTF-8");
}

// اختبار أداء المحلل المعجمي
void test_lexer_performance() {
    test_start("أداء المحلل المعجمي");
    
    const char* test_file = "test_large.arabic";
    FILE* file = create_large_test_file(test_file, 100); // 100 KB
    
    if (file) {
        double start_time = get_time();
        size_t start_memory = get_memory_usage();
        
        Lexer* lexer = initLexer(test_file);
        if (lexer) {
            Token* token;
            int token_count = 0;
            
            // قراءة جميع الرموز
            while ((token = getNextToken(lexer)) != NULL && token->type != TOKEN_EOF) {
                token_count++;
                freeToken(token);
            }
            
            double end_time = get_time();
            size_t end_memory = get_memory_usage();
            
            double duration = end_time - start_time;
            size_t memory_used = end_memory - start_memory;
            
            printf("  عدد الرموز: %d\n", token_count);
            printf("  الوقت المستغرق: %.3f ثانية\n", duration);
            printf("  الذاكرة المستخدمة: %zu بايت\n", memory_used);
            printf("  سرعة المعالجة: %.0f رمز/ثانية\n", token_count / duration);
            
            if (duration < 1.0) { // يجب أن يكتمل في أقل من ثانية
                test_pass("أداء المحلل المعجمي");
            } else {
                test_fail("أداء المحلل المعجمي", "بطيء جداً");
            }
            
            freeLexer(lexer);
        }
        
        fclose(file);
        remove(test_file);
    }
}

// اختبار أداء المحلل النحوي
void test_parser_performance() {
    test_start("أداء المحلل النحوي");
    
    const char* test_file = "test_large.arabic";
    FILE* file = create_large_test_file(test_file, 50); // 50 KB
    
    if (file) {
        double start_time = get_time();
        size_t start_memory = get_memory_usage();
        
        Lexer* lexer = initLexer(test_file);
        if (lexer) {
            Parser* parser = initParser(lexer);
            if (parser) {
                ParseTree* tree = parse(parser);
                
                double end_time = get_time();
                size_t end_memory = get_memory_usage();
                
                double duration = end_time - start_time;
                size_t memory_used = end_memory - start_memory;
                
                if (tree) {
                    int node_count = getNodeCount(tree);
                    int depth = getTreeDepth(tree);
                    
                    printf("  عدد العقد: %d\n", node_count);
                    printf("  عمق الشجرة: %d\n", depth);
                    printf("  الوقت المستغرق: %.3f ثانية\n", duration);
                    printf("  الذاكرة المستخدمة: %zu بايت\n", memory_used);
                    printf("  سرعة المعالجة: %.0f عقدة/ثانية\n", node_count / duration);
                    
                    if (duration < 2.0) { // يجب أن يكتمل في أقل من ثانيتين
                        test_pass("أداء المحلل النحوي");
                    } else {
                        test_fail("أداء المحلل النحوي", "بطيء جداً");
                    }
                    
                    freeParseTree(tree);
                } else {
                    test_fail("أداء المحلل النحوي", "فشل في بناء شجرة الإعراب");
                }
                
                freeParser(parser);
            }
            freeLexer(lexer);
        }
        
        fclose(file);
        remove(test_file);
    }
}

// اختبار أداء المحلل الدلالي
void test_semantic_performance() {
    test_start("أداء المحلل الدلالي");
    
    const char* test_file = "test_large.arabic";
    FILE* file = create_large_test_file(test_file, 30); // 30 KB
    
    if (file) {
        double start_time = get_time();
        size_t start_memory = get_memory_usage();
        
        Lexer* lexer = initLexer(test_file);
        if (lexer) {
            Parser* parser = initParser(lexer);
            if (parser) {
                ParseTree* tree = parse(parser);
                if (tree) {
                    SemanticAnalyzer* analyzer = initSemanticAnalyzer(tree);
                    if (analyzer) {
                        int result = analyzeProgram(analyzer);
                        
                        double end_time = get_time();
                        size_t end_memory = get_memory_usage();
                        
                        double duration = end_time - start_time;
                        size_t memory_used = end_memory - start_memory;
                        
                        printf("  نتيجة التحليل: %s\n", result == 0 ? "نجح" : "فشل");
                        printf("  الوقت المستغرق: %.3f ثانية\n", duration);
                        printf("  الذاكرة المستخدمة: %zu بايت\n", memory_used);
                        
                        if (duration < 1.5) { // يجب أن يكتمل في أقل من 1.5 ثانية
                            test_pass("أداء المحلل الدلالي");
                        } else {
                            test_fail("أداء المحلل الدلالي", "بطيء جداً");
                        }
                        
                        freeSemanticAnalyzer(analyzer);
                    } else {
                        test_fail("أداء المحلل الدلالي", "فشل في تهيئة المحلل الدلالي");
                    }
                    freeParseTree(tree);
                }
                freeParser(parser);
            }
            freeLexer(lexer);
        }
        
        fclose(file);
        remove(test_file);
    }
}

// اختبار أداء مولد الكود
void test_codegen_performance() {
    test_start("أداء مولد الكود");
    
    const char* test_file = "test_large.arabic";
    FILE* file = create_large_test_file(test_file, 20); // 20 KB
    
    if (file) {
        double start_time = get_time();
        size_t start_memory = get_memory_usage();
        
        Lexer* lexer = initLexer(test_file);
        if (lexer) {
            Parser* parser = initParser(lexer);
            if (parser) {
                ParseTree* tree = parse(parser);
                if (tree) {
                    SemanticAnalyzer* analyzer = initSemanticAnalyzer(tree);
                    if (analyzer) {
                        ASTNode* ast = buildASTFromParseTree(analyzer);
                        if (ast) {
                            CodeGenerator* generator = initCodeGenerator();
                            if (generator) {
                                int result = generateCode(generator, ast);
                                
                                double end_time = get_time();
                                size_t end_memory = get_memory_usage();
                                
                                double duration = end_time - start_time;
                                size_t memory_used = end_memory - start_memory;
                                
                                printf("  نتيجة توليد الكود: %s\n", result == 0 ? "نجح" : "فشل");
                                printf("  الوقت المستغرق: %.3f ثانية\n", duration);
                                printf("  الذاكرة المستخدمة: %zu بايت\n", memory_used);
                                
                                if (duration < 2.0) { // يجب أن يكتمل في أقل من ثانيتين
                                    test_pass("أداء مولد الكود");
                                } else {
                                    test_fail("أداء مولد الكود", "بطيء جداً");
                                }
                                
                                freeCodeGenerator(generator);
                            } else {
                                test_fail("أداء مولد الكود", "فشل في تهيئة مولد الكود");
                            }
                            freeAST(ast);
                        } else {
                            test_fail("أداء مولد الكود", "فشل في بناء AST");
                        }
                        freeSemanticAnalyzer(analyzer);
                    } else {
                        test_fail("أداء مولد الكود", "فشل في تهيئة المحلل الدلالي");
                    }
                    freeParseTree(tree);
                }
                freeParser(parser);
            }
            freeLexer(lexer);
        }
        
        fclose(file);
        remove(test_file);
    }
}

// اختبار الأداء الشامل
void test_overall_performance() {
    test_start("الأداء الشامل");
    
    const char* test_file = "test_complete.arabic";
    FILE* file = create_large_test_file(test_file, 100); // 100 KB
    
    if (file) {
        double start_time = get_time();
        size_t start_memory = get_memory_usage();
        
        // المرحلة 1: التحليل المعجمي
        double lexer_start = get_time();
        Lexer* lexer = initLexer(test_file);
        if (!lexer) {
            test_fail("الأداء الشامل", "فشل في تهيئة المحلل المعجمي");
            fclose(file);
            remove(test_file);
            return;
        }
        
        Token* token;
        int token_count = 0;
        while ((token = getNextToken(lexer)) != NULL && token->type != TOKEN_EOF) {
            token_count++;
            freeToken(token);
        }
        double lexer_time = get_time() - lexer_start;
        
        // المرحلة 2: التحليل النحوي
        double parser_start = get_time();
        Parser* parser = initParser(lexer);
        if (!parser) {
            test_fail("الأداء الشامل", "فشل في تهيئة المحلل النحوي");
            freeLexer(lexer);
            fclose(file);
            remove(test_file);
            return;
        }
        
        ParseTree* tree = parse(parser);
        if (!tree) {
            test_fail("الأداء الشامل", "فشل في بناء شجرة الإعراب");
            freeParser(parser);
            freeLexer(lexer);
            fclose(file);
            remove(test_file);
            return;
        }
        double parser_time = get_time() - parser_start;
        
        // المرحلة 3: التحليل الدلالي
        double semantic_start = get_time();
        SemanticAnalyzer* analyzer = initSemanticAnalyzer(tree);
        if (!analyzer) {
            test_fail("الأداء الشامل", "فشل في تهيئة المحلل الدلالي");
            freeParseTree(tree);
            freeParser(parser);
            freeLexer(lexer);
            fclose(file);
            remove(test_file);
            return;
        }
        
        int semantic_result = analyzeProgram(analyzer);
        double semantic_time = get_time() - semantic_start;
        
        // المرحلة 4: توليد الكود
        double codegen_start = get_time();
        ASTNode* ast = buildASTFromParseTree(analyzer);
        if (!ast) {
            test_fail("الأداء الشامل", "فشل في بناء AST");
            freeSemanticAnalyzer(analyzer);
            freeParseTree(tree);
            freeParser(parser);
            freeLexer(lexer);
            fclose(file);
            remove(test_file);
            return;
        }
        
        CodeGenerator* generator = initCodeGenerator();
        if (!generator) {
            test_fail("الأداء الشامل", "فشل في تهيئة مولد الكود");
            freeAST(ast);
            freeSemanticAnalyzer(analyzer);
            freeParseTree(tree);
            freeParser(parser);
            freeLexer(lexer);
            fclose(file);
            remove(test_file);
            return;
        }
        
        int codegen_result = generateCode(generator, ast);
        double codegen_time = get_time() - codegen_start;
        
        double total_time = get_time() - start_time;
        size_t total_memory = get_memory_usage() - start_memory;
        
        // عرض النتائج
        printf("  === نتائج الأداء الشامل ===\n");
        printf("  عدد الرموز: %d\n", token_count);
        printf("  عدد عقد شجرة الإعراب: %d\n", getNodeCount(tree));
        printf("  عمق شجرة الإعراب: %d\n", getTreeDepth(tree));
        printf("  \n");
        printf("  === أوقات المراحل ===\n");
        printf("  التحليل المعجمي: %.3f ثانية (%.1f%%)\n", lexer_time, (lexer_time/total_time)*100);
        printf("  التحليل النحوي: %.3f ثانية (%.1f%%)\n", parser_time, (parser_time/total_time)*100);
        printf("  التحليل الدلالي: %.3f ثانية (%.1f%%)\n", semantic_time, (semantic_time/total_time)*100);
        printf("  توليد الكود: %.3f ثانية (%.1f%%)\n", codegen_time, (codegen_time/total_time)*100);
        printf("  \n");
        printf("  === الإجمالي ===\n");
        printf("  الوقت الإجمالي: %.3f ثانية\n", total_time);
        printf("  الذاكرة المستخدمة: %zu بايت\n", total_memory);
        printf("  سرعة المعالجة: %.0f رمز/ثانية\n", token_count / total_time);
        
        // تقييم الأداء
        if (total_time < 5.0) { // يجب أن يكتمل في أقل من 5 ثوان
            test_pass("الأداء الشامل");
        } else {
            test_fail("الأداء الشامل", "بطيء جداً");
        }
        
        // تحرير الذاكرة
        freeCodeGenerator(generator);
        freeAST(ast);
        freeSemanticAnalyzer(analyzer);
        freeParseTree(tree);
        freeParser(parser);
        freeLexer(lexer);
        
        fclose(file);
        remove(test_file);
    }
}

// اختبار الأداء مع أحجام مختلفة
void test_scalability() {
    test_start("قابلية التوسع");
    
    int sizes[] = {10, 25, 50, 100}; // أحجام بالكيلوبايت
    int size_count = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("  اختبار قابلية التوسع مع أحجام مختلفة:\n");
    
    for (int i = 0; i < size_count; i++) {
        const char* test_file = "test_scalability.arabic";
        FILE* file = create_large_test_file(test_file, sizes[i]);
        
        if (file) {
            double start_time = get_time();
            
            Lexer* lexer = initLexer(test_file);
            if (lexer) {
                Parser* parser = initParser(lexer);
                if (parser) {
                    ParseTree* tree = parse(parser);
                    if (tree) {
                        double end_time = get_time();
                        double duration = end_time - start_time;
                        
                        int token_count = 0;
                        Token* token;
                        rewind(file);
                        while ((token = getNextToken(lexer)) != NULL && token->type != TOKEN_EOF) {
                            token_count++;
                            freeToken(token);
                        }
                        
                        printf("    %d KB: %.3f ثانية, %d رمز (%.0f رمز/ثانية)\n", 
                               sizes[i], duration, token_count, token_count / duration);
                        
                        freeParseTree(tree);
                    }
                    freeParser(parser);
                }
                freeLexer(lexer);
            }
            
            fclose(file);
            remove(test_file);
        }
    }
    
    test_pass("قابلية التوسع");
}

// اختبار استهلاك الذاكرة
void test_memory_consumption() {
    test_start("استهلاك الذاكرة");
    
    const char* test_file = "test_memory.arabic";
    FILE* file = create_large_test_file(test_file, 50); // 50 KB
    
    if (file) {
        size_t initial_memory = get_memory_usage();
        
        // تشغيل المترجم
        Lexer* lexer = initLexer(test_file);
        if (lexer) {
            size_t after_lexer = get_memory_usage();
            
            Parser* parser = initParser(lexer);
            if (parser) {
                size_t after_parser = get_memory_usage();
                
                ParseTree* tree = parse(parser);
                if (tree) {
                    size_t after_parse = get_memory_usage();
                    
                    SemanticAnalyzer* analyzer = initSemanticAnalyzer(tree);
                    if (analyzer) {
                        size_t after_semantic = get_memory_usage();
                        
                        ASTNode* ast = buildASTFromParseTree(analyzer);
                        if (ast) {
                            size_t after_ast = get_memory_usage();
                            
                            CodeGenerator* generator = initCodeGenerator();
                            if (generator) {
                                size_t after_codegen = get_memory_usage();
                                
                                printf("  استهلاك الذاكرة في كل مرحلة:\n");
                                printf("    بعد المحلل المعجمي: %zu بايت\n", after_lexer - initial_memory);
                                printf("    بعد المحلل النحوي: %zu بايت\n", after_parser - initial_memory);
                                printf("    بعد بناء شجرة الإعراب: %zu بايت\n", after_parse - initial_memory);
                                printf("    بعد التحليل الدلالي: %zu بايت\n", after_semantic - initial_memory);
                                printf("    بعد بناء AST: %zu بايت\n", after_ast - initial_memory);
                                printf("    بعد توليد الكود: %zu بايت\n", after_codegen - initial_memory);
                                printf("    إجمالي الذاكرة: %zu بايت\n", after_codegen - initial_memory);
                                
                                // تقييم استهلاك الذاكرة
                                if ((after_codegen - initial_memory) < 1024 * 1024) { // أقل من 1 MB
                                    test_pass("استهلاك الذاكرة");
                                } else {
                                    test_fail("استهلاك الذاكرة", "استهلاك عالي جداً للذاكرة");
                                }
                                
                                freeCodeGenerator(generator);
                            }
                            freeAST(ast);
                        }
                        freeSemanticAnalyzer(analyzer);
                    }
                    freeParseTree(tree);
                }
                freeParser(parser);
            }
            freeLexer(lexer);
        }
        
        fclose(file);
        remove(test_file);
    }
}

// اختبار الأداء تحت الضغط
void test_stress_performance() {
    test_start("الأداء تحت الضغط");
    
    printf("  تشغيل 10 ترجمات متتالية...\n");
    
    double total_time = 0.0;
    int success_count = 0;
    
    for (int i = 0; i < 10; i++) {
        const char* test_file = "test_stress.arabic";
        FILE* file = create_large_test_file(test_file, 20); // 20 KB
        
        if (file) {
            double start_time = get_time();
            
            Lexer* lexer = initLexer(test_file);
            if (lexer) {
                Parser* parser = initParser(lexer);
                if (parser) {
                    ParseTree* tree = parse(parser);
                    if (tree) {
                        double end_time = get_time();
                        double duration = end_time - start_time;
                        
                        total_time += duration;
                        success_count++;
                        
                        printf("    الترجمة %d: %.3f ثانية\n", i + 1, duration);
                        
                        freeParseTree(tree);
                    }
                    freeParser(parser);
                }
                freeLexer(lexer);
            }
            
            fclose(file);
            remove(test_file);
        }
    }
    
    if (success_count == 10) {
        double avg_time = total_time / 10;
        printf("  متوسط الوقت: %.3f ثانية\n", avg_time);
        
        if (avg_time < 1.0) {
            test_pass("الأداء تحت الضغط");
        } else {
            test_fail("الأداء تحت الضغط", "متوسط الوقت عالي جداً");
        }
    } else {
        test_fail("الأداء تحت الضغط", "فشل في بعض الترجمات");
    }
}

// الدالة الرئيسية للاختبارات
int main() {
    printf("=== بدء اختبارات الأداء ===\n\n");
    
    // تعيين اللغة العربية
    setlocale(LC_ALL, "en_US.UTF-8");
    
    // تشغيل جميع اختبارات الأداء
    test_lexer_performance();
    test_parser_performance();
    test_semantic_performance();
    test_codegen_performance();
    test_overall_performance();
    test_scalability();
    test_memory_consumption();
    test_stress_performance();
    
    // عرض ملخص النتائج
    printf("\n");
    test_summary();
    
    return (failed_count == 0) ? 0 : 1;
}

