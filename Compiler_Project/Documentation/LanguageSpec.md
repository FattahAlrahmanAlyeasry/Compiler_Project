# مواصفات اللغة البرمجة العربية
## Arabic Programming Language Specification

### نظرة عامة (Overview)
اللغة البرمجة العربية هي لغة برمجة عالية المستوى مصممة خصيصاً للبرمجة باللغة العربية. تدعم اللغة جميع المفاهيم الأساسية للبرمجة مع الحفاظ على البساطة والوضوح.

The Arabic Programming Language is a high-level programming language specifically designed for programming in Arabic. It supports all fundamental programming concepts while maintaining simplicity and clarity.

### خصائص اللغة (Language Features)
- **كتابة من اليمين إلى اليسار**: تدعم اتجاه الكتابة العربي
- **كلمات محجوزة عربية**: جميع الكلمات المحجوزة مكتوبة بالعربية
- **دعم Unicode كامل**: يدعم جميع أحرف اللغة العربية
- **أنواع بيانات متعددة**: أعداد صحيحة، أعداد حقيقية، نصوص، منطقية
- **هياكل تحكم قوية**: شروط، حلقات، إجراءات
- **هياكل بيانات مركبة**: مصفوفات، سجلات، قوائم

### قواعد النحو (Grammar Rules) - BNF/EBNF

#### 1. البرنامج (Program)
```
<program> ::= 'برنامج' <identifier> '؛'
              <block>
              'نهاية' '؛'
```

#### 2. الكتلة (Block)
```
<block> ::= '[' <definitions_part> ']' ':'
            <statements_list>
            ']'
```

#### 3. جزء التعريفات (Definitions Part)
```
<definitions_part> ::= <constants_definition>
                     | <types_definition>
                     | <variables_definition>
                     | <procedures_definition>
                     | ε
```

#### 4. تعريف الثوابت (Constants Definition)
```
<constants_definition> ::= 'ثوابت' ':'
                          <constant_declaration_list>
                          '؛'
```

```
<constant_declaration_list> ::= <constant_declaration>
                               | <constant_declaration_list> <constant_declaration>
```

```
<constant_declaration> ::= <identifier> '=' <constant_value> '؛'
```

```
<constant_value> ::= <integer_literal>
                   | <float_literal>
                   | <string_literal>
                   | <boolean_literal>
```

#### 5. تعريف الأنواع (Types Definition)
```
<types_definition> ::= 'أنواع' ':'
                      <type_declaration_list>
                      '؛'
```

```
<type_declaration_list> ::= <type_declaration>
                           | <type_declaration_list> <type_declaration>
```

```
<type_declaration> ::= <identifier> '=' <type_definition> '؛'
```

```
<type_definition> ::= 'عدد'
                    | 'حقيقي'
                    | 'حرف'
                    | 'منطقي'
                    | 'نص'
                    | 'مصفوفة' '[' <integer_literal> ']' 'من' <type_definition>
                    | 'سجل' '{' <field_list> '}'
```

```
<field_list> ::= <field_declaration>
                | <field_list> '؛' <field_declaration>
```

```
<field_declaration> ::= <identifier> ':' <type_definition>
```

#### 6. تعريف المتغيرات (Variables Definition)
```
<variables_definition> ::= 'متغيرات' ':'
                          <variable_declaration_list>
                          '؛'
```

```
<variable_declaration_list> ::= <variable_declaration>
                               | <variable_declaration_list> <variable_declaration>
```

```
<variable_declaration> ::= <identifier_list> ':' <type_definition> '؛'
```

```
<identifier_list> ::= <identifier>
                    | <identifier_list> '،' <identifier>
```

#### 7. تعريف الإجراءات (Procedures Definition)
```
<procedures_definition> ::= <procedure_declaration>
                           | <procedures_definition> <procedure_declaration>
```

```
<procedure_declaration> ::= 'إجراء' <identifier>
                           '(' <parameter_list> ')' ':'
                           <block>
                           '؛'
```

```
<parameter_list> ::= ε
                   | <parameter>
                   | <parameter_list> '،' <parameter>
```

```
<parameter> ::= <identifier> ':' <type_definition>
```

