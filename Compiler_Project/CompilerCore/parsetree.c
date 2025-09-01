#include "parsetree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// إنشاء شجرة إعراب جديدة
ParseTree* createParseTree() {
    ParseTree* tree = (ParseTree*)malloc(sizeof(ParseTree));
    if (!tree) {
        return NULL;
    }
    
    tree->root = NULL;
    tree->node_count = 0;
    tree->max_nodes = 1000;
    tree->current_line = 1;
    tree->current_column = 1;
    
    return tree;
}

// إنشاء عقدة إعراب جديدة
ParseTreeNode* createParseTreeNode(ParseTreeNodeType type, const wchar_t* value, int line, int column) {
    ParseTreeNode* node = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    if (!node) {
        return NULL;
    }
    
    node->type = type;
    node->line = line;
    node->column = column;
    node->parent = NULL;
    node->children = NULL;
    node->children_count = 0;
    node->max_children = 0;
    node->extra_data = NULL;
    
    if (value) {
        size_t len = wcslen(value);
        node->value = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
        if (node->value) {
            wcscpy(node->value, value);
        } else {
            node->value = NULL;
        }
    } else {
        node->value = NULL;
    }
    
    return node;
}

// إضافة طفل إلى عقدة
int addChild(ParseTreeNode* parent, ParseTreeNode* child) {
    if (!parent || !child) {
        return 0;
    }
    
    if (parent->children_count >= parent->max_children) {
        int new_size = parent->max_children == 0 ? 4 : parent->max_children * 2;
        ParseTreeNode** new_children = (ParseTreeNode**)realloc(parent->children, new_size * sizeof(ParseTreeNode*));
        if (!new_children) {
            return 0;
        }
        parent->children = new_children;
        parent->max_children = new_size;
    }
    
    parent->children[parent->children_count] = child;
    child->parent = parent;
    parent->children_count++;
    
    return 1;
}

// الحصول على طفل بواسطة الفهرس
ParseTreeNode* getChild(ParseTreeNode* parent, int index) {
    if (!parent || index < 0 || index >= parent->children_count) {
        return NULL;
    }
    return parent->children[index];
}

// الحصول على عدد الأطفال
int getChildCount(ParseTreeNode* parent) {
    return parent ? parent->children_count : 0;
}

// الحصول على الأب
ParseTreeNode* getParent(ParseTreeNode* node) {
    return node ? node->parent : NULL;
}

// بناء عقدة برنامج
ParseTreeNode* buildProgramNode(const wchar_t* program_name) {
    return createParseTreeNode(PARSE_NODE_PROGRAM, program_name, 1, 1);
}

// بناء عقدة كتلة
ParseTreeNode* buildBlockNode() {
    return createParseTreeNode(PARSE_NODE_BLOCK, L"", 0, 0);
}

// بناء عقدة جزء التعريفات
ParseTreeNode* buildDefinitionsPartNode() {
    return createParseTreeNode(PARSE_NODE_DEFINITIONS_PART, L"", 0, 0);
}

// بناء عقدة قائمة العبارات
ParseTreeNode* buildStatementsListNode() {
    return createParseTreeNode(PARSE_NODE_STATEMENTS_LIST, L"", 0, 0);
}

// بناء عقدة إسناد
ParseTreeNode* buildAssignmentNode(const wchar_t* var_name, ParseTreeNode* expression) {
    ParseTreeNode* node = createParseTreeNode(PARSE_NODE_ASSIGNMENT_STATEMENT, L"=", 0, 0);
    if (node && expression) {
        addChild(node, createParseTreeNode(PARSE_NODE_IDENTIFIER, var_name, 0, 0));
        addChild(node, expression);
    }
    return node;
}

// بناء عقدة عبارة إدخال/إخراج
ParseTreeNode* buildIOStatementNode(ParseTreeNodeType io_type, ParseTreeNode* target) {
    const wchar_t* value = (io_type == PARSE_NODE_IO_STATEMENT) ? L"اطبع" : L"اقرأ";
    ParseTreeNode* node = createParseTreeNode(io_type, value, 0, 0);
    if (node && target) {
        addChild(node, target);
    }
    return node;
}

