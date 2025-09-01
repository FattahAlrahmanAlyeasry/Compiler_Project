# دليل المطور لمترجم اللغة البرمجة العربية
## Developer Guide for the Arabic Programming Language Compiler

### نظرة عامة (Overview)
هذا الدليل موجه للمطورين الذين يرغبون في المساهمة في تطوير مترجم اللغة البرمجة العربية أو إضافة ميزات جديدة إليه. الدليل يغطي البنية المعمارية، كيفية البناء من المصدر، وإجراءات التطوير.

This guide is intended for developers who want to contribute to the development of the Arabic Programming Language Compiler or add new features to it. The guide covers the architecture, how to build from source, and development procedures.

### متطلبات التطوير (Development Requirements)

#### متطلبات الأجهزة (Hardware Requirements):
- **المعالج**: Intel Core i3 أو أحدث
- **الذاكرة**: 4 GB RAM على الأقل
- **مساحة القرص**: 2 GB متاحة
- **الشاشة**: دقة 1920×1080 أو أعلى

#### متطلبات البرامج (Software Requirements):
- **نظام التشغيل**: Windows 10/11
- **Visual Studio**: Visual Studio 2015 أو أحدث
- **.NET Framework**: .NET Framework 4.5.2 أو أحدث
- **Git**: لإدارة الإصدارات
- **CMake**: لبناء المشروع (اختياري)

### هيكل المشروع (Project Structure)

#### 1. المجلدات الرئيسية (Main Directories)

```
Compiler_Project/
├── CompilerCore/           # نواة المترجم بلغة C
│   ├── include/           # ملفات الرؤوس
│   ├── src/               # ملفات المصدر
│   ├── lib/               # المكتبات
│   ├── obj/               # ملفات الكائنات
│   └── Makefile           # ملف البناء
├── GUI/                   # واجهة المستخدم بلغة C#
│   ├── Forms/             # نماذج Windows Forms
│   ├── Controls/          # عناصر التحكم المخصصة
│   ├── Utils/             # أدوات مساعدة
│   └── ArabicCompiler.csproj
├── Documentation/          # التوثيق
│   ├── README.md
│   ├── LanguageSpec.md
│   ├── Architecture.md
│   ├── UserGuide.md
│   └── DeveloperGuide.md
├── Tests/                  # الاختبارات
│   ├── UnitTests/
│   ├── IntegrationTests/
│   ├── SystemTests/
│   └── TestCases/
├── Output/                 # المخرجات
│   ├── MIPS/
│   ├── Logs/
│   └── Reports/
└── Build/                  # ملفات البناء
    ├── Debug/
    ├── Release/
    └── Installer/
```

#### 2. ملفات المشروع (Project Files)

##### ملفات C++:
- **`CompilerCore/CompilerCore.vcxproj`**: مشروع Visual Studio C++
- **`CompilerCore/CompilerCore.vcxproj.filters`**: تصفية الملفات
- **`CompilerCore/CompilerCore.vcxproj.user`**: إعدادات المستخدم

##### ملفات C#:
- **`GUI/ArabicCompiler.csproj`**: مشروع Visual Studio C#
- **`GUI/App.config`**: إعدادات التطبيق
- **`GUI/Properties/AssemblyInfo.cs`**: معلومات التجميع

##### ملفات البناء:
- **`Makefile`**: بناء المشروع C
- **`CMakeLists.txt`**: بناء CMake (اختياري)
- **`build.bat`**: سكريبت البناء لـ Windows

### بناء المشروع من المصدر (Building from Source)

#### 1. إعداد بيئة التطوير (Setting Up Development Environment)

##### تثبيت Visual Studio 2015:
```
1. قم بتحميل Visual Studio 2015 Community (مجاني)
2. اختر "Visual C++" و ".NET Framework 4.5.2"
3. تأكد من تثبيت Windows SDK
4. أعد تشغيل الكمبيوتر
```

##### تثبيت أدوات إضافية:
```
1. Git for Windows
2. CMake (اختياري)
3. Python 3.x (للمساعدة في البناء)
```

#### 2. استنساخ المشروع (Cloning the Project)

```bash
# استنساخ المستودع
git clone https://github.com/username/arabic-compiler.git
cd arabic-compiler

# إنشاء فرع للتطوير
git checkout -b feature/new-feature
```

