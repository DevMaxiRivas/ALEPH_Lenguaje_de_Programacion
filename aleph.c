/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* funciones de ayuda para la calculadora */
#include <math.h>
#include "aleph.h"

/*Operaciones Lista - Conjunto*/

int SonDistintosConj(struct dataType* Cand, struct dataType* P);
int NoEsRepetidoConjunto(struct dataType* Cand, struct dataType* P);

struct dataType* CreaNodoSET(){
	struct dataType* nvo;
	nvo=(struct dataType*)malloc(sizeof(struct dataType));
	nvo->nodeType=SET;
	nvo->data=NULL;
	nvo->next=NULL;
	return nvo;
}
struct dataType* CreaNodoLIST(){
	struct dataType* nvo;
	nvo=(struct dataType*)malloc(sizeof(struct dataType));
	nvo->nodeType=LIST;
	nvo->data=NULL;
	nvo->next=NULL;
	return nvo;
}
	
struct dataType* CreaNodoSTR(char* *C){
	struct dataType* nvo;
	nvo=(struct dataType*)malloc(sizeof(struct dataType));
	nvo->nodeType=STR;
	nvo->dataStr=*C;
	return nvo;
}
struct dataType* NodoPadre(char* c){
	struct dataType* CLS;
		if(c[0]=='{') 
			CLS=CreaNodoSET();
		else 
			CLS=CreaNodoLIST();
	
	return CLS;
	}
