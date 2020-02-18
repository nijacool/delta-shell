%{
  /* This section is included into the output file verbatim */
  #include "susql-parser.h"
%}

/* May flex options that make the code thread-safe */
%option noinput nounput noyywrap 8bit nodefault                                 
%option reentrant bison-bridge bison-locations                                  
%option extra-type="int"

/* Useful macros; do you still remember regular expressions? */
intnum        [-+]?[[:digit:]]+
floatnum      [-+]?([[:digit:]]+\.[[:digit:]]*)|(\.[[:digit:]]+)
id            (?:\w+\s+)([a-zA-Z_][a-zA-Z0-9_]*)
string        \"([^\n\\\"]|\\.)*\"

/* Rules for recognizing tokens; (?i:...) allows case-insensitive
 * matching. Extract comments, keywords, operators, literals. We
 * ignore spaces. Everything else is reported to the parser, let it
 * handle junk */ 
%%
"-- ".*
"#".*
(?i:char)   { return YY_CHR; }
(?i:create) { return YY_CREATE; }
(?i:delete) { return YY_DELETE; } 
(?i:drop)   { return YY_DROP; }
(?i:from)   { return YY_FROM; }
(?i:insert) { return YY_INSERT; }  
(?i:int)    { return YY_INT; }
(?i:into)   { return YY_INTO; }
(?i:float)  { return YY_FLOAT; }
(?i:select) { return YY_SELECT; } 
(?i:set)    { return YY_SET; }
(?i:table)  { return YY_TABLE; }
(?i:update) { return YY_UPDATE; } 
(?i:values) { return YY_VALUES; }
(?i:where)  { return YY_WHERE; }

"!="        { return YY_NEQ; }
"<="        { return YY_LE; }
">="        { return YY_GE; }
[()*,.;<>=] { return yytext[0]; }

{intnum}    { int i; i = atoi(yytext); return YY_INTNUM; }
{floatnum}  { float f; f = atof(yytext); return YY_FLOATNUM; }
{id}        { return YY_ID; }
{string}    { return YY_STRING; }

[[:space:]]+

.           { return yytext[0]; }

%%

/* Parser driver */
int sudba_parse(int socket /* unused */)
{
  socket = 0; // Keep the compiler happy; will fix later
  
  yyscan_t scanner;  
  yylex_init(&scanner);
  int status = yyparse(scanner);
  yylex_destroy(scanner);
  
  return status;
}
