#include "lexer.h"
#include <locale.h>

// الكلمات المحجوزة
const wchar_t* reserved_words[] = {
    L"برنامج", L"إذا", L"وإلا", L"وإلا إذا", L"طالما", L"كرر", L"حتى",
    L"لكل", L"اطبع", L"اقرأ", L"أرجع", L"اكسر", L"استمر",
    L"صحيح", L"حقيقي", L"حرف", L"منطقي", L"رمزي", L"قائمة", L"سجل"
};

const int reserved_words_count = 20;

// الرموز الخاصة
const wchar_t* special_symbols[] = {
    L"،", L"؛", L":", L"=", L"<", L">", L"==", L"!=", L"<=", L">=",
    L"+", L"-", L"*", L"/", L"^", L"%", L"\\", L"&&", L"||", L"!",
    L"(", L")", L"[", L"]", L"{", L"}", L"\"", L"'", L"."
};

const int special_symbols_count = 29;

// تهيئة المحلل المعجمي
Lexer* initLexer(const char* filename) {
    setlocale(LC_ALL, "en_US.UTF-8");
    
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    if (!lexer) {
        return NULL;
    }
    
    lexer->source = fopen(filename, "r,ccs=UTF-8");
    if (!lexer->source) {
        free(lexer);
        return NULL;
    }
    
    lexer->current_line = NULL;
    lexer->line_number = 1;
    lexer->column_number = 1;
    lexer->current_pos = 0;
    lexer->line_length = 0;
    
    return lexer;
}

// تحرير ذاكرة المحلل المعجمي
void freeLexer(Lexer* lexer) {
    if (lexer) {
        if (lexer->source) {
            fclose(lexer->source);
        }
        if (lexer->current_line) {
            free(lexer->current_line);
        }
        free(lexer);
    }
}

// قراءة السطر التالي
static int readNextLine(Lexer* lexer) {
    if (lexer->current_line) {
        free(lexer->current_line);
    }
    
    lexer->current_line = NULL;
    lexer->current_pos = 0;
    lexer->column_number = 1;
    
    wchar_t buffer[1024];
    if (fgetws(buffer, 1024, lexer->source)) {
        lexer->line_length = wcslen(buffer);
        lexer->current_line = (wchar_t*)malloc((lexer->line_length + 1) * sizeof(wchar_t));
        if (lexer->current_line) {
            wcscpy(lexer->current_line, buffer);
            return 1;
        }
    }
    
    return 0;
}

// تجاهل الفراغات
static void skipWhitespace(Lexer* lexer) {
    while (lexer->current_pos < lexer->line_length && 
           isWhitespace(lexer->current_line[lexer->current_pos])) {
        lexer->current_pos++;
        lexer->column_number++;
    }
}

