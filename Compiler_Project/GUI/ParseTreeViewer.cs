using System;
using System.Drawing;
using System.Windows.Forms;

namespace ArabicCompiler.GUI
{
    public partial class ParseTreeViewer : UserControl
    {
        private TextBox treeTextBox;
        private Label titleLabel;

        public ParseTreeViewer()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.Dock = DockStyle.Fill;
            this.BackColor = Color.White;

            // Create title label
            titleLabel = new Label();
            titleLabel.Text = "شجرة الإعراب - Parse Tree";
            titleLabel.Font = new Font("Tahoma", 12, FontStyle.Bold);
            titleLabel.TextAlign = ContentAlignment.MiddleCenter;
            titleLabel.Dock = DockStyle.Top;
            titleLabel.Height = 30;
            titleLabel.BackColor = Color.LightBlue;

            // Create tree display text box
            treeTextBox = new TextBox();
            treeTextBox.Multiline = true;
            treeTextBox.ScrollBars = ScrollBars.Both;
            treeTextBox.Font = new Font("Consolas", 9);
            treeTextBox.ReadOnly = true;
            treeTextBox.Dock = DockStyle.Fill;
            treeTextBox.BackColor = Color.White;
            treeTextBox.ForeColor = Color.Black;
            treeTextBox.Text = "سيتم عرض شجرة الإعراب هنا بعد الترجمة.\nParse tree will be displayed here after compilation.";

            // Add controls
            this.Controls.Add(treeTextBox);
            this.Controls.Add(titleLabel);
        }

        public void SetParseTree(string parseTree)
        {
            if (!string.IsNullOrEmpty(parseTree))
            {
                treeTextBox.Text = parseTree;
            }
            else
            {
                treeTextBox.Text = "لا توجد شجرة إعراب لعرضها.\nNo parse tree to display.";
            }
        }

        public void RefreshTreeDisplay()
        {
            treeTextBox.Refresh();
        }

        public void ResetView()
        {
            treeTextBox.Text = "شجرة الإعراب - Parse Tree\n\nاكتب الكود واضغط على زر الترجمة لعرض شجرة الإعراب.\nWrite code and click compile to display the parse tree.";
        }

        public void CenterOnNode(string nodeName)
        {
            if (!string.IsNullOrEmpty(nodeName))
            {
                int index = treeTextBox.Text.IndexOf(nodeName);
                if (index >= 0)
                {
                    treeTextBox.Select(index, nodeName.Length);
                    treeTextBox.ScrollToCaret();
                }
            }
        }

        public void ExportToDot(string filename)
        {
            try
            {
                System.IO.File.WriteAllText(filename, treeTextBox.Text);
                MessageBox.Show($"تم تصدير شجرة الإعراب إلى: {filename}\nParse tree exported to: {filename}", 
                    "تصدير - Export", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"خطأ في التصدير:\n{ex.Message}\n\nExport error:\n{ex.Message}", 
                    "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