#### 8. قائمة العبارات (Statements List)
```
<statements_list> ::= <statement>
                    | <statements_list> <statement>
```

#### 9. العبارات (Statements)
```
<statement> ::= <assignment_statement>
              | <input_statement>
              | <output_statement>
              | <conditional_statement>
              | <loop_statement>
              | <procedure_call_statement>
              | <compound_statement>
              | <empty_statement>
```

#### 10. عبارة الإسناد (Assignment Statement)
```
<assignment_statement> ::= <variable> ':=' <expression> '؛'
```

```
<variable> ::= <identifier>
             | <array_element>
             | <record_field>
```

```
<array_element> ::= <identifier> '[' <expression> ']'
```

```
<record_field> ::= <identifier> '.' <identifier>
```

#### 11. عبارة الإدخال (Input Statement)
```
<input_statement> ::= 'اقرأ' '(' <variable_list> ')' '؛'
```

```
<variable_list> ::= <variable>
                  | <variable_list> '،' <variable>
```

#### 12. عبارة الإخراج (Output Statement)
```
<output_statement> ::= 'اطبع' '(' <expression_list> ')' '؛'
```

```
<expression_list> ::= <expression>
                    | <expression_list> '،' <expression>
```

#### 13. عبارة الشرط (Conditional Statement)
```
<conditional_statement> ::= 'إذا' <condition> 'فإن'
                           <statement>
                           'وإلا' <statement>
                           'نهاية' '؛'
```

```
<condition> ::= <expression>
```

#### 14. عبارة الحلقة (Loop Statement)
```
<loop_statement> ::= 'طالما' <condition> 'كرر'
                    <statement>
                    'نهاية' '؛'
                  | 'كرر' <statement> 'حتى' <condition> '؛'
```

#### 15. استدعاء الإجراء (Procedure Call Statement)
```
<procedure_call_statement> ::= <identifier> '(' <argument_list> ')' '؛'
```

```
<argument_list> ::= ε
                  | <expression>
                  | <argument_list> '،' <expression>
```

#### 16. العبارة المركبة (Compound Statement)
```
<compound_statement> ::= 'بداية' <statements_list> 'نهاية' '؛'
```

#### 17. العبارة الفارغة (Empty Statement)
```
<empty_statement> ::= '؛'
```

#### 18. التعبيرات (Expressions)
```
<expression> ::= <simple_expression>
               | <simple_expression> <relational_operator> <simple_expression>
```

```
<simple_expression> ::= <term>
                      | <simple_expression> <additive_operator> <term>
```

```
<term> ::= <factor>
         | <term> <multiplicative_operator> <factor>
```

```
<factor> ::= <variable>
           | <constant_value>
           | '(' <expression> ')'
           | <unary_operator> <factor>
```

#### 19. العوامل (Operators)

##### العوامل الحسابية (Arithmetic Operators)
```
<additive_operator> ::= '+' | '-'
<multiplicative_operator> ::= '*' | '/' | '^' | '%'
<unary_operator> ::= '+' | '-'
```

##### العوامل المقارنة (Relational Operators)
```
<relational_operator> ::= '=' | '<>' | '<' | '>' | '<=' | '>='
```

##### العوامل المنطقية (Logical Operators)
```
<logical_operator> ::= 'و' | 'أو' | 'لا'
```

#### 20. المعرفات (Identifiers)
```
<identifier> ::= <letter> <identifier_part>
```

```
<identifier_part> ::= ε
                    | <letter> <identifier_part>
                    | <digit> <identifier_part>
```

```
<letter> ::= 'أ' | 'ب' | 'ت' | 'ث' | 'ج' | 'ح' | 'خ' | 'د' | 'ذ' | 'ر' | 'ز' | 'س' | 'ش' | 'ص' | 'ض' | 'ط' | 'ظ' | 'ع' | 'غ' | 'ف' | 'ق' | 'ك' | 'ل' | 'م' | 'ن' | 'ه' | 'و' | 'ي' | 'ة' | 'ى'
```

```
<digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
```

#### 21. الثوابت (Literals)
```
<integer_literal> ::= <digit> <digit_part>
```

```
<digit_part> ::= ε
                | <digit> <digit_part>
```