// بناء عقدة شرطية
ParseTreeNode* buildConditionalNode(ParseTreeNode* condition, ParseTreeNode* then_part, ParseTreeNode* else_part) {
    ParseTreeNode* node = createParseTreeNode(PARSE_NODE_CONDITIONAL_STATEMENT, L"إذا", 0, 0);
    if (node) {
        if (condition) addChild(node, condition);
        if (then_part) addChild(node, then_part);
        if (else_part) addChild(node, else_part);
    }
    return node;
}

// بناء عقدة حلقة
ParseTreeNode* buildLoopNode(ParseTreeNodeType loop_type, ParseTreeNode* condition, ParseTreeNode* body) {
    const wchar_t* value = (loop_type == PARSE_NODE_LOOP_STATEMENT) ? L"طالما" : L"كرر";
    ParseTreeNode* node = createParseTreeNode(loop_type, value, 0, 0);
    if (node) {
        if (condition) addChild(node, condition);
        if (body) addChild(node, body);
    }
    return node;
}

// بناء عقدة تعبير
ParseTreeNode* buildExpressionNode(ParseTreeNodeType expr_type, ParseTreeNode* left, ParseTreeNode* right) {
    ParseTreeNode* node = createParseTreeNode(expr_type, L"", 0, 0);
    if (node) {
        if (left) addChild(node, left);
        if (right) addChild(node, right);
    }
    return node;
}

// بناء عقدة معرف
ParseTreeNode* buildIdentifierNode(const wchar_t* name) {
    return createParseTreeNode(PARSE_NODE_IDENTIFIER, name, 0, 0);
}

// بناء عقدة ثابت
ParseTreeNode* buildLiteralNode(const wchar_t* value, int literal_type) {
    return createParseTreeNode(PARSE_NODE_LITERAL, value, 0, 0);
}

// طباعة شجرة الإعراب
void printParseTree(ParseTree* tree) {
    if (!tree || !tree->root) {
        printf("شجرة إعراب فارغة\n");
        return;
    }
    
    printf("=== شجرة الإعراب ===\n");
    printParseTreeNode(tree->root, 0);
    printf("==================\n");
}

// طباعة عقدة إعراب
void printParseTreeNode(ParseTreeNode* node, int depth) {
    if (!node) {
        return;
    }
    
    // طباعة المسافات
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    // طباعة نوع العقدة
    printf("Node: %s", getNodeTypeName(node->type));
    
    // طباعة القيمة
    if (node->value && wcslen(node->value) > 0) {
        printf(" - Value: %ls", node->value);
    }
    
    // طباعة الموقع
    if (node->line > 0) {
        printf(" (Line: %d, Column: %d)", node->line, node->column);
    }
    
    printf("\n");
    
    // طباعة الأطفال
    for (int i = 0; i < node->children_count; i++) {
        printParseTreeNode(node->children[i], depth + 1);
    }
}

// طباعة شجرة الإعراب إلى ملف
void printParseTreeToFile(ParseTree* tree, const char* filename) {
    if (!tree || !filename) {
        return;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        return;
    }
    
    fprintf(file, "=== شجرة الإعراب ===\n");
    printParseTreeNodeToFile(tree->root, file, 0);
    fprintf(file, "==================\n");
    
    fclose(file);
}

// طباعة شجرة الإعراب إلى بفر
void printParseTreeToBuffer(ParseTree* tree, wchar_t* buffer, int buffer_size, int* pos) {
    if (!tree || !buffer || !pos || *pos >= buffer_size) {
        return;
    }
    
    printParseTreeNodeToBuffer(tree->root, buffer, buffer_size, pos, 0);
}

// طباعة عقدة إعراب إلى ملف
static void printParseTreeNodeToFile(ParseTreeNode* node, FILE* file, int depth) {
    if (!node || !file) {
        return;
    }
    
    // طباعة المسافات
    for (int i = 0; i < depth; i++) {
        fprintf(file, "  ");
    }
    
    // طباعة نوع العقدة
    fprintf(file, "Node: %s", getNodeTypeName(node->type));
    
    // طباعة القيمة
    if (node->value && wcslen(node->value) > 0) {
        fprintf(file, " - Value: %ls", node->value);
    }
    
    // طباعة الموقع
    if (node->line > 0) {
        fprintf(file, " (Line: %d, Column: %d)", node->line, node->column);
    }
    
    fprintf(file, "\n");
    
    // طباعة الأطفال
    for (int i = 0; i < node->children_count; i++) {
        printParseTreeNodeToFile(node->children[i], file, depth + 1);
    }
}

