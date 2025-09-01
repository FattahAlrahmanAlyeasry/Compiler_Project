using System;
using System.Drawing;
using System.Windows.Forms;

namespace ArabicCompiler.GUI
{
    public partial class MipsCodeViewer : UserControl
    {
        private TextBox mipsTextBox;
        private Label titleLabel;
        private Button copyButton;
        private Button saveButton;

        public string MipsCode
        {
            get { return mipsTextBox.Text; }
            set { mipsTextBox.Text = value; }
        }

        public MipsCodeViewer()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.Dock = DockStyle.Fill;
            this.BackColor = Color.White;

            // Create title label
            titleLabel = new Label();
            titleLabel.Text = "كود MIPS - MIPS Code";
            titleLabel.Font = new Font("Tahoma", 12, FontStyle.Bold);
            titleLabel.TextAlign = ContentAlignment.MiddleCenter;
            titleLabel.Dock = DockStyle.Top;
            titleLabel.Height = 30;
            titleLabel.BackColor = Color.LightGreen;

            // Create button panel
            var buttonPanel = new Panel();
            buttonPanel.Dock = DockStyle.Top;
            buttonPanel.Height = 40;
            buttonPanel.BackColor = Color.LightGray;

            // Create copy button
            copyButton = new Button();
            copyButton.Text = "نسخ - Copy";
            copyButton.Location = new Point(10, 8);
            copyButton.Size = new Size(80, 24);
            copyButton.Click += CopyButton_Click;

            // Create save button
            saveButton = new Button();
            saveButton.Text = "حفظ - Save";
            saveButton.Location = new Point(100, 8);
            saveButton.Size = new Size(80, 24);
            saveButton.Click += SaveButton_Click;

            // Add buttons to panel
            buttonPanel.Controls.Add(copyButton);
            buttonPanel.Controls.Add(saveButton);

            // Create MIPS code display text box
            mipsTextBox = new TextBox();
            mipsTextBox.Multiline = true;
            mipsTextBox.ScrollBars = ScrollBars.Both;
            mipsTextBox.Font = new Font("Consolas", 9);
            mipsTextBox.ReadOnly = true;
            mipsTextBox.Dock = DockStyle.Fill;
            mipsTextBox.BackColor = Color.Black;
            mipsTextBox.ForeColor = Color.LightGreen;
            mipsTextBox.Text = "سيتم عرض كود MIPS هنا بعد الترجمة.\nMIPS code will be displayed here after compilation.";

            // Add controls
            this.Controls.Add(mipsTextBox);
            this.Controls.Add(buttonPanel);
            this.Controls.Add(titleLabel);
        }

        private void CopyButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (!string.IsNullOrEmpty(mipsTextBox.Text))
                {
                    Clipboard.SetText(mipsTextBox.Text);
                    MessageBox.Show("تم نسخ كود MIPS إلى الحافظة.\nMIPS code copied to clipboard.", 
                        "نسخ - Copy", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("لا يوجد كود MIPS للنسخ.\nNo MIPS code to copy.", 
                        "نسخ - Copy", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"خطأ في النسخ:\n{ex.Message}\n\nCopy error:\n{ex.Message}", 
                    "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (string.IsNullOrEmpty(mipsTextBox.Text))
                {
                    MessageBox.Show("لا يوجد كود MIPS للحفظ.\nNo MIPS code to save.", 
                        "حفظ - Save", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                using (SaveFileDialog saveDialog = new SaveFileDialog())
                {
                    saveDialog.Filter = "ملفات MIPS (*.s)|*.s|ملفات النصوص (*.txt)|*.txt|جميع الملفات (*.*)|*.*";
                    saveDialog.DefaultExt = "s";
                    saveDialog.Title = "حفظ كود MIPS - Save MIPS Code";
                    saveDialog.FileName = "program.s";

                    if (saveDialog.ShowDialog() == DialogResult.OK)
                    {
                        System.IO.File.WriteAllText(saveDialog.FileName, mipsTextBox.Text);
                        MessageBox.Show($"تم حفظ كود MIPS إلى:\n{saveDialog.FileName}\n\nMIPS code saved to:\n{saveDialog.FileName}", 
                            "حفظ - Save", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"خطأ في الحفظ:\n{ex.Message}\n\nSave error:\n{ex.Message}", 
                    "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public void SetMIPSCode(string mipsCode)
        {
            if (!string.IsNullOrEmpty(mipsCode))
            {
                mipsTextBox.Text = mipsCode;
            }
            else
            {
                mipsTextBox.Text = "لا يوجد كود MIPS لعرضه.\nNo MIPS code to display.";
            }
        }

        public void RefreshDisplay()
        {
            mipsTextBox.Refresh();
        }

        public void Clear()
        {
            mipsTextBox.Text = "كود MIPS - MIPS Code\n\nاكتب الكود واضغط على زر الترجمة لعرض كود MIPS.\nWrite code and click compile to display MIPS code.";
        }

        public void HighlightSyntax()
        {
            // Simple syntax highlighting for MIPS
            // In a full implementation, this would highlight different parts of the MIPS code
            mipsTextBox.Refresh();
        }

        public void ExportToFile(string filename)
        {
            try
            {
                if (!string.IsNullOrEmpty(mipsTextBox.Text))
                {
                    System.IO.File.WriteAllText(filename, mipsTextBox.Text);
                    MessageBox.Show($"تم تصدير كود MIPS إلى: {filename}\nMIPS code exported to: {filename}", 
                        "تصدير - Export", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("لا يوجد كود MIPS للتصدير.\nNo MIPS code to export.", 
                        "تصدير - Export", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"خطأ في التصدير:\n{ex.Message}\n\nExport error:\n{ex.Message}", 
                    "خطأ - Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
