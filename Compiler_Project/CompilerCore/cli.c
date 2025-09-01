#include "main.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
static wchar_t* duplicate_wcs(const wchar_t* s)
{
    if (!s) return NULL;
    size_t len = wcslen(s) + 1;
    wchar_t* out = (wchar_t*)malloc(len * sizeof(wchar_t));
    if (!out) return NULL;
    wcscpy(out, s);
    return out;
}

static void print_wide(const wchar_t* title, const wchar_t* content)
{
    wprintf(L"\n==== %ls ====\n%ls\n", title, content);
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");

    if (initCompiler() != 1) {
        fprintf(stderr, "Failed to initialize compiler.\n");
        return 1;
    }

    // Read source code from file or use a default sample
    wchar_t* source = NULL;
    if (argc > 1) {
        const char* path = argv[1];
        FILE* f = fopen(path, "rb");
        if (!f) {
            fprintf(stderr, "Failed to open input file: %s\n", path);
            freeCompiler();
            return 1;
        }
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        // Allocate wchar buffer (assuming UTF-8 input, simplistic conversion)
        char* utf8 = (char*)malloc((size_t)size + 1);
        if (!utf8) { fclose(f); freeCompiler(); return 1; }
        fread(utf8, 1, (size_t)size, f);
        utf8[size] = '\0';
        fclose(f);

        // Convert UTF-8 to wchar_t using mbsrtowcs
        mbstate_t st;
        memset(&st, 0, sizeof(st));
        const char* p = utf8;
        size_t wlen = mbsrtowcs(NULL, &p, 0, &st);
        if (wlen == (size_t)-1) {
            fprintf(stderr, "Invalid UTF-8 in input file.\n");
            free(utf8);
            freeCompiler();
            return 1;
        }
        source = (wchar_t*)malloc((wlen + 1) * sizeof(wchar_t));
        if (!source) { free(utf8); freeCompiler(); return 1; }
        p = utf8;
        mbsrtowcs(source, &p, wlen + 1, &st);
        free(utf8);
    } else {
        source = duplicate_wcs(L"برنامج\n[\n  متغيرات\n    متغير صحيح ؛\n]\n:\n  اطبع \"مرحباً بالعالم\" ؛\n؛\n");
    }

    if (compileSourceCode(source) != 1) {
        wchar_t errbuf[1024];
        getErrorMessages(errbuf, 1024);
        fwprintf(stderr, L"Compilation failed: %ls\n", errbuf);
        free(source);
        freeCompiler();
        return 1;
    }

    wchar_t buf[4096];

    if (getParseTree(buf, 4096)) {
        print_wide(L"Parse Tree", buf);
    }
    if (getAST(buf, 4096)) {
        print_wide(L"AST", buf);
    }
    if (getMIPSCode(buf, 4096)) {
        print_wide(L"MIPS", buf);
    }
    if (getSymbolTable(buf, 4096)) {
        print_wide(L"Symbol Table", buf);
    }

    free(source);
    freeCompiler();
    return 0;
}

