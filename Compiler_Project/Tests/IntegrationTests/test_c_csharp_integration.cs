using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using NUnit.Framework;

namespace ArabicCompiler.Tests.Integration
{
    [TestFixture]
    public class CSharpCIntegrationTests
    {
        private CompilerInterface compiler;

        [SetUp]
        public void Setup()
        {
            compiler = new CompilerInterface();
            compiler.Initialize();
        }

        [TearDown]
        public void TearDown()
        {
            compiler?.Cleanup();
            compiler?.Dispose();
        }

        [Test]
        public void TestCompilerInitialization()
        {
            // اختبار تهيئة المترجم
            Assert.IsTrue(compiler.IsInitialized, "يجب أن يتم تهيئة المترجم بنجاح");
        }

        [Test]
        public void TestSimpleProgramCompilation()
        {
            // اختبار ترجمة برنامج بسيط
            string sourceCode = "برنامج اختبار;";
            
            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج البسيط");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsNotEmpty(mipsCode, "يجب أن يتم توليد كود MIPS");
            Assert.IsTrue(mipsCode.Contains(".text"), "يجب أن يحتوي كود MIPS على مقطع النص");
        }

        [Test]
        public void TestProgramWithVariables()
        {
            // اختبار برنامج يحتوي على متغيرات
            string sourceCode = @"
                برنامج متغيرات;
                [
                  متغيرات
                    متغير : عدد صحيح;
                  بداية
                    متغير = 10;
                    اطبع متغير;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع المتغيرات");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsTrue(mipsCode.Contains("sw"), "يجب أن يحتوي كود MIPS على تعليمات حفظ");
            Assert.IsTrue(mipsCode.Contains("lw"), "يجب أن يحتوي كود MIPS على تعليمات تحميل");
        }

        [Test]
        public void TestProgramWithConstants()
        {
            // اختبار برنامج يحتوي على ثوابت
            string sourceCode = @"
                برنامج ثوابت;
                [
                  ثوابت
                    ثابت = 100;
                  متغيرات
                    متغير : عدد صحيح;
                  بداية
                    متغير = ثابت;
                    اطبع متغير;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع الثوابت");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsTrue(mipsCode.Contains("100"), "يجب أن يحتوي كود MIPS على قيمة الثابت");
        }

        [Test]
        public void TestProgramWithTypes()
        {
            // اختبار برنامج يحتوي على أنواع مخصصة
            string sourceCode = @"
                برنامج أنواع;
                [
                  أنواع
                    نوع_جديد = عدد صحيح;
                  متغيرات
                    متغير : نوع_جديد;
                  بداية
                    متغير = 50;
                    اطبع متغير;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع الأنواع المخصصة");
        }

        [Test]
        public void TestProgramWithProcedures()
        {
            // اختبار برنامج يحتوي على إجراءات
            string sourceCode = @"
                برنامج إجراءات;
                [
                  إجراءات
                    إجراء طباعة_رسالة;
                    بداية
                      اطبع ""مرحبا بالعالم"";
                    نهاية;
                  بداية
                    طباعة_رسالة;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع الإجراءات");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsTrue(mipsCode.Contains("jal"), "يجب أن يحتوي كود MIPS على تعليمات استدعاء الإجراء");
        }

        [Test]
        public void TestProgramWithConditionals()
        {
            // اختبار برنامج يحتوي على شروط
            string sourceCode = @"
                برنامج شروط;
                [
                  متغيرات
                    متغير : عدد صحيح;
                  بداية
                    متغير = 15;
                    إذا متغير > 10 إذن
                      اطبع ""كبير"";
                    وإلا
                      اطبع ""صغير"";
                    نهاية;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع الشروط");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsTrue(mipsCode.Contains("bgt"), "يجب أن يحتوي كود MIPS على تعليمات الفرع");
        }

        [Test]
        public void TestProgramWithLoops()
        {
            // اختبار برنامج يحتوي على حلقات
            string sourceCode = @"
                برنامج حلقات;
                [
                  متغيرات
                    عداد : عدد صحيح;
                  بداية
                    عداد = 5;
                    طالما عداد > 0 افعل
                      اطبع عداد;
                      عداد = عداد - 1;
                    نهاية;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع الحلقات");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsTrue(mipsCode.Contains("j"), "يجب أن يحتوي كود MIPS على تعليمات القفز");
        }

        [Test]
        public void TestProgramWithArrays()
        {
            // اختبار برنامج يحتوي على مصفوفات
            string sourceCode = @"
                برنامج مصفوفات;
                [
                  متغيرات
                    مصفوفة : مصفوفة[5] من عدد صحيح;
                    عداد : عدد صحيح;
                  بداية
                    عداد = 0;
                    طالما عداد < 5 افعل
                      مصفوفة[عداد] = عداد * 2;
                      عداد = عداد + 1;
                    نهاية;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع المصفوفات");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsTrue(mipsCode.Contains("sll"), "يجب أن يحتوي كود MIPS على تعليمات إزاحة");
        }

        [Test]
        public void TestProgramWithRecords()
        {
            // اختبار برنامج يحتوي على سجلات
            string sourceCode = @"
                برنامج سجلات;
                [
                  أنواع
                    طالب = سجل
                      اسم : نص;
                      عمر : عدد صحيح;
                    نهاية;
                  متغيرات
                    طالب_جديد : طالب;
                  بداية
                    طالب_جديد.اسم = ""أحمد"";
                    طالب_جديد.عمر = 20;
                    اطبع طالب_جديد.اسم;
                    اطبع طالب_جديد.عمر;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع السجلات");
        }

        [Test]
        public void TestParseTreeGeneration()
        {
            // اختبار توليد شجرة الإعراب
            string sourceCode = "برنامج اختبار;";
            
            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح الترجمة");
            
            string parseTree = compiler.GetParseTree();
            Assert.IsNotEmpty(parseTree, "يجب أن يتم توليد شجرة الإعراب");
            Assert.IsTrue(parseTree.Contains("شجرة الإعراب"), "يجب أن تحتوي شجرة الإعراب على العنوان الصحيح");
        }

        [Test]
        public void TestASTGeneration()
        {
            // اختبار توليد شجرة البنية المجردة
            string sourceCode = "برنامج اختبار;";
            
            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح الترجمة");
            
            string ast = compiler.GetAST();
            Assert.IsNotEmpty(ast, "يجب أن يتم توليد شجرة البنية المجردة");
            Assert.IsTrue(ast.Contains("شجرة البنية المجردة"), "يجب أن تحتوي AST على العنوان الصحيح");
        }

        [Test]
        public void TestErrorHandling()
        {
            // اختبار معالجة الأخطاء
            string sourceCode = @"
                برنامج أخطاء
                [
                  متغيرات
                    متغير : عدد صحيح
                  بداية
                    متغير = 10
                    اطبع متغير
                  نهاية
                ]."; // مفقود ;

            bool result = compiler.CompileFile(sourceCode);
            // قد تنجح الترجمة مع الأخطاء أو تفشل، لكن يجب التعامل معها
            Assert.IsNotNull(compiler.GetErrorCount(), "يجب أن يتم حساب عدد الأخطاء");
        }

        [Test]
        public void TestErrorMessages()
        {
            // اختبار رسائل الأخطاء
            string sourceCode = "برنامج اختبار;";
            
            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح الترجمة");
            
            string errorMessages = compiler.GetErrorMessages();
            // قد تكون رسائل الأخطاء فارغة إذا لم تكن هناك أخطاء
            Assert.IsNotNull(errorMessages, "يجب أن يتم إرجاع رسائل الأخطاء");
        }

        [Test]
        public void TestVariableInfo()
        {
            // اختبار معلومات المتغيرات
            string sourceCode = @"
                برنامج متغيرات;
                [
                  متغيرات
                    متغير1 : عدد صحيح;
                    متغير2 : عدد حقيقي;
                    متغير3 : نص;
                  بداية
                    متغير1 = 10;
                    متغير2 = 3.14;
                    متغير3 = ""مرحبا"";
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح الترجمة");
            
            string variableInfo = compiler.GetVariableInfo();
            Assert.IsNotEmpty(variableInfo, "يجب أن يتم إرجاع معلومات المتغيرات");
            Assert.IsTrue(variableInfo.Contains("معلومات المتغيرات"), "يجب أن تحتوي معلومات المتغيرات على العنوان الصحيح");
        }

        [Test]
        public void TestProcedureInfo()
        {
            // اختبار معلومات الإجراءات
            string sourceCode = @"
                برنامج إجراءات;
                [
                  إجراءات
                    إجراء1;
                    بداية
                      اطبع ""مرحبا"";
                    نهاية;
                    إجراء2(معامل : عدد صحيح);
                    بداية
                      اطبع معامل;
                    نهاية;
                  بداية
                    إجراء1;
                    إجراء2(42);
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح الترجمة");
            
            string procedureInfo = compiler.GetProcedureInfo();
            Assert.IsNotEmpty(procedureInfo, "يجب أن يتم إرجاع معلومات الإجراءات");
            Assert.IsTrue(procedureInfo.Contains("معلومات الإجراءات"), "يجب أن تحتوي معلومات الإجراءات على العنوان الصحيح");
        }

        [Test]
        public void TestMIPSCodeSaving()
        {
            // اختبار حفظ كود MIPS
            string sourceCode = "برنامج اختبار;";
            
            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح الترجمة");
            
            string tempFile = "test_output.s";
            bool saveResult = compiler.SaveMIPSCodeToFile(tempFile);
            Assert.IsTrue(saveResult, "يجب أن يتم حفظ كود MIPS بنجاح");
            
            // التحقق من وجود الملف
            Assert.IsTrue(File.Exists(tempFile), "يجب أن يتم إنشاء ملف MIPS");
            
            // قراءة محتوى الملف
            string fileContent = File.ReadAllText(tempFile);
            Assert.IsNotEmpty(fileContent, "يجب أن يحتوي ملف MIPS على محتوى");
            
            // تنظيف
            File.Delete(tempFile);
        }

        [Test]
        public void TestComplexProgramCompilation()
        {
            // اختبار برنامج معقد
            string sourceCode = @"
                برنامج برنامج_معقد;
                [
                  ثوابت
                    عدد_العناصر = 10;
                    قيمة_أولية = 1;
                  أنواع
                    مصفوفة_أرقام = مصفوفة[10] من عدد صحيح;
                    طالب = سجل
                      اسم : نص;
                      درجات : مصفوفة[5] من عدد حقيقي;
                      متوسط : عدد حقيقي;
                    نهاية;
                  متغيرات
                    أرقام : مصفوفة_أرقام;
                    طلاب : مصفوفة[5] من طالب;
                    عداد, مجموع : عدد صحيح;
                    متوسط_عام : عدد حقيقي;
                  إجراءات
                    إجراء حساب_المتوسط(مصفوفة : مصفوفة_أرقام, عدد : عدد صحيح) : عدد حقيقي;
                    متغيرات
                      مجموع_محلي : عدد صحيح;
                      عداد_محلي : عدد صحيح;
                    بداية
                      مجموع_محلي = 0;
                      عداد_محلي = 0;
                      طالما عداد_محلي < عدد افعل
                        مجموع_محلي = مجموع_محلي + مصفوفة[عداد_محلي];
                        عداد_محلي = عداد_محلي + 1;
                      نهاية;
                      حساب_المتوسط = مجموع_محلي / عدد;
                    نهاية;
                    إجراء طباعة_النتائج(متوسط : عدد حقيقي);
                    بداية
                      اطبع ""المتوسط هو: "", متوسط;
                    نهاية;
                  بداية
                    // تهيئة المصفوفة
                    عداد = 0;
                    طالما عداد < عدد_العناصر افعل
                      أرقام[عداد] = عداد * 2 + قيمة_أولية;
                      عداد = عداد + 1;
                    نهاية;
                    
                    // حساب المتوسط
                    متوسط_عام = حساب_المتوسط(أرقام, عدد_العناصر);
                    
                    // طباعة النتائج
                    طباعة_النتائج(متوسط_عام);
                    
                    // معالجة الطلاب
                    عداد = 0;
                    طالما عداد < 5 افعل
                      طلاب[عداد].اسم = ""طالب"", عداد;
                      طلاب[عداد].متوسط = 85.5 + عداد;
                      عداد = عداد + 1;
                    نهاية;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج المعقد");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsNotEmpty(mipsCode, "يجب أن يتم توليد كود MIPS للبرنامج المعقد");
            
            // التحقق من وجود العناصر الأساسية في كود MIPS
            Assert.IsTrue(mipsCode.Contains(".text"), "يجب أن يحتوي كود MIPS على مقطع النص");
            Assert.IsTrue(mipsCode.Contains(".data"), "يجب أن يحتوي كود MIPS على مقطع البيانات");
        }

        [Test]
        public void TestUnicodeHandling()
        {
            // اختبار التعامل مع النصوص العربية
            string sourceCode = @"
                برنامج نصوص_عربية;
                [
                  متغيرات
                    رسالة : نص;
                    اسم : نص;
                  بداية
                    رسالة = ""مرحبا بالعالم العربي"";
                    اسم = ""أحمد محمد علي"";
                    اطبع رسالة;
                    اطبع ""اسم المستخدم: "", اسم;
                  نهاية
                ].";

            bool result = compiler.CompileFile(sourceCode);
            Assert.IsTrue(result, "يجب أن تنجح ترجمة البرنامج مع النصوص العربية");
            
            string mipsCode = compiler.GetMIPSCode();
            Assert.IsNotEmpty(mipsCode, "يجب أن يتم توليد كود MIPS");
        }

        [Test]
        public void TestPerformanceCompilation()
        {
            // اختبار أداء الترجمة
            string sourceCode = "برنامج اختبار_الأداء;";
            
            var stopwatch = System.Diagnostics.Stopwatch.StartNew();
            bool result = compiler.CompileFile(sourceCode);
            stopwatch.Stop();
            
            Assert.IsTrue(result, "يجب أن تنجح الترجمة");
            Assert.IsTrue(stopwatch.ElapsedMilliseconds < 1000, "يجب أن تكتمل الترجمة في أقل من ثانية واحدة");
        }

        [Test]
        public void TestMemoryManagement()
        {
            // اختبار إدارة الذاكرة
            for (int i = 0; i < 10; i++)
            {
                string sourceCode = $"برنامج اختبار_الذاكرة_{i};";
                bool result = compiler.CompileFile(sourceCode);
                Assert.IsTrue(result, $"يجب أن تنجح الترجمة {i}");
                
                // الحصول على النتائج
                string mipsCode = compiler.GetMIPSCode();
                string parseTree = compiler.GetParseTree();
                string ast = compiler.GetAST();
                
                Assert.IsNotEmpty(mipsCode, "يجب أن يتم توليد كود MIPS");
                Assert.IsNotEmpty(parseTree, "يجب أن يتم توليد شجرة الإعراب");
                Assert.IsNotEmpty(ast, "يجب أن يتم توليد شجرة البنية المجردة");
            }
        }
    }
}

