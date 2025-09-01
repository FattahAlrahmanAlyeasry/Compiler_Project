# البنية المعمارية لمترجم اللغة البرمجة العربية
## Architecture of the Arabic Programming Language Compiler

### نظرة عامة (Overview)
يتبع مترجم اللغة البرمجة العربية البنية المعمارية التقليدية للمترجمات، مع مراعاة خصوصيات اللغة العربية مثل دعم Unicode والكتابة من اليمين إلى اليسار. يتكون المترجم من عدة مراحل متتالية تعالج الكود المصدري العربي وتحوله إلى كود MIPS قابل للتنفيذ.

The Arabic Programming Language Compiler follows the traditional compiler architecture while considering Arabic language specificities such as Unicode support and right-to-left writing. The compiler consists of several consecutive phases that process Arabic source code and transform it into executable MIPS code.

### البنية المعمارية العامة (General Architecture)

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   الكود المصدري  │    │   المحلل المعجمي  │    │   المحلل النحوي  │    │  المحلل الدلالي  │
│  Arabic Source │───▶│    Lexer       │───▶│    Parser      │───▶│   Semantic     │
│     Code       │    │                │    │                │    │   Analyzer     │
└─────────────────┘    └─────────────────┘    └─────────────────┘    └─────────────────┘
                                                       │                        │
                                                       ▼                        ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   كود MIPS      │    │   مولد الكود      │    │   شجرة البنية    │    │   جدول الرموز    │
│   MIPS Code    │◀───│   Code Gen      │◀───│   Abstract      │◀───│   Symbol        │
│                │    │                │    │   Syntax Tree   │    │   Table         │
└─────────────────┘    └─────────────────┘    └─────────────────┘    └─────────────────┘
```

### تدفق البيانات (Data Flow)

#### 1. مرحلة الإدخال (Input Phase)
```
الكود المصدري العربي (UTF-8) → معالجة Unicode → إعداد المحلل المعجمي
```

#### 2. مرحلة التحليل المعجمي (Lexical Analysis)
```
النص العربي → تقسيم إلى رموز → إنشاء قائمة الرموز → تمرير للمحلل النحوي
```

#### 3. مرحلة التحليل النحوي (Syntax Analysis)
```
قائمة الرموز → بناء شجرة الإعراب → التحقق من صحة البنية → تمرير للمحلل الدلالي
```

#### 4. مرحلة التحليل الدلالي (Semantic Analysis)
```
شجرة الإعراب → بناء جدول الرموز → التحقق من صحة الدلالة → بناء شجرة البنية المجردة
```

#### 5. مرحلة توليد الكود (Code Generation)
```
شجرة البنية المجردة → تحليل الأنواع → توليد كود MIPS → تحسين الكود
```

### مكونات المترجم (Compiler Components)

#### 1. المحلل المعجمي (Lexical Analyzer - Lexer)

##### الوظائف الرئيسية:
- قراءة الكود المصدري حرفاً حرفاً
- دعم Unicode الكامل للغة العربية
- التعرف على الكلمات المحجوزة والمعرفات
- توليد الرموز (Tokens)

##### البنية الداخلية:
```
Lexer
├── source_file: FILE*
├── current_line: wchar_t*
├── line_number: int
├── column_number: int
├── current_pos: int
└── line_length: int
```

##### خوارزمية العمل:
```
1. قراءة السطر التالي من الملف
2. تخطي الفراغات والتعليقات
3. التعرف على نوع الرمز التالي:
   - معرف (identifier)
   - كلمة محجوزة (reserved word)
   - ثابت (literal)
   - رمز خاص (special symbol)
