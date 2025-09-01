#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../lexer.h"

// Test function declarations
void test_initLexer();
void test_getNextToken();
void test_peekNextToken();
void test_lexerError();
void test_arabic_keywords();
void test_operators();
void test_identifiers();
void test_literals();
void test_comments();
void test_whitespace();
void test_unicode_support();

int main() {
    printf("=== Testing Lexical Analyzer ===\n\n");
    
    test_initLexer();
    test_getNextToken();
    test_peekNextToken();
    test_lexerError();
    test_arabic_keywords();
    test_operators();
    test_identifiers();
    test_literals();
    test_comments();
    test_whitespace();
    test_unicode_support();
    
    printf("\n=== All Lexical Analyzer Tests Passed! ===\n");
    return 0;
}

void test_initLexer() {
    printf("Testing initLexer...\n");
    
    Lexer lexer;
    int result = initLexer(&lexer);
    
    assert(result == 1);
    assert(lexer.source != NULL);
    assert(lexer.current_pos == 0);
    assert(lexer.line_number == 1);
    assert(lexer.column_number == 1);
    
    printf("✓ initLexer test passed\n");
}

void test_getNextToken() {
    printf("Testing getNextToken...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    // Test with simple Arabic program
    const wchar_t* source = L"برنامج متغير صحيح";
    setSource(&lexer, source);
    
    Token token = getNextToken(&lexer);
    assert(token.type == TOKEN_KEYWORD_PROGRAM);
    assert(wcscmp(token.value, L"برنامج") == 0);
    
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_KEYWORD_VARIABLE);
    assert(wcscmp(token.value, L"متغير") == 0);
    
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_KEYWORD_INTEGER);
    assert(wcscmp(token.value, L"صحيح") == 0);
    
    printf("✓ getNextToken test passed\n");
}

void test_peekNextToken() {
    printf("Testing peekNextToken...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    const wchar_t* source = L"اطبع 42";
    setSource(&lexer, source);
    
    Token peeked = peekNextToken(&lexer);
    assert(peeked.type == TOKEN_KEYWORD_PRINT);
    
    Token actual = getNextToken(&lexer);
    assert(actual.type == TOKEN_KEYWORD_PRINT);
    
    // Position should not change after peek
    assert(lexer.current_pos == 0);
    
    printf("✓ peekNextToken test passed\n");
}

void test_lexerError() {
    printf("Testing lexerError...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    lexerError(&lexer, L"Test error message");
    
    assert(lexer.error_count > 0);
    assert(lexer.last_error != NULL);
    
    printf("✓ lexerError test passed\n");
}

void test_arabic_keywords() {
    printf("Testing Arabic keywords...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    const wchar_t* keywords[] = {
        L"برنامج", L"متغير", L"صحيح", L"حقيقي", L"حرف", L"منطقي",
        L"إذا", L"وإلا", L"وإلا إذا", L"طالما", L"كرر", L"حتى",
        L"اطبع", L"اقرأ", L"إرجاع", L"إجراء", L"نوع", L"ثابت"
    };
    
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        setSource(&lexer, keywords[i]);
        Token token = getNextToken(&lexer);
        assert(token.type >= TOKEN_KEYWORD_PROGRAM && token.type <= TOKEN_KEYWORD_CONSTANT);
        assert(wcscmp(token.value, keywords[i]) == 0);
    }
    
    printf("✓ Arabic keywords test passed\n");
}

void test_operators() {
    printf("Testing operators...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    const wchar_t* operators[] = {
        L"+", L"-", L"*", L"/", L"^", L"%", L"\\",
        L"=", L"==", L"!=", L"<", L">", L"<=", L">=",
        L"&&", L"||", L"!", L"(", L")", L"[", L"]", L"{", L"}",
        L",", L";", L":", L".", L"\"", L"'"
    };
    
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        setSource(&lexer, operators[i]);
        Token token = getNextToken(&lexer);
        assert(token.type >= TOKEN_OPERATOR_PLUS && token.type <= TOKEN_OPERATOR_QUOTE);
        assert(wcscmp(token.value, operators[i]) == 0);
    }
    
    printf("✓ Operators test passed\n");
}

void test_identifiers() {
    printf("Testing identifiers...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    const wchar_t* identifiers[] = {
        L"متغير_1", L"متغير2", L"_متغير", L"متغير_متغير"
    };
    
    for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); i++) {
        setSource(&lexer, identifiers[i]);
        Token token = getNextToken(&lexer);
        assert(token.type == TOKEN_IDENTIFIER);
        assert(wcscmp(token.value, identifiers[i]) == 0);
    }
    
    printf("✓ Identifiers test passed\n");
}

void test_literals() {
    printf("Testing literals...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    // Test integer literals
    setSource(&lexer, L"123 456 789");
    Token token = getNextToken(&lexer);
    assert(token.type == TOKEN_LITERAL_INTEGER);
    assert(wcscmp(token.value, L"123") == 0);
    
    // Test float literals
    setSource(&lexer, L"3.14 2.718");
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_LITERAL_FLOAT);
    assert(wcscmp(token.value, L"3.14") == 0);
    
    // Test string literals
    setSource(&lexer, L"\"مرحبا بالعالم\"");
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_LITERAL_STRING);
    assert(wcscmp(token.value, L"مرحبا بالعالم") == 0);
    
    // Test character literals
    setSource(&lexer, L"'أ' 'ب'");
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_LITERAL_CHAR);
    assert(wcscmp(token.value, L"أ") == 0);
    
    printf("✓ Literals test passed\n");
}

void test_comments() {
    printf("Testing comments...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    const wchar_t* source = L"// هذا تعليق\nبرنامج";
    setSource(&lexer, source);
    
    Token token = getNextToken(&lexer);
    assert(token.type == TOKEN_KEYWORD_PROGRAM);
    assert(wcscmp(token.value, L"برنامج") == 0);
    
    printf("✓ Comments test passed\n");
}

void test_whitespace() {
    printf("Testing whitespace handling...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    const wchar_t* source = L"  \t  \n  برنامج  ";
    setSource(&lexer, source);
    
    Token token = getNextToken(&lexer);
    assert(token.type == TOKEN_KEYWORD_PROGRAM);
    assert(wcscmp(token.value, L"برنامج") == 0);
    
    printf("✓ Whitespace test passed\n");
}

void test_unicode_support() {
    printf("Testing Unicode support...\n");
    
    Lexer lexer;
    initLexer(&lexer);
    
    // Test Arabic text with mixed characters
    const wchar_t* source = L"متغير صحيح = 42; // تعليق بالعربية";
    setSource(&lexer, source);
    
    Token token = getNextToken(&lexer);
    assert(token.type == TOKEN_KEYWORD_VARIABLE);
    
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_KEYWORD_INTEGER);
    
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_OPERATOR_ASSIGN);
    
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_LITERAL_INTEGER);
    
    token = getNextToken(&lexer);
    assert(token.type == TOKEN_OPERATOR_SEMICOLON);
    
    printf("✓ Unicode support test passed\n");
}
