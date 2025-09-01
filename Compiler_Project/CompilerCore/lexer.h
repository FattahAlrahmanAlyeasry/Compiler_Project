#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// أنواع الرموز (Tokens)
typedef enum {
    // الكلمات المحجوزة
    TOKEN_PROGRAM,      // برنامج
    TOKEN_IF,           // إذا
    TOKEN_ELSE,         // وإلا
    TOKEN_ELSEIF,       // وإلا إذا
    TOKEN_WHILE,        // طالما
    TOKEN_REPEAT,       // كرر
    TOKEN_UNTIL,        // حتى
    TOKEN_FOR,          // لكل
    TOKEN_PRINT,        // اطبع
    TOKEN_READ,         // اقرأ
    TOKEN_RETURN,       // أرجع
    TOKEN_BREAK,        // اكسر
    TOKEN_CONTINUE,     // استمر
    
    // أنواع البيانات
    TOKEN_INT,          // صحيح
    TOKEN_FLOAT,        // حقيقي
    TOKEN_CHAR,         // حرف
    TOKEN_BOOL,         // منطقي
    TOKEN_STRING,       // رمزي
    TOKEN_LIST,         // قائمة
    TOKEN_RECORD,       // سجل
    
    // الرموز الخاصة
    TOKEN_COMMA,        // ،
    TOKEN_SEMICOLON,    // ؛
    TOKEN_COLON,        // :
    TOKEN_ASSIGN,       // =
    TOKEN_LT,           // <
    TOKEN_GT,           // >
    TOKEN_EQ,           // ==
    TOKEN_NE,           // !=
    TOKEN_LE,           // <=
    TOKEN_GE,           // >=
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_MULTIPLY,     // *
    TOKEN_DIVIDE,       // /
    TOKEN_POWER,        // ^
    TOKEN_MODULO,       // %
    TOKEN_BACKSLASH,    // \
    TOKEN_AND,          // &&
    TOKEN_OR,           // ||
    TOKEN_NOT,          // !
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACKET,     // [
    TOKEN_RBRACKET,     // ]
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_QUOTE,        // "
    TOKEN_SQUOTE,       // '
    TOKEN_DOT,          // .
    
    // أنواع أخرى
    TOKEN_IDENTIFIER,   // معرف
    TOKEN_INT_LITERAL,  // ثابت صحيح
    TOKEN_FLOAT_LITERAL, // ثابت حقيقي
    TOKEN_CHAR_LITERAL, // ثابت حرفي
    TOKEN_STRING_LITERAL, // ثابت رمزي
    TOKEN_BOOL_LITERAL, // ثابت منطقي
    TOKEN_EOF,          // نهاية الملف
    TOKEN_ERROR         // خطأ
} TokenType;

// هيكل الرمز
typedef struct {
    TokenType type;
    wchar_t* value;
    int line;
    int column;
} Token;

// هيكل المحلل المعجمي
typedef struct {
    FILE* source;
    wchar_t* current_line;
    int line_number;
    int column_number;
    int current_pos;
    int line_length;
} Lexer;

// الكلمات المحجوزة
extern const wchar_t* reserved_words[];
extern const int reserved_words_count;

// الرموز الخاصة
extern const wchar_t* special_symbols[];
extern const int special_symbols_count;

// دوال المحلل المعجمي
Lexer* initLexer(const char* filename);
void freeLexer(Lexer* lexer);
Token* getNextToken(Lexer* lexer);
Token* peekNextToken(Lexer* lexer);
void lexerError(Lexer* lexer, const wchar_t* message);
void freeToken(Token* token);

// دوال مساعدة
int isReservedWord(const wchar_t* word);
int isSpecialSymbol(const wchar_t* symbol);
int isWhitespace(wchar_t ch);
int isDigit(wchar_t ch);
int isLetter(wchar_t ch);
int isIdentifierStart(wchar_t ch);
int isIdentifierPart(wchar_t ch);

#endif // LEXER_H

