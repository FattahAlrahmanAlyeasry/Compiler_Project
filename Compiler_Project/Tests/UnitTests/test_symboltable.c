#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <assert.h>
#include "../CompilerCore/symboltable.h"

// Test framework functions
static int test_count = 0;
static int test_passed = 0;
static int test_failed = 0;

void test_start(const char* test_name) {
    printf("Running test: %s\n", test_name);
    test_count++;
}

void test_pass(const char* test_name) {
    printf("✓ PASS: %s\n", test_name);
    test_passed++;
}

void test_fail(const char* test_name, const char* reason) {
    printf("✗ FAIL: %s - %s\n", test_name, reason);
    test_failed++;
}

void test_summary() {
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", test_count);
    printf("Passed: %d\n", test_passed);
    printf("Failed: %d\n", test_failed);
    printf("Success rate: %.1f%%\n", (float)test_passed / test_count * 100);
}

// Test symbol table initialization
void test_init_symbol_table() {
    test_start("Symbol Table Initialization");
    
    SymbolTable table;
    int result = initSymbolTable(&table);
    
    if (result == 0) {
        test_pass("Symbol Table Initialization");
        assert(table.scopes != NULL);
        assert(table.scope_count == 0);
        assert(table.scope_capacity > 0);
        assert(table.current_scope == -1);
        
        freeSymbolTable(&table);
    } else {
        test_fail("Symbol Table Initialization", "Failed to initialize");
    }
}

// Test symbol table cleanup
void test_free_symbol_table() {
    test_start("Symbol Table Cleanup");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Add some test data
    enterScope(&table, L"نطاق1");
    addSymbol(&table, L"متغير", VARIABLE_SYMBOL, INT_TYPE, 0);
    
    freeSymbolTable(&table);
    
    // Check if cleanup was successful
    if (table.scopes == NULL) {
        test_pass("Symbol Table Cleanup");
    } else {
        test_fail("Symbol Table Cleanup", "Memory not properly freed");
    }
}

// Test scope management
void test_scope_management() {
    test_start("Scope Management");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Enter first scope
    int result1 = enterScope(&table, L"نطاق1");
    assert(result1 == 0);
    assert(table.current_scope == 0);
    assert(table.scope_count == 1);
    assert(wcscmp(table.scopes[0].name, L"نطاق1") == 0);
    
    // Enter second scope
    int result2 = enterScope(&table, L"نطاق2");
    assert(result2 == 0);
    assert(table.current_scope == 1);
    assert(table.scope_count == 2);
    assert(wcscmp(table.scopes[1].name, L"نطاق2") == 0);
    
    // Exit current scope
    int result3 = exitScope(&table);
    assert(result3 == 0);
    assert(table.current_scope == 0);
    
    // Exit first scope
    int result4 = exitScope(&table);
    assert(result4 == 0);
    assert(table.current_scope == -1);
    
    test_pass("Scope Management");
    
    freeSymbolTable(&table);
}

// Test symbol addition
void test_add_symbol() {
    test_start("Add Symbol");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Enter a scope
    enterScope(&table, L"نطاق1");
    
    // Add different types of symbols
    int result1 = addSymbol(&table, L"متغير", VARIABLE_SYMBOL, INT_TYPE, 0);
    int result2 = addSymbol(&table, L"ثابت", CONSTANT_SYMBOL, FLOAT_TYPE, 1);
    int result3 = addSymbol(&table, L"دالة", PROCEDURE_SYMBOL, VOID_TYPE, 2);
    int result4 = addSymbol(&table, L"نوع", TYPE_SYMBOL, INT_TYPE, 3);
    
    if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0) {
        test_pass("Add Symbol");
        
        // Check symbol count in current scope
        assert(table.scopes[table.current_scope].symbol_count == 4);
        
        // Check symbol details
        SymbolEntry* var = lookupSymbol(&table, L"متغير");
        assert(var != NULL);
        assert(var->type == VARIABLE_SYMBOL);
        assert(var->data_type == INT_TYPE);
        assert(var->line_number == 0);
        
        SymbolEntry* const_sym = lookupSymbol(&table, L"ثابت");
        assert(const_sym != NULL);
        assert(const_sym->type == CONSTANT_SYMBOL);
        assert(const_sym->data_type == FLOAT_TYPE);
        
    } else {
        test_fail("Add Symbol", "Failed to add symbols");
    }
    
    freeSymbolTable(&table);
}

