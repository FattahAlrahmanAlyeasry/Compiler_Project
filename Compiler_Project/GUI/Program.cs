using System;
using System.Windows.Forms;
using System.Globalization;
using System.Threading;

namespace ArabicCompiler.GUI
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            // Set culture for Arabic support
            Thread.CurrentThread.CurrentCulture = new CultureInfo("ar-SA");
            Thread.CurrentThread.CurrentUICulture = new CultureInfo("ar-SA");
            
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            try
            {
                // Initialize compiler interface
                if (!CompilerInterface.Initialize())
                {
                    MessageBox.Show("فشل في تهيئة المترجم.\nFailed to initialize compiler.", 
                        "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                
                // Run the main window
                Application.Run(new MainWindow());
            }
            catch (Exception ex)
            {
                MessageBox.Show($"خطأ في تشغيل التطبيق:\n{ex.Message}\n\nApplication error:\n{ex.Message}", 
                    "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                try
                {
                    CompilerInterface.Cleanup();
                }
                catch (Exception ex)
                {
                    // Log cleanup error but don't show to user
                    System.Diagnostics.Debug.WriteLine($"Cleanup error: {ex.Message}");
                }
            }
        }
    }
}
