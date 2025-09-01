using System;
using System.Windows.Forms;
using System.Drawing;

namespace ArabicCompiler.GUI
{
    public partial class MainWindow : Form
    {
        private CodeEditor codeEditor;
        private ParseTreeViewer parseTreeViewer;
        private MipsCodeViewer mipsCodeViewer;
        private TextBox outputTextBox;
        private MenuStrip menuStrip;
        private ToolStrip toolStrip;

        public MainWindow()
        {
            InitializeComponent();
            InitializeCompiler();
        }

        private void InitializeComponent()
        {
            this.Text = "مترجم اللغة البرمجة العربية - Arabic Programming Language Compiler";
            this.Size = new Size(1200, 800);
            this.StartPosition = FormStartPosition.CenterScreen;
            this.RightToLeft = RightToLeft.Yes;
            this.RightToLeftLayout = true;

            // Create menu strip
            CreateMenuStrip();

            // Create tool strip
            CreateToolStrip();

            // Create main layout
            CreateMainLayout();

            // Set up event handlers
            SetupEventHandlers();
        }

        private void CreateMenuStrip()
        {
            menuStrip = new MenuStrip();
            menuStrip.RightToLeft = RightToLeft.Yes;

            // File menu
            var fileMenu = new ToolStripMenuItem("ملف");
            fileMenu.DropDownItems.Add("جديد", null, OnNewFile);
            fileMenu.DropDownItems.Add("فتح", null, OnOpenFile);
            fileMenu.DropDownItems.Add("حفظ", null, OnSaveFile);
            fileMenu.DropDownItems.Add("حفظ باسم", null, OnSaveFileAs);
            fileMenu.DropDownItems.Add("-");
            fileMenu.DropDownItems.Add("خروج", null, OnExit);

            // Edit menu
            var editMenu = new ToolStripMenuItem("تحرير");
            editMenu.DropDownItems.Add("تراجع", null, OnUndo);
            editMenu.DropDownItems.Add("إعادة", null, OnRedo);
            editMenu.DropDownItems.Add("-");
            editMenu.DropDownItems.Add("قص", null, OnCut);
            editMenu.DropDownItems.Add("نسخ", null, OnCopy);
            editMenu.DropDownItems.Add("لصق", null, OnPaste);

            // Compile menu
            var compileMenu = new ToolStripMenuItem("ترجمة");
            compileMenu.DropDownItems.Add("ترجمة", null, OnCompile);
            compileMenu.DropDownItems.Add("ترجمة وتشغيل", null, OnCompileAndRun);

            // View menu
            var viewMenu = new ToolStripMenuItem("عرض");
            viewMenu.DropDownItems.Add("شجرة الإعراب", null, OnToggleParseTree);
            viewMenu.DropDownItems.Add("كود MIPS", null, OnToggleMipsCode);

            // Help menu
            var helpMenu = new ToolStripMenuItem("مساعدة");
            helpMenu.DropDownItems.Add("حول", null, OnAbout);

            menuStrip.Items.Add(fileMenu);
            menuStrip.Items.Add(editMenu);
            menuStrip.Items.Add(compileMenu);
            menuStrip.Items.Add(viewMenu);
            menuStrip.Items.Add(helpMenu);

            this.Controls.Add(menuStrip);
            this.MainMenuStrip = menuStrip;
        }

        private void CreateToolStrip()
        {
            toolStrip = new ToolStrip();
            toolStrip.RightToLeft = RightToLeft.Yes;

            var newButton = new ToolStripButton("جديد", null, OnNewFile);
            var openButton = new ToolStripButton("فتح", null, OnOpenFile);
            var saveButton = new ToolStripButton("حفظ", null, OnSaveFile);
            var compileButton = new ToolStripButton("ترجمة", null, OnCompile);
            var runButton = new ToolStripButton("تشغيل", null, OnCompileAndRun);

            toolStrip.Items.Add(newButton);
            toolStrip.Items.Add(openButton);
            toolStrip.Items.Add(saveButton);
            toolStrip.Items.Add(new ToolStripSeparator());
            toolStrip.Items.Add(compileButton);
            toolStrip.Items.Add(runButton);

            this.Controls.Add(toolStrip);
        }