```
<float_literal> ::= <integer_literal> '.' <digit_part>
```

```
<string_literal> ::= '"' <string_content> '"'
```

```
<string_content> ::= ε
                   | <character> <string_content>
```

```
<character> ::= <letter> | <digit> | <special_character>
```

```
<special_character> ::= ' ' | '!' | '@' | '#' | '$' | '%' | '&' | '*' | '(' | ')' | '-' | '_' | '+' | '=' | '[' | ']' | '{' | '}' | '|' | '\' | ':' | ';' | '"' | ''' | '<' | '>' | ',' | '.' | '?' | '/'
```

```
<boolean_literal> ::= 'صحيح' | 'خطأ'
```

### أنواع البيانات المدعومة (Supported Data Types)

#### 1. الأنواع الأساسية (Basic Types)
- **عدد (Integer)**: أعداد صحيحة 32 بت
- **حقيقي (Float)**: أعداد حقيقية 32 بت
- **حرف (Char)**: حرف واحد 16 بت (Unicode)
- **منطقي (Boolean)**: صحيح أو خطأ
- **نص (String)**: سلسلة من الأحرف

#### 2. الأنواع المركبة (Composite Types)
- **مصفوفة (Array)**: مجموعة من العناصر من نفس النوع
- **سجل (Record)**: مجموعة من الحقول بأسماء مختلفة
- **قائمة (List)**: مجموعة ديناميكية من العناصر

### هياكل التحكم المدعومة (Supported Control Structures)

#### 1. الشروط (Conditionals)
- **إذا-فإن-وإلا**: تنفيذ شرطي
- **إذا-فإن**: تنفيذ شرطي بدون بديل

#### 2. الحلقات (Loops)
- **طالما**: حلقة مع شرط مسبق
- **كرر-حتى**: حلقة مع شرط لاحق
- **كرر**: حلقة لا نهائية مع خروج شرطي

#### 3. الإجراءات (Procedures)
- **إجراء**: تعريف دالة بدون إرجاع قيمة
- **استدعاء**: تنفيذ إجراء مع معاملات

### أمثلة على البرامج (Program Examples)

#### مثال 1: برنامج بسيط (Simple Program)
```
برنامج مرحبا؛
[
  متغيرات:
    اسم: نص؛
    عمر: عدد؛
]:
  اطبع("أدخل اسمك: ")؛
  اقرأ(اسم)؛
  اطبع("أدخل عمرك: ")؛
  اقرأ(عمر)؛
  اطبع("مرحباً "، اسم، "، عمرك "، عمر، " سنة")؛
]
نهاية؛
```

#### مثال 2: برنامج مع شروط (Program with Conditions)
```
برنامج حساب؛
[
  متغيرات:
    رقم: عدد؛
]:
  اطبع("أدخل رقماً: ")؛
  اقرأ(رقم)؛
  إذا رقم > 0 فإن
    اطبع("الرقم موجب")؛
  وإلا
    اطبع("الرقم سالب أو صفر")؛
  نهاية؛
]
نهاية؛
```

#### مثال 3: برنامج مع حلقات (Program with Loops)
```
برنامج جدول_الضرب؛
[
  متغيرات:
    رقم: عدد؛
    عداد: عدد؛
]:
  اطبع("أدخل رقماً: ")؛
  اقرأ(رقم)؛
  عداد := 1؛
  طالما عداد <= 10 كرر
    اطبع(رقم، " × "، عداد، " = "، رقم * عداد)؛
    عداد := عداد + 1؛
  نهاية؛
]
نهاية؛
```

#### مثال 4: برنامج مع إجراءات (Program with Procedures)
```
برنامج إجراءات؛
[
  متغيرات:
    أ: عدد؛
    ب: عدد؛
    نتيجة: عدد؛
  
  إجراء جمع(س: عدد، ص: عدد):
    اطبع("مجموع "، س، " و "، ص، " هو "، س + ص)؛
  ؛
]:
  اطبع("أدخل الرقم الأول: ")؛
  اقرأ(أ)؛
  اطبع("أدخل الرقم الثاني: ")؛
  اقرأ(ب)؛
  جمع(أ، ب)؛
]
نهاية؛
```

