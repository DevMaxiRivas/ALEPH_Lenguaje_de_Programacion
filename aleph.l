/* reconocimiento de tokens para aleph */
%option noyywrap nodefault yylineno case-insensitive
%{
#include "aleph.h"
#include "aleph.tab.h"
%}


%%
 /*Comentarios de una linea*/
"/*".*"*/"[\n]*  {}
 /* operadores de caracter simple */
"+" |
"-" |
"*" |
"/" |
"=" |
"|" |
"," |
";" |
"(" |
")" |
"{" |
"}" |
"[" |
"]"         {return yytext[0];}

 /* Operadores logicos */
"!"              {return NOT;}
"&&"             {return AND;}
"||"             {return OR;}

 /* operadores de comparación, todos como el token CMP */
"<"         {yylval.fn = 4; return CMP;}
"!="        {yylval.fn = 5; return CMP;}
"=="        {yylval.fn = 6; return CMP;}
"<="        {yylval.fn = 7; return CMP;}
"//"        {yylval.fn = 8; return CMP;}

 /* palabras clave */
"if"             {return IF;}
"else"           {return ELSE;}
"while"          {return WHILE;}
"let"            {return LET;}
"for"[ \t]"each" {return FOR_EACH;}
"in"             {return IN;}
"return"         {return RETURN;}


"pop"        {return POP;}
"push"       {return PUSH;}
"card"       {return CARD;}
"size"       {return SIZE;}

 /* funciones incluidas */
"print"     {yylval.fn = B_print; return FUNC;}

 /*Elementos*/
"'"[a-z0-9]*"'"                    {yylval.cad = strdup(yytext); return ELEM;}
 /* nombres */
[a-z][a-z0-9]*"_"*[a-z0-9]*        {yylval.s = lookup(yytext); return NAME;}


[ \t]                       {}

\\\n                           {printf("c> ");}  /* ignora continuación de línea */
[\n]*                          {return EOL;}

.                              {yyerror("Caracter misterioso %c\n", *yytext);}

%%