//quita primer y �ltimo caracter de la cadena
//asume que inicia y termina con {} o con []
char* clean(char* s){
	int i=1;
	char* aux;
	aux=(char*) malloc(sizeof(char)*255); 
	while (s[i]!='\0'){
		aux[i-1]=s[i];
		i++;
	}
	aux[i-2]='\0';
	return aux;
}
//de la cadena s extrae el primer elemento de la estructura
//asume llaves y corchetes balanceados
void getElem(char**s, char**aux){
	char* s_aux;
	int i=0;
	int cont_abre_l=0;
	int cont_cierra_l=0;
	int cont_abre_c=0;
	int cont_cierra_c=0;
	int j,salir;
		
	*aux=(char*) malloc(sizeof(char)*255); 
	s_aux=(char*) malloc(sizeof(char)*255);
	if ((*s)[0]!='{' && (*s)[0]!='[') {
		while ((*s)[i]!='\0' && (*s)[i]!=',') {
			(*aux)[i]=(*s)[i];
			i++;
		}
		(*aux)[i]='\0';
	} else {
		salir=0;
		while ((*s)[i]!='\0' && !salir) {
			(*aux)[i]=(*s)[i];
			switch ((*s)[i]) {
			case '{':
				cont_abre_l++;
				break;
			case '[':
				cont_abre_c++;
				break;	
			case '}':
				cont_cierra_l++;
				break;
			case ']':
				cont_cierra_c++;
				break;
			}
			if (cont_abre_l-cont_cierra_l==0 && cont_abre_c-cont_cierra_c==0) 
				salir=1;
			i++;
		}
		(*aux)[i]='\0';
	}
	i++;
	if (i<strlen(*s)) {
		j=0;
		while ((*s)[i]!='\0'){
			s_aux[j]=(*s)[i];
			j++;
			i++;
		}
		s_aux[j]='\0';
		*s=s_aux;
	} else *s=NULL;
}
void ConcatenaCaracter(struct dataType* P, char* *cad){ //Muestra Caracteres Nodo Char
 strcat(*cad,P->dataStr);
}	
void FormaAdentro(struct dataType* P, char* *cad){ //Caso Recursivo de los Nodos Hijos
	if(P->data!=NULL){					//Mientras que no sea el ultimo elemento
		switch((P->data)->nodeType){	//Determina el tipo de Nodo guardado en el Dato del Nodo Original
		case 1: 
			if(P->data!=NULL) ConcatenaCaracter(P->data, cad);	//Encontro un nodo char	
			if(P->next!=NULL) {
				strcat((*cad),",");			//Si hay siguiente pone , 
				FormaAdentro(P->next, cad);	}	// sigue analizando los demas elementos
			break;
		case 2: 
			strcat((*cad),"{");	
			if(P->data!=NULL)	FormaAdentro(P->data, cad); // Debe analizar que tiene adentro Nodo->data (puede ser SET o STR o LIST)
			strcat((*cad),"}");
			if(P->next!=NULL) {
				strcat((*cad),",");		
				FormaAdentro(P->next, cad);
			} 	// sigue analizando los demas elementos
			break;
		default: 
			strcat((*cad),"[");	
			if(P->data!=NULL) FormaAdentro(P->data, cad);  // Debe analizar que tiene adentro Nodo->data (puede ser SET o STR o LIST)
			strcat((*cad),"]");	
			if(P->next!=NULL) {
				strcat((*cad),",");		
				FormaAdentro(P->next, cad);  // sigue analizando los demas elementos
			}
		};
	}
}
char* 	FormaCadena(struct dataType* P){ //Caso Particular del Nodo Padre
	char*cad;
	cad=(char*) malloc(sizeof(char)*255);
	memset(cad, 0, 3); //Quita La Basura que tenia(Caso particular conjunto de un solo elemento)
	if(P->nodeType==2){
		strcat(cad,"{");
		if(P->data!= NULL) FormaAdentro(P,&cad);
		strcat(cad,"}");
	}
	else {
		if(P->nodeType==3){
		strcat(cad,"[");
		if(P->data!= NULL) FormaAdentro(P,&cad);
		strcat(cad,"]");
		}
		else if(P->dataStr!=NULL) {	
			strcat(cad,P->dataStr); //Caso un solo elemento 1611
		}
	}
	return cad;
}
int NoEsRepetidoCaracter(char* C, struct dataType* P){
	int aux; 
	
	if(C[0]!='[' && C[0]!='{'){//Si el elemento no es un conjunto o lista
	if(P->data!=NULL){//Mientras que el elemento no sea analizado hasta el final del conjunto
		if((P->data)->nodeType==1) { //Se comparan solo cadenas del conjunto
			
			aux=strcmp((P->data)->dataStr,C);//Se comparan dos elementos caracter
			if(aux==0) return 0;//Si es repetido retorna 0
			else return NoEsRepetidoCaracter(C, P->next);//Si no era repetido debe seguir analizandose con los demas elementos del conjunto
		}
		else return NoEsRepetidoCaracter(C, P->next);//Si el elemento del conjunto es una lista o conjunto se lo saltea
		}
	else {
		
		return 1;	//Caso en el que la cadena fue analizada por todo elemento del conjunto -> ya no tiene con quien compararse
		}
	}
	else return 1;//Caso en el que la cadena a analizar es un conjunto o lista
}
void RetornaNoRepetidoCaracter(char * *eleme, char* *resto, struct dataType* CLS){
	char *cand, *resto1; int b;
	cand=NULL;
	
	cand=(char*) malloc(sizeof(char)*255);
	resto1=(char*) malloc(sizeof(char)*255);
	
	strcpy(resto1,*resto);
	getElem(&resto1, &cand);//Caso particular daba error con el resto
	*resto=resto1;

	if(CLS->data==NULL) *eleme=cand;//Si es el primer elemento se retorna de igual forma
	else { 
		b=NoEsRepetidoCaracter(cand,CLS);//Compara elemento extraido
			if(b!=0)(*eleme)=cand;	//Encontro un no repetido
			else {
				if(*resto!=NULL){//Si no encontro pregunta si hay mas cadena para analizar
				RetornaNoRepetidoCaracter(eleme, resto, CLS);	//Si Hay cadena todavia se sigue analizando
				}
			}
	}
}
int Pertenece(struct dataType* Cand, struct dataType* P){
	int aux; 

	char *candLis, *PLis; //Caso particular de las listas
	if(P==NULL) return 1;//Si se hizo NULL el Elemento de Conj 1 no esta en el Conj 2 
	else{
		switch((Cand->data)->nodeType){	//Analizo que tipo de nodo es el elemento
		case 1: //Caso cadena
			if((P->data)->nodeType==1){ //Condiciono que se comparen si son cadenas los 2
				aux=strcmp((P->data)->dataStr,(Cand->data)->dataStr); //Comparo ambas cadenas
				if(aux==0) return 0; //Si son Cadenas el elemento pertenece al conjunto ->fin de la recursion
				else return Pertenece(Cand,P->next);	//Si no debo analizar con otro elemento del Conj 2
			}
			else return Pertenece(Cand,P->next); // Si el elemeto del Conj 2 no era cadena debo buscar otro elemento que si lo sea
			break;
		case 2: //Caso Conjunto
			if((P->data)->nodeType==2) return SonDistintosConj(Cand->data,P->data); //Condiciono que solo se comparen conjuntos y los Analizo particularmente
			else return Pertenece(Cand,P->next); // Si el elemeto del Conj 2 no era conjunto debo buscar otro elemento que si lo sea
			break;
		default:
			if((P->data)->nodeType==3){ //Condiciono que se comparen si son Listas los 2
				candLis=(char*) malloc(sizeof(char)*255);
				PLis=(char*) malloc(sizeof(char)*255);
				candLis=FormaCadena(Cand->data);
				PLis=FormaCadena(P->data);
				aux=strcmp(candLis, PLis); //Comparo ambas listas pues estas pueden analizarse como cadenas pues no son como los conjuntos
				if(aux==0) return 0;	//Si son iguales el elemento lista pertence a Conj 2 -> Fin de la Recursion
				else return Pertenece(Cand,P->next);	//Si no son Iguales debo seguir analizando si hay un lista igual en Conj 2
			}
			else return Pertenece(Cand,P->next);// Si el elemeto del Conj 2 no era lista debo buscar otro elemento que si lo sea
		};
	}
}
int SonDistintosConj(struct dataType* Cand, struct dataType* P){
	int aux;
if(Cand!=NULL){	//Mientras que halla un candidato a analizar
	if((Cand->data)->nodeType==1){
		aux=Pertenece(Cand,P); //Recorro el Conjunto 2 analizando Elemento de Conjunto 1
		if(aux==0){	//Si era repetido
			if(Cand->next==NULL ) return 0; //Si era repetido y ya no tengo siguiente para analizar corto la recursion
			else return SonDistintosConj(Cand->next,P);} //Si lo era tengo que seguir analizando
		else return 1;//No era repetido por tanto como no coinciden en un elemento los 2 conjuntos eran distintos
	}
	else{ if((P->data)->nodeType==2) aux=NoEsRepetidoConjunto(Cand->data,P);
		if(aux==1) return 1;
		else  return SonDistintosConj(Cand->next,P);
	}
}
else return 1; //Si Cand se hizo NULL quiere decir que recorrio todo elemento de Conj 1
	
}
int CuentaElem(struct dataType* C){
	if(C!=NULL && C->data!=NULL) {
		return 1 + CuentaElem(C->next);
	}
	else return 0;
}
int CuentaElementos(struct dataType* C){
	char* aux=FormaCadena(C);
	if(strcmp(aux,"{}")==0) {
		return 0;
	}
	else {
		if(strcmp(aux,"[]")==0) {
			return 0;
		}
		else return CuentaElem(C);
	}
}
int NoEsRepetidoConjunto(struct dataType* Cand, struct dataType* P){
	
	int aux, card1, card2; 
	if(P!=NULL && P->next!=NULL){		//Mientras no se haya analizado hasta el final
		if((P->data)->nodeType==2) {
			card1=CuentaElementos(Cand);
			card2=CuentaElementos(P->data);
			if(card1!=card2){ //Si no son de igual tamanio son distintos
				if(P->next==NULL) {
					aux=1; 
				}
				else aux=NoEsRepetidoConjunto(Cand, P->next); //Si son de Igual Tamaño se analizan los conjuntos
			}
			else {
				if(card1!=0 && card2!=0) aux=SonDistintosConj(Cand, P->data); //Analizo Si son Iguales los Conjuntos
				else aux=0;
			}
			if(aux==0) return 0;		//Si son Iguales retorno 0 fin de la recursion
			else return NoEsRepetidoConjunto(Cand, P->next); //Sigo revisando si hay algun conjunto igual
		}
		else return NoEsRepetidoConjunto(Cand, P->next); //Si el elemento no es conjunto los Salteo
	}
	else { //Si analizo todo el conjunto 2 entonces el conjunto 1 no estaba repetido
		return 1;	
	}
}
int NoEsRepetidoLista(char* C, struct dataType* P){
	char* cand;
	cand=(char*) malloc(sizeof(char)*255);
	int aux;
	if(P!=NULL && P->data!=NULL){//Mientras tenga elementos del por comparar conjunto
		if((P->data)->nodeType==3) { //Solo analizo listas
			cand=FormaCadena(P->data); //Genero cadena Lista con la lista perteneciente al conjunto
			aux=strcmp(cand,C);//Comparo cadenas
			if(aux==0) return 0;//Si son iguales->Fin de la Recursion
			else return NoEsRepetidoLista(C, P->next); //Si son distintas sigo comparando con el resto del conjunto
		}
		else return NoEsRepetidoLista(C, P->next);//Si no es lista sigo comparando con el resto del conjunto
	}
	else {
		return 1;	//Ya no tengo elementos con quien compararlo entonces no es repetido
		
	}
}
void Profundidad(char* C, struct dataType* *CLS, struct dataType* Base){
	char *resto, *eleme, *EsRep; 
	int aux;
	struct dataType*auxnodo; 
	eleme=NULL;
	if(C!=NULL){ //Mientras tenga cadena
	resto=(char*) malloc(sizeof(char)*255);
	EsRep=(char*) malloc(sizeof(char)*255);
	strcpy(resto,C);

	switch(C[0]){ //Se modifico caso {}
	case'{': 
		getElem(&resto,&eleme);
		if(Base->nodeType==2){//Caso Conjunto -> no debe haber repetidos
			auxnodo=CreaNodoSET();//Creo Nodo Auxiliar pues la cadena puede modificarse
				eleme=clean(eleme);
				Profundidad(eleme,&auxnodo,auxnodo);//Analizo los elementos del conjunto particularmente
				EsRep=FormaCadena(auxnodo);
				aux=NoEsRepetidoConjunto(auxnodo,Base);//Verifico que el conjunto no sea repetido
				if(aux != 0) { //Si no es Repetido lo agrego
					(*CLS)->data=auxnodo; 
					if(resto!=NULL){ //Si todavia tengo cadena sigo analizando el resto
						(*CLS)->next=CreaNodoSET();//Creo el siguiente nodo
						Profundidad(resto, &(*CLS)->next, Base);//Sigo analizando la cadena
					}
				}
				else {//Si era repetido el conjunto
					if(resto!=NULL) Profundidad(resto, CLS, Base); //Si me queda cadena sigo analizando
					else (*CLS)=NULL; //Si no apunto a NULL pues es el siguiente elemento de algun anterior
				}
		}
		else{//Caso Lista -> Se agregan repetidos
			(*CLS)->data=CreaNodoSET();//Creo Elemento Conjunto
			eleme=clean(eleme);
			Profundidad(eleme,&(*CLS)->data,(*CLS)->data);//Analizo los elementos del conjunto
			if(resto!=NULL){//Si todavia tengo cadena sigo analizandola
			(*CLS)->next=CreaNodoLIST(); 
			Profundidad(resto,&(*CLS)->next,(*CLS)->next);
			}
			else (*CLS)->next=NULL; //Si no apunto el siguiente a NULL 
		}
		break;
	case'[': 	
		getElem(&resto,&eleme);
		if(Base->nodeType==2){
			auxnodo=CreaNodoLIST(); //Creo Nodo Auxiliar
			eleme=clean(eleme);
			Profundidad(eleme,&auxnodo,auxnodo);//Analizo particularmente la lista
			EsRep=FormaCadena(auxnodo); //Verifico que la lista no este repetida mediante comparaciones de cadenas
			aux=NoEsRepetidoLista(EsRep,Base);
			if(aux!=0){ //Si no era repetida la agrego a la estructura
				(*CLS)->data=auxnodo;
				if(resto!=NULL){ //Si me queda cadena por analizar sigo
					(*CLS)->next=CreaNodoSET();
					Profundidad(resto, &(*CLS)->next, Base);
				}
			}
			else {//Si era una lista repetida
				if(resto!=NULL) Profundidad(resto, CLS, Base);//Analizo si tengo mas cadena para analizar
				else (*CLS)=NULL; //Si no tengo mas cadena apunto a NULL pues CLS es el siguiente del anterior
				}
		}
		else{//Caso Lista -> puede haber repetidos
			(*CLS)->data=CreaNodoLIST(); 
			eleme=clean(eleme);
			Profundidad(eleme,&(*CLS)->data,(*CLS)->data);
			if(resto!=NULL){
				(*CLS)->next=CreaNodoLIST();
				Profundidad(resto,&(*CLS)->next,Base);
			}
			else (*CLS)->next=NULL;
		}
		
		break;
	default: 
		if((*CLS)->nodeType==2){ 	//Caso Conjunto -> Sin Repetidos
			RetornaNoRepetidoCaracter(&eleme, &resto, Base); //Retorno Algun NO repetido
			if(eleme!=NULL){//Si encontre alguno
				if(eleme[0]!='[' && eleme[0]!='{'){ //Si no me devolvio una lista o conjunto
					(*CLS)->data=CreaNodoSTR(&eleme); //Agrego la cadena a la estructura
					if(resto!=NULL){ //Si tengo cadena por analizar
						(*CLS)->next=CreaNodoSET();
						Profundidad(resto, &(*CLS)->next, Base);
					}
				}
				else {//Si me devolvio una lista o conj debo analizarla particularmente
					auxnodo=CreaNodoSET(); 
					Profundidad(eleme, &auxnodo,Base);
					if(auxnodo!=NULL){ //Si no tuvo problemas de ser repetido entonces me devolvio distinto de NULL
						*CLS=auxnodo; //lo agrego a la estructura
						if(resto!=NULL){ //Si tengo cadena sigo analizando
							(*CLS)->next=CreaNodoSET(); 
							Profundidad(resto, &(*CLS)->next, Base);
						}
					}
					else {//Si era repetido el conj o la list entonces
						if(resto!=NULL)	Profundidad(resto, CLS, Base); //Si tengo cadena sigo analizando
						else (*CLS)=NULL; //Si no, apunto a NULL pues CLS es el siguiente de algun anterior
						}
					}
				}
			else (*CLS)=NULL;//si eleme == NULL no encontro ninguno NO repetido por tanto se termino la cadena, luego apunto a NULL pues CLS es el siguiente de algun anterior
		}
		else{//Caso Lista -> Permito Repetidos
			getElem(&resto,&eleme);
			(*CLS)->data=CreaNodoSTR(&eleme); 
			if(resto!=NULL){
				(*CLS)->next=CreaNodoLIST();
				Profundidad(resto, &(*CLS)->next, Base);
			}
		}
		};
	  }
	}