// Test symbol lookup
void test_lookup_symbol() {
    test_start("Lookup Symbol");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Enter scope and add symbols
    enterScope(&table, L"نطاق1");
    addSymbol(&table, L"متغير1", VARIABLE_SYMBOL, INT_TYPE, 0);
    addSymbol(&table, L"متغير2", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    
    // Enter nested scope
    enterScope(&table, L"نطاق2");
    addSymbol(&table, L"متغير3", VARIABLE_SYMBOL, STRING_TYPE, 2);
    
    // Lookup symbols in current scope
    SymbolEntry* sym1 = lookupSymbol(&table, L"متغير3");
    assert(sym1 != NULL);
    assert(sym1->type == VARIABLE_SYMBOL);
    assert(sym1->data_type == STRING_TYPE);
    
    // Lookup symbols in parent scope
    SymbolEntry* sym2 = lookupSymbol(&table, L"متغير1");
    assert(sym2 != NULL);
    assert(sym2->type == VARIABLE_SYMBOL);
    assert(sym2->data_type == INT_TYPE);
    
    // Lookup non-existent symbol
    SymbolEntry* sym3 = lookupSymbol(&table, L"غير موجود");
    assert(sym3 == NULL);
    
    test_pass("Lookup Symbol");
    
    freeSymbolTable(&table);
}

// Test nested scope symbol lookup
void test_nested_scope_lookup() {
    test_start("Nested Scope Symbol Lookup");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Global scope
    enterScope(&table, L"عالمي");
    addSymbol(&table, L"متغير_عالمي", VARIABLE_SYMBOL, INT_TYPE, 0);
    
    // Function scope
    enterScope(&table, L"دالة");
    addSymbol(&table, L"معامل", VARIABLE_SYMBOL, INT_TYPE, 1);
    addSymbol(&table, L"متغير_محلي", VARIABLE_SYMBOL, FLOAT_TYPE, 2);
    
    // Block scope
    enterScope(&table, L"كتلة");
    addSymbol(&table, L"متغير_كتلة", VARIABLE_SYMBOL, STRING_TYPE, 3);
    
    // Lookup in current scope
    SymbolEntry* local = lookupSymbol(&table, L"متغير_كتلة");
    assert(local != NULL);
    assert(local->data_type == STRING_TYPE);
    
    // Lookup in parent scope
    SymbolEntry* param = lookupSymbol(&table, L"معامل");
    assert(param != NULL);
    assert(param->data_type == INT_TYPE);
    
    // Lookup in global scope
    SymbolEntry* global = lookupSymbol(&table, L"متغير_عالمي");
    assert(global != NULL);
    assert(global->data_type == INT_TYPE);
    
    test_pass("Nested Scope Symbol Lookup");
    
    freeSymbolTable(&table);
}

// Test symbol redefinition
void test_symbol_redefinition() {
    test_start("Symbol Redefinition");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    enterScope(&table, L"نطاق1");
    
    // Add symbol first time
    int result1 = addSymbol(&table, L"متغير", VARIABLE_SYMBOL, INT_TYPE, 0);
    assert(result1 == 0);
    
    // Try to add same symbol again
    int result2 = addSymbol(&table, L"متغير", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    
    if (result2 != 0) {
        test_pass("Symbol Redefinition");
        
        // Check that original symbol remains unchanged
        SymbolEntry* sym = lookupSymbol(&table, L"متغير");
        assert(sym != NULL);
        assert(sym->data_type == INT_TYPE);
        assert(sym->line_number == 0);
        
    } else {
        test_fail("Symbol Redefinition", "Allowed duplicate symbol");
    }
    
    freeSymbolTable(&table);
}

// Test symbol type checking
void test_symbol_type_checking() {
    test_start("Symbol Type Checking");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    enterScope(&table, L"نطاق1");
    
    // Add symbols with different types
    addSymbol(&table, L"متغير_صحيح", VARIABLE_SYMBOL, INT_TYPE, 0);
    addSymbol(&table, L"متغير_حقيقي", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    addSymbol(&table, L"متغير_نص", VARIABLE_SYMBOL, STRING_TYPE, 2);
    addSymbol(&table, L"متغير_حرف", VARIABLE_SYMBOL, CHAR_TYPE, 3);
    addSymbol(&table, L"متغير_منطقي", VARIABLE_SYMBOL, BOOLEAN_TYPE, 4);
    
    // Check symbol types
    SymbolEntry* int_var = lookupSymbol(&table, L"متغير_صحيح");
    assert(int_var != NULL);
    assert(int_var->data_type == INT_TYPE);
    
    SymbolEntry* float_var = lookupSymbol(&table, L"متغير_حقيقي");
    assert(float_var != NULL);
    assert(float_var->data_type == FLOAT_TYPE);
    
    SymbolEntry* string_var = lookupSymbol(&table, L"متغير_نص");
    assert(string_var != NULL);
    assert(string_var->data_type == STRING_TYPE);
    
    SymbolEntry* char_var = lookupSymbol(&table, L"متغير_حرف");
    assert(char_var != NULL);
    assert(char_var->data_type == CHAR_TYPE);
    
    SymbolEntry* bool_var = lookupSymbol(&table, L"متغير_منطقي");
    assert(bool_var != NULL);
    assert(bool_var->data_type == BOOLEAN_TYPE);
    
    test_pass("Symbol Type Checking");
    
    freeSymbolTable(&table);
}

// Test symbol line numbers
void test_symbol_line_numbers() {
    test_start("Symbol Line Numbers");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    enterScope(&table, L"نطاق1");
    
    // Add symbols with different line numbers
    addSymbol(&table, L"متغير1", VARIABLE_SYMBOL, INT_TYPE, 10);
    addSymbol(&table, L"متغير2", VARIABLE_SYMBOL, FLOAT_TYPE, 25);
    addSymbol(&table, L"متغير3", VARIABLE_SYMBOL, STRING_TYPE, 42);
    
    // Check line numbers
    SymbolEntry* var1 = lookupSymbol(&table, L"متغير1");
    assert(var1 != NULL);
    assert(var1->line_number == 10);
    
    SymbolEntry* var2 = lookupSymbol(&table, L"متغير2");
    assert(var2 != NULL);
    assert(var2->line_number == 25);
    
    SymbolEntry* var3 = lookupSymbol(&table, L"متغير3");
    assert(var3 != NULL);
    assert(var3->line_number == 42);
    
    test_pass("Symbol Line Numbers");
    
    freeSymbolTable(&table);
}

// Test symbol table error handling
void test_symbol_table_error_handling() {
    test_start("Symbol Table Error Handling");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Test invalid scope operations
    int result1 = exitScope(&table); // Exit when no scope
    assert(result1 != 0);
    
    // Test invalid symbol operations
    int result2 = addSymbol(&table, L"متغير", VARIABLE_SYMBOL, INT_TYPE, 0); // Add without scope
    assert(result2 != 0);
    
    // Test lookup without scope
    SymbolEntry* sym = lookupSymbol(&table, L"متغير");
    assert(sym == NULL);
    
    test_pass("Symbol Table Error Handling");
    
    freeSymbolTable(&table);
}

// Test symbol table statistics
void test_symbol_table_statistics() {
    test_start("Symbol Table Statistics");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Add symbols in different scopes
    enterScope(&table, L"عالمي");
    addSymbol(&table, L"متغير1", VARIABLE_SYMBOL, INT_TYPE, 0);
    addSymbol(&table, L"متغير2", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    
    enterScope(&table, L"دالة");
    addSymbol(&table, L"معامل", VARIABLE_SYMBOL, INT_TYPE, 2);
    addSymbol(&table, L"متغير_محلي", VARIABLE_SYMBOL, STRING_TYPE, 3);
    
    // Check statistics
    assert(getSymbolCount(&table) == 4);
    assert(getScopeCount(&table) == 2);
    assert(getCurrentScopeLevel(&table) == 1);
    
    // Check symbol count by type
    assert(getSymbolCountByType(&table, VARIABLE_SYMBOL) == 4);
    assert(getSymbolCountByType(&table, CONSTANT_SYMBOL) == 0);
    assert(getSymbolCountByType(&table, PROCEDURE_SYMBOL) == 0);
    
    test_pass("Symbol Table Statistics");
    
    freeSymbolTable(&table);
}

// Test symbol table search functions
void test_symbol_table_search() {
    test_start("Symbol Table Search");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    enterScope(&table, L"نطاق1");
    
    // Add symbols
    addSymbol(&table, L"متغير_صحيح", VARIABLE_SYMBOL, INT_TYPE, 0);
    addSymbol(&table, L"متغير_حقيقي", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    addSymbol(&table, L"ثابت_نص", CONSTANT_SYMBOL, STRING_TYPE, 2);
    
    // Search by name pattern
    SymbolEntry** int_vars = searchSymbolsByName(&table, L"متغير_*");
    assert(int_vars != NULL);
    
    // Search by type
    SymbolEntry** var_symbols = searchSymbolsByType(&table, VARIABLE_SYMBOL);
    assert(var_symbols != NULL);
    
    // Search by data type
    SymbolEntry** int_symbols = searchSymbolsByDataType(&table, INT_TYPE);
    assert(int_symbols != NULL);
    
    test_pass("Symbol Table Search");
    
    // Cleanup search results
    if (int_vars) free(int_vars);
    if (var_symbols) free(var_symbols);
    if (int_symbols) free(int_symbols);
    
    freeSymbolTable(&table);
}

// Test symbol table export
void test_symbol_table_export() {
    test_start("Symbol Table Export");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Add some symbols
    enterScope(&table, L"نطاق1");
    addSymbol(&table, L"متغير1", VARIABLE_SYMBOL, INT_TYPE, 0);
    addSymbol(&table, L"متغير2", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    
    // Test export to buffer
    wchar_t buffer[1024];
    int pos = 0;
    int result = exportSymbolTableToBuffer(&table, buffer, 1024, &pos);
    
    if (result == 0 && pos > 0) {
        test_pass("Symbol Table Export");
        
        // Check buffer content
        assert(wcslen(buffer) > 0);
        assert(wcsstr(buffer, L"نطاق1") != NULL);
        assert(wcsstr(buffer, L"متغير1") != NULL);
        assert(wcsstr(buffer, L"متغير2") != NULL);
        
    } else {
        test_fail("Symbol Table Export", "Failed to export symbol table");
    }
    
    freeSymbolTable(&table);
}

// Test symbol table import
void test_symbol_table_import() {
    test_start("Symbol Table Import");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Test import from buffer
    wchar_t buffer[] = L"نطاق1:متغير1:VARIABLE:INT:0;متغير2:VARIABLE:FLOAT:1";
    int result = importSymbolTableFromBuffer(&table, buffer);
    
    if (result == 0) {
        test_pass("Symbol Table Import");
        
        // Check imported symbols
        SymbolEntry* var1 = lookupSymbol(&table, L"متغير1");
        assert(var1 != NULL);
        assert(var1->type == VARIABLE_SYMBOL);
        assert(var1->data_type == INT_TYPE);
        
        SymbolEntry* var2 = lookupSymbol(&table, L"متغير2");
        assert(var2 != NULL);
        assert(var2->type == VARIABLE_SYMBOL);
        assert(var2->data_type == FLOAT_TYPE);
        
    } else {
        test_fail("Symbol Table Import", "Failed to import symbol table");
    }
    
    freeSymbolTable(&table);
}

// Test symbol table validation
void test_symbol_table_validation() {
    test_start("Symbol Table Validation");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Add valid symbols
    enterScope(&table, L"نطاق1");
    addSymbol(&table, L"متغير1", VARIABLE_SYMBOL, INT_TYPE, 0);
    addSymbol(&table, L"متغير2", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    
    // Validate symbol table
    int result = validateSymbolTable(&table);
    
    if (result == 1) {
        test_pass("Symbol Table Validation");
        
        // Check validation details
        assert(validateScope(&table, 0) == 1);
        assert(validateSymbol(&table, L"متغير1") == 1);
        assert(validateSymbol(&table, L"متغير2") == 1);
        assert(validateSymbol(&table, L"غير موجود") == 0);
        
    } else {
        test_fail("Symbol Table Validation", "Symbol table validation failed");
    }
    
    freeSymbolTable(&table);
}

// Test symbol table performance
void test_symbol_table_performance() {
    test_start("Symbol Table Performance");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Add many symbols to test performance
    enterScope(&table, L"نطاق1");
    
    for (int i = 0; i < 1000; i++) {
        wchar_t name[32];
        swprintf(name, 32, L"متغير%d", i);
        addSymbol(&table, name, VARIABLE_SYMBOL, INT_TYPE, i);
    }
    
    // Test lookup performance
    SymbolEntry* sym = lookupSymbol(&table, L"متغير500");
    assert(sym != NULL);
    assert(sym->line_number == 500);
    
    // Test search performance
    SymbolEntry** results = searchSymbolsByName(&table, L"متغير*");
    assert(results != NULL);
    
    test_pass("Symbol Table Performance");
    
    // Cleanup
    if (results) free(results);
    freeSymbolTable(&table);
}

// Test symbol table memory management
void test_symbol_table_memory_management() {
    test_start("Symbol Table Memory Management");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Test scope array resizing
    for (int i = 0; i < 100; i++) {
        wchar_t scope_name[32];
        swprintf(scope_name, 32, L"نطاق%d", i);
        enterScope(&table, scope_name);
        
        // Add symbols to each scope
        for (int j = 0; j < 10; j++) {
            wchar_t symbol_name[32];
            swprintf(symbol_name, 32, L"متغير%d_%d", i, j);
            addSymbol(&table, symbol_name, VARIABLE_SYMBOL, INT_TYPE, i * 100 + j);
        }
    }
    
    test_pass("Symbol Table Memory Management");
    
    // Check memory allocation
    assert(table.scope_count == 100);
    assert(table.scope_capacity >= 100);
    
    // Test scope array resizing
    resizeScopeArray(&table, 200);
    assert(table.scope_capacity >= 200);
    
    // Test scope array compacting
    compactScopeArray(&table);
    assert(table.scope_capacity >= 100);
    
    freeSymbolTable(&table);
}

// Test symbol table error reporting
void test_symbol_table_error_reporting() {
    test_start("Symbol Table Error Reporting");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Add some symbols
    enterScope(&table, L"نطاق1");
    addSymbol(&table, L"متغير1", VARIABLE_SYMBOL, INT_TYPE, 0);
    
    // Test error reporting
    symbolTableError(&table, UNDEFINED_SYMBOL, L"متغير_غير_موجود", 5, L"Symbol not defined");
    symbolTableError(&table, DUPLICATE_SYMBOL, L"متغير1", 10, L"Duplicate symbol declaration");
    
    // Check error count
    int error_count = getSymbolTableErrorCount(&table);
    if (error_count == 2) {
        test_pass("Symbol Table Error Reporting");
        
        // Check error details
        SymbolTableError* errors = getSymbolTableErrors(&table);
        assert(errors != NULL);
        assert(errors[0].type == UNDEFINED_SYMBOL);
        assert(errors[1].type == DUPLICATE_SYMBOL);
        
    } else {
        test_fail("Symbol Table Error Reporting", "Error count incorrect");
    }
    
    freeSymbolTable(&table);
}

// Test symbol table serialization
void test_symbol_table_serialization() {
    test_start("Symbol Table Serialization");
    
    SymbolTable table;
    initSymbolTable(&table);
    
    // Add symbols
    enterScope(&table, L"نطاق1");
    addSymbol(&table, L"متغير1", VARIABLE_SYMBOL, INT_TYPE, 0);
    addSymbol(&table, L"متغير2", VARIABLE_SYMBOL, FLOAT_TYPE, 1);
    
    // Serialize to string
    wchar_t* serialized = serializeSymbolTable(&table);
    
    if (serialized) {
        test_pass("Symbol Table Serialization");
        
        // Check serialized content
        assert(wcslen(serialized) > 0);
        assert(wcsstr(serialized, L"نطاق1") != NULL);
        assert(wcsstr(serialized, L"متغير1") != NULL);
        assert(wcsstr(serialized, L"متغير2") != NULL);
        
        // Deserialize
        SymbolTable new_table;
        initSymbolTable(&new_table);
        int result = deserializeSymbolTable(&new_table, serialized);
        
        if (result == 0) {
            // Check deserialized content
            SymbolEntry* var1 = lookupSymbol(&new_table, L"متغير1");
            assert(var1 != NULL);
            assert(var1->type == VARIABLE_SYMBOL);
            assert(var1->data_type == INT_TYPE);
            
            freeSymbolTable(&new_table);
        }
        
        free(serialized);
        
    } else {
        test_fail("Symbol Table Serialization", "Failed to serialize symbol table");
    }
    
    freeSymbolTable(&table);
}

// Main test function
int main() {
    // Set locale for Unicode support
    setlocale(LC_ALL, "");
    
    printf("=== Symbol Table Unit Tests ===\n\n");
    
    // Run all tests
    test_init_symbol_table();
    test_free_symbol_table();
    test_scope_management();
    test_add_symbol();
    test_lookup_symbol();
    test_nested_scope_lookup();
    test_symbol_redefinition();
    test_symbol_type_checking();
    test_symbol_line_numbers();
    test_symbol_table_error_handling();
    test_symbol_table_statistics();
    test_symbol_table_search();
    test_symbol_table_export();
    test_symbol_table_import();
    test_symbol_table_validation();
    test_symbol_table_performance();
    test_symbol_table_memory_management();
    test_symbol_table_error_reporting();
    test_symbol_table_serialization();
    
    // Print test summary
    test_summary();
    
    return (test_failed == 0) ? 0 : 1;
}

