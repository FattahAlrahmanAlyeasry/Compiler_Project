# Makefile لمشروع مترجم اللغة البرمجة العربية
# Compiler Project Makefile

# المتغيرات
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC -D_UNICODE -DUNICODE
DEBUG_CFLAGS = -g -O0 -DDEBUG
RELEASE_CFLAGS = -O2 -DNDEBUG
LDFLAGS = -lm
TARGET = arabic_compiler
LIBRARY = libarabic_compiler.dll
SOURCES = main.c lexer.c parser.c ast.c symboltable.c codegen.c semantic.c parsetree.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = lexer.h parser.h ast.h symboltable.h codegen.h semantic.h parsetree.h

# الهدف الافتراضي
all: $(TARGET)

# بناء المترجم التنفيذي
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# بناء المكتبة الديناميكية
library: $(OBJECTS)
	$(CC) -shared -o $(LIBRARY) $(OBJECTS) $(LDFLAGS)

# بناء نسخة التطوير
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(TARGET)

# بناء نسخة الإنتاج
release: CFLAGS += $(RELEASE_CFLAGS)
release: $(TARGET)

# قواعد بناء الملفات الكائنية
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# تنظيف الملفات
clean:
	rm -f $(OBJECTS) $(TARGET) $(LIBRARY) *.exe

# تنظيف شامل
distclean: clean
	rm -f *.s *.o *.dll *.so *.dylib

# اختبار المترجم
test: $(TARGET)
	@echo "اختبار المترجم..."
	@echo "برنامج" > test_input.txt
	@echo "[" >> test_input.txt
	@echo "]" >> test_input.txt
	@echo ":" >> test_input.txt
	@echo "اطبع \"مرحباً بالعالم\"" >> test_input.txt
	@echo "؛" >> test_input.txt
	./$(TARGET) test_input.txt
	@rm -f test_input.txt

# تثبيت المكتبة
install: library
	@echo "تثبيت المكتبة..."
	@cp $(LIBRARY) /usr/local/lib/
	@cp $(HEADERS) /usr/local/include/
	@echo "تم التثبيت بنجاح"

# إلغاء التثبيت
uninstall:
	@echo "إلغاء تثبيت المكتبة..."
	@rm -f /usr/local/lib/$(LIBRARY)
	@rm -f /usr/local/include/$(HEADERS)
	@echo "تم إلغاء التثبيت بنجاح"

# مساعدة
help:
	@echo "أهداف البناء المتاحة:"
	@echo "  all        - بناء المترجم التنفيذي (افتراضي)"
	@echo "  library    - بناء المكتبة الديناميكية"
	@echo "  debug      - بناء نسخة التطوير مع معلومات التصحيح"
	@echo "  release    - بناء نسخة الإنتاج محسنة"
	@echo "  test       - اختبار المترجم"
	@echo "  clean      - تنظيف الملفات المؤقتة"
	@echo "  distclean  - تنظيف شامل"
	@echo "  install    - تثبيت المكتبة"
	@echo "  uninstall  - إلغاء تثبيت المكتبة"
	@echo "  help       - عرض هذه المساعدة"

# قواعد خاصة
.PHONY: all debug release library test clean distclean install uninstall help

# تبعيات خاصة
main.o: lexer.h parser.h ast.h symboltable.h codegen.h
lexer.o: lexer.h
parser.o: parser.h lexer.h
ast.o: ast.h lexer.h parser.h
symboltable.o: symboltable.h ast.h
codegen.o: codegen.h ast.h symboltable.h
semantic.o: semantic.h ast.h symboltable.h
parsetree.o: parsetree.h parser.h

# قواعد للأنظمة المختلفة
ifeq ($(OS),Windows_NT)
    # Windows
    TARGET = arabic_compiler.exe
    LIBRARY = arabic_compiler.dll
    LDFLAGS += -lkernel32 -luser32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux
        LIBRARY = libarabic_compiler.so
        LDFLAGS += -ldl
    endif
    ifeq ($(UNAME_S),Darwin)
        # macOS
        LIBRARY = libarabic_compiler.dylib
        LDFLAGS += -framework CoreFoundation
    endif
endif