#### 3. بناء المشروع C++ (Building C++ Project)

##### باستخدام Visual Studio:
```
1. افتح CompilerCore/CompilerCore.vcxproj
2. اختر الإعداد (Debug/Release)
3. اضغط F7 أو Build → Build Solution
4. تأكد من عدم وجود أخطاء
```

##### باستخدام Makefile:
```bash
cd CompilerCore
make clean
make all
make library
```

##### باستخدام CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### 4. بناء المشروع C# (Building C# Project)

##### باستخدام Visual Studio:
```
1. افتح GUI/ArabicCompiler.csproj
2. اختر الإعداد (Debug/Release)
3. اضغط F7 أو Build → Build Solution
4. تأكد من عدم وجود أخطاء
```

##### باستخدام Command Line:
```bash
cd GUI
dotnet restore
dotnet build --configuration Release
```

#### 5. بناء المشروع بالكامل (Building Complete Project)

##### سكريبت البناء:
```bash
# Windows
build.bat

# أو يدوياً
cd CompilerCore && make all
cd ../GUI && dotnet build
```

### هيكل الكود (Code Structure)

#### 1. نواة المترجم (Compiler Core)

##### المحلل المعجمي (Lexer):
```c
// lexer.h
typedef struct {
    FILE* source;
    wchar_t* current_line;
    int line_number;
    int column_number;
    int current_pos;
    int line_length;
} Lexer;

// lexer.c
Lexer* initLexer(const char* filename);
Token* getNextToken(Lexer* lexer);
void freeLexer(Lexer* lexer);
```

##### المحلل النحوي (Parser):
```c
// parser.h
typedef struct {
    Lexer* lexer;
    Token* current_token;
    ParseTreeNode* parse_tree;
    int error_count;
} Parser;

// parser.c
Parser* initParser(Lexer* lexer);
ParseTreeNode* parse(Parser* parser);
void freeParser(Parser* parser);
```

##### المحلل الدلالي (Semantic Analyzer):
```c
// semantic.h
typedef struct {
    SymbolTable* symbol_table;
    ASTNode* ast;
    Scope* current_scope;
    int error_count;
} SemanticAnalyzer;

// semantic.c
SemanticAnalyzer* initSemanticAnalyzer();
ASTNode* analyzeProgram(SemanticAnalyzer* analyzer, ParseTreeNode* parse_tree);
void freeSemanticAnalyzer(SemanticAnalyzer* analyzer);
```

#### 2. واجهة المستخدم (User Interface)

##### النافذة الرئيسية:
```csharp
// MainWindow.cs
public partial class MainWindow : Form
{
    private CodeEditor codeEditor;
    private ParseTreeViewer parseTreeViewer;
    private MipsCodeViewer mipsCodeViewer;
    private CompilerInterface compilerInterface;
    
    private void InitializeComponent()
    {
        // تهيئة المكونات
    }
}
```

##### واجهة المترجم:
```csharp
// CompilerInterface.cs
public class CompilerInterface : IDisposable
{
    [DllImport("arabic_compiler.dll", CharSet = CharSet.Unicode)]
    private static extern IntPtr EXPORT_CompileFile(string filename);
    
    public bool CompileFile(string filename)
    {
        // استدعاء المترجم
    }
}
```

### إضافة ميزات جديدة (Adding New Features)

#### 1. إضافة كلمة محجوزة جديدة (Adding New Reserved Word)

##### الخطوة 1: تحديث lexer.h
```c
// إضافة نوع الرمز الجديد
typedef enum {
    // ... الأنواع الموجودة
    TOKEN_NEW_KEYWORD,  // الكلمة الجديدة
    // ...
} TokenType;
```

##### الخطوة 2: تحديث lexer.c
```c
// إضافة الكلمة إلى المصفوفة
static const wchar_t* reserved_words[] = {
    // ... الكلمات الموجودة
    L"كلمة_جديدة",  // الكلمة الجديدة
    NULL
};

// تحديث دالة التعرف
static bool isReservedWord(const wchar_t* word) {
    // ... الكود الموجود
}
```

##### الخطوة 3: تحديث parser.h
```c
// إضافة نوع العقدة الجديد
typedef enum {
    // ... الأنواع الموجودة
    NODE_NEW_STATEMENT,  // العقدة الجديدة
    // ...
} ParseTreeNodeType;
```