// طباعة عقدة إعراب إلى بفر
static void printParseTreeNodeToBuffer(ParseTreeNode* node, wchar_t* buffer, int buffer_size, int* pos, int depth) {
    if (!node || !buffer || !pos || *pos >= buffer_size) {
        return;
    }
    
    // طباعة المسافات
    for (int i = 0; i < depth && *pos < buffer_size - 1; i++) {
        buffer[(*pos)++] = L' ';
        buffer[(*pos)++] = L' ';
    }
    
    // طباعة نوع العقدة
    const wchar_t* type_name = getNodeTypeName(node->type);
    for (int i = 0; type_name[i] && *pos < buffer_size - 1; i++) {
        buffer[(*pos)++] = type_name[i];
    }
    
    // طباعة القيمة
    if (node->value && wcslen(node->value) > 0) {
        if (*pos < buffer_size - 3) {
            buffer[(*pos)++] = L' ';
            buffer[(*pos)++] = L'-';
            buffer[(*pos)++] = L' ';
        }
        
        for (int i = 0; node->value[i] && *pos < buffer_size - 1; i++) {
            buffer[(*pos)++] = node->value[i];
        }
    }
    
    // طباعة الموقع
    if (node->line > 0 && *pos < buffer_size - 20) {
        swprintf(buffer + *pos, buffer_size - *pos, L" (Line: %d, Column: %d)", node->line, node->column);
        *pos += wcslen(buffer + *pos);
    }
    
    if (*pos < buffer_size - 1) {
        buffer[(*pos)++] = L'\n';
    }
    
    // طباعة الأطفال
    for (int i = 0; i < node->children_count; i++) {
        printParseTreeNodeToBuffer(node->children[i], buffer, buffer_size, pos, depth + 1);
    }
}

// تصدير شجرة الإعراب إلى صيغة DOT
int exportParseTreeToDot(ParseTree* tree, const char* filename) {
    if (!tree || !filename) {
        return 0;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        return 0;
    }
    
    fprintf(file, "digraph ParseTree {\n");
    fprintf(file, "  node [shape=box, fontname=\"Arial\"];\n");
    fprintf(file, "  edge [fontname=\"Arial\"];\n");
    
    exportNodeToDot(tree->root, file);
    
    fprintf(file, "}\n");
    fclose(file);
    
    return 1;
}

// تصدير عقدة إلى صيغة DOT
static void exportNodeToDot(ParseTreeNode* node, FILE* file) {
    if (!node || !file) {
        return;
    }
    
    static int node_id = 0;
    int current_id = node_id++;
    
    // تعريف العقدة
    const wchar_t* label = node->value && wcslen(node->value) > 0 ? node->value : getNodeTypeName(node->type);
    fprintf(file, "  %d [label=\"%ls\"];\n", current_id, label);
    
    // تعريف الروابط مع الأطفال
    for (int i = 0; i < node->children_count; i++) {
        int child_id = node_id;
        exportNodeToDot(node->children[i], file);
        fprintf(file, "  %d -> %d;\n", current_id, child_id);
    }
}

// البحث عن عقدة بالموقع
ParseTreeNode* findNodeByPosition(ParseTree* tree, int line, int column) {
    if (!tree || !tree->root) {
        return NULL;
    }
    
    return findNodeByPositionRecursive(tree->root, line, column);
}

// البحث التكراري عن عقدة بالموقع
static ParseTreeNode* findNodeByPositionRecursive(ParseTreeNode* node, int line, int column) {
    if (!node) {
        return NULL;
    }
    
    // التحقق من العقدة الحالية
    if (node->line == line && node->column == column) {
        return node;
    }
    
    // البحث في الأطفال
    for (int i = 0; i < node->children_count; i++) {
        ParseTreeNode* found = findNodeByPositionRecursive(node->children[i], line, column);
        if (found) {
            return found;
        }
    }
    
    return NULL;
}

// عكس ترتيب الأطفال لدعم العرض من اليمين إلى اليسار
void reverseChildrenOrder(ParseTreeNode* node) {
    if (!node || node->children_count <= 1) {
        return;
    }
    
    int left = 0;
    int right = node->children_count - 1;
    
    while (left < right) {
        ParseTreeNode* temp = node->children[left];
        node->children[left] = node->children[right];
        node->children[right] = temp;
        left++;
        right--;
    }
}

