%define api.pure full
%locations
%param { yyscan_t scanner }

%code top {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudba.h"
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

%union{
  int i;
  float f;
  char *s;
  Column column;
  Columns columns;
  }

/* Data types */
%token<f> YY_FLOATNUM
%token<s> YY_ID
%token<i> YY_INTNUM
%token<s> YY_STRING

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

%type <column> type spec
%type <columns> spec-list

%start script

%%

script:
  query ';'
| script query ';'

query:
  create-query 
| drop-query   
| insert-query { fputs("200 inserted\n", stderr); }
| select-query { fputs("200 selected\n", stderr); }
| update-query { fputs("200 updated\n", stderr); }
| delete-query { fputs("200 deleted\n", stderr); }
;

create-query:
  YY_CREATE YY_TABLE YY_ID '(' spec-list ')' {
    sudba_create_database($3, $5);
  };

spec-list: /* a comma-separated list of column declarations */
  spec {
    /* The first column declaration */
    $$.declarations = malloc(sizeof(Column));
    $$.declarations[0] = $1;
    $$.number = 1;
  }
| spec-list ',' spec {
  /* More column declarations */
  $$.declarations = realloc($1.declarations, sizeof(Column) * ($1.number + 1));
  $$.declarations[$$.number] = $3;
  $$.number++;
  }
;

spec: /* column name and datatype, as in "id int, name char(16)" */
  YY_ID type { $2.name = $1; $$ = $2; } ;

type: /* on of the three supported datatypes */
  YY_INT                   { Column c = { COL_INT,   0, NULL }; $$ = c; } 
| YY_FLOAT                 { Column c = { COL_FLOAT, 0, NULL }; $$ = c; } 
| YY_CHR '(' YY_INTNUM ')' { Column c = { COL_STR,  $3, NULL }; $$ = c; } 
;

drop-query:
  YY_DROP YY_TABLE YY_ID { sudba_drop_database($3); }
;

insert-query: /* TODO */
  YY_INSERT YY_INTO YY_ID YY_VALUES '(' valist ')'
;

valist:
  value
| valist ',' value
;

value:
  YY_INTNUM
| YY_FLOATNUM
| YY_STRING
;

select-query: /* TODO */
  YY_SELECT column-list YY_FROM tables where-clause
;

column-list:
  column
| column-list ',' column
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

column-ref:
  YY_ID
| YY_ID '.' YY_ID
;

column-or-val:
  column-ref
| value
;

condition:
  column-ref '='    column-or-val
| column-ref '>'    column-or-val
| column-ref '<'    column-or-val
| column-ref YY_NEQ column-or-val
| column-ref YY_GE  column-or-val
| column-ref YY_LE  column-or-val
;

delete-query: /* TODO */
  YY_DELETE YY_FROM YY_ID where-clause
;

update-query: /* TODO */
  YY_UPDATE YY_ID YY_SET YY_ID '=' column-or-val where-clause
; 

%%