##### الخطوة 4: تحديث parser.c
```c
// إضافة دالة التحليل الجديدة
static ParseTreeNode* parseNewStatement(Parser* parser) {
    // ... كود التحليل
}
```

#### 2. إضافة نوع بيانات جديد (Adding New Data Type)

##### الخطوة 1: تحديث ast.h
```c
// إضافة النوع الجديد
typedef enum {
    // ... الأنواع الموجودة
    TYPE_NEW_TYPE,  // النوع الجديد
    // ...
} DataType;
```

##### الخطوة 2: تحديث symboltable.h
```c
// إضافة معلومات النوع الجديد
typedef struct {
    // ... الحقول الموجودة
    union {
        // ... الأنواع الموجودة
        NewTypeInfo new_type;  // معلومات النوع الجديد
    } data;
} SymbolEntry;
```

##### الخطوة 3: تحديث codegen.h
```c
// إضافة توليد الكود للنوع الجديد
void generateNewTypeCode(CodeGenerator* generator, ASTNode* node);
```

#### 3. إضافة تعليمات MIPS جديدة (Adding New MIPS Instructions)

##### الخطوة 1: تحديث codegen.h
```c
// إضافة نوع التعليمات الجديد
typedef enum {
    // ... الأنواع الموجودة
    INSTR_NEW_INSTRUCTION,  // التعليمات الجديدة
    // ...
} MIPSInstructionType;
```

##### الخطوة 2: تحديث codegen.c
```c
// إضافة دالة إنشاء التعليمات الجديدة
MIPSInstruction* createNewInstruction(MIPSRegister rd, MIPSRegister rs, MIPSRegister rt) {
    MIPSInstruction* instr = malloc(sizeof(MIPSInstruction));
    instr->type = INSTR_NEW_INSTRUCTION;
    instr->rd = rd;
    instr->rs = rs;
    instr->rt = rt;
    return instr;
}
```

### إصلاح الأخطاء (Bug Fixing)

#### 1. عملية إصلاح الأخطاء (Bug Fixing Process)

##### الخطوة 1: تحديد المشكلة
```
1. قراءة تقرير الخطأ بعناية
2. تكرار الخطأ في بيئة التطوير
3. تحديد المكون المسؤول
4. تحديد نوع الخطأ (معجمي، نحوي، دلالي)
```

##### الخطوة 2: تحليل السبب
```
1. فحص الكود المصدر
2. استخدام أدوات التصحيح
3. تتبع تدفق البيانات
4. تحديد نقطة الفشل
```

##### الخطوة 3: إصلاح المشكلة
```
1. كتابة الحل
2. اختبار الحل
3. التأكد من عدم إدخال أخطاء جديدة
4. تحديث الاختبارات
```

#### 2. أدوات التصحيح (Debugging Tools)

##### Visual Studio Debugger:
```
1. تعيين نقاط التوقف (Breakpoints)
2. فحص المتغيرات (Watch Window)
3. تتبع المكدس (Call Stack)
4. فحص الذاكرة (Memory Window)
```

##### أدوات إضافية:
```
1. Valgrind (للكشف عن تسريب الذاكرة)
2. GDB (مصحح GNU)
3. AddressSanitizer (للكشف عن أخطاء الذاكرة)
```

#### 3. أنواع الأخطاء الشائعة (Common Bug Types)

##### أخطاء الذاكرة:
```c
// خطأ: تسريب الذاكرة
char* str = malloc(100);
// ... استخدام str
// نسيان free(str)

// إصلاح: إدارة صحيحة للذاكرة
char* str = malloc(100);
// ... استخدام str
free(str);
str = NULL;
```

##### أخطاء المؤشرات:
```c
// خطأ: مؤشر فارغ
char* str = NULL;
strcpy(str, "hello");  // خطأ!

// إصلاح: فحص المؤشر
if (str != NULL) {
    strcpy(str, "hello");
}
```

##### أخطاء Unicode:
```c
// خطأ: عدم دعم Unicode
char str[100];
wscanf(L"%s", str);  // خطأ!

// إصلاح: دعم Unicode
wchar_t str[100];
wscanf(L"%s", str);
```

