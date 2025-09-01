#ifndef PARSETREE_H
#define PARSETREE_H

#include <wchar.h>

// أنواع عقد شجرة الإعراب
typedef enum {
    PARSE_NODE_PROGRAM,
    PARSE_NODE_BLOCK,
    PARSE_NODE_DEFINITIONS_PART,
    PARSE_NODE_CONSTANTS_DEFINITION,
    PARSE_NODE_TYPES_DEFINITION,
    PARSE_NODE_VARIABLES_DEFINITION,
    PARSE_NODE_PROCEDURES_DEFINITION,
    PARSE_NODE_STATEMENTS_LIST,
    PARSE_NODE_ASSIGNMENT_STATEMENT,
    PARSE_NODE_IO_STATEMENT,
    PARSE_NODE_CONDITIONAL_STATEMENT,
    PARSE_NODE_LOOP_STATEMENT,
    PARSE_NODE_PROCEDURE_CALL,
    PARSE_NODE_COMPOUND_STATEMENT,
    PARSE_NODE_EMPTY_STATEMENT,
    PARSE_NODE_EXPRESSION,
    PARSE_NODE_ARITHMETIC_EXPRESSION,
    PARSE_NODE_RELATIONAL_EXPRESSION,
    PARSE_NODE_LOGICAL_EXPRESSION,
    PARSE_NODE_IDENTIFIER,
    PARSE_NODE_LITERAL,
    PARSE_NODE_TYPE_SPECIFIER,
    PARSE_NODE_PARAMETER_LIST,
    PARSE_NODE_ARRAY_TYPE,
    PARSE_NODE_RECORD_TYPE,
    PARSE_NODE_FIELD_LIST,
    PARSE_NODE_INDEX_LIST
} ParseTreeNodeType;

// عقدة شجرة الإعراب
typedef struct ParseTreeNode {
    ParseTreeNodeType type;
    wchar_t* value;
    int line;
    int column;
    struct ParseTreeNode* parent;
    struct ParseTreeNode** children;
    int children_count;
    int max_children;
    void* extra_data; // بيانات إضافية حسب نوع العقدة
} ParseTreeNode;

// هيكل شجرة الإعراب
typedef struct {
    ParseTreeNode* root;
    int node_count;
    int max_nodes;
    int current_line;
    int current_column;
} ParseTree;

// دوال إنشاء وتحرير الذاكرة
ParseTree* createParseTree();
void freeParseTree(ParseTree* tree);
ParseTreeNode* createParseTreeNode(ParseTreeNodeType type, const wchar_t* value, int line, int column);
void freeParseTreeNode(ParseTreeNode* node);

// دوال إدارة العقد
int addChild(ParseTreeNode* parent, ParseTreeNode* child);
ParseTreeNode* getChild(ParseTreeNode* parent, int index);
int getChildCount(ParseTreeNode* parent);
ParseTreeNode* getParent(ParseTreeNode* node);

// دوال بناء شجرة الإعراب
ParseTreeNode* buildProgramNode(const wchar_t* program_name);
ParseTreeNode* buildBlockNode();
ParseTreeNode* buildDefinitionsPartNode();
ParseTreeNode* buildStatementsListNode();
ParseTreeNode* buildAssignmentNode(const wchar_t* var_name, ParseTreeNode* expression);
ParseTreeNode* buildIOStatementNode(ParseTreeNodeType io_type, ParseTreeNode* target);
ParseTreeNode* buildConditionalNode(ParseTreeNode* condition, ParseTreeNode* then_part, ParseTreeNode* else_part);
ParseTreeNode* buildLoopNode(ParseTreeNodeType loop_type, ParseTreeNode* condition, ParseTreeNode* body);
ParseTreeNode* buildExpressionNode(ParseTreeNodeType expr_type, ParseTreeNode* left, ParseTreeNode* right);
ParseTreeNode* buildIdentifierNode(const wchar_t* name);
ParseTreeNode* buildLiteralNode(const wchar_t* value, int literal_type);

// دوال عرض شجرة الإعراب
void printParseTree(ParseTree* tree);
void printParseTreeNode(ParseTreeNode* node, int depth);
void printParseTreeToFile(ParseTree* tree, const char* filename);
void printParseTreeToBuffer(ParseTree* tree, wchar_t* buffer, int buffer_size, int* pos);

// دوال تصدير شجرة الإعراب
int exportParseTreeToDot(ParseTree* tree, const char* filename);
int exportParseTreeToXML(ParseTree* tree, const char* filename);
int exportParseTreeToJSON(ParseTree* tree, const char* filename);

// دوال البحث في شجرة الإعراب
ParseTreeNode* findNodeByType(ParseTree* tree, ParseTreeNodeType type);
ParseTreeNode* findNodeByValue(ParseTree* tree, const wchar_t* value);
ParseTreeNode* findNodeByPosition(ParseTree* tree, int line, int column);
ParseTreeNode** findAllNodesByType(ParseTree* tree, ParseTreeNodeType type, int* count);

// دوال التحقق من صحة شجرة الإعراب
int validateParseTree(ParseTree* tree);
int validateParseTreeNode(ParseTreeNode* node);
int checkNodeConsistency(ParseTreeNode* node);

// دوال إحصائيات شجرة الإعراب
int getNodeCount(ParseTree* tree);
int getNodeCountByType(ParseTree* tree, ParseTreeNodeType type);
int getTreeDepth(ParseTree* tree);
int getNodeDepth(ParseTreeNode* node);

// دوال دعم العرض من اليمين إلى اليسار
void reverseChildrenOrder(ParseTreeNode* node);
void reverseTreeDirection(ParseTree* tree);
int isRightToLeftNode(ParseTreeNode* node);

// دوال مساعدة
const wchar_t* getNodeTypeName(ParseTreeNodeType type);
void setNodeExtraData(ParseTreeNode* node, void* data);
void* getNodeExtraData(ParseTreeNode* node);
int isLeafNode(ParseTreeNode* node);
int isInternalNode(ParseTreeNode* node);

// دوال إدارة الذاكرة
void resizeChildrenArray(ParseTreeNode* node, int new_size);
void compactChildrenArray(ParseTreeNode* node);

#endif // PARSETREE_H
