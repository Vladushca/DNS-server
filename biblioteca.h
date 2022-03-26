#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>


typedef void (*TAF)(void*, FILE *);
typedef int (*TFCmp)(void*, void*);  /* functie de comparare doua elemente */
typedef void (*TF)(void*);           /* functie afisare/eliberare un element */
typedef int (*TFHash)(void*, int);


typedef struct celula2 {
  void *info;                   /* adresa element extern */
  struct celula2 *pre, *urm;    /* legaturi spre celulele vecine */
} TCelula2, *TLDI;

typedef struct {
    char *Key;
    char *Value;
} Server;


typedef struct {
    size_t M;
    TFHash fh;
    TLDI *v;
} TH;

/* functii TLDI */
TLDI AlocareLDI(void*);
void AfisareLDI(TLDI *lista, TAF afiesareSite, FILE *out);
void EliminareCelulaLDI (TLDI * lista, TLDI addresa, TF distrugeEl);
void DistrugeTLDI (TLDI * l, TF Distruge);
void InserareDupaLDI(TLDI *lista, void *server, TFCmp cmp);
TLDI findKey(void *key, TH *ah, TFCmp cmp);
void afisareSite (void * server, FILE * out);


/* functii tabela hash */
TH* InitializareTH (size_t M, TFHash fh);
void AfisareTH (TH*a, TAF afiesareSite, FILE * out);
void DistrugeTH (TH** ah, TF fe);
Server * AlocareServer(void* a, void* b);
void DistrugeServer (void * serv);
int cmp (void *a, void *b);
void Remove(TH* h, void *el,TF DistrugeServer);