### إضافة اختبارات جديدة (Adding New Tests)

#### 1. هيكل الاختبارات (Test Structure)

##### اختبارات الوحدة (Unit Tests):
```
Tests/UnitTests/
├── LexerTests/
│   ├── test_lexer_basic.c
│   ├── test_lexer_keywords.c
│   └── test_lexer_errors.c
├── ParserTests/
│   ├── test_parser_program.c
│   ├── test_parser_statements.c
│   └── test_parser_expressions.c
└── SemanticTests/
    ├── test_symbol_table.c
    ├── test_type_checking.c
    └── test_scope_management.c
```

##### اختبارات التكامل (Integration Tests):
```
Tests/IntegrationTests/
├── test_lexer_parser_integration.c
├── test_parser_semantic_integration.c
└── test_semantic_codegen_integration.c
```

##### اختبارات النظام (System Tests):
```
Tests/SystemTests/
├── test_simple_programs.c
├── test_complex_programs.c
└── test_error_handling.c
```

#### 2. كتابة اختبارات الوحدة (Writing Unit Tests)

##### مثال على اختبار المحلل المعجمي:
```c
// test_lexer_basic.c
#include <unity.h>
#include "lexer.h"

void setUp(void) {
    // إعداد الاختبار
}

void tearDown(void) {
    // تنظيف الاختبار
}

void test_lexer_init(void) {
    Lexer* lexer = initLexer("test.arab");
    TEST_ASSERT_NOT_NULL(lexer);
    TEST_ASSERT_EQUAL(1, lexer->line_number);
    TEST_ASSERT_EQUAL(0, lexer->column_number);
    freeLexer(lexer);
}

void test_lexer_keywords(void) {
    Lexer* lexer = initLexer("test.arab");
    Token* token = getNextToken(lexer);
    TEST_ASSERT_EQUAL(TOKEN_PROGRAM, token->type);
    TEST_ASSERT_EQUAL_STRING(L"برنامج", token->value);
    freeToken(token);
    freeLexer(lexer);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_lexer_init);
    RUN_TEST(test_lexer_keywords);
    return UNITY_END();
}
```

##### مثال على اختبار المحلل النحوي:
```c
// test_parser_program.c
#include <unity.h>
#include "parser.h"

void test_parse_simple_program(void) {
    // إنشاء كود مصدري بسيط
    const char* source = "برنامج اختبار؛\n[]:\nاطبع(\"مرحباً\")؛\n]\nنهاية؛";
    
    // اختبار التحليل
    Lexer* lexer = initLexer_from_string(source);
    Parser* parser = initParser(lexer);
    ParseTreeNode* tree = parse(parser);
    
    TEST_ASSERT_NOT_NULL(tree);
    TEST_ASSERT_EQUAL(NODE_PROGRAM, tree->type);
    TEST_ASSERT_EQUAL(0, parser->error_count);
    
    freeParseTree(tree);
    freeParser(parser);
    freeLexer(lexer);
}
```

#### 3. تشغيل الاختبارات (Running Tests)

##### تشغيل جميع الاختبارات:
```bash
# من مجلد المشروع
make test

# أو يدوياً
cd Tests/UnitTests
make all
./run_tests
```

##### تشغيل اختبارات محددة:
```bash
# اختبارات المحلل المعجمي فقط
cd Tests/UnitTests/LexerTests
make test_lexer_basic
./test_lexer_basic

# اختبارات المحلل النحوي فقط
cd Tests/UnitTests/ParserTests
make test_parser_program
./test_parser_program
```

### إدارة الإصدارات (Version Control)

#### 1. إرشادات Git (Git Guidelines)

##### تسمية الفروع:
```
feature/اسم-الميزة-الجديدة
bugfix/وصف-الخطأ
hotfix/إصلاح-عاجل
release/رقم-الإصدار
```

##### رسائل الالتزام:
```
feat: إضافة دعم للأنواع المركبة
fix: إصلاح تسريب الذاكرة في المحلل المعجمي
docs: تحديث دليل المستخدم
test: إضافة اختبارات للعوامل المنطقية
refactor: إعادة هيكلة جدول الرموز
```

#### 2. عملية التطوير (Development Workflow)