struct dataType* SeparaElemento(char* Cad){
		struct dataType* Orig; char* aux; 
		Orig=NodoPadre(Cad);
		aux=clean(Cad);
		Profundidad(aux, &Orig ,Orig);
		return Orig;
	}

	//Operaciones Conjuntos
char* CopiaCar(struct dataType* P){ //Muestra Caracteres Nodo Char
	char *cad;
	cad=(char*) malloc(sizeof(char)*255); 
	strcpy(cad,P->dataStr);
	return cad;
}	
	
void CopiaNodo(struct dataType* P,struct dataType**C){ //Caso Recursivo de los Nodos Hijos
	char* aux;
	if(P!=NULL&&P->data!=NULL){					//Mientras que no sea el ultimo elemento
		switch((P->data)->nodeType){	//Determina el tipo de Nodo guardado en el Dato del Nodo Original
		case 1: 
			aux=CopiaCar(P->data);
			(*C)->data=CreaNodoSTR(&aux);
														//Encontro un nodo char	
			if(P->next!=NULL){
				if(P->nodeType==2)(*C)->next=CreaNodoSET();
				else (*C)->next=CreaNodoLIST();
			CopiaNodo(P->next,&(*C)->next);			// sigue analizando los demas elementos
			}
			break;
		case 2: 
			(*C)->data=CreaNodoSET();
			CopiaNodo(P->data,&(*C)->data); // Debe analizar que tiene adentro Nodo->data (puede ser SET o STR o LIST)
			if(P->next!=NULL){
				if(P->nodeType==2)(*C)->next=CreaNodoSET();
				else (*C)->next=CreaNodoLIST();
			CopiaNodo(P->next,&(*C)->next);
			}	// sigue analizando los demas elementos
			break;
			default: 
				(*C)->data=CreaNodoLIST();
				CopiaNodo(P->data,&(*C)->data);  // Debe analizar que tiene adentro Nodo->data (puede ser SET o STR o LIST)
				if(P->next!=NULL){
					if(P->nodeType==2)(*C)->next=CreaNodoSET();
					else (*C)->next=CreaNodoLIST();
						CopiaNodo(P->next,&(*C)->next);
				}	
		};
	}
}
struct dataType* CopiaEstructura(struct dataType* P){ //Caso Particular del Nodo Padre
	struct dataType* cop;
	char *cad; 
	if(P!=NULL){
		switch(P->nodeType){
		case 2:
			cop=CreaNodoSET();		
			CopiaNodo(P, &cop);
		break;
		case 3:
			cop=CreaNodoLIST();		
			CopiaNodo(P, &cop);
		break;
		case 1: 
			cad=(char*) malloc(sizeof(char)*255);
			strcpy(cad,P->dataStr);
			cop=CreaNodoSTR(&cad);
			break;
		default: printf("error in function CopiaEst");
		}
		return cop;
	}
	else yyerror("error is NULL"); 
}
struct dataType* retornaUltimoNodo(struct dataType* P){
	struct dataType* aux;
	aux=P;
	
