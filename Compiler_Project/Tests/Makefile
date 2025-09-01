# Makefile لاختبارات مترجم اللغة البرمجة العربية
# Visual Studio 2015 Compatible

# المتغيرات الأساسية
CC = gcc
CXX = g++
CFLAGS = -std=c11 -Wall -Wextra -g -O2 -DUNICODE -D_UNICODE
CXXFLAGS = -std=c++11 -Wall -Wextra -g -O2 -DUNICODE -D_UNICODE
LDFLAGS = -lm

# مسارات الملفات
COMPILER_CORE = ../CompilerCore
TEST_DIR = .
UNIT_TESTS_DIR = $(TEST_DIR)/UnitTests
INTEGRATION_TESTS_DIR = $(TEST_DIR)/IntegrationTests
SYSTEM_TESTS_DIR = $(TEST_DIR)/SystemTests
PERFORMANCE_TESTS_DIR = $(TEST_DIR)/PerformanceTests

# ملفات الاختبارات
LEXER_TEST = $(UNIT_TESTS_DIR)/test_lexer
PARSER_TEST = $(UNIT_TESTS_DIR)/test_parser
SEMANTIC_TEST = $(UNIT_TESTS_DIR)/test_semantic
CODEGEN_TEST = $(UNIT_TESTS_DIR)/test_codegen
AST_TEST = $(UNIT_TESTS_DIR)/test_ast
SYMBOL_TABLE_TEST = $(UNIT_TESTS_DIR)/test_symboltable
PARSE_TREE_TEST = $(UNIT_TESTS_DIR)/test_parsetree

INTEGRATION_TEST = $(INTEGRATION_TESTS_DIR)/test_integration
SYSTEM_TEST = $(SYSTEM_TESTS_DIR)/test_system
PERFORMANCE_TEST = $(PERFORMANCE_TESTS_DIR)/test_performance

# جميع الاختبارات
ALL_TESTS = $(LEXER_TEST) $(PARSER_TEST) $(SEMANTIC_TEST) $(CODEGEN_TEST) \
            $(AST_TEST) $(SYMBOL_TABLE_TEST) $(PARSE_TREE_TEST) \
            $(INTEGRATION_TEST) $(SYSTEM_TEST) $(PERFORMANCE_TEST)

# المكتبات المطلوبة
LEXER_OBJ = $(COMPILER_CORE)/lexer.o
PARSER_OBJ = $(COMPILER_CORE)/parser.o
SEMANTIC_OBJ = $(COMPILER_CORE)/semantic.o
CODEGEN_OBJ = $(COMPILER_CORE)/codegen.o
AST_OBJ = $(COMPILER_CORE)/ast.o
SYMBOL_TABLE_OBJ = $(COMPILER_CORE)/symboltable.o
PARSE_TREE_OBJ = $(COMPILER_CORE)/parsetree.o

# الهدف الافتراضي
all: $(ALL_TESTS)

# اختبارات الوحدة
unit-tests: $(LEXER_TEST) $(PARSER_TEST) $(SEMANTIC_TEST) $(CODEGEN_TEST) \
            $(AST_TEST) $(SYMBOL_TABLE_TEST) $(PARSE_TREE_TEST)

# اختبارات التكامل
integration-tests: $(INTEGRATION_TEST)

# اختبارات النظام
system-tests: $(SYSTEM_TEST)

# اختبارات الأداء
performance-tests: $(PERFORMANCE_TEST)

# بناء اختبار المحلل المعجمي
$(LEXER_TEST): $(UNIT_TESTS_DIR)/test_lexer.c $(LEXER_OBJ)
	@echo "بناء اختبار المحلل المعجمي..."
	$(CC) $(CFLAGS) -o $@ $< $(LEXER_OBJ) $(LDFLAGS)

# بناء اختبار المحلل النحوي
$(PARSER_TEST): $(UNIT_TESTS_DIR)/test_parser.c $(PARSER_OBJ) $(LEXER_OBJ)
	@echo "بناء اختبار المحلل النحوي..."
	$(CC) $(CFLAGS) -o $@ $< $(PARSER_OBJ) $(LEXER_OBJ) $(LDFLAGS)

# بناء اختبار المحلل الدلالي
$(SEMANTIC_TEST): $(UNIT_TESTS_DIR)/test_semantic.c $(SEMANTIC_OBJ) $(PARSER_OBJ) $(LEXER_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ)
	@echo "بناء اختبار المحلل الدلالي..."
	$(CC) $(CFLAGS) -o $@ $< $(SEMANTIC_OBJ) $(PARSER_OBJ) $(LEXER_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(LDFLAGS)

# بناء اختبار مولد الكود
$(CODEGEN_TEST): $(UNIT_TESTS_DIR)/test_codegen.c $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ)
	@echo "بناء اختبار مولد الكود..."
	$(CC) $(CFLAGS) -o $@ $< $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(LDFLAGS)