4. إنشاء رمز جديد
5. إرجاع الرمز
```

#### 2. المحلل النحوي (Syntax Analyzer - Parser)

##### الوظائف الرئيسية:
- بناء شجرة الإعراب (Parse Tree)
- دعم البنية من اليمين إلى اليسار
- الكشف عن الأخطاء النحوية
- التحقق من صحة البنية

##### البنية الداخلية:
```
Parser
├── lexer: Lexer*
├── current_token: Token*
├── parse_tree: ParseTreeNode*
└── error_count: int
```

##### شجرة الإعراب:
```
ParseTreeNode
├── type: ParseTreeNodeType
├── token: Token*
├── parent: ParseTreeNode*
├── children: ParseTreeNode**
└── child_count: int
```

##### خوارزمية التحليل:
```
1. قراءة الرمز التالي
2. تحديد قاعدة النحو المناسبة
3. بناء عقدة في شجرة الإعراب
4. إضافة الأبناء (recursive descent)
5. التحقق من صحة البنية
```

#### 3. المحلل الدلالي (Semantic Analyzer)

##### الوظائف الرئيسية:
- بناء جدول الرموز
- التحقق من صحة الدلالة
- فحص أنواع البيانات
- بناء شجرة البنية المجردة

##### البنية الداخلية:
```
SemanticAnalyzer
├── symbol_table: SymbolTable*
├── ast: ASTNode*
├── current_scope: Scope*
└── error_count: int
```

##### خوارزمية التحليل:
```
1. زيارة عقد شجرة الإعراب
2. بناء جدول الرموز
3. فحص أنواع البيانات
4. التحقق من صحة العمليات
5. بناء شجرة البنية المجردة
```

#### 4. جدول الرموز (Symbol Table)

##### الوظائف الرئيسية:
- إدارة النطاقات (Scopes)
- تخزين معلومات الرموز
- البحث عن الرموز
- إدارة الذاكرة

##### البنية الداخلية:
```
SymbolTable
├── global_scope: Scope*
├── current_scope: Scope*
├── current_level: int
└── error_count: int

Scope
├── level: int
├── name: wchar_t*
├── symbols: SymbolEntry**
├── parent: Scope*
├── children: Scope**
└── next_sibling: Scope*

