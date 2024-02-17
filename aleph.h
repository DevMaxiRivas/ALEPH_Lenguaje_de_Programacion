#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Declaraciones para una calculadora con funciones
 */

/* interfaz para el lexer */
extern int yylineno;
void yyerror(char *s, ...);

/* tabla de símbolos */
struct symbol{
    char *name;
    struct dataType* value;
    struct ast *func;
    struct symlist *syms;
};

/* tabla de símbolos simple de tamaño fijo */
#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

/* lista de símbolos, para una lista de argumentos */
struct symlist{
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* tipos de nodos
 * + - * / |
 * 0-7 ops de comparación, codificados en bits 04 igual, 02 menor, 01 mayor
 * M menos unario
 * L expresión o lista de sentencias
 * I sentencia IF
 * W sentencia WHILE
 * N referencia a un símbolo
 * = asignación
 * S lista de símbolos
 * F llamada a una función incorporada
 * C llamada a una función de usuario
 */

enum bifs{              /* funciones incorporadas */
    B_sqrt = 1,
    B_exp,
    B_log,
    B_print
};

/* nodos en el árbol de sintaxis abstracta */
/* todos tienen un nodetype inicial en común */

struct ast{
    int nodetype;
    struct ast *l;
    struct ast *r;
};
struct fncall{
    int nodetype;
    struct ast *l;
    enum bifs functype;
};
struct ufncall{         /* funciones de usuario */
    int nodetype;
    struct ast *l;
    struct symbol *s;
};
struct flow{
    int nodetype;       /* tipo I o W */
    struct ast *cond;   /* condición */
    struct ast *tl;     /* rama "then" o lista "do" */
    struct ast *el;     /* rama opcional "else" */
};
struct symref{
    int nodetype;       /* tipo N */
    struct symbol *s;
};
struct symasgn{
    int nodetype;       /* tipo = */
    struct symbol *s;
    struct ast *v;      /* valor */
};

/*Aleph*/
#define STR 1
#define SET 2
#define LIST 3

struct dataType{
	int nodeType;
	union{
		char *dataStr;
		struct{
			struct dataType *data;
			struct dataType *next; 
		};
	};
};

/* construcción del AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
/*struct ast *newnum(double d);*/
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);
struct ast *newfor(int nodetype, struct symbol *s, struct ast *e, struct ast *l);
/* definir una función */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* evaluar un AST */
struct dataType* eval(struct ast *);

/* borrar y liberar un AST */
void treefree(struct ast *);

/*crear un datatype de tipo ELEM"*/
struct ast* newElem(char* s);

/*crear cadena a partir de un dataType*/
char* dataStr(struct dataType* CL);

/*Inicio*/
void start(struct ast *a);

/*dataType*/
struct dataType* CreaNodoSET();
struct dataType* CreaNodoLIST();
struct dataType* CreaNodoSTR(char* *C);

int Size(struct dataType* CL1);
int Card(struct dataType* CL1);
struct dataType* Difference(struct dataType* A, struct dataType* B);
struct dataType* Intersection(struct dataType* A, struct dataType* B);
struct dataType* Union(struct dataType* A, struct dataType* B);

void Push(struct dataType* Cab, struct dataType* C);
struct dataType* Pop(struct dataType* Cab);
struct dataType* RemoverPorCabeza(struct dataType* *Cab);
void AgregarPorCabeza(struct dataType* *Cab, char *C);
char* dataStr(struct dataType* P);
struct dataType* CopiaEstructura(struct dataType* P);
int returnType(struct dataType* CL);