# مترجم اللغة البرمجة العربية - Arabic Programming Language Compiler

مشروع مترجم كامل للغة برمجة عربية مخصصة، يترجم الكود العربي إلى كود MIPS assembly. المشروع مكتوب بلغة C للجزء الأساسي و C# لواجهة المستخدم.

A complete compiler project for a custom Arabic programming language that translates Arabic code to MIPS assembly code. The project is written in C for the core compiler and C# for the user interface.

## المميزات - Features

### المترجم الأساسي - Core Compiler
- **محلل معجمي (Lexer)**: يدعم Unicode واللغة العربية
- **محلل نحوي (Parser)**: يبني شجرة إعراب من اليمين إلى اليسار
- **محلل دلالي (Semantic Analyzer)**: يتحقق من صحة البرنامج
- **مولد كود MIPS**: ينتج كود MIPS assembly صحيح
- **جدول رموز**: يدير المتغيرات والإجراءات والأنواع
- **شجرة بنية مجردة (AST)**: تمثل بنية البرنامج

### واجهة المستخدم - User Interface
- **محرر أكواد**: يدعم النصوص العربية وتمييز الصيغة
- **عارض شجرة الإعراب**: يعرض الشجرة من اليمين إلى اليسار
- **عارض كود MIPS**: يعرض الكود المُنشأ مع تمييز الصيغة
- **نافذة مخرجات**: تعرض الأخطاء والرسائل
- **قوائم وأدوات**: للتحكم في المشروع

### التوافق - Compatibility
- **Visual Studio 2015**: دعم كامل
- **Windows**: نظام التشغيل المدعوم
- **Unicode**: دعم كامل للغة العربية
- **P/Invoke**: تكامل بين C# و C

## المتطلبات - Requirements

### النظام - System
- Windows 7/8/10/11
- Visual Studio 2015 (Community/Professional/Enterprise)
- .NET Framework 4.5.2 أو أحدث

### المكونات - Components
- Visual C++ 2015
- Windows SDK 8.1
- MSBuild 14.0

## التثبيت - Installation

### 1. تثبيت Visual Studio 2015
```bash
# تحميل Visual Studio 2015 Community (مجاني)
# https://visualstudio.microsoft.com/vs/older-downloads/
```

### 2. استنساخ المشروع
```bash
git clone https://github.com/your-username/arabic-compiler.git
cd arabic-compiler
```

### 3. فتح المشروع
```bash
# فتح ملف الحل في Visual Studio
ArabicCompiler.sln
```

### 4. بناء المشروع
```bash
# استخدام Makefile
make build

# أو استخدام Visual Studio مباشرة
# Build -> Build Solution (Ctrl+Shift+B)
```

## الاستخدام - Usage

### 1. تشغيل التطبيق
```bash
make run
# أو
# Debug -> Start Debugging (F5)
```

### 2. كتابة برنامج عربي
```arabic
برنامج
[
  متغيرات
    متغير صحيح ؛
    رسالة نص ؛
]
:
  متغير = 42 ؛
  رسالة = "مرحبا بالعالم" ؛
  اطبع رسالة ؛
  اطبع متغير ؛
؛
```

### 3. ترجمة البرنامج
- اضغط على زر "ترجمة" أو استخدم Ctrl+Shift+B
- عرض شجرة الإعراب
- عرض كود MIPS المُنشأ
- حفظ النتائج

## هيكل المشروع - Project Structure

