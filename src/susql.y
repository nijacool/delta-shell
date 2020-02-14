%define api.pure full
%locations
%param { yyscan_t scanner }

%code top {
#include <stdio.h>
 }
%code requires {
  int sudba_parse(int socket /* unused */);
  typedef void* yyscan_t;
  FILE *get_yyout(yyscan_t scanner);
 }
%code {
  int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
  void yyerror(YYLTYPE* yyllocp, yyscan_t unused, const char* msg);
 }

/* Data types */
%token YY_FLOATNUM
%token YY_ID
%token YY_INTNUM
%token YY_STRING

 /* Keywords */
%token YY_CHR
%token YY_CREATE
%token YY_DELETE
%token YY_DROP
%token YY_FLOAT
%token YY_FROM
%token YY_GE
%token YY_INSERT
%token YY_INT
%token YY_INTO
%token YY_LE
%token YY_NEQ
%token YY_SELECT
%token YY_SET
%token YY_TABLE
%token YY_UPDATE
%token YY_VALUES
%token YY_WHERE

%start script

%%

script:
  query ';'
| script query ';'

query:
  %empty /* empty query is ok! */
| create-query { fputs("200 created\n", stderr); }
| drop-query { fputs("200 dropped\n", stderr); }
| insert-query { fputs("200 inserted\n", stderr); }
| select-query { fputs("200 selected\n", stderr); }
| update-query { fputs("200 updated\n", stderr); }
| delete-query { fputs("200 deleted\n", stderr); }
;

create-query:
  YY_CREATE YY_TABLE YY_ID '(' specs ')';

specs:
  spec
| specs ',' spec
;

spec:
  YY_ID type ;

type:
  YY_INT
| YY_FLOAT
| YY_CHR '(' YY_INTNUM ')'
;

drop-query:
  YY_DROP YY_TABLE YY_ID
;

insert-query:
  YY_INSERT YY_INTO YY_ID YY_VALUES '(' valist ')'
;

valist:
  value
| valist ',' value
;

value:
  YY_INTNUM
| YY_FLOATNUM
| YY_STRING ;

select-query:
  YY_SELECT columns YY_FROM tables where-clause
;

columns:
  column
| columns ',' column
;

column:
  YY_ID
| YY_ID '.' YY_ID
| '*'
;

tables :
  YY_ID
| tables ',' YY_ID
;

where-clause:
  %empty /* empty clause */
| YY_WHERE condition
;

column-spec:
  YY_ID
| YY_ID '.' YY_ID
;

column-or-val:
  column-spec
| value
;

condition:
  column-spec '=' column-or-val
| column-spec '>' column-or-val
| column-spec '<' column-or-val
| column-spec YY_NEQ column-or-val
| column-spec YY_GE column-or-val
| column-spec YY_LE column-or-val
;

delete-query:
  YY_DELETE YY_FROM YY_ID where-clause
;

update-query:
  YY_UPDATE YY_ID YY_SET YY_ID '=' column-or-val where-clause
; 

%%
