#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "map.h"
#include "SortedMap.h"

typedef struct{
   char* cons;
   char* creado;
   char* modificado;
   char* descripcion;
   char* titulo;
   char* url;
   char* tipo;
   double imb_rating;
   int minutos;
   int fecha2;
   char* genero;
   int votos;
   char* fecha;
   char* director;
   int favorito;
}pelicula;

typedef struct{
char *nombre;
list *lista_archivos;
int   puntaje;
}lista_pelicula;

lista_pelicula *crear_nombre_archivo(const char*file);
pelicula *crear_pelicula (char* cons, char* creado,char* modificado,char* descripcion,char* titulo,char* url,char* tipo,double imb_rating,int minutos,int fecha2,char* genero,int votos,char* fecha,char* director);
const char *get_csv_field (char * tmp, int k) ;

int cmp(const void * key1, const void * key2) {
    const double * Key1 = key1;
    const double * Key2 = key2;
    if(*Key1 > *Key2) return -1;
    if(*Key1 < *Key2) return +1;
    if(*Key1 == *Key2) return 0;
}

int cmp2(const void * key1, const void * key2) {
    const char * Key1 = key1;
    const char * Key2 = key2;
    return strcmp(Key1,Key2);
}



int main(){

    SortedMap *todas_las_peliculas = createSortedMap(cmp);
    SortedMap *mapa_de_archivos = createSortedMap(cmp2);
    int boton = 1;
    printf("------------------------------------------------\n");
    printf("|             BIENVENIDO A CINE.COM            |\n");
    printf("------------------------------------------------\n");
    char file [1000];
    while(boton!=0){
    printf("INGRESE EL NOMBRE DEL ARCHIVO CSV\n");
    printf("EJEMPLO: BestAction.csv (DEBE AGREGAR .csv)\n");
    scanf("%100[^\n]s", file);
    getchar();
    printf("\n");
    agregar_pelicula (file,todas_las_peliculas,mapa_de_archivos);
    printf("DESEA CONTINUAR?\nPRESIONE\n");
    printf("1- PARA CONTINUAR\n0- PARA SALIR\n");
    scanf("%d",&boton);
    getchar();
    printf("\n");
    }


}

void agregar_pelicula (char* file,SortedMap*todas_las_peliculas,SortedMap* mapa_de_archivos){
if(searchSortedMap(mapa_de_archivos,file) != NULL){
    printf("ESTE ARCHIVO YA SE ENCUENTRA\n\n");
    return;
}

FILE* fp = fopen (file , "r");
char linea [1000];
lista_pelicula* n = NULL;
SortedMap* peliculas_individual = createSortedMap(cmp);
n = crear_nombre_archivo (file);
pelicula* comprobar = NULL;
fgets(linea,1000,fp);
while (fgets (linea, 1023, fp) != NULL) {
   comprobar = searchSortedMap (todas_las_peliculas,get_csv_field(linea,1));
   if(comprobar==NULL){
   char* titulo      = get_csv_field(linea,5);
   char* cons        = get_csv_field(linea,1);
   char* creado      = get_csv_field(linea,2);
   char* modificado  = get_csv_field(linea,3);
   char* descripcion = get_csv_field(linea,4);
   char* url         = get_csv_field(linea,6);
   char* tipo        = get_csv_field(linea,7);
   double imb_rating = atof(get_csv_field(linea,8));
   int minutos       = atoi(get_csv_field(linea,9));
   int* fecha2       = atoi(get_csv_field(linea,10));
   char* genero      = get_csv_field(linea,11);
   int votos         = atoi(get_csv_field(linea,12));
   char* fecha       = get_csv_field(linea,13);
   char* director    = get_csv_field(linea,14);
   pelicula *p = crear_pelicula (cons,creado,modificado,descripcion,titulo,url,tipo,imb_rating,minutos,fecha2,genero,votos,fecha,director);
   list_push_back (n->lista_archivos,peliculas_individual);
   insertSortedMap(peliculas_individual,&p->imb_rating,p);
   insertSortedMap(todas_las_peliculas,&p->imb_rating,p);
   }else{
   printf("YA SE ENCUENTRA\n");
   }
   }

    fclose(fp);
    insertSortedMap(mapa_de_archivos,n->nombre,n);

    //mostrar lista de peliculas
    lista_pelicula *aux = firstSortedMap(mapa_de_archivos);
     while(aux){
        printf("NOMBRE: %s | PUNTAJE: %d\n",aux->nombre,aux->puntaje);
        aux=nextSortedMap(mapa_de_archivos);
     }
     printf("\n");
}

lista_pelicula *crear_nombre_archivo (const char* file){
lista_pelicula *n = malloc (sizeof(lista_pelicula));
n->nombre= file;
n->lista_archivos = list_create_empty ();
n->puntaje = 0;
return n;
}




pelicula *crear_pelicula (char* cons, char* creado,char* modificado,char* descripcion,char* titulo,char* url,char* tipo,double imb_rating,int minutos,int fecha2,char* genero,int votos,char* fecha,char* director){
pelicula*p=malloc(sizeof(pelicula));
p->cons        = cons;
p->creado      = creado;
p->modificado  = modificado;
p->descripcion = descripcion;
p->titulo      = titulo;
p->url         = url;
p->tipo        = tipo;
p->imb_rating  = imb_rating;
p->minutos     = minutos;
p->fecha2      = fecha2;
p->genero      = genero;
p->votos       = votos;
p->fecha       = fecha;
p->director    = director;
p->favorito    = 0;
return p;
}

const char *get_csv_field (char * tmp, int k) {
    //se crea una copia del string tmp

    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}