```
Compiler_Project/
├── CompilerCore/           # المترجم الأساسي بلغة C
│   ├── lexer.h/c          # المحلل المعجمي
│   ├── parser.h/c         # المحلل النحوي
│   ├── semantic.h/c       # المحلل الدلالي
│   ├── codegen.h/c        # مولد كود MIPS
│   ├── ast.h/c            # شجرة البنية المجردة
│   ├── parsetree.h/c      # شجرة الإعراب
│   ├── symboltable.h/c    # جدول الرموز
│   ├── main.h/c           # النقطة الرئيسية
│   └── CompilerCore.vcxproj # ملف مشروع Visual Studio
├── GUI/                    # واجهة المستخدم بلغة C#
│   ├── MainWindow.cs      # النافذة الرئيسية
│   ├── CodeEditor.cs      # محرر الأكواد
│   ├── ParseTreeViewer.cs # عارض شجرة الإعراب
│   ├── MipsCodeViewer.cs  # عارض كود MIPS
│   ├── CompilerInterface.cs # واجهة الاتصال بالمترجم
│   ├── Program.cs         # نقطة الدخول
│   └── ArabicCompiler.GUI.csproj # ملف مشروع C#
├── Documentation/          # التوثيق
│   ├── README.md          # هذا الملف
│   ├── LanguageSpec.md    # مواصفات اللغة
│   ├── Architecture.md    # البنية المعمارية
│   ├── UserGuide.md       # دليل المستخدم
│   ├── DeveloperGuide.md  # دليل المطور
│   ├── Performance.md     # تحليل الأداء
│   └── Features.md        # الميزات المتقدمة
├── Tests/                  # الاختبارات
│   ├── UnitTests/         # اختبارات الوحدة
│   ├── IntegrationTests/  # اختبارات التكامل
│   ├── SystemTests/       # اختبارات النظام
│   ├── PerformanceTests/  # اختبارات الأداء
│   └── TestCases/         # حالات الاختبار
├── Output/                 # المخرجات
├── ArabicCompiler.sln      # ملف الحل
└── Makefile               # ملف البناء
```

## البناء - Building

### استخدام Makefile
```bash
# بناء كامل
make build

# بناء المترجم فقط
make build-compiler

# بناء الواجهة فقط
make build-gui

# تنظيف
make clean

# تشغيل
make run

# فتح في Visual Studio
make open

# المساعدة
make help
```

### استخدام Visual Studio
1. افتح `ArabicCompiler.sln`
2. اختر الإعداد (Debug/Release) والمنصة (Win32/x64)
3. اضغط Ctrl+Shift+B للبناء
4. اضغط F5 للتشغيل

## الاختبار - Testing

### تشغيل الاختبارات
```bash
# جميع الاختبارات
make test

# اختبارات الوحدة
cd Tests/UnitTests
make test

# اختبارات التكامل
cd Tests/IntegrationTests
make test
```

### أنواع الاختبارات
- **اختبارات الوحدة**: اختبار كل مكون منفرداً
- **اختبارات التكامل**: اختبار التكامل بين المكونات
- **اختبارات النظام**: اختبار النظام كاملاً
- **اختبارات الأداء**: قياس الأداء والذاكرة

## التطوير - Development

### إضافة ميزات جديدة
1. اكتب الكود في الملف المناسب
2. أضف الاختبارات
3. حدث التوثيق
4. اختبر التغييرات

### إصلاح الأخطاء
1. حدد المشكلة
2. اكتب اختبار يظهر المشكلة
3. أصلح الكود
4. تأكد من نجاح الاختبار

### إرشادات التطوير
- استخدم التعليقات العربية
- اتبع معايير الترميز
- اكتب اختبارات شاملة
- حدث التوثيق

## المساهمة - Contributing

نرحب بالمساهمات! يرجى:

1. Fork المشروع
2. إنشاء فرع للميزة الجديدة
3. إجراء التغييرات
4. إضافة الاختبارات
5. تحديث التوثيق
6. إرسال Pull Request

## الترخيص - License

هذا المشروع مرخص تحت رخصة MIT. راجع ملف `LICENSE` للتفاصيل.

## الدعم - Support

### المشاكل المعروفة
- يتطلب Visual Studio 2015
- يعمل على Windows فقط
- يحتاج إلى .NET Framework 4.5.2

### الحصول على المساعدة
- افتح Issue في GitHub
- راجع التوثيق
- ابحث في الكود
- اتصل بالفريق

## الإصدارات - Versions

### الإصدار الحالي: 1.0.0
- مترجم أساسي كامل
- واجهة مستخدم متقدمة
- دعم كامل للغة العربية
- توافق مع Visual Studio 2015

### الإصدارات القادمة
- تحسينات الأداء
- ميزات متقدمة
- دعم منصات إضافية
- وثائق موسعة

## الفريق - Team

- **المطور الرئيسي**: فريق التطوير
- **المساهمون**: المجتمع المفتوح
- **المديرون**: فريق الإدارة

## التحديثات - Updates

### آخر تحديث: 2024
- إكمال المترجم الأساسي
- إكمال واجهة المستخدم
- إضافة التوثيق الشامل
- إعداد ملفات المشروع

---

**ملاحظة**: هذا المشروع مصمم خصيصاً للعمل مع Visual Studio 2015. يرجى التأكد من توافق النظام قبل الاستخدام.

**Note**: This project is specifically designed to work with Visual Studio 2015. Please ensure system compatibility before use.