// قراءة معرف
static Token* readIdentifier(Lexer* lexer) {
    int start_pos = lexer->current_pos;
    int start_col = lexer->column_number;
    
    while (lexer->current_pos < lexer->line_length && 
           isIdentifierPart(lexer->current_line[lexer->current_pos])) {
        lexer->current_pos++;
        lexer->column_number++;
    }
    
    int length = lexer->current_pos - start_pos;
    wchar_t* value = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
    wcsncpy(value, lexer->current_line + start_pos, length);
    value[length] = L'\0';
    
    Token* token = (Token*)malloc(sizeof(Token));
    token->line = lexer->line_number;
    token->column = start_col;
    token->value = value;
    
    // التحقق من الكلمات المحجوزة
    if (isReservedWord(value)) {
        if (wcscmp(value, L"برنامج") == 0) token->type = TOKEN_PROGRAM;
        else if (wcscmp(value, L"إذا") == 0) token->type = TOKEN_IF;
        else if (wcscmp(value, L"وإلا") == 0) token->type = TOKEN_ELSE;
        else if (wcscmp(value, L"وإلا إذا") == 0) token->type = TOKEN_ELSEIF;
        else if (wcscmp(value, L"طالما") == 0) token->type = TOKEN_WHILE;
        else if (wcscmp(value, L"كرر") == 0) token->type = TOKEN_REPEAT;
        else if (wcscmp(value, L"حتى") == 0) token->type = TOKEN_UNTIL;
        else if (wcscmp(value, L"لكل") == 0) token->type = TOKEN_FOR;
        else if (wcscmp(value, L"اطبع") == 0) token->type = TOKEN_PRINT;
        else if (wcscmp(value, L"اقرأ") == 0) token->type = TOKEN_READ;
        else if (wcscmp(value, L"أرجع") == 0) token->type = TOKEN_RETURN;
        else if (wcscmp(value, L"اكسر") == 0) token->type = TOKEN_BREAK;
        else if (wcscmp(value, L"استمر") == 0) token->type = TOKEN_CONTINUE;
        else if (wcscmp(value, L"صحيح") == 0) token->type = TOKEN_INT;
        else if (wcscmp(value, L"حقيقي") == 0) token->type = TOKEN_FLOAT;
        else if (wcscmp(value, L"حرف") == 0) token->type = TOKEN_CHAR;
        else if (wcscmp(value, L"منطقي") == 0) token->type = TOKEN_BOOL;
        else if (wcscmp(value, L"رمزي") == 0) token->type = TOKEN_STRING;
        else if (wcscmp(value, L"قائمة") == 0) token->type = TOKEN_LIST;
        else if (wcscmp(value, L"سجل") == 0) token->type = TOKEN_RECORD;
        else token->type = TOKEN_IDENTIFIER;
    } else {
        token->type = TOKEN_IDENTIFIER;
    }
    
    return token;
}

// قراءة ثابت رقمي
static Token* readNumber(Lexer* lexer) {
    int start_pos = lexer->current_pos;
    int start_col = lexer->column_number;
    int is_float = 0;
    
    // قراءة الأرقام
    while (lexer->current_pos < lexer->line_length && 
           isDigit(lexer->current_line[lexer->current_pos])) {
        lexer->current_pos++;
        lexer->column_number++;
    }
    
    // التحقق من النقطة العشرية
    if (lexer->current_pos < lexer->line_length && 
        lexer->current_line[lexer->current_pos] == L'.') {
        is_float = 1;
        lexer->current_pos++;
        lexer->column_number++;
        
        while (lexer->current_pos < lexer->line_length && 
               isDigit(lexer->current_line[lexer->current_pos])) {
            lexer->current_pos++;
            lexer->column_number++;
        }
    }
    
    int length = lexer->current_pos - start_pos;
    wchar_t* value = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
    wcsncpy(value, lexer->current_line + start_pos, length);
    value[length] = L'\0';
    
    Token* token = (Token*)malloc(sizeof(Token));
    token->line = lexer->line_number;
    token->column = start_col;
    token->value = value;
    token->type = is_float ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL;
    
    return token;
}

// قراءة ثابت نصي
static Token* readString(Lexer* lexer) {
    wchar_t quote = lexer->current_line[lexer->current_pos];
    lexer->current_pos++;
    lexer->column_number++;
    
    int start_pos = lexer->current_pos;
    int start_col = lexer->column_number;
    
    while (lexer->current_pos < lexer->line_length && 
           lexer->current_line[lexer->current_pos] != quote) {
        lexer->current_pos++;
        lexer->column_number++;
    }
    
    if (lexer->current_pos >= lexer->line_length) {
        lexerError(lexer, L"نص غير مكتمل");
        return NULL;
    }
    
    int length = lexer->current_pos - start_pos;
    wchar_t* value = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
    wcsncpy(value, lexer->current_line + start_pos, length);
    value[length] = L'\0';
    
    lexer->current_pos++; // تخطي علامة الاقتباس
    lexer->column_number++;
    
    Token* token = (Token*)malloc(sizeof(Token));
    token->line = lexer->line_number;
    token->column = start_col;
    token->value = value;
    token->type = (quote == L'"') ? TOKEN_STRING_LITERAL : TOKEN_CHAR_LITERAL;
    
    return token;
}

