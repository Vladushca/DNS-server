#include "biblioteca.h"

//functia de comparatie a 2 servere dupa numele site-ului
int cmp (void *a, void *b) {
    Server * serv1 = (Server *) a;
    Server * serv2 = (Server *) b;
    
    int rez = strcmp(serv1->Key, serv2->Key);
    
    return rez;
}

//functia de hash a unui server
int hashing (void *a, int M) {
    int k = 0, rez;
    Server * serv = (Server *) a;
    char * site = serv->Key;
    
    while (*site != '\0'){
        k = k + (*site);
        site++;
    }
    rez = k % M;
    
    return rez;
}




int main (int argc, char *argv[]) {

    char *comand, *site, *ip;
    char *line = malloc (75 * sizeof (char));
    Server * serv;
	
	int i, M = atoi(argv[1]);    // M
	TH * h = InitializareTH (M, hashing);

    char *file_in = argv[2];     //numele fisierului input
    FILE *in = fopen(file_in, "r");
  
    char *file_out = argv[3];    //numele fisierului output
    FILE *out = fopen(file_out, "w");

	for (;fgets (line, 75, in);) {
		// obtinerea numelui functiei necesare de realizat
		comand = strtok (line, " ");
		if (comand[strlen (comand) - 1] == '\n')  //eliminarea \n
        comand[(strlen (comand) - 1)] = '\0';

		if (strcmp(comand,"put") == 0) {
			site = strtok (NULL, " ");//obtinerea site-ului necesar de introdus
			ip = strtok(NULL," ");
			if (ip[strlen (ip) - 1] == '\n') 
        	ip[(strlen (ip) - 1)] = '\0';

            serv = AlocareServer(site, ip);

			if (!(findKey (serv, h, cmp))) {//inserarea in tabel if not already
                i = hashing (serv, h->M);
                InserareDupaLDI (&h->v[i], serv, cmp);
            } else {
                DistrugeServer (serv);
            }
		}

		if (strcmp (comand, "print") == 0) {
	       	AfisareTH (h, afisareSite, out);
            continue;
        }

        if (strcmp (comand, "find") == 0) {
            site = strtok (NULL," ");
			if (site[strlen (site) - 1] == '\n') 
       		site[(strlen (site) - 1)] = '\0';

	        serv = AlocareServer (site, site);
	        
	        if (!(findKey (serv, h, cmp))) {
	            fprintf (out, "False\n");
	        } else {
	            fprintf (out, "True\n");
	        } 
	        DistrugeServer (serv);
	        
	        continue;
	    }

	    if (strcmp (comand, "get") == 0) {
	        site = strtok (NULL, " ");
			if (site[strlen (site) - 1] == '\n') 
       		site[(strlen (site) - 1)] = '\0';

	        serv = AlocareServer (site, site);
	        TLDI addr = findKey (serv, h, cmp);
	    
	        if (!addr){
	            fprintf (out, "NULL\n");
	        } else {
	            afisareSite (addr->info, out);
	            fprintf (out, "\n");
	        } 
	        DistrugeServer (serv);
	        
	        continue;
	    }

	    if (strcmp (comand, "print_bucket") == 0) {
	        int value_of_M = atoi (strtok (NULL, " "));
	        
	        if (value_of_M >= h->M) {
	        	continue;
	        }

	        if (!h->v[value_of_M]){
	            fprintf(out, "VIDA\n");
	            continue;
            }

			AfisareLDI (&h->v[value_of_M], afisareSite, out);

    	}

		if (strcmp (comand, "remove") == 0) {	
            site = strtok (NULL, " ");
			if (site[strlen (site) - 1] == '\n') 
       		site[(strlen (site) - 1)] = '\0';

            serv = AlocareServer (site, site);
			
			Remove (h, serv, DistrugeServer);
			DistrugeServer(serv);
            continue;
          }
      }

    free(line);

    DistrugeTH(&h, DistrugeServer);
    fclose(in);
    fclose(out);
    return 0;
}