        private void CreateMainLayout()
        {
            // Create split containers for resizable panels
            var mainSplitContainer = new SplitContainer();
            mainSplitContainer.Dock = DockStyle.Fill;
            mainSplitContainer.Orientation = Orientation.Vertical;
            mainSplitContainer.SplitterDistance = 400;

            var leftSplitContainer = new SplitContainer();
            leftSplitContainer.Dock = DockStyle.Fill;
            leftSplitContainer.Orientation = Orientation.Horizontal;
            leftSplitContainer.SplitterDistance = 300;

            // Code editor (top left)
            codeEditor = new CodeEditor();
            codeEditor.Dock = DockStyle.Fill;
            leftSplitContainer.Panel1.Controls.Add(codeEditor);

            // Output/errors (bottom left)
            outputTextBox = new TextBox();
            outputTextBox.Multiline = true;
            outputTextBox.ScrollBars = ScrollBars.Vertical;
            outputTextBox.ReadOnly = true;
            outputTextBox.Font = new Font("Tahoma", 9);
            outputTextBox.Text = "مرحباً بك في مترجم اللغة البرمجة العربية!\n\nاكتب الكود في المحرر أعلاه ثم اضغط على زر الترجمة.\n\nWelcome to the Arabic Programming Language Compiler!\n\nWrite your code in the editor above and click the Compile button.";
            leftSplitContainer.Panel2.Controls.Add(outputTextBox);

            // Right panel for parse tree and MIPS code
            var rightSplitContainer = new SplitContainer();
            rightSplitContainer.Dock = DockStyle.Fill;
            rightSplitContainer.Orientation = Orientation.Horizontal;
            rightSplitContainer.SplitterDistance = 300;

            // Parse tree viewer (top right)
            parseTreeViewer = new ParseTreeViewer();
            parseTreeViewer.Dock = DockStyle.Fill;
            rightSplitContainer.Panel1.Controls.Add(parseTreeViewer);

            // MIPS code viewer (bottom right)
            mipsCodeViewer = new MipsCodeViewer();
            mipsCodeViewer.Dock = DockStyle.Fill;
            rightSplitContainer.Panel2.Controls.Add(mipsCodeViewer);

            // Add panels to main container
            mainSplitContainer.Panel1.Controls.Add(leftSplitContainer);
            mainSplitContainer.Panel2.Controls.Add(rightSplitContainer);

            // Add main container to form
            this.Controls.Add(mainSplitContainer);

            // Set initial sample code
            codeEditor.SourceCode = @"برنامج
[
    متغير صحيح
] :
    اطبع ""مرحباً بالعالم!"" ؛
    متغير = 42 ؛
    اطبع متغير ؛
    ؛";
        }

        private void SetupEventHandlers()
        {
            this.FormClosing += MainWindow_FormClosing;
        }

        private void InitializeCompiler()
        {
            try
            {
                if (CompilerInterface.Initialize())
                {
                    outputTextBox.AppendText("\n\nتم تهيئة المترجم بنجاح.\nCompiler initialized successfully.");
                }
                else
                {
                    outputTextBox.AppendText("\n\nفشل في تهيئة المترجم.\nFailed to initialize compiler.");
                }
            }
            catch (Exception ex)
            {
                outputTextBox.AppendText($"\n\nخطأ في التهيئة: {ex.Message}\nInitialization error: {ex.Message}");
            }
        }

        #region Menu Event Handlers