// قراءة الرمز التالي
Token* getNextToken(Lexer* lexer) {
    if (!lexer || !lexer->source) {
        return NULL;
    }
    
    // قراءة سطر جديد إذا لزم الأمر
    if (!lexer->current_line || lexer->current_pos >= lexer->line_length) {
        if (!readNextLine(lexer)) {
            // إنشاء رمز نهاية الملف
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_EOF;
            token->value = NULL;
            token->line = lexer->line_number;
            token->column = lexer->column_number;
            return token;
        }
    }
    
    // تجاهل الفراغات
    skipWhitespace(lexer);
    
    // التحقق من نهاية السطر
    if (lexer->current_pos >= lexer->line_length) {
        lexer->line_number++;
        return getNextToken(lexer);
    }
    
    wchar_t current_char = lexer->current_line[lexer->current_pos];
    
    // قراءة المعرفات
    if (isIdentifierStart(current_char)) {
        return readIdentifier(lexer);
    }
    
    // قراءة الأرقام
    if (isDigit(current_char)) {
        return readNumber(lexer);
    }
    
    // قراءة النصوص
    if (current_char == L'"' || current_char == L'\'') {
        return readString(lexer);
    }
    
    // قراءة الرموز الخاصة
    if (current_char == L'=') {
        lexer->current_pos++;
        lexer->column_number++;
        
        if (lexer->current_pos < lexer->line_length && 
            lexer->current_line[lexer->current_pos] == L'=') {
            lexer->current_pos++;
            lexer->column_number++;
            
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_EQ;
            token->value = wcsdup(L"==");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 2;
            return token;
        } else {
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_ASSIGN;
            token->value = wcsdup(L"=");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 1;
            return token;
        }
    }
    
    if (current_char == L'!') {
        lexer->current_pos++;
        lexer->column_number++;
        
        if (lexer->current_pos < lexer->line_length && 
            lexer->current_line[lexer->current_pos] == L'=') {
            lexer->current_pos++;
            lexer->column_number++;
            
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_NE;
            token->value = wcsdup(L"!=");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 2;
            return token;
        } else {
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_NOT;
            token->value = wcsdup(L"!");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 1;
            return token;
        }
    }
    
    if (current_char == L'<') {
        lexer->current_pos++;
        lexer->column_number++;
        
        if (lexer->current_pos < lexer->line_length && 
            lexer->current_line[lexer->current_pos] == L'=') {
            lexer->current_pos++;
            lexer->column_number++;
            
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_LE;
            token->value = wcsdup(L"<=");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 2;
            return token;
        } else {
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_LT;
            token->value = wcsdup(L"<");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 1;
            return token;
        }
    }
    
    if (current_char == L'>') {
        lexer->current_pos++;
        lexer->column_number++;
        
        if (lexer->current_pos < lexer->line_length && 
            lexer->current_line[lexer->current_pos] == L'=') {
            lexer->current_pos++;
            lexer->column_number++;
            
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_GE;
            token->value = wcsdup(L">=");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 2;
            return token;
        } else {
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_GT;
            token->value = wcsdup(L">");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 1;
            return token;
        }
    }
    
    if (current_char == L'&') {
        lexer->current_pos++;
        lexer->column_number++;
        
        if (lexer->current_pos < lexer->line_length && 
            lexer->current_line[lexer->current_pos] == L'&') {
            lexer->current_pos++;
            lexer->column_number++;
            
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_AND;
            token->value = wcsdup(L"&&");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 2;
            return token;
        } else {
            lexerError(lexer, L"رمز غير صحيح: &");
            return NULL;
        }
    }
    
    if (current_char == L'|') {
        lexer->current_pos++;
        lexer->column_number++;
        
        if (lexer->current_pos < lexer->line_length && 
            lexer->current_line[lexer->current_pos] == L'|') {
            lexer->current_pos++;
            lexer->column_number++;
            
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_OR;
            token->value = wcsdup(L"||");
            token->line = lexer->line_number;
            token->column = lexer->column_number - 2;
            return token;
        } else {
            lexerError(lexer, L"رمز غير صحيح: |");
            return NULL;
        }
    }
    
    // الرموز المفردة
    Token* token = (Token*)malloc(sizeof(Token));
    token->line = lexer->line_number;
    token->column = lexer->column_number;
    
    switch (current_char) {
        case L'+':
            token->type = TOKEN_PLUS;
            token->value = wcsdup(L"+");
            break;
        case L'-':
            token->type = TOKEN_MINUS;
            token->value = wcsdup(L"-");
            break;
        case L'*':
            token->type = TOKEN_MULTIPLY;
            token->value = wcsdup(L"*");
            break;
        case L'/':
            token->type = TOKEN_DIVIDE;
            token->value = wcsdup(L"/");
            break;
        case L'^':
            token->type = TOKEN_POWER;
            token->value = wcsdup(L"^");
            break;
        case L'%':
            token->type = TOKEN_MODULO;
            token->value = wcsdup(L"%");
            break;
        case L'\\':
            token->type = TOKEN_BACKSLASH;
            token->value = wcsdup(L"\\");
            break;
        case L'(':
            token->type = TOKEN_LPAREN;
            token->value = wcsdup(L"(");
            break;
        case L')':
            token->type = TOKEN_RPAREN;
            token->value = wcsdup(L")");
            break;
        case L'[':
            token->type = TOKEN_LBRACKET;
            token->value = wcsdup(L"[");
            break;
        case L']':
            token->type = TOKEN_RBRACKET;
            token->value = wcsdup(L"]");
            break;
        case L'{':
            token->type = TOKEN_LBRACE;
            token->value = wcsdup(L"{");
            break;
        case L'}':
            token->type = TOKEN_RBRACE;
            token->value = wcsdup(L"}");
            break;
        case L'،':
            token->type = TOKEN_COMMA;
            token->value = wcsdup(L"،");
            break;
        case L'؛':
            token->type = TOKEN_SEMICOLON;
            token->value = wcsdup(L"؛");
            break;
        case L':':
            token->type = TOKEN_COLON;
            token->value = wcsdup(L":");
            break;
        case L'.':
            token->type = TOKEN_DOT;
            token->value = wcsdup(L".");
            break;
        default:
            lexerError(lexer, L"رمز غير معروف");
            free(token);
            return NULL;
    }
    
    lexer->current_pos++;
    lexer->column_number++;
    
    return token;
}