# بناء اختبار شجرة البنية المجردة
$(AST_TEST): $(UNIT_TESTS_DIR)/test_ast.c $(AST_OBJ) $(PARSE_TREE_OBJ)
	@echo "بناء اختبار شجرة البنية المجردة..."
	$(CC) $(CFLAGS) -o $@ $< $(AST_OBJ) $(PARSE_TREE_OBJ) $(LDFLAGS)

# بناء اختبار جدول الرموز
$(SYMBOL_TABLE_TEST): $(UNIT_TESTS_DIR)/test_symboltable.c $(SYMBOL_TABLE_OBJ)
	@echo "بناء اختبار جدول الرموز..."
	$(CC) $(CFLAGS) -o $@ $< $(SYMBOL_TABLE_OBJ) $(LDFLAGS)

# بناء اختبار شجرة الإعراب
$(PARSE_TREE_TEST): $(UNIT_TESTS_DIR)/test_parsetree.c $(PARSE_TREE_OBJ)
	@echo "بناء اختبار شجرة الإعراب..."
	$(CC) $(CFLAGS) -o $@ $< $(PARSE_TREE_OBJ) $(LDFLAGS)

# بناء اختبار التكامل
$(INTEGRATION_TEST): $(INTEGRATION_TESTS_DIR)/test_integration.c $(LEXER_OBJ) $(PARSER_OBJ) $(SEMANTIC_OBJ) $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(PARSE_TREE_OBJ)
	@echo "بناء اختبار التكامل..."
	$(CC) $(CFLAGS) -o $@ $< $(LEXER_OBJ) $(PARSER_OBJ) $(SEMANTIC_OBJ) $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(PARSE_TREE_OBJ) $(LDFLAGS)

# بناء اختبار النظام
$(SYSTEM_TEST): $(SYSTEM_TESTS_DIR)/test_system.c $(LEXER_OBJ) $(PARSER_OBJ) $(SEMANTIC_OBJ) $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(PARSE_TREE_OBJ)
	@echo "بناء اختبار النظام..."
	$(CC) $(CFLAGS) -o $@ $< $(LEXER_OBJ) $(PARSER_OBJ) $(SEMANTIC_OBJ) $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(PARSE_TREE_OBJ) $(LDFLAGS)