        private void OnNewFile(object sender, EventArgs e)
        {
            if (MessageBox.Show("هل تريد إنشاء ملف جديد؟\nDo you want to create a new file?", "ملف جديد - New File", 
                MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                codeEditor.Clear();
                outputTextBox.Text = "تم إنشاء ملف جديد.\nNew file created.";
            }
        }

        private void OnOpenFile(object sender, EventArgs e)
        {
            using (var openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "ملفات النصوص (*.txt)|*.txt|جميع الملفات (*.*)|*.*";
                openFileDialog.Title = "فتح ملف - Open File";
                
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        codeEditor.LoadFromFile(openFileDialog.FileName);
                        outputTextBox.Text = $"تم فتح الملف: {openFileDialog.FileName}\nFile opened: {openFileDialog.FileName}";
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"خطأ في فتح الملف:\n{ex.Message}\n\nError opening file:\n{ex.Message}", 
                            "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void OnSaveFile(object sender, EventArgs e)
        {
            try
            {
                codeEditor.SaveToFile("program.txt");
                outputTextBox.Text = "تم حفظ الملف بنجاح.\nFile saved successfully.";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"خطأ في حفظ الملف:\n{ex.Message}\n\nError saving file:\n{ex.Message}", 
                    "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void OnSaveFileAs(object sender, EventArgs e)
        {
            using (var saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.Filter = "ملفات النصوص (*.txt)|*.txt|جميع الملفات (*.*)|*.*";
                saveFileDialog.Title = "حفظ باسم - Save As";
                saveFileDialog.FileName = "program.txt";
                
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        codeEditor.SaveToFile(saveFileDialog.FileName);
                        outputTextBox.Text = $"تم حفظ الملف: {saveFileDialog.FileName}\nFile saved: {saveFileDialog.FileName}";
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"خطأ في حفظ الملف:\n{ex.Message}\n\nError saving file:\n{ex.Message}", 
                            "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void OnExit(object sender, EventArgs e)
        {
            this.Close();
        }

        private void OnUndo(object sender, EventArgs e)
        {
            // Implement undo functionality
            outputTextBox.Text = "تراجع - Undo";
        }

        private void OnRedo(object sender, EventArgs e)
        {
            // Implement redo functionality
            outputTextBox.Text = "إعادة - Redo";
        }

        private void OnCut(object sender, EventArgs e)
        {
            codeEditor.Cut();
        }

        private void OnCopy(object sender, EventArgs e)
        {
            codeEditor.Copy();
        }

        private void OnPaste(object sender, EventArgs e)
        {
            codeEditor.Paste();
        }

        private void OnCompile(object sender, EventArgs e)
        {
            CompileCode();
        }

        private void OnCompileAndRun(object sender, EventArgs e)
        {
            if (CompileCode())
            {
                outputTextBox.AppendText("\n\nتم الترجمة بنجاح! يمكنك الآن تشغيل البرنامج.\nCompilation successful! You can now run the program.");
            }
        }

        private void OnToggleParseTree(object sender, EventArgs e)
        {
            // Toggle parse tree visibility
            outputTextBox.AppendText("\n\nتم تبديل عرض شجرة الإعراب.\nParse tree display toggled.");
        }

        private void OnToggleMipsCode(object sender, EventArgs e)
        {
            // Toggle MIPS code visibility
            outputTextBox.AppendText("\n\nتم تبديل عرض كود MIPS.\nMIPS code display toggled.");
        }

        private void OnAbout(object sender, EventArgs e)
        {
            MessageBox.Show(
                "مترجم اللغة البرمجة العربية\nArabic Programming Language Compiler\n\n" +
                "الإصدار: 1.0\nVersion: 1.0\n\n" +
                "تم تطويره لدعم البرمجة باللغة العربية\nDeveloped to support programming in Arabic\n\n" +
                "© 2024 فريق التطوير\n© 2024 Development Team",
                "حول - About",
                MessageBoxButtons.OK,
                MessageBoxIcon.Information);
        }

        #endregion

        private bool CompileCode()
        {
            try
            {
                outputTextBox.Text = "جاري الترجمة...\nCompiling...";
                Application.DoEvents();

                string sourceCode = codeEditor.SourceCode;
                
                if (string.IsNullOrWhiteSpace(sourceCode))
                {
                    outputTextBox.Text = "لا يوجد كود للترجمة.\nNo code to compile.";
                    return false;
                }

                bool success = CompilerInterface.Compile(sourceCode);
                
                if (success)
                {
                    outputTextBox.Text = "تم الترجمة بنجاح!\nCompilation successful!\n\n";
                    
                    // Get compilation results
                    string parseTree = CompilerInterface.GetParseTree();
                    string ast = CompilerInterface.GetAST();
                    string mipsCode = CompilerInterface.GetMIPSCode();
                    string symbolTable = CompilerInterface.GetSymbolTable();
                    
                    outputTextBox.AppendText("=== نتائج الترجمة ===\n");
                    outputTextBox.AppendText("=== Compilation Results ===\n\n");
                    
                    outputTextBox.AppendText("شجرة الإعراب:\nParse Tree:\n");
                    outputTextBox.AppendText(parseTree);
                    outputTextBox.AppendText("\n\n");
                    
                    outputTextBox.AppendText("شجرة البنية المجردة:\nAbstract Syntax Tree:\n");
                    outputTextBox.AppendText(ast);
                    outputTextBox.AppendText("\n\n");
                    
                    outputTextBox.AppendText("جدول الرموز:\nSymbol Table:\n");
                    outputTextBox.AppendText(symbolTable);
                    outputTextBox.AppendText("\n\n");
                    
                    outputTextBox.AppendText("كود MIPS:\nMIPS Code:\n");
                    outputTextBox.AppendText(mipsCode);
                    
                    // Update viewers
                    parseTreeViewer.SetParseTree(parseTree);
                    mipsCodeViewer.SetMIPSCode(mipsCode);
                    
                    return true;
                }
                else
                {
                    string errors = CompilerInterface.GetErrors();
                    outputTextBox.Text = $"فشلت الترجمة:\nCompilation failed:\n\n{errors}";
                    return false;
                }
            }
            catch (Exception ex)
            {
                outputTextBox.Text = $"خطأ في الترجمة:\n{ex.Message}\n\nCompilation error:\n{ex.Message}";
                return false;
            }
        }

        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                CompilerInterface.Cleanup();
            }
            catch (Exception ex)
            {
                // Log error but don't prevent closing
                System.Diagnostics.Debug.WriteLine($"Error during cleanup: {ex.Message}");
            }
        }
    }
}
