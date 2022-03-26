#include "biblioteca.h"

//functia de initializare a unei tabele hash cu M vectori de elemente
TH* InitializareTH (size_t M, TFHash fh) {
    TH* h_table = (TH *) calloc(sizeof(TH), 1);
    if (!h_table) {
        printf("Eroare alocare hash\n");
        return NULL;
    }

    h_table->M = M;   
    h_table->fh = fh;   //functia de hashuire
    h_table->v = (TLDI *) calloc(M, sizeof(TLDI));  //vectorul de liste generice

    if(!h_table->v) {
        printf("Eroare alocare vector in hash\n");
        free(h_table);
        return NULL;
    } 

    return h_table;
}


//functia de afisare a unui tabel hash
void AfisareTH(TH* h, TAF afisareSite, FILE * out) {
    TLDI p, addr;
    int i;
    
    //pentru a traversa fiecare TLDI
    for(i = 0; i < h->M; i++) {
        p = h->v[i];  //adresa inceputului TLDI
        
        if(p) {
            //afisarea elementului de inceput a listei
            fprintf (out, "%d: ",i);
  			addr = p;                   // salvez inceputul listei
            afisareSite (p->info, out);
            fprintf (out, " ");
            p = p->urm;
            
           // afisarea restului listei
            for(; p != addr; p = p->urm) {
            	afisareSite (p->info, out);
            	fprintf (out, " ");
            } 
            fprintf (out, "\n");
        }
    }
}

//functia de distrugere de tabela hash
void DistrugeTH(TH ** h, TF fe) {
    TLDI * p, cell;

    //parcurgere cu pointeri
    for (p = (*h)->v; p < (*h)->v + (*h)->M; p++) {
        cell = *p; //adresa de inceput a listei
        if((*p)) {
            DistrugeTLDI (&cell, fe);
        } else {
            continue;
        }
    }

    free((*h)->v);
    free(*h);
    *h = NULL;
}
