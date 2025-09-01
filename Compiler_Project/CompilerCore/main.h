#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
    #ifdef COMPILERCORE_EXPORTS
        #define COMPILERCORE_API __declspec(dllexport)
    #else
        #define COMPILERCORE_API __declspec(dllimport)
    #endif
#else
    #define COMPILERCORE_API
#endif

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

// Compiler initialization and cleanup
COMPILERCORE_API int initCompiler(void);
COMPILERCORE_API void freeCompiler(void);

// Source code compilation
COMPILERCORE_API int compileSourceCode(const wchar_t* sourceCode);

// Error handling
COMPILERCORE_API int getErrorCount(void);
COMPILERCORE_API int getErrorMessages(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int getLexicalErrors(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int getSyntaxErrors(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int getSemanticErrors(wchar_t* buffer, int bufferSize);

// Parse tree access
COMPILERCORE_API int getParseTree(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int exportParseTreeToDot(const wchar_t* filename);
COMPILERCORE_API int exportParseTreeToXML(const wchar_t* filename);
COMPILERCORE_API int exportParseTreeToJSON(const wchar_t* filename);

// AST access
COMPILERCORE_API int getAST(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int exportASTToDot(const wchar_t* filename);
COMPILERCORE_API int exportASTToXML(const wchar_t* filename);

// Symbol table access
COMPILERCORE_API int getSymbolTable(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int exportSymbolTableToCSV(const wchar_t* filename);

// MIPS code generation
COMPILERCORE_API int getMIPSCode(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int saveMIPSCodeToFile(const wchar_t* filename);

// Variable and procedure information
COMPILERCORE_API int getVariableInfo(wchar_t* buffer, int bufferSize);
COMPILERCORE_API int getProcedureInfo(wchar_t* buffer, int bufferSize);

// Compiler statistics
COMPILERCORE_API int getTokenCount(void);
COMPILERCORE_API int getParseTreeNodeCount(void);
COMPILERCORE_API int getASTNodeCount(void);
COMPILERCORE_API int getSymbolCount(void);
COMPILERCORE_API int getMIPSInstructionCount(void);

// Compiler options
COMPILERCORE_API int setCompilerOption(const wchar_t* option, const wchar_t* value);
COMPILERCORE_API int getCompilerOption(const wchar_t* option, wchar_t* buffer, int bufferSize);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H

