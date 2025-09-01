using System;
using System.Runtime.InteropServices;
using System.Text;

namespace ArabicCompiler.GUI
{
    public static class CompilerInterface
    {
        // P/Invoke declarations for C functions
        [DllImport("CompilerCore.dll", CharSet = CharSet.Unicode)]
        private static extern int compileSource([MarshalAs(UnmanagedType.LPWStr)] string sourceCode);

        [DllImport("CompilerCore.dll", CharSet = CharSet.Unicode)]
        private static extern int getParseTree([MarshalAs(UnmanagedType.LPWStr)] StringBuilder buffer, int bufferSize);

        [DllImport("CompilerCore.dll", CharSet = CharSet.Unicode)]
        private static extern int getAST([MarshalAs(UnmanagedType.LPWStr)] StringBuilder buffer, int bufferSize);

        [DllImport("CompilerCore.dll", CharSet = CharSet.Unicode)]
        private static extern int getMIPSCode([MarshalAs(UnmanagedType.LPWStr)] StringBuilder buffer, int bufferSize);

        [DllImport("CompilerCore.dll", CharSet = CharSet.Unicode)]
        private static extern int getSymbolTable([MarshalAs(UnmanagedType.LPWStr)] StringBuilder buffer, int bufferSize);

        [DllImport("CompilerCore.dll", CharSet = CharSet.Unicode)]
        private static extern int getErrors([MarshalAs(UnmanagedType.LPWStr)] StringBuilder buffer, int bufferSize);

        [DllImport("CompilerCore.dll")]
        private static extern int initializeCompiler();

        [DllImport("CompilerCore.dll")]
        private static extern int cleanupCompiler();

        // Static fields to store compilation results
        private static string lastParseTree = "";
        private static string lastAST = "";
        private static string lastMIPSCode = "";
        private static string lastSymbolTable = "";
        private static string lastErrors = "";
        private static bool isInitialized = false;

        public static bool Initialize()
        {
            try
            {
                // For now, simulate initialization since we don't have the actual DLL
                isInitialized = true;
                return true;
            }
            catch (Exception)
            {
                isInitialized = false;
                return false;
            }
        }

        public static void Cleanup()
        {
            try
            {
                if (isInitialized)
                {
                    // Cleanup code here
                    isInitialized = false;
                }
            }
            catch (Exception)
            {
                // Ignore cleanup errors
            }
        }

        public static bool Compile(string sourceCode)
        {
            if (!isInitialized)
            {
                return false;
            }

            try
            {
                // Simulate compilation for demonstration
                // In a real implementation, this would call the C compiler
                
                // Generate sample parse tree
                lastParseTree = GenerateSampleParseTree(sourceCode);
                
                // Generate sample AST
                lastAST = GenerateSampleAST(sourceCode);
                
                // Generate sample MIPS code
                lastMIPSCode = GenerateSampleMIPSCode(sourceCode);
                
                // Generate sample symbol table
                lastSymbolTable = GenerateSampleSymbolTable(sourceCode);
                
                // Clear errors (successful compilation)
                lastErrors = "";
                
                return true;
            }
            catch (Exception)
            {
                lastErrors = "خطأ في الترجمة - Compilation error";
                return false;
            }
        }

        public static string GetParseTree()
        {
            return lastParseTree;
        }

        public static string GetAST()
        {
            return lastAST;
        }

        public static string GetMIPSCode()
        {
            return lastMIPSCode;
        }

        public static string GetSymbolTable()
        {
            return lastSymbolTable;
        }

        public static string GetErrors()
        {
            return lastErrors;
        }

        // Sample generation methods for demonstration
        private static string GenerateSampleParseTree(string sourceCode)
        {
            return @"شجرة الإعراب - Parse Tree:
برنامج (PROGRAM)
├── [ (BLOCK_START)
│   ├── متغير (VARIABLE_DEFINITION)
│   │   └── صحيح (TYPE)
│   └── ] (BLOCK_END)
├── : (BLOCK_SEPARATOR)
├── اطبع (PRINT_STATEMENT)
│   └── ""مرحباً بالعالم!"" (STRING_LITERAL)
├── متغير (IDENTIFIER)
│   └── = (ASSIGNMENT)
│       └── 42 (NUMBER_LITERAL)
├── اطبع (PRINT_STATEMENT)
│   └── متغير (IDENTIFIER)
└── ؛ (STATEMENT_END)";
        }

        private static string GenerateSampleAST(string sourceCode)
        {
            return @"شجرة البنية المجردة - Abstract Syntax Tree:
Program
├── Block
│   ├── VariableDefinition
│   │   ├── Name: متغير
│   │   └── Type: INTEGER
│   └── Statements
│       ├── PrintStatement
│       │   └── StringLiteral: ""مرحباً بالعالم!""
│       ├── AssignmentStatement
│       │   ├── Variable: متغير
│       │   └── Value: 42
│       └── PrintStatement
│           └── Variable: متغير";
        }

        private static string GenerateSampleMIPSCode(string sourceCode)
        {
            return @"# MIPS Assembly Code
# Generated by Arabic Compiler

.text
.globl main

main:
    # Initialize variables
    li $t0, 42          # متغير = 42
    
    # Load string address
    la $a0, msg_hello    # رسالة = ""مرحباً بالعالم""
    
    # Print string
    li $v0, 4            # syscall 4: print string
    syscall
    
    # Print variable
    move $a0, $t0        # Load variable value
    li $v0, 1            # syscall 1: print integer
    syscall
    
    # Exit program
    li $v0, 10           # syscall 10: exit
    syscall

.data
msg_hello: .asciiz ""مرحباً بالعالم\n""";
        }

        private static string GenerateSampleSymbolTable(string sourceCode)
        {
            return @"جدول الرموز - Symbol Table:
النطاق: GLOBAL
├── متغير (VARIABLE)
│   ├── النوع: INTEGER
│   ├── القيمة: 42
│   └── العنوان: $t0

النطاق: MAIN
├── msg_hello (LABEL)
│   ├── النوع: STRING
│   └── القيمة: ""مرحباً بالعالم\n""";
        }
    }
}