	if(aux->next!=NULL) {
		return retornaUltimoNodo(aux->next);
	}
	else {
		return aux;
	}
}//Se pudo hacer sin variable auxiliar
	
struct dataType* Union(struct dataType* A, struct dataType* B){
	struct dataType *Uni, *fin; Uni=NULL;
	char *CadB; CadB=NULL;
	CadB=(char*) malloc(sizeof(char)*255);
	if(A->nodeType==2 && B->nodeType==2){
		if(Card(A) == 0) return CopiaEstructura(B);
		if(Card(B) == 0) return CopiaEstructura(A);
		Uni=CopiaEstructura(A);
		CadB=FormaCadena(B);
		CadB=clean(CadB);
		fin=NULL;
		fin=retornaUltimoNodo(Uni);
		if(fin->data!=NULL)	{
			if(CadB[0]!='\0') {
				fin->next=CreaNodoSET();
				Profundidad(CadB, &fin->next, Uni); //ppor si clesan me devuelve una csadensa con \0
			}
		}
		else{
			Profundidad(CadB, &fin, Uni); //Se modifico caso en que el primero este vacio
		}
	}
	else {
		yyerror("Type error in union function");
  		exit(0);
	}
	return Uni;
	}

void Intersec(struct dataType* *nvo ,struct dataType* Cand, struct dataType* P){
	int aux; 
	char *aux1;
	aux1=(char*) malloc(sizeof(char)*255);
	if(Cand!=NULL){		
		switch((Cand->data)->nodeType){
		case STR: 
			aux=Pertenece(Cand,P);
			if(aux==0) {
				strcpy(aux1,(Cand->data)->dataStr);
				(*nvo)->data=CreaNodoSTR(&aux1);
				(*nvo)->next=CreaNodoSET();
				Intersec(&(*nvo)->next,Cand->next, P);
			}else Intersec(nvo, Cand->next, P);
			break;
		case LIST:
			aux=Pertenece(Cand,P);
			if(aux==0) {
				(*nvo)->data=CreaNodoSET();
				CopiaNodo(Cand->data,&(*nvo)->data);
				(*nvo)->next=CreaNodoSET();
				Intersec(&(*nvo)->next,Cand->next, P);
			}else Intersec(nvo, Cand->next, P);
			break;
		default:
			aux=Pertenece(Cand,P);
			if(aux==0) {
				(*nvo)->data=CreaNodoLIST();
				CopiaNodo(Cand->data,&(*nvo)->data);
				(*nvo)->next=CreaNodoSET();
				Intersec(&(*nvo)->next,Cand->next, P);
			}else Intersec(nvo, Cand->next, P);
			break;
		};
	}else (*nvo)=NULL;

}
struct dataType* Intersection(struct dataType* A, struct dataType* B){
	struct dataType *nvo, *aux;
	if(A->nodeType==2 && B->nodeType==2){
		nvo=CreaNodoSET();
		aux=CreaNodoSET();
		Intersec(&aux, A, B);
		if(aux!=NULL) nvo=aux;
	}
	else{
		yyerror("Type error in intersection function");
    exit(0);
		nvo=NULL;
	}
		return nvo;
}