// النظر إلى الرمز التالي بدون استهلاكه
Token* peekNextToken(Lexer* lexer) {
    if (!lexer) {
        return NULL;
    }
    
    // حفظ الموقع الحالي
    int saved_pos = lexer->current_pos;
    int saved_col = lexer->column_number;
    int saved_line = lexer->line_number;
    
    // قراءة الرمز التالي
    Token* token = getNextToken(lexer);
    
    // إعادة الموقع
    lexer->current_pos = saved_pos;
    lexer->column_number = saved_col;
    lexer->line_number = saved_line;
    
    return token;
}

// الإبلاغ عن الأخطاء المعجمية
void lexerError(Lexer* lexer, const wchar_t* message) {
    if (lexer) {
        fwprintf(stderr, L"خطأ معجمي في السطر %d، العمود %d: %ls\n", 
                lexer->line_number, lexer->column_number, message);
    }
}

// تحرير ذاكرة الرمز
void freeToken(Token* token) {
    if (token) {
        if (token->value) {
            free(token->value);
        }
        free(token);
    }
}

// دوال مساعدة
int isReservedWord(const wchar_t* word) {
    for (int i = 0; i < reserved_words_count; i++) {
        if (wcscmp(word, reserved_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isSpecialSymbol(const wchar_t* symbol) {
    for (int i = 0; i < special_symbols_count; i++) {
        if (wcscmp(symbol, special_symbols[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isWhitespace(wchar_t ch) {
    return ch == L' ' || ch == L'\t' || ch == L'\r' || ch == L'\n';
}

int isDigit(wchar_t ch) {
    return ch >= L'0' && ch <= L'9';
}

int isLetter(wchar_t ch) {
    return (ch >= L'A' && ch <= L'Z') || 
           (ch >= L'a' && ch <= L'z') ||
           (ch >= L'ء' && ch <= L'ي'); // الحروف العربية
}

int isIdentifierStart(wchar_t ch) {
    return isLetter(ch) || ch == L'_';
}

int isIdentifierPart(wchar_t ch) {
    return isLetter(ch) || isDigit(ch) || ch == L'_';
}

