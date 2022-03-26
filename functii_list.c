#include "biblioteca.h"

//alocarea unei structuri cu Key, Value
Server * AlocareServer(void *a, void *b) {
   
    char * site = (char*) a;
    char * ip = (char*) b;
    
    Server * serv = malloc (sizeof (Server));
    serv->Key = malloc (sizeof (char) * strlen(site) + 1);
    serv->Value = malloc (sizeof (char) * strlen(ip) + 1);
    
    strcpy(serv->Key, site);
    strcpy(serv->Value, ip);
    
    return serv;
}

//afisarea campului Value, unei structuri Server, adica ip-ul site-ului
void afisareSite (void * server, FILE * out){
    Server * site = (Server *) server;
    fprintf (out, "%s", site->Value);
}

//distrugerea unei structuri Server
void DistrugeServer (void * serv){
    Server * s = (Server *) serv;
    
    free(s->Key);
    free(s->Value);
    free(s);
}



//functia de alocare a unei liste generice
TLDI AlocareLDI (void * x) {
	TLDI cell = (TLDI) malloc (sizeof (TCelula2));
	if(!cell) return 0;
	
	cell->info = x;
	cell->pre = cell;
	cell->urm = cell;
    
    return cell;
}


//functia de afisare a unei liste generice folosind functia de afisare de value
void AfisareLDI(TLDI *lista, TAF afisareSite, FILE *out) {
	TLDI p = *lista;
	
	if(!p) {
		return;
    }

	afisareSite (p->info, out);
    fprintf(out," ");
    p = p->urm;

	for(; p != *lista; p = p->urm) {
		afisareSite (p->info, out);
		fprintf (out, " ");
	}

	fprintf (out, "\n");
}


//functia de inserare a unei celule in TLDI
void InserareDupaLDI (TLDI *lista, void * server, TFCmp cmp) {
    TLDI aux = AlocareLDI(server);
    TLDI p = *lista;

    if (!(*lista)){    //daca lista nu contine elemente
        *lista = aux;
        return;
    }

//cazul de inserare a unui elem mai "mic" decat unicul elem existent in lista
    if (cmp (p->info, server) > 0){
        aux->urm = *lista;
        aux->pre = (*lista)->pre;
        (*lista)->pre->urm = aux;
        (*lista)->pre = aux;
        *lista = aux;
        return;
    }
//cazul de insearare a unui elem in interiorul unei liste cu multiple elemente
//verific cu 2 cmp-uri pentru a-i gasi pozitia necesara de inserat
    //cazul cand e al 2 elem de inserat
    if ((cmp (p->info, server) < 0) && (cmp (p->urm->info, server) > 0)) {
                aux->urm = p->urm;
                aux->pre = p;
                p->urm->pre = aux;
                p->urm = aux;
                return; 
        }
    //cazul cand trebuie de inserat in restul listei 
    for (; p != (*lista)->pre; p = p->urm) { 
        if ((cmp (p->info, server) < 0) && (cmp (p->urm->info, server) > 0)) {
                aux->urm = p->urm;
                aux->pre = p;
                p->urm->pre = aux;
                p->urm = aux;
                return; 
        }
    }

    //inserarea la urma in caz ca nu s-a inserat in cazurile de mai sus
    aux->urm = *lista;
    aux->pre = (*lista)->pre;
    (*lista)->pre->urm = aux;
    (*lista)->pre = aux;
    return;
}


//functia de distrugere a unei TLDI
void DistrugeTLDI (TLDI * l, TF Distruge) {
    TLDI aux, start = *l;

    if (!(*l)) {
        return;
    }

    //distrugerea primului element
    aux = *l;
    *l = (*l)->urm;
    Distruge(aux->info);        
    free(aux);

    //distrugerea restului elementelor
    for (; *l != start; ) {
        aux = *l;
        *l = (*l)->urm;
        Distruge(aux->info);
        free(aux);
    } 
}


//functia de eliminare a unui Server dintr-un tabel hash
void Remove (TH * h, void * server, TF DistrugeServer) {
    int i = h->fh(server, h->M);  //codul hash aferent serverului de eliminat
    TLDI p = h->v[i];             //adresa de inceput a TLDI corespunzatoare
    TLDI aux = p;

    if (!p) {                      
        return;
    }

    // daca trebuie de eliminat primul el dintr-o lista cu 1 el
    if (p == p->urm) { 
        if (cmp (p->info, server) == 0){
            h->v[i] = NULL;
            DistrugeServer (p->info);
            free(p);
            return;
        }
    }

    // daca trebuie de eliminat primul el dintr-o lista cu multiple elemente
    if (cmp (p->info, server) == 0) {
        h->v[i]->pre->urm = h->v[i]->urm;
        h->v[i]->urm->pre = h->v[i]->pre;
        h->v[i] = h->v[i]->urm;
        DistrugeServer(p->info);
        free(p);
        return;
    }

    //daca trebuie de eliminat 1 el din interiorul unei TLDI cu multiple elem
    //cazul in care e primul element
    if (cmp(aux->info, server) == 0){
            aux->pre->urm = aux->urm;
            aux->urm->pre = aux->pre;
            DistrugeServer(aux->info);
            free(aux);
            return;
        }
        aux = aux->urm;
    //cazul cand e in restul listei
    for (; aux != p; aux = aux->urm){ 
        if (cmp(aux->info, server) == 0){
            aux->pre->urm = aux->urm;
            aux->urm->pre = aux->pre;
            DistrugeServer(aux->info);
            free(aux);
            return;
        }
    }
}


//functia de gasire a 
TLDI findKey (void *site, TH *h, TFCmp cmp) {
    int value_of_M = (*h).fh(site, (*h).M); //codul hash al TLDI necesar
    TLDI lista = (*h).v[value_of_M], adress = lista;
   
    if (!lista) {
    	return NULL;
    }

    //daca e chiar prima celula din lista
    if (cmp (site, adress->info) == 0){
            return adress;
        }
    adress = adress->urm;

    //daca e in restul listei
   	for (; adress != lista; adress = adress->urm) {
        if (cmp (site, adress->info) == 0){
            return adress;
        }
    }

    return NULL;
}