bool estaEnEstruct(struct dataType* elem,struct dataType* Orig);

struct dataType* Difference(struct dataType* A, struct dataType* B){
	struct dataType *aux = A;
	struct dataType *nvo = CreaNodoSET();
	struct dataType *ele = CreaNodoSET();
	if(A->nodeType==2 && B->nodeType==2){
		if(Card(A)!= 0){
			while(aux!=NULL){
				if(!estaEnEstruct(aux->data,B)) {
					ele->data=aux->data;
					nvo=Union(nvo,ele);
				}
				aux=aux->next;
			}	
		}
		return nvo;
	}
	else{
		yyerror("Type error in difference function");
		exit(0);
		nvo=NULL;
	}
}

int Card(struct dataType* CL1){
	if(CL1->nodeType == 2)	return CuentaElementos(CL1);
	else {
	  yyerror("Type error in cardinal function");
   	  exit(0);
	  return -1;
	}
}
	//Operaciones Listas
int Size(struct dataType* CL1){
	if(CL1->nodeType == 3) return CuentaElementos(CL1);
	else {
		yyerror("Type error in size function");
    	exit(0);
		return -1;
	}
}
void Push(struct dataType *Cab, struct dataType *elem){
	struct dataType *aux;
	struct dataType *nvaCola = CreaNodoLIST();
	if((Cab)->nodeType == LIST){
		if(CuentaElementos(Cab)!=0){
			nvaCola->data=elem;
			aux=retornaUltimoNodo(Cab);
			aux->next=nvaCola;
		}
		else{
			Cab->data = elem;
		}
	}
	else {
		yyerror("Type error in push function");
    	exit(0);
	}
}
struct dataType* Pop(struct dataType* cab){
	struct dataType *aux;
	int tam;
  if((cab)->nodeType == 3){
    tam = Size(cab);
    if(tam>1){
      aux = cab->next;
      while (aux->next != NULL) {
        cab=cab->next;
        aux=cab->next;
      }
      cab->next=NULL;
      return aux->data;
    }
    else{
      if(tam == 1){
      aux=cab->data;
      cab->data=NULL;
      return aux;
      }
      else {
        yyerror("Error empty list in pop function"); 
        return NULL;
      }
    }
  }
  else{
    yyerror("Type error in pop function");
    exit(0);
  }
}
struct dataType* sacarPrim(struct dataType* cab){
	struct dataType *aux;
	int tam;
    tam = CuentaElementos(cab);
    if(tam>1){
      aux = cab->next;
      while (aux->next != NULL) {
        cab=cab->next;
        aux=cab->next;
      }
      cab->next=NULL;
      return aux->data;
    }
    else{
      if(tam == 1){
      aux=cab->data;
      cab->data=NULL;
      return aux;
      }
      else {
        yyerror("Error empty list in pop function"); 
        return NULL;
      }
    }
}

bool estaEnEstruct(struct dataType* elem,struct dataType* Orig){
	struct dataType* auxOrig = Orig;
	if(Orig!=NULL){
		char* elem2 = FormaCadena(elem);
		char* elem1 = FormaCadena(auxOrig->data);
		while(auxOrig != NULL && strcmp(elem1,elem2)!=0){
			free(elem1);
			auxOrig=auxOrig->next;
			if(auxOrig!=NULL)elem1 = FormaCadena(auxOrig->data);
		}
		free(elem2);
		if(auxOrig == NULL) return false;
		else return true;
	}
	else{
		yyerror("error in comparation");
		return false;
	}
}
bool contenidoIG(struct dataType* conj1,struct dataType* conj2){
	struct dataType* auxconj1 = conj1;
		if(Card(conj1) == 0) return true; 
		if(Card(conj1) <= Card(conj2)){
			while(auxconj1 != NULL && estaEnEstruct(auxconj1->data,conj2)){
				auxconj1 = auxconj1->next;
			}
			if(auxconj1 == NULL) return true;
			else return false;
		}
		else return false;
}
bool contenidoNIG(struct dataType* conj1,struct dataType* conj2){
		if(Card(conj1) == 0) return true;
		if(Card(conj1) < Card(conj2)){
			return contenidoIG(conj1,conj2);
		}
		else return false;
}
bool sublistaIG(struct dataType* list1,struct dataType* list2){
	struct dataType* auxlist1 = list1;
	struct dataType* auxlist2 = list2;
	char *elem1, *elem2;
	int cmp = 0;
		if(Size(list1) <= Size(list2)){
			while(auxlist1 != NULL && cmp!=1){
				elem1 = FormaCadena(auxlist1->data);
				elem2 = FormaCadena(auxlist2->data);
				cmp = strcmp(elem1,elem2);
				auxlist1 = auxlist1->next;
				auxlist2 = auxlist2->next;
			}
			if(auxlist1 == NULL && cmp == 0) return true;
			else return false;
		}
		else return false;
}
bool sublistaNIG(struct dataType* list1,struct dataType* list2){
		if(Size(list1) < Size(list2)){
			return contenidoIG(list1,list2);
		}
		else return false;
}
bool igualesEst(struct dataType* s1,struct dataType* s2){
	char *c1,*c2;
	int cmp;
	if(s1!=NULL && s2!=NULL){
		c1 = FormaCadena(s1);
		c2 = FormaCadena(s2);
		cmp=strcmp(c1,c2);
		free(c1);
		free(c2);
		if(cmp==0) return true;
		else return false;
	}
	else yyerror("error in comparation, any source is null");
}
bool noIgualesEst(struct dataType* s1,struct dataType* s2){
	return !(igualesEst(s1,s2));
}
int returnType(struct dataType* CL){
	return CL->nodeType;
}