// عكس اتجاه الشجرة بالكامل
void reverseTreeDirection(ParseTree* tree) {
    if (!tree || !tree->root) {
        return;
    }
    
    reverseTreeDirectionRecursive(tree->root);
}

// عكس اتجاه الشجرة بشكل تكراري
static void reverseTreeDirectionRecursive(ParseTreeNode* node) {
    if (!node) {
        return;
    }
    
    // عكس ترتيب الأطفال
    reverseChildrenOrder(node);
    
    // عكس الأطفال بشكل تكراري
    for (int i = 0; i < node->children_count; i++) {
        reverseTreeDirectionRecursive(node->children[i]);
    }
}

// الحصول على اسم نوع العقدة
const wchar_t* getNodeTypeName(ParseTreeNodeType type) {
    switch (type) {
        case PARSE_NODE_PROGRAM: return L"برنامج";
        case PARSE_NODE_BLOCK: return L"كتلة";
        case PARSE_NODE_DEFINITIONS_PART: return L"جزء التعريفات";
        case PARSE_NODE_CONSTANTS_DEFINITION: return L"تعريف الثوابت";
        case PARSE_NODE_TYPES_DEFINITION: return L"تعريف الأنواع";
        case PARSE_NODE_VARIABLES_DEFINITION: return L"تعريف المتغيرات";
        case PARSE_NODE_PROCEDURES_DEFINITION: return L"تعريف الإجراءات";
        case PARSE_NODE_STATEMENTS_LIST: return L"قائمة العبارات";
        case PARSE_NODE_ASSIGNMENT_STATEMENT: return L"عبارة إسناد";
        case PARSE_NODE_IO_STATEMENT: return L"عبارة إدخال/إخراج";
        case PARSE_NODE_CONDITIONAL_STATEMENT: return L"عبارة شرطية";
        case PARSE_NODE_LOOP_STATEMENT: return L"عبارة حلقة";
        case PARSE_NODE_PROCEDURE_CALL: return L"استدعاء إجراء";
        case PARSE_NODE_COMPOUND_STATEMENT: return L"عبارة مركبة";
        case PARSE_NODE_EMPTY_STATEMENT: return L"عبارة فارغة";
        case PARSE_NODE_EXPRESSION: return L"تعبير";
        case PARSE_NODE_ARITHMETIC_EXPRESSION: return L"تعبير حسابي";
        case PARSE_NODE_RELATIONAL_EXPRESSION: return L"تعبير علائقي";
        case PARSE_NODE_LOGICAL_EXPRESSION: return L"تعبير منطقي";
        case PARSE_NODE_IDENTIFIER: return L"معرف";
        case PARSE_NODE_LITERAL: return L"ثابت";
        case PARSE_NODE_TYPE_SPECIFIER: return L"محدد نوع";
        case PARSE_NODE_PARAMETER_LIST: return L"قائمة المعاملات";
        case PARSE_NODE_ARRAY_TYPE: return L"نوع مصفوفة";
        case PARSE_NODE_RECORD_TYPE: return L"نوع سجل";
        case PARSE_NODE_FIELD_LIST: return L"قائمة الحقول";
        case PARSE_NODE_INDEX_LIST: return L"قائمة المؤشرات";
        default: return L"غير معروف";
    }
}

// تحرير ذاكرة عقدة إعراب
void freeParseTreeNode(ParseTreeNode* node) {
    if (!node) {
        return;
    }
    
    // تحرير الأطفال أولاً
    for (int i = 0; i < node->children_count; i++) {
        freeParseTreeNode(node->children[i]);
    }
    
    // تحرير القيمة
    if (node->value) {
        free(node->value);
    }
    
    // تحرير مصفوفة الأطفال
    if (node->children) {
        free(node->children);
    }
    
    // تحرير العقدة نفسها
    free(node);
}

// تحرير ذاكرة شجرة الإعراب
void freeParseTree(ParseTree* tree) {
    if (!tree) {
        return;
    }
    
    if (tree->root) {
        freeParseTreeNode(tree->root);
    }
    
    free(tree);
}