# بناء اختبار الأداء
$(PERFORMANCE_TEST): $(PERFORMANCE_TESTS_DIR)/test_performance.c $(LEXER_OBJ) $(PARSER_OBJ) $(SEMANTIC_OBJ) $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(PARSE_TREE_OBJ)
	@echo "بناء اختبار الأداء..."
	$(CC) $(CFLAGS) -o $@ $< $(LEXER_OBJ) $(PARSER_OBJ) $(SEMANTIC_OBJ) $(CODEGEN_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(PARSE_TREE_OBJ) $(LDFLAGS)

# تشغيل جميع الاختبارات
test: $(ALL_TESTS)
	@echo "=== تشغيل جميع الاختبارات ==="
	@echo ""
	
	@echo "--- اختبارات الوحدة ---"
	@echo "اختبار المحلل المعجمي:"
	@$(LEXER_TEST) || echo "فشل اختبار المحلل المعجمي"
	@echo ""
	
	@echo "اختبار المحلل النحوي:"
	@$(PARSER_TEST) || echo "فشل اختبار المحلل النحوي"
	@echo ""
	
	@echo "اختبار المحلل الدلالي:"
	@$(SEMANTIC_TEST) || echo "فشل اختبار المحلل الدلالي"
	@echo ""
	
	@echo "اختبار مولد الكود:"
	@$(CODEGEN_TEST) || echo "فشل اختبار مولد الكود"
	@echo ""
	
	@echo "اختبار شجرة البنية المجردة:"
	@$(AST_TEST) || echo "فشل اختبار شجرة البنية المجردة"
	@echo ""
	
	@echo "اختبار جدول الرموز:"
	@$(SYMBOL_TABLE_TEST) || echo "فشل اختبار جدول الرموز"
	@echo ""
	
	@echo "اختبار شجرة الإعراب:"
	@$(PARSE_TREE_TEST) || echo "فشل اختبار شجرة الإعراب"
	@echo ""
	
	@echo "--- اختبارات التكامل ---"
	@echo "اختبار التكامل:"
	@$(INTEGRATION_TEST) || echo "فشل اختبار التكامل"
	@echo ""
	
	@echo "--- اختبارات النظام ---"
	@echo "اختبار النظام:"
	@$(SYSTEM_TEST) || echo "فشل اختبار النظام"
	@echo ""
	
	@echo "--- اختبارات الأداء ---"
	@echo "اختبار الأداء:"
	@$(PERFORMANCE_TEST) || echo "فشل اختبار الأداء"
	@echo ""
	
	@echo "=== انتهت جميع الاختبارات ==="

# تشغيل اختبارات الوحدة فقط
test-unit: unit-tests
	@echo "=== تشغيل اختبارات الوحدة ==="
	@$(LEXER_TEST) || echo "فشل اختبار المحلل المعجمي"
	@$(PARSER_TEST) || echo "فشل اختبار المحلل النحوي"
	@$(SEMANTIC_TEST) || echo "فشل اختبار المحلل الدلالي"
	@$(CODEGEN_TEST) || echo "فشل اختبار مولد الكود"
	@$(AST_TEST) || echo "فشل اختبار شجرة البنية المجردة"
	@$(SYMBOL_TABLE_TEST) || echo "فشل اختبار جدول الرموز"
	@$(PARSE_TREE_TEST) || echo "فشل اختبار شجرة الإعراب"

# تشغيل اختبارات التكامل فقط
test-integration: integration-tests
	@echo "=== تشغيل اختبارات التكامل ==="
	@$(INTEGRATION_TEST) || echo "فشل اختبار التكامل"

# تشغيل اختبارات النظام فقط
test-system: system-tests
	@echo "=== تشغيل اختبارات النظام ==="
	@$(SYSTEM_TEST) || echo "فشل اختبار النظام"

# تشغيل اختبارات الأداء فقط
test-performance: performance-tests
	@echo "=== تشغيل اختبارات الأداء ==="
	@$(PERFORMANCE_TEST) || echo "فشل اختبار الأداء"

# اختبار سريع (اختبارات الوحدة الأساسية فقط)
test-quick: $(LEXER_TEST) $(PARSER_TEST)
	@echo "=== اختبار سريع ==="
	@$(LEXER_TEST) || echo "فشل اختبار المحلل المعجمي"
	@$(PARSER_TEST) || echo "فشل اختبار المحلل النحوي"

# اختبار التغطية
coverage: CFLAGS += -fprofile-arcs -ftest-coverage
coverage: CXXFLAGS += -fprofile-arcs -ftest-coverage
coverage: LDFLAGS += -lgcov
coverage: clean test
	@echo "=== توليد تقرير التغطية ==="
	@mkdir -p coverage
	@lcov --capture --directory . --output-file coverage/coverage.info
	@genhtml coverage/coverage.info --output-directory coverage/html
	@echo "تقرير التغطية متاح في coverage/html/index.html"

# اختبار الذاكرة (مع Valgrind)
test-memory: $(ALL_TESTS)
	@echo "=== اختبار الذاكرة مع Valgrind ==="
	@echo "اختبار المحلل المعجمي:"
	@valgrind --tool=memcheck --leak-check=full $(LEXER_TEST) || echo "فشل اختبار الذاكرة للمحلل المعجمي"
	@echo ""
	@echo "اختبار المحلل النحوي:"
	@valgrind --tool=memcheck --leak-check=full $(PARSER_TEST) || echo "فشل اختبار الذاكرة للمحلل النحوي"

# اختبار الأداء مع تقرير مفصل
test-performance-detailed: performance-tests
	@echo "=== اختبار الأداء مفصل ==="
	@echo "تشغيل اختبار الأداء مع تقرير مفصل..."
	@$(PERFORMANCE_TEST) > performance_report.txt 2>&1
	@echo "تم حفظ تقرير الأداء في performance_report.txt"

# اختبار الضغط
test-stress: $(ALL_TESTS)
	@echo "=== اختبار الضغط ==="
	@echo "تشغيل الاختبارات 5 مرات متتالية..."
	@for i in 1 2 3 4 5; do \
		echo "--- الجولة $$i ---"; \
		$(LEXER_TEST) && $(PARSER_TEST) || echo "فشل في الجولة $$i"; \
	done

# تنظيف الملفات
clean:
	@echo "تنظيف ملفات الاختبارات..."
	@rm -f $(ALL_TESTS)
	@rm -f *.o *.gcno *.gcda
	@rm -f performance_report.txt
	@rm -f test_*.arabic
	@rm -f test_*.dot
	@rm -f test_*.s

# تنظيف شامل
distclean: clean
	@echo "تنظيف شامل..."
	@rm -rf coverage/
	@rm -f *.log

# إنشاء مجلدات الاختبار
setup:
	@echo "إنشاء مجلدات الاختبار..."
	@mkdir -p $(UNIT_TESTS_DIR)
	@mkdir -p $(INTEGRATION_TESTS_DIR)
	@mkdir -p $(SYSTEM_TESTS_DIR)
	@mkdir -p $(PERFORMANCE_TESTS_DIR)
	@mkdir -p $(TEST_DIR)/TestCases
	@echo "تم إنشاء مجلدات الاختبار"

# مساعدة
help:
	@echo "Makefile لاختبارات مترجم اللغة البرمجة العربية"
	@echo ""
	@echo "الأهداف المتاحة:"
	@echo "  all              - بناء جميع الاختبارات"
	@echo "  unit-tests       - بناء اختبارات الوحدة"
	@echo "  integration-tests - بناء اختبارات التكامل"
	@echo "  system-tests     - بناء اختبارات النظام"
	@echo "  performance-tests - بناء اختبارات الأداء"
	@echo ""
	@echo "تشغيل الاختبارات:"
	@echo "  test             - تشغيل جميع الاختبارات"
	@echo "  test-unit        - تشغيل اختبارات الوحدة فقط"
	@echo "  test-integration - تشغيل اختبارات التكامل فقط"
	@echo "  test-system      - تشغيل اختبارات النظام فقط"
	@echo "  test-performance - تشغيل اختبارات الأداء فقط"
	@echo "  test-quick       - اختبار سريع (أساسي)"
	@echo ""
	@echo "اختبارات متقدمة:"
	@echo "  coverage         - اختبار التغطية"
	@echo "  test-memory      - اختبار الذاكرة مع Valgrind"
	@echo "  test-performance-detailed - اختبار أداء مفصل"
	@echo "  test-stress      - اختبار الضغط"
	@echo ""
	@echo "صيانة:"
	@echo "  clean            - تنظيف ملفات الاختبارات"
	@echo "  distclean        - تنظيف شامل"
	@echo "  setup            - إنشاء مجلدات الاختبار"
	@echo "  help             - عرض هذه المساعدة"

# قواعد خاصة بـ Visual Studio
.PHONY: all test test-unit test-integration test-system test-performance test-quick coverage test-memory test-performance-detailed test-stress clean distclean setup help

# دعم Windows
ifeq ($(OS),Windows_NT)
    RM = del /Q
    MKDIR = mkdir
    RMDIR = rmdir /S /Q
else
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
endif

# تحديث المكتبات من CompilerCore
update-libs:
	@echo "تحديث المكتبات من CompilerCore..."
	@cd $(COMPILER_CORE) && $(MAKE) clean && $(MAKE) all
	@echo "تم تحديث المكتبات"

# اختبار التجميع
build-test: $(ALL_TESTS)
	@echo "=== اختبار التجميع ==="
	@echo "جميع الاختبارات تم تجميعها بنجاح"
	@echo "عدد الاختبارات: $(words $(ALL_TESTS))"

# تشغيل اختبار محدد
run-lexer: $(LEXER_TEST)
	@echo "تشغيل اختبار المحلل المعجمي..."
	@$(LEXER_TEST)

run-parser: $(PARSER_TEST)
	@echo "تشغيل اختبار المحلل النحوي..."
	@$(PARSER_TEST)

run-semantic: $(SEMANTIC_TEST)
	@echo "تشغيل اختبار المحلل الدلالي..."
	@$(SEMANTIC_TEST)

run-codegen: $(CODEGEN_TEST)
	@echo "تشغيل اختبار مولد الكود..."
	@$(CODEGEN_TEST)

# تقرير حالة الاختبارات
status:
	@echo "=== حالة الاختبارات ==="
	@echo "اختبارات الوحدة:"
	@for test in $(LEXER_TEST) $(PARSER_TEST) $(SEMANTIC_TEST) $(CODEGEN_TEST) $(AST_TEST) $(SYMBOL_TABLE_TEST) $(PARSE_TREE_TEST); do \
		if [ -f $$test ]; then \
			echo "  ✓ $$(basename $$test) - متاح"; \
		else \
			echo "  ✗ $$(basename $$test) - غير متاح"; \
		fi; \
	done
	@echo ""
	@echo "اختبارات التكامل:"
	@for test in $(INTEGRATION_TEST) $(SYSTEM_TEST) $(PERFORMANCE_TEST); do \
		if [ -f $$test ]; then \
			echo "  ✓ $$(basename $$test) - متاح"; \
		else \
			echo "  ✗ $$(basename $$test) - غير متاح"; \
		fi; \
	done