/*Aleph*/
/*---------------------------------------------*/

/* tabla de símbolos */
/* aplicar hash a un símbolo */
static unsigned symhash(char *sym){
    unsigned int hash = 0;
    unsigned c;
    
    while(c = *sym++) hash = hash*9 ^ c;
    
    return hash;
}

struct symbol *lookup(char *sym){
    struct symbol *sp = &symtab[symhash(sym)%NHASH];
    int scount = NHASH;
    
    while(--scount >= 0){
        if(sp->name && !strcmp(sp->name, sym)) 
            return sp;
        
        if(!sp->name){
            sp->name = strdup(sym);
            sp->value = 0;
            sp->func = NULL;
            sp->syms = NULL;
            return sp;
        }
        
        if(++sp >= symtab+NHASH)    /* prueba con la siguiente entrada */
            sp = symtab;
    }
    yyerror("la tabla de símbolos está agotada\n");
    abort();
}

/* creación de nodos del árbol de sintaxis abstracta */

struct ast *newast(int nodetype, struct ast *l, struct ast *r){
    struct ast *a = malloc(sizeof(struct ast));
    
    if(!a){
        yyerror("There is no space");
        exit(0);
    }
    
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r){
    struct ast *a = malloc(sizeof(struct ast));
    
    if(!a){
        yyerror("There is no space");
        exit(0);
    }
    
    a->nodetype = '0' + cmptype;
    a->l = l;
    a->r = r;
    return a;
}
struct ast *newfunc(int functype, struct ast *l){
    struct fncall *a = malloc(sizeof(struct fncall));
    
    if(!a){
        yyerror("There is no space");
        exit(0);
    }
    
    a->nodetype = 'F';
    a->l = l;
    a->functype = functype;
    return (struct ast *)a;
}
struct ast *newcall(struct symbol *s, struct ast *l){
    struct ufncall *a = malloc(sizeof(struct ufncall));
    
    if(!a){
        yyerror("There is no space");
        exit(0);
    }
    
    a->nodetype = 'C';
    a->l = l;
    a->s = s;
    return (struct ast *)a;
}

struct ast *newref(struct symbol *s){
    struct symref *a = malloc(sizeof(struct symref));
    
    if(!a){
        yyerror("There is no space");
        exit(0);
    }
    
    a->nodetype = 'N';
    a->s = s;
    return (struct ast *)a;
}
struct ast *newasgn(struct symbol *s, struct ast *v){
    struct symasgn *a = malloc(sizeof(struct symasgn));
    
    if(!a){
        yyerror("There is no space");
        exit(0);
    }
    
    a->nodetype = '=';
    a->s = s;
    a->v = v;
    return (struct ast *)a;
}
struct ast *newflow(int nodetype, struct ast *cond, 
                    struct ast *tl, struct ast *el){
    struct flow *a = malloc(sizeof(struct flow));
    
    if(!a){
        yyerror("There is no space");
        exit(0);
    }
    
    a->nodetype = nodetype;
    a->cond = cond;
    a->tl = tl;
    a->el = el;
    return (struct ast *)a;
}
struct ast *newfor(int nodetype, struct symbol *s, 
                    struct ast *e, struct ast *l){
	return newflow(nodetype,(struct ast*)s,e,l);
}

struct symlist *newsymlist(struct symbol *sym, struct symlist *next){
    struct symlist *sl = malloc(sizeof(struct symlist));
    
    if(!sl){
        yyerror("There is no space");
        exit(0);
    }
    