SymbolEntry
├── name: wchar_t*
├── type: SymbolType
├── data_type: DataType
├── scope_level: int
├── line_number: int
├── column_number: int
├── data: union {...}
└── next: SymbolEntry*
```

#### 5. شجرة البنية المجردة (Abstract Syntax Tree - AST)

##### الوظائف الرئيسية:
- تمثيل بنية البرنامج
- تسهيل توليد الكود
- دعم التحليل والتحسين

##### البنية الداخلية:
```
ASTNode
├── type: ASTNodeType
├── data_type: DataType
├── token: Token*
├── parent: ASTNode*
├── children: ASTNode**
├── child_count: int
└── data: union {...}
```

##### أنواع العقد:
- **ProgramNode**: عقدة البرنامج الرئيسية
- **BlockNode**: عقدة الكتلة البرمجية
- **StatementNode**: عقدة العبارة
- **ExpressionNode**: عقدة التعبير
- **DeclarationNode**: عقدة التصريح
- **TypeNode**: عقدة النوع

#### 6. مولد الكود (Code Generator)

##### الوظائف الرئيسية:
- تحويل AST إلى كود MIPS
- إدارة التسجيلات
- إدارة الذاكرة
- تحسين الكود

##### البنية الداخلية:
```
CodeGenerator
├── instructions: MIPSInstruction**
├── instruction_count: int
├── symbol_table: SymbolTable*
├── current_label: int
├── stack_offset: int
├── data_section_size: int
└── string_literals: wchar_t**
```

##### خوارزمية توليد الكود:
```
1. زيارة عقد AST
2. توليد كود MIPS لكل عقدة
3. إدارة التسجيلات والذاكرة
4. توليد مقطع البيانات
5. تحسين الكود الناتج
```

### استراتيجية الترجمة إلى MIPS (MIPS Translation Strategy)

#### 1. تمثيل أنواع البيانات (Data Type Representation)

##### الأنواع الأساسية:
- **عدد (int)**: 32 بت في التسجيل
- **حقيقي (float)**: 32 بت في التسجيل FP
- **حرف (char)**: 16 بت في التسجيل
- **منطقي (bool)**: 8 بت في التسجيل
- **نص (string)**: مؤشر إلى مقطع البيانات

##### الأنواع المركبة:
- **مصفوفة**: مؤشر + حجم + نوع
- **سجل**: مجموعة من الحقول في الذاكرة

#### 2. إدارة الذاكرة (Memory Management)

##### المتغيرات المحلية:
```
# تخصيص مساحة في المكدس
addi $sp, $sp, -4    # تخصيص 4 بايت
sw $t0, 0($sp)       # حفظ القيمة
```

##### المتغيرات العالمية:
```
.data
global_var: .word 0   # متغير عالمي
```

#### 3. إدارة التسجيلات (Register Management)

##### استراتيجية التخصيص:
- **$t0-$t9**: تسجيلات مؤقتة للتعبيرات
- **$s0-$s7**: تسجيلات محفوظة للمتغيرات
- **$a0-$a3**: معاملات الإجراءات
- **$v0-$v1**: قيم الإرجاع

#### 4. توليد التعليمات (Instruction Generation)

##### العمليات الحسابية:
```
# جمع: a + b
add $t0, $s0, $s1    # $t0 = $s0 + $s1
```

##### العمليات المنطقية:
```
# و منطقي: a && b
and $t0, $s0, $s1    # $t0 = $s0 & $s1
```

##### هياكل التحكم:
```
# إذا: if (condition)
beq $t0, $zero, else # if !condition goto else
# كود إذا كان الشرط صحيح
j end_if
else:
# كود إذا كان الشرط خاطئ
end_if:
```

### التكامل بين C و C# (C and C# Integration)

#### 1. تقنية P/Invoke

##### في C#:
```csharp
[DllImport("arabic_compiler.dll", CharSet = CharSet.Unicode)]
private static extern IntPtr EXPORT_CompileFile(string filename);
```

##### في C:
```c
__declspec(dllexport) wchar_t* EXPORT_CompileFile(const char* filename);
```

#### 2. إدارة الذاكرة

##### تمرير النصوص:
- C# → C: UTF-8 encoding
- C → C#: Unicode (wchar_t*)
- إدارة الذاكرة المشتركة

#### 3. معالجة الأخطاء

##### تمرير رسائل الخطأ:
- كود الخطأ
- رسالة الخطأ
- موقع الخطأ (سطر، عمود)

### دعم Visual Studio 2015

#### 1. إعدادات المشروع C++

##### إعدادات المترجم:
- **Standard**: C++11 (متوافق مع VS 2015)
- **Character Set**: Use Unicode Character Set
- **Runtime Library**: Multi-threaded DLL (/MD)

##### إعدادات الرابط:
- **Output File**: arabic_compiler.dll
- **Module Definition File**: arabic_compiler.def

#### 2. إعدادات المشروع C#

##### إطار العمل:
- **Target Framework**: .NET Framework 4.5.2
- **Platform Target**: Any CPU
- **Output Type**: Windows Application

##### إعدادات P/Invoke:
- **DllImport**: arabic_compiler.dll
- **CharSet**: Unicode
- **CallingConvention**: StdCall

### تحسينات الأداء (Performance Optimizations)

#### 1. تحسينات المحلل المعجمي
- استخدام buffer للقراءة
- تحسين البحث في الكلمات المحجوزة
- cache للرموز المتكررة

#### 2. تحسينات المحلل النحوي
- تحليل توقع الأخطاء
- تحسين بناء شجرة الإعراب
- تقليل عمليات الذاكرة

#### 3. تحسينات مولد الكود
- تحسين تخصيص التسجيلات
- تقليل عمليات الذاكرة
- تحسين التعبيرات

### اختبار البنية المعمارية (Architecture Testing)

#### 1. اختبارات الوحدة
- اختبار كل مكون على حدة
- اختبار الواجهات الداخلية
- اختبار معالجة الأخطاء

#### 2. اختبارات التكامل
- اختبار التكامل بين المكونات
- اختبار تدفق البيانات
- اختبار إدارة الذاكرة

#### 3. اختبارات النظام
- اختبار البرامج الكاملة
- اختبار الأداء
- اختبار الاستقرار

### خاتمة (Conclusion)

هذه البنية المعمارية توفر أساساً قوياً لبناء مترجم فعال للغة البرمجة العربية. التصميم يراعي خصوصيات اللغة العربية ويضمن التوافق مع Visual Studio 2015. كل مكون مصمم ليكون قابلاً للاختبار والصيانة والتطوير المستقبلي.

This architecture provides a solid foundation for building an effective compiler for the Arabic Programming Language. The design considers Arabic language specificities and ensures compatibility with Visual Studio 2015. Each component is designed to be testable, maintainable, and extensible for future development.

