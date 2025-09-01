using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace ArabicCompiler.GUI
{
    public partial class CodeEditor : UserControl
    {
        private TextBox textBox;
        private Panel lineNumberPanel;
        private bool showLineNumbers = true;
        private Font codeFont;

        public string SourceCode
        {
            get { return textBox.Text; }
            set { textBox.Text = value; }
        }

        public CodeEditor()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.Dock = DockStyle.Fill;
            this.BackColor = Color.White;

            // Create line number panel
            lineNumberPanel = new Panel();
            lineNumberPanel.Width = 50;
            lineNumberPanel.Dock = DockStyle.Left;
            lineNumberPanel.BackColor = Color.LightGray;
            lineNumberPanel.BorderStyle = BorderStyle.FixedSingle;

            // Create main text box
            textBox = new TextBox();
            textBox.Multiline = true;
            textBox.ScrollBars = ScrollBars.Both;
            textBox.Font = new Font("Tahoma", 10);
            textBox.AcceptsTab = true;
            textBox.WordWrap = false;
            textBox.Dock = DockStyle.Fill;
            textBox.BackColor = Color.White;
            textBox.ForeColor = Color.Black;

            // Set up event handlers
            textBox.TextChanged += TextBox_TextChanged;
            textBox.KeyDown += TextBox_KeyDown;

            // Add controls
            this.Controls.Add(textBox);
            this.Controls.Add(lineNumberPanel);

            // Set initial text
            textBox.Text = "اكتب الكود العربي هنا...\nWrite Arabic code here...";
        }

        private void TextBox_TextChanged(object sender, EventArgs e)
        {
            if (showLineNumbers)
            {
                UpdateLineNumbers();
            }
        }

        private void TextBox_KeyDown(object sender, KeyEventArgs e)
        {
            // Handle Tab key
            if (e.KeyCode == Keys.Tab)
            {
                e.SuppressKeyPress = true;
                textBox.SelectedText = "    "; // Insert 4 spaces
            }
        }

        private void UpdateLineNumbers()
        {
            // Simple line number display
            // In a full implementation, this would draw line numbers on the panel
        }

        public void Clear()
        {
            textBox.Clear();
        }

        public void LoadFromFile(string filename)
        {
            try
            {
                textBox.Text = File.ReadAllText(filename, System.Text.Encoding.UTF8);
            }
            catch (Exception ex)
            {
                throw new Exception($"Failed to load file: {ex.Message}");
            }
        }

        public void SaveToFile(string filename)
        {
            try
            {
                File.WriteAllText(filename, textBox.Text, System.Text.Encoding.UTF8);
            }
            catch (Exception ex)
            {
                throw new Exception($"Failed to save file: {ex.Message}");
            }
        }

        public void Cut()
        {
            textBox.Cut();
        }

        public void Copy()
        {
            textBox.Copy();
        }

        public void Paste()
        {
            textBox.Paste();
        }

        public bool FindText(string searchText)
        {
            int index = textBox.Text.IndexOf(searchText);
            if (index >= 0)
            {
                textBox.Select(index, searchText.Length);
                textBox.ScrollToCaret();
                return true;
            }
            return false;
        }

        public int ReplaceText(string searchText, string replaceText)
        {
            int count = 0;
            string text = textBox.Text;
            while (text.Contains(searchText))
            {
                text = text.Replace(searchText, replaceText);
                count++;
            }
            textBox.Text = text;
            return count;
        }

        public void GoToLine(int lineNumber)
        {
            string[] lines = textBox.Text.Split('\n');
            if (lineNumber > 0 && lineNumber <= lines.Length)
            {
                int startIndex = 0;
                for (int i = 0; i < lineNumber - 1; i++)
                {
                    startIndex += lines[i].Length + 1; // +1 for newline
                }
                textBox.Select(startIndex, 0);
                textBox.ScrollToCaret();
            }
        }
    }
}