    sl->sym = sym;
    sl->next = next;
    return sl;
}
/* liberar una lista de símbolos */
void symlistfree(struct symlist *sl){
    struct symlist *nsl;
    
    while(sl){
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}
struct ast* newElem(char* cad){
  struct dataType* nuevo = (struct dataType*)malloc(sizeof(struct dataType));
	nuevo->dataStr= (char*) malloc (sizeof(char) * 255);

	if(!nuevo){
		yyerror("There is no space");
		exit(0);
	}

	nuevo->nodeType = STR;
	strcpy(nuevo->dataStr,cad);
  
  return (struct ast*)nuevo;
}
struct dataType* evalList(struct ast* ast, int nodeType){
  struct dataType *aux;
   struct dataType *nvo;
   if(ast!=NULL){
    switch (nodeType) {
    case SET: 
        nvo = CreaNodoSET();
        if(ast->l!=NULL) { 
          aux=eval(ast->l);
          nvo->data=aux;
          }
        if(ast->r!=NULL) nvo->next = evalList(ast->r,SET);
      break;
    case LIST: 
        nvo = CreaNodoLIST(); 
        if(ast->l!=NULL) { 
          aux=eval(ast->l);
          nvo->data=aux;
          }
        if(ast->r!=NULL) nvo->next = evalList(ast->r,LIST);
          break;
    case STR: 
        nvo = eval(ast); 
    break;
    default:printf("\nError in function evalist, node found %d", ast->nodetype);
    };
  }
  else{
    switch (nodeType) {
    case SET: 
        nvo = CreaNodoSET();
      break;
    case LIST: 
        nvo = CreaNodoLIST(); 
      break;
    case STR: 
        printf("\n Entro con NULL por elemento");
    break;
    default:printf("\nError in function evalist, node found %d", ast->nodetype);
    };
  }
      return nvo;
}
bool symL(struct dataType* a1,struct dataType* a2){
	if(a1->nodeType == SET && a2->nodeType == SET) return contenidoNIG(a1,a2);
	else{
		if(a1->nodeType == LIST && a2->nodeType == LIST) return sublistaNIG(a1,a2);
		else yyerror("error in comparation\n");
	}
}
bool symLE(struct dataType* a1,struct dataType* a2){
	if(a1->nodeType == SET && a2->nodeType == SET) return contenidoIG(a1,a2);
	else{
		if(a1->nodeType == LIST && a2->nodeType == LIST) return sublistaIG(a1,a2);
		else yyerror("error in comparation\n");
	}
}
bool symD(struct dataType* s1,struct dataType* s2){
	return noIgualesEst(s1,s2);
}
bool symE(struct dataType* s1,struct dataType* s2){
	return igualesEst(s1,s2);
}
bool symB(struct dataType* s1,struct dataType* s2){
	return estaEnEstruct(s1,s2);
}
char* dataStr(struct dataType* CL){
    if(CL!=NULL) return FormaCadena(CL);
	else{
		yyerror("struct is NULL in function dataSTR");
		return " ";
	}
}

struct dataType* evalFE(struct flow *aux){
	struct symbol* s = (struct symbol*)aux->cond;
	struct dataType* v = NULL;
	struct dataType* exp;
	switch(eval(aux->tl)->nodeType){
	case SET:
		exp = CopiaEstructura(eval(aux->tl));
		struct dataType* analiz = CreaNodoSET(); 
		struct dataType* res= CreaNodoSET();
			while(CuentaElementos(exp) != 0){
				s->value = CopiaEstructura(sacarPrim(exp));
				analiz->data=CopiaEstructura(s->value);
				res=Union(res,analiz);
				v = eval(aux->el);				
				exp=Difference(eval(aux->tl),res);
			}
			free(res);
			free(exp);
			free(analiz);
	break;
	case LIST:
		exp = eval(aux->tl);
		while(exp!=NULL){
			s->value = CopiaEstructura(exp->data);
			v = eval(aux->el);	
			exp = exp->next;
		}
	break;
	default: yyerror("Error in sentece for_each");
	};
	return v;
}

struct dataType* evalLog(struct ast* a){
	struct dataType* v;
	if(a->nodetype != '&' && a->nodetype != '|'){ 
		return eval(a);
	}
	else{
		switch(a->nodetype){
			case '&': 
				if((bool)eval(a->l) == true && (bool)evalLog(a->r) == true){
					return (struct dataType*)true;	
				}
				else{
					return (struct dataType*)false;	
				}
			break;
			case '|':
				if((bool)eval(a->l) == false && (bool)evalLog(a->r) == false){
					return (struct dataType*)false;
				}
				else{
					return (struct dataType*)true;
				}				
			break;
			default: yyerror("Error in function evalLog");
			return NULL;
		}
	}
}
struct dataType* evalRNum(struct ast *a, int caso){
  struct dataType* nvo;
  char*cad; char*aux; int num;
  cad=(char*) malloc(sizeof(char)*255);
  aux=(char*) malloc(sizeof(char)*255);
  if(caso=='C') {
    num=Card(eval(a->l));
    aux=itoa(num,aux,10);
  }
  else {
    num=Size(eval(a->l));
    aux=itoa(num,aux,10);
  }
  memset(cad, 0, 3); 
  strcat(cad,"'");
  strcat(cad,aux);
  strcat(cad,"'");
  nvo=CreaNodoSTR(&cad);
  return nvo;
}
static struct dataType* callbuiltin(struct fncall *);
static struct dataType* calluser(struct ufncall *);

struct dataType* eval(struct ast *a){
    struct dataType* v;
    char*cadAux;
    if(!a){
        yyerror("internal error, null evaluation");
        return NULL;
    }
    switch(a->nodetype){
      
        /* referencia */
        case 'N': v = ((struct symref *)a)->s->value; break;
        
        /* asignación */
        case '=': v = ((struct symasgn *)a)->s->value = CopiaEstructura(eval(((struct symasgn *)a)->v)); 
		break;
        
        /* expresiones */
        case '+': v = Union(eval(a->l),eval(a->r)); break;
        case '-': v = Difference(eval(a->l),eval(a->r)); break;
        case '*': v = Intersection(eval(a->l),eval(a->r)); break;
		case 'T': v = evalRNum(a,'C'); break;
        case 'S': v = evalRNum(a,'S'); break;
        case 'P': v = Pop(eval(a->l)); break;
        case 'H': v = NULL; Push(eval(a->l),eval(a->r)); break;  

        /* arbol dataType*/
        case 'D': printf("Error: node found D in function eval"); break;
        case SET: cadAux=FormaCadena(evalList(a->l,SET));
                  v = SeparaElemento(cadAux); 
                  free(cadAux);
                  break;
        case LIST: v =evalList(a->l,LIST); break;
        case STR: v = CreaNodoSTR(&((struct dataType*)a)->dataStr); break;

        /* comparaciones */
        case '4': v = (struct dataType *)(symL(eval(a->l),eval(a->r))); break; 
        case '5': v = (struct dataType *)(symD(eval(a->l),eval(a->r))); break;
        case '6': v = (struct dataType *)(symE(eval(a->l),eval(a->r))); break;
        case '7': v = (struct dataType *)(symLE(eval(a->l),eval(a->r))); break;
        case '8': v = (struct dataType *)(symB(eval(a->l),eval(a->r))); break;
		
		/* operaciones logicas*/
		case '!': if((bool)eval(a->l)) v = (struct dataType *)false;
				  else v = (struct dataType *)true;
		break;
        /* flujo de control */
        /* las expresiones nulas están permitidas en la gramática
         * por lo tanto se deben chequear
         */
        
        /* if-else */
        case 'I':
            if((bool)(evalLog(((struct flow *)a)->cond)) != false){
                if(((struct flow *)a)->tl){
                    v = eval(((struct flow *)a)->tl);
                }else
                    v = NULL;        /* valor por defecto */
            }else{
                if(((struct flow *)a)->el){
                    v = eval(((struct flow *)a)->el);
                }else
                    v = NULL;
            }
            break;
            
        /* while */
        case 'W':
            v = NULL;        
            if(((struct flow *)a)->tl){
                while((bool)(evalLog(((struct flow *)a)->cond)) != false)
                    v = eval(((struct flow *)a)->tl);
            }
            break;

		/*	for each in */
        case 'E':
			v = NULL;
			if(((struct flow *)a)->el)
				v = evalFE((struct flow *)a);
		break;
        /* lista de sentencias */
        case 'L': eval(a->l); v = eval(a->r); break;

		/*Llamada a una funcion definida*/
        case 'F': v = callbuiltin((struct fncall *)a); break;

		/*Llamada a una funcion definida por el usuario*/
        case 'C': v = calluser((struct ufncall *)a); break;

        /*Sentencia return*/
		case 'R': v = (struct dataType*)a; break;
        
        default: printf("internal error: incorrect node %d\n", a->nodetype);
    }
    return v;
}
/* liberar un árbol AST */
void treefree(struct ast *a){
    switch(a->nodetype){
        case '+':
        case '-':
        case '*':
        case '4': case '5': case '6': case '7': case '8': 
        case '|': case '&':
        case 'L':
            treefree(a->r);   
        case 'C': case 'F': case '!':
		case 'T': case 'S': case 'P': case 'H':
            treefree(a->l);   
        case 'N': case 'D': case SET: case LIST: case STR: case 'E': case 'R':
            break;     
        case '=':
            free(((struct symasgn*)a)->v);
            break;
        case 'I': case 'W':
            free(((struct flow *)a)->cond);
            if(((struct flow *)a)->tl)
                treefree(((struct flow *)a)->tl);
            if(((struct flow *)a)->el)
                treefree(((struct flow *)a)->el);
            break;
        default: 
            printf("error interno: libera un nodo incorrecto %c\n", a->nodetype);
    }
    free(a);    /* siempre liberar el nodo actual */
}
void start(struct ast *a){
	switch(a->nodetype){
		case 'F':  case '=': case 'I': case 'W': case 'N': case 'C': case'H': case 'E':
			eval(a);
		break;
		case 'R': yyerror("invalid return statement"); break;
		default: printf("= %s\n",dataStr(eval(a)));
		treefree(a);
	}
}
static struct dataType* callbuiltin(struct fncall *f){
    enum bifs functype = f->functype;
    struct dataType* v = eval(f->l);
    switch(functype){ 
        case B_print:
            if(((struct ast*)(f->l))->nodetype == 'N') printf("%s ",((struct symref *)(f->l))->s->name);
            printf("= %s\n", dataStr(v));
            return v;
        default:
            yyerror("unknown built-in function %d", functype);
            return NULL;
    }
}

/* definir una función */
void dodef(struct symbol *name, struct symlist *syms, struct ast *func){
    if(name->syms) symlistfree(name->syms);
    if(name->func) treefree(name->func);
    name->syms = syms;
    name->func = func;
}

static struct dataType* calluser(struct ufncall *f){
    struct symbol *fn = f->s;   /* nombre de la función */
    struct symlist *sl;         /* argumentos formales */
    struct ast *args = f->l;    /* argumentos actuales */
    struct dataType* v;
    int nargs;
    int i;
    
    if(!fn->func){
        yyerror("undefined function call: %s", fn->name);
        return NULL;
    }
    
    /* cuenta los argumentos */
    sl = fn->syms;
    for(nargs = 0; sl; sl = sl->next)
        nargs++;
    
    /* preparación para salvarlos */
    struct dataType* oldval[nargs]; 	/*Estructuras para salvar argumentos*/
    struct dataType* newval[nargs];
    if(!oldval || !newval){
        yyerror("There is no place in %s", fn->name); 
        return NULL;
    }
    
    /* evalúa los argumentos */
    for(i=0; i<nargs; i++){
        if(!args){
            yyerror("Arguments are missing in the call to %s", fn->name);
            free(oldval);
            free(newval);
            return NULL;
        }
        
        if(args->nodetype=='L'){    /* si es un nodo lista */
            newval[i] = CopiaEstructura(eval(args->l));
            args = args->r;
        }else{                      /* si es el final de la lista */
            newval[i] = CopiaEstructura(eval(args));
            args = NULL;
        }
    }
    
    /* salva los viejos valores de los argumentos formales, 
     * asigna los nuevos nodos
     */
    sl = fn->syms;
    for(i=0; i<nargs; i++){
        struct symbol* s = sl->sym;
        
        oldval[i] = s->value;
        s->value = newval[i];
        sl = sl->next;
    }
    
    free(newval);
    
    /* evalúa la función */
    v = eval(fn->func);
    
    /* pone los valores reales de los args formales de nuevo */
    sl = fn->syms;
    for(i=0; i<nargs; i++){
        struct symbol *s = sl->sym;
        
        s->value = oldval[i];
        sl = sl->next;
    }
    
    free(oldval);
	if(((struct ast*)v)->nodetype == 'R') {
		return CopiaEstructura(eval(((struct ast*)v)->l));
		}
	else {
		return NULL;
	}
}