#### مثال 5: برنامج مع مصفوفات (Program with Arrays)
```
برنامج مصفوفة؛
[
  متغيرات:
    أرقام: مصفوفة[5] من عدد؛
    عداد: عدد؛
]:
  عداد := 0؛
  طالما عداد < 5 كرر
    اطبع("أدخل الرقم "، عداد + 1، ": ")؛
    اقرأ(أرقام[عداد])؛
    عداد := عداد + 1؛
  نهاية؛
  
  اطبع("الأرقام المدخلة: ")؛
  عداد := 0؛
  طالما عداد < 5 كرر
    اطبع(أرقام[عداد])؛
    إذا عداد < 4 فإن
      اطبع("، ")؛
    نهاية؛
    عداد := عداد + 1؛
  نهاية؛
]
نهاية؛
```

#### مثال 6: برنامج مع سجلات (Program with Records)
```
برنامج طالب؛
[
  أنواع:
    طالب = سجل {
      اسم: نص؛
      عمر: عدد؛
      معدل: حقيقي؛
    }؛
  
  متغيرات:
    ط: طالب؛
]:
  اطبع("أدخل اسم الطالب: ")؛
  اقرأ(ط.اسم)؛
  اطبع("أدخل عمر الطالب: ")؛
  اقرأ(ط.عمر)؛
  اطبع("أدخل معدل الطالب: ")؛
  اقرأ(ط.معدل)؛
  
  اطبع("معلومات الطالب:")؛
  اطبع("الاسم: "، ط.اسم)؛
  اطبع("العمر: "، ط.عمر)؛
  اطبع("المعدل: "، ط.معدل)؛
]
نهاية؛
```

### قواعد إضافية (Additional Rules)

#### 1. التعليقات (Comments)
```
<comment> ::= '//' <comment_content> <new_line>
```

#### 2. الفراغات (Whitespace)
- يتم تجاهل الفراغات والتباعدات
- يتم تجاهل الأسطر الفارغة
- يتم تجاهل علامات التبويب

#### 3. أسماء المعرفات (Identifier Naming)
- يجب أن تبدأ بحرف عربي
- يمكن أن تحتوي على أحرف عربية وأرقام
- لا يمكن أن تكون كلمة محجوزة
- حساسة لحالة الأحرف

#### 4. نطاق المتغيرات (Variable Scope)
- المتغيرات معرفة في النطاق الذي تم إعلانها فيه
- يمكن الوصول للمتغيرات من النطاقات الداخلية
- لا يمكن إعادة تعريف متغير في نفس النطاق

### أخطاء شائعة (Common Errors)

#### 1. أخطاء معجمية (Lexical Errors)
- استخدام أحرف غير مسموحة
- أسماء معرفات غير صحيحة
- ثوابت غير صحيحة

#### 2. أخطاء نحوية (Syntax Errors)
- عدم تطابق الأقواس
- عدم استخدام الفاصلة المنقوطة
- ترتيب خاطئ للكلمات المحجوزة

#### 3. أخطاء دلالية (Semantic Errors)
- استخدام متغير غير معرف
- عدم تطابق الأنواع
- إعادة تعريف متغير

### ملاحظات التطبيق (Implementation Notes)

#### 1. دعم Unicode
- جميع النصوص تُخزن كـ UTF-8
- دعم كامل للغة العربية
- معالجة صحيحة لاتجاه النص

#### 2. إدارة الذاكرة
- تخصيص ديناميكي للمصفوفات
- إدارة تلقائية للنطاقات
- تنظيف تلقائي للذاكرة

#### 3. التحسينات
- تحسين التعبيرات
- تقليل استخدام الذاكرة
- تحسين سرعة التنفيذ

### خاتمة (Conclusion)
هذه المواصفات تحدد اللغة البرمجة العربية بشكل كامل وتوفر أساساً قوياً لبناء مترجم فعال. اللغة مصممة لتكون سهلة التعلم والفهم مع الحفاظ على القوة والمرونة في البرمجة.

This specification completely defines the Arabic Programming Language and provides a solid foundation for building an effective compiler. The language is designed to be easy to learn and understand while maintaining power and flexibility in programming.

