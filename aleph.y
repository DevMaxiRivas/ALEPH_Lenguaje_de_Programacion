%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "aleph.h"
extern FILE *yyin;
extern int yylex();
int yyerrork;
%}

%union{
    struct ast *a;
    char *cad;
    struct symbol *s;
    struct symlist *sl;
    int fn;
}

/* declaración de tokens */
%token <s> NAME
%token <fn> FUNC
%token <cad> ELEM   
%token EOL
%token IF ELSE WHILE LET PUSH POP CARD SIZE FOR_EACH IN AND OR NOT RETURN

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*'

%type <a> exp stmt list explist conj lis expr_list exprCP exprSP oplog
%type <sl> symlist

%start aleph

%%
stmt:   IF oplog '{' EOL list '}'                        {$$ = newflow('I',$2,$5,NULL);}
|       IF oplog '{' EOL list '}' ELSE '{' EOL list '}'  {$$ = newflow('I',$2,$5,$10);}
|       WHILE oplog '{' EOL list '}'                     {$$ = newflow('W',$2,$5,NULL);}
|       FOR_EACH NAME IN exp '{' EOL list '}'            {$$ = newfor('E',$2,$4,$7);}
|       RETURN exp ';'                                   {$$ = newast('R',$2,NULL);}
|       exp ';'                                          {$$ = $1;}
;
list:                               {$$ = NULL;}
|      stmt EOL list                {if ($3 == NULL)
                                        $$ = $1;
                                     else
                                        $$ = newast('L',$1,$3);
                                    }
;
oplog:  exp AND oplog           {$$ = newast('&',$1,$3);}
|       exp OR oplog            {$$ = newast('|',$1,$3);}
|       exp                     
;
exp:    NOT exp CMP exp         {$$ = newast('!',newcmp($3,$2,$4),NULL);}
|       exp CMP exp             {$$ = newcmp($2,$1,$3);}
|       exprCP
|       exprSP
|       NAME '=' exp            {$$ = newasgn($1,$3);}
;
exprSP:    CARD '(' exprCP ')'            { $$ = newast('T',$3,NULL); }
        | SIZE '(' exprCP ')'            { $$ = newast('S',$3,NULL); }
        | POP '(' exprCP ')'             { $$ = newast('P',$3,NULL); }
        | PUSH '(' exprCP  ',' exprCP ')'  { $$ = newast('H',$3,$5); }
;
exprCP: exprCP '+' exprCP           {$$ = newast('+',$1,$3);}
|       exprCP '-' exprCP           {$$ = newast('-',$1,$3);}
|       exprCP '*' exprCP           {$$ = newast('*',$1,$3);}
|       '(' exprCP ')'              {$$ = $2;}
|       conj                        {$$ = $1;}
|       lis                         {$$ = $1;}
|       ELEM                        {$$ = newElem($1); } 
|       NAME                        {$$ = newref($1);}
|       FUNC '(' explist ')'        {$$ = newfunc($1,$3);}
|       NAME '(' explist ')'        {$$ = newcall($1,$3);}
;
conj:'{''}'                               {$$ = newast(SET,NULL,NULL);}
        |'{' expr_list '}'                {$$ = newast(SET,$2,NULL);}
;
lis:'['']'                                {$$ = newast(LIST,NULL,NULL);}
        |'[' expr_list ']'                {$$ = newast(LIST,$2,NULL);}
;
expr_list: exprCP                         {$$ = newast('D',$1,NULL);}
        | exprCP ',' expr_list            {$$ = newast('D',$1,$3);}
;
explist:    exp
|           exp ',' explist         {$$ = newast('L',$1,$3);}
;

symlist:    NAME                    {$$ = newsymlist($1,NULL);}
|           NAME ',' symlist        {$$ = newsymlist($1,$3);}
;

aleph:       
|           aleph stmt EOL          {start($2); }
|           aleph LET NAME '(' symlist ')' '{' EOL list '}' EOL
                                    {dodef($3,$5,$9);
                                     printf("Defined %s\n",$3->name);}
|           aleph error EOL         {yyerrork;}
;
%%

void yyerror(char *s, ...){
    va_list ap;
    va_start(ap,s);

    fprintf(stderr, "%d: error: ", yylineno-1);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

int main(int argc, char *argv[]){
    yyin = stdin;
    printf("Starting\n");

    if(argc>1){
        FILE *archivo = fopen(argv[1],"rt");
        if(!archivo){
            printf("Error reading the file\n");
            return -1;
        }
        else yyin = archivo;
    }
    else yyin = stdin;

    do{
        if(yyparse()!=0){
            printf("Syntax error\n");
            exit(0);
        }
    }while(!feof(yyin));

    return 0;
}