##### الخطوة 1: إنشاء فرع جديد
```bash
git checkout main
git pull origin main
git checkout -b feature/new-feature
```

##### الخطوة 2: التطوير والاختبار
```bash
# تطوير الميزة
# كتابة الاختبارات
# اختبار الميزة
```

##### الخطوة 3: الالتزام والتحديث
```bash
git add .
git commit -m "feat: إضافة ميزة جديدة"
git push origin feature/new-feature
```

##### الخطوة 4: إنشاء طلب السحب
```
1. انتقل إلى GitHub/GitLab
2. أنشئ طلب سحب جديد
3. اكتب وصفاً مفصلاً للميزة
4. اطلب مراجعة الكود
```

### التوثيق (Documentation)

#### 1. توثيق الكود (Code Documentation)

##### تعليقات C:
```c
/**
 * @brief تهيئة المحلل المعجمي
 * @param filename اسم الملف المصدري
 * @return مؤشر إلى المحلل المعجمي أو NULL في حالة الفشل
 * 
 * هذه الدالة تقوم بتهيئة المحلل المعجمي وفتح الملف المصدري.
 * تدعم الملفات بترميز UTF-8 للغة العربية.
 */
Lexer* initLexer(const char* filename);
```

##### تعليقات C#:
```csharp
/// <summary>
/// تهيئة واجهة المترجم
/// </summary>
/// <param name="dllPath">مسار ملف DLL المترجم</param>
/// <returns>true إذا نجحت التهيئة، false خلاف ذلك</returns>
/// <remarks>
/// هذه الدالة تقوم بتهيئة واجهة المترجم وربطها بملف DLL.
/// يجب استدعاؤها قبل استخدام أي دالة أخرى.
/// </remarks>
public bool Initialize(string dllPath)
{
    // ... كود التهيئة
}
```

#### 2. تحديث التوثيق (Updating Documentation)

##### عند إضافة ميزة جديدة:
```
1. تحديث LanguageSpec.md (إذا كانت ميزة لغوية)
2. تحديث Architecture.md (إذا كانت ميزة معمارية)
3. تحديث UserGuide.md (إذا كانت ميزة للمستخدم)
4. تحديث README.md (ملخص التغييرات)
```

##### عند إصلاح خطأ:
```
1. تحديث DeveloperGuide.md (إضافة إلى قسم الأخطاء الشائعة)
2. تحديث UserGuide.md (إضافة إلى قسم استكشاف الأخطاء)
3. تحديث README.md (إصلاحات الأخطاء)
```

### الأداء والتحسين (Performance and Optimization)

#### 1. قياس الأداء (Performance Measurement)

##### أدوات القياس:
```
1. Visual Studio Profiler
2. Intel VTune Profiler
3. AMD CodeAnalyst
4. Custom timing functions
```

##### مثال على قياس الأداء:
```c
#include <time.h>

clock_t start, end;
double cpu_time_used;

start = clock();
// الكود المراد قياسه
end = clock();

cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("الوقت المستغرق: %f ثانية\n", cpu_time_used);
```

#### 2. تحسينات شائعة (Common Optimizations)

##### تحسين الذاكرة:
```c
// قبل التحسين: تخصيص متكرر
for (int i = 0; i < 1000; i++) {
    char* str = malloc(100);
    // ... استخدام str
    free(str);
}

// بعد التحسين: إعادة استخدام الذاكرة
char* str = malloc(100);
for (int i = 0; i < 1000; i++) {
    // ... استخدام str
}
free(str);
```

##### تحسين الخوارزميات:
```c
// قبل التحسين: بحث خطي
bool findSymbol(const char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

// بعد التحسين: بحث ثنائي
bool findSymbol(const char* name) {
    int left = 0, right = symbol_count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(symbols[mid].name, name);
        if (cmp == 0) return true;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return false;
}
```

### خاتمة (Conclusion)

هذا الدليل يوفر أساساً شاملاً لتطوير مترجم اللغة البرمجة العربية. يغطي جميع جوانب التطوير من البناء الأساسي إلى إضافة الميزات المتقدمة. نأمل أن يساعدك في المساهمة في تطوير المشروع.

This guide provides a comprehensive foundation for developing the Arabic Programming Language Compiler. It covers all aspects of development from basic building to adding advanced features. We hope it helps you contribute to the project development.

