#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char *last_name;
    char *first_name;
    int score;
}Player;

typedef struct
{
    char *name;
    int nr_players;
    int global_score;
    Player *players;
}Country;

typedef struct nod                                              ///Sub aceasta forma arata un nod din lista
{
    Country nation;
    struct nod* next;
    struct nod* prev;
}nod;

void insert_after(nod **root, nod *dummy, FILE *f) {            ///Functia pentru crearea listei
    nod *p = malloc(sizeof(nod));
    p->next = dummy;                                            ///Am ales santinela sa fie la final si va pointa mereu catre root
    dummy->prev = p;

    nod *iter;
    iter = (*root);
    while(iter->next != dummy)
    {
        iter = iter->next;
    }

    int j, nr_tari,len_tara, len_nume, len_prenume;
    char aux_tara[20], aux_nume[20], aux_prenume[20];                                   ///Auxiliare pentru stocarea dinamica din lista

    fscanf(f, "%d", &p->nation.nr_players);                                      ///Nr de jucatori pentru fiecare tara
    p->nation.players = malloc((p->nation.nr_players) * sizeof(Player));

    fscanf(f, "%s", aux_tara);
    len_tara = strlen(aux_tara);

    p->nation.name = malloc((len_tara + 1) * sizeof(char));
    strcpy(p->nation.name, aux_tara);

    fscanf(f, "%d", &p->nation.nr_players);
    p->nation.players = malloc((p->nation.nr_players) * sizeof(Player));

    for (j = 0; j < p->nation.nr_players; j++)
    {
        fscanf(f,"%s %s %d", aux_nume, aux_prenume, &p->nation.players[j].score);
        len_nume = strlen(aux_nume);
        p->nation.players[j].last_name = malloc((len_nume + 1) * sizeof(char));
        strcpy(p->nation.players[j].last_name, aux_nume);

        len_prenume = strlen(aux_prenume);
        p->nation.players[j].first_name = malloc((len_prenume + 1) * sizeof(char));
        strcpy(p->nation.players[j].first_name, aux_prenume);

    }

    p->nation.global_score = 0;                                                          ///Scorul global initial pt fiecare tara este 0


    p->prev = iter;
    iter->next = p;

}
/////ex1



void print_list(nod *root, FILE *f)                                       ///Functie de afisarea a fiecarui nod din lista
{
    nod *iter;
    iter = root;
    iter = iter->next;
    int i;

    while(iter->next != root)
    {
        fprintf(f,"%s\n", iter->nation.name);
        iter = iter->next;
    }
}

void InitialScoreArray(nod *root, int nr_tari, float **scor_initial)                ///Functie de calculare a mediei scorurilor
{                                                                                   ///Personale ale jucatorilor unei tari
    int i = 0,j;                                                                    ///Stocata intr-un vector de tip float
    nod *iter = root;
    iter = iter->next;

    while(iter->next != root)
    {
        for(j = 0; j < iter->nation.nr_players; j++)
        {
            (*scor_initial)[i] += iter->nation.players[j].score;
        }
        (*scor_initial)[i] = (float)(*scor_initial)[i] / (iter->nation.nr_players);
        i++;

        iter = iter->next;
    }
}



void SortArray(float **array, int n)                                         ///Algorim de sortare a unui vector
{                                                                            ///Care nu ajuta sa sortam vectorul de scoruri initiale
    int i,j;
    float aux;
    for(i = 0; i < n-1; i++)
    {
        for(j = i+1; j < n; j++)
        {
            if((*array)[i] > (*array)[j])
            {
                aux = (*array)[i];
                (*array)[i] = (*array)[j];
                (*array)[j] = aux;
            }
        }
    }
}

float MedValue(nod *iter)                                           ///Calculeaza media scorurilor unei tari
{
    int i;
    float val_med = 0;
    for(i = 0; i < iter->nation.nr_players; i++)
    {
        val_med += iter->nation.players[i].score;
    }
    val_med = val_med/(float)iter->nation.nr_players;

    return val_med;
}

void DeleteElem(nod **root, nod *dummy, int *nr_tari, int tari_ramase, float *scor_sortat)
{                                                                                           ///Cum vectorule este sortat stim
    nod *iter;                                                                              ///care este valoarea minima a tarilor
    int i,pos = 0;                                                                         ///care vor trece mai departe
    while((*nr_tari) > tari_ramase)
    {
        iter = dummy;                                                        ///In tari_ramase se stocheaza cate tari trebuie sa
        while(MedValue(iter) != scor_sortat[pos])                            ///ramana in concurs, iar nr_tari cate tari au fost initial
        {
            iter = iter->next;
        }
        iter->prev->next = iter->next;                                       ///Atat timp cat nr de tari nu este egal cu tari_ramase
        iter->next->prev = iter->prev;                                       ///Se elimina tarile cu media scorurilor personale mai mica
        (*nr_tari) --;                                                       ///sau egala cu valoarea minima permisa
        pos ++;
        free(iter);
    }
}

int Tari_concurs(int nr_tari)                        ///Functie de aflare a celei mai apropiate puterei a lui 2 de numarul de tari
{
    int count = 0, i = 0, val = 1, cop = nr_tari;           ///Atat timp cat partea intreaga a copiei nr de tari impartita la 2
    if(cop == 0)                                            ///Nu este 1, inseamna ca nr se mai poate imparti la 2
        return 0;
    else if(cop == 1)                                       ///Pentru fiecare impartire puterea lui 2 va creste
        return 1;
    else{
        while(cop != 1)
        {
            cop /= 2;
            count++;
        }
    }
    while(i != count)
    {
        val = val*2;                                    ///Valoarea nr de tari ramase in turneu este 2 la puterea
        i++;                                            ///Numarului de impartiri la 2
    }
    return val;
}
//////////ex2


typedef struct nod_stiva                                    ///Un nod din stiva va arata astfel
{
    Country tara;
    struct nod_stiva* urm;
}nod_stiva;

void PushFromList(nod_stiva **top, nod* dummy)              ///Adaugare in stiva din lista
{
    int j;
    nod* iter = dummy;
    iter = iter->next->next;

    while(iter != dummy)                                            ///Se adauga in stiva toate elementele din lista
    {
        nod_stiva *newNode = malloc(sizeof(nod_stiva));

        newNode->tara.nr_players = iter->nation.nr_players;
        newNode->tara.global_score = iter->nation.global_score;

        newNode->tara.name = malloc(((strlen(iter->nation.name)) + 1) * sizeof(char));
        strcpy(newNode->tara.name, iter->nation.name);

        newNode->tara.players = malloc(iter->nation.nr_players * sizeof(Player));

        for (j = 0; j < (iter->nation.nr_players); j++)
        {
            newNode->tara.players[j].first_name = malloc(((strlen(iter->nation.players[j].first_name)) + 1) * sizeof(char));
            strcpy(newNode->tara.players[j].first_name, iter->nation.players[j].first_name);

            newNode->tara.players[j].last_name = malloc(((strlen(iter->nation.players[j].last_name)) + 1) * sizeof(char));
            strcpy(newNode->tara.players[j].last_name, iter->nation.players[j].last_name);

            newNode->tara.players[j].score = iter->nation.players[j].score;
        }

        newNode->urm =(*top);
        (*top) = newNode;

        iter = iter->next;
    }
}

int isEmptyStack(nod_stiva *top)
{
    return top == NULL;
}                       ///Functie care verifica daca stiva este goala

Country pop(nod_stiva **top)
{
    int j;
    nod_stiva *temp = (*top);

    Country tara_aux;

    tara_aux.nr_players = temp->tara.nr_players;
    tara_aux.global_score = temp->tara.global_score;

    tara_aux.name = malloc((strlen(temp->tara.name)+1) * sizeof(char));
    strcpy(tara_aux.name, temp->tara.name);

    tara_aux.players = malloc(temp->tara.nr_players * sizeof(Player));
    for (j = 0; j < temp->tara.nr_players; j++)
    {
        tara_aux.players[j].first_name = malloc(((strlen(temp->tara.players[j].first_name)) + 1) * sizeof(char));
        strcpy(tara_aux.players[j].first_name, temp->tara.players[j].first_name);

        tara_aux.players[j].last_name = malloc(((strlen(temp->tara.players[j].last_name)) + 1) * sizeof(char));
        strcpy(tara_aux.players[j].last_name, temp->tara.players[j].last_name);

        tara_aux.players[j].score = temp->tara.players[j].score;
    }

    (*top) = (*top)->urm;
    free(temp);

    return tara_aux;
}                          ///Functie care scoate cate o tara din stiva

typedef struct match                                        ///Structura prin care se pot gasi informatii despre un match
{
    Player *juc1;
    Player *juc2;
}match;

typedef struct nod_coada                                    ///Un nod din coada va arata astfel
{
    match *rez_match;
    struct nod_coada* next;
}nod_coada;


typedef struct Queue
{
    nod_coada *front, *rear;
}Queue;                             ///Structura prin care se retine primul si ultimul nod din coada

Queue* createQueue()                                        ///Functie pentru crearea unei cozi
{
    Queue *q;
    q = malloc(sizeof(Queue));
    if(q == NULL) return NULL;

    q->front = q->rear = NULL;

    return q;
}

void enQueue(Queue *q, match *info_match)                   ///Functie pentru adaugarea unui nod in coada
{
    nod_coada *newNode = malloc(sizeof(nod_coada));

    newNode->rez_match = malloc(sizeof(match));

    newNode->rez_match->juc1 = malloc(sizeof(Player));

    newNode->rez_match->juc1->first_name = malloc(((strlen(info_match->juc1->first_name)) + 1) * sizeof(char));
    newNode->rez_match->juc1->last_name = malloc(((strlen(info_match->juc1->last_name)) + 1) * sizeof(char));

    strcpy(newNode->rez_match->juc1->first_name, info_match->juc1->first_name);
    strcpy(newNode->rez_match->juc1->last_name, info_match->juc1->last_name);
    newNode->rez_match->juc1->score = info_match->juc1->score;


    newNode->rez_match->juc2 = malloc(sizeof(Player));

    newNode->rez_match->juc2->first_name = malloc(((strlen(info_match->juc2->first_name)) + 1) * sizeof(char));
    newNode->rez_match->juc2->last_name = malloc(((strlen(info_match->juc2->last_name)) + 1) * sizeof(char));

    strcpy(newNode->rez_match->juc2->first_name, info_match->juc2->first_name);
    strcpy(newNode->rez_match->juc2->last_name, info_match->juc2->last_name);
    newNode->rez_match->juc2->score = info_match->juc2->score;

    newNode->next = NULL;

    if(q->rear == NULL) q->rear = newNode;
    else{
        (q->rear)->next = newNode;
        (q->rear) = newNode;
    }

    if(q->front == NULL)    q->front = q->rear;
}

int isEmptyQueue(Queue *q)
{
    return (q->front == NULL);
}                            ///Functie care verifica daca mai este vreun element in coada


void deQueue(Queue *q, FILE *f)                             ///Functie care scoate cand un nod din coada
{
    nod_coada *aux;                                         ///Se afiseaza direct din functie rezultatele match-urilor

    aux = q->front;
    fprintf(f,"%s %s %d vs %s %s %d\n", aux->rez_match->juc1->last_name, aux->rez_match->juc1->first_name, aux->rez_match->juc1->score,
           aux->rez_match->juc2->last_name, aux->rez_match->juc2->first_name, aux->rez_match->juc2->score);

    q->front = (q->front)->next;
    ///free(aux);                    ///free(aux) creeaza probleme la afisare
}


void push(nod_stiva **top, Country nation)
{
    int j;
    nod_stiva *newNode = malloc(sizeof(nod_stiva));

    newNode->tara.nr_players = nation.nr_players;
    newNode->tara.global_score = nation.global_score;

    newNode->tara.name = malloc(((strlen(nation.name)) + 1) * sizeof(char));
    strcpy(newNode->tara.name, nation.name);

    newNode->tara.players = malloc(nation.nr_players * sizeof(Player));

    for (j = 0; j < (nation.nr_players); j++)
    {
        newNode->tara.players[j].first_name = malloc(((strlen(nation.players[j].first_name)) + 1) * sizeof(char));
        strcpy(newNode->tara.players[j].first_name, nation.players[j].first_name);

        newNode->tara.players[j].last_name = malloc(((strlen(nation.players[j].last_name)) + 1) * sizeof(char));
        strcpy(newNode->tara.players[j].last_name, nation.players[j].last_name);

        newNode->tara.players[j].score = nation.players[j].score;
    }

    newNode->urm = (*top);
    (*top) = newNode;
}           ///Functie de adaugare in stiva

int MaxScorPersonal(Country aux1, Country aux2)            ///Functie ce verifica care tara are jucatorul cu scorul personal maxim
{
    int max1 = 0, max2 =0, i;
    for(i = 0; i < aux1.nr_players; i++)
    {
        if(max1 < aux1.players[i].score)
        {
            max1 = aux1.players[i].score;
        }
    }

    for(i = 0; i < aux2.nr_players; i++)
    {
        if(max2 < aux2.players[i].score)
        {
            max2 = aux2.players[i].score;
        }
    }
    if(max1 >= max2) return 33;
    else return 44;

}

void etape_turneu(nod_stiva **stackTop, Queue **q, nod_stiva **WINNERTop, Country *tari_BST, int nr_tari, int *nr_juc_BST, FILE *f)
{
    int nr = 0, count_etapa = 1, i ,j, k = 0, m;

    while(nr != 1)                                                                      ///Cat timp in stiva WINNER se afla mai mult de o tara
    {
        nr = 0;
        fprintf(f,"\n====== ETAPA %d ======\n", count_etapa);
        while (!isEmptyStack(*stackTop))                                                ///Pana se goleste stiva initiala se extrage cate o pereche de tari
        {
            Country aux1, aux2;

            aux1 = pop(&(*stackTop));                                                   ///Prima tara din pereche extrasa
            aux2 = pop(&(*stackTop));                                                   ///A doua tara din pereche extrasa
            int scor_local1 = 0, scor_local2 = 0;                                       ///Scorurile locale ale tarilor care decid ce tara trece mai departe

            fprintf(f,"\n%s %d ----- %s %d\n", aux1.name, aux1.global_score, aux2.name, aux2.global_score);

            for (i = 0; i < aux1.nr_players; i++)                                   ///Se copiaza in aux1 si aux2 toate datele tarilor
            {
                for (j = 0; j < aux2.nr_players; j++)
                {
                    match *jucatori = malloc(sizeof(match));

                    jucatori->juc1 = malloc(sizeof(Player));

                    jucatori->juc1->first_name = malloc((strlen(aux1.players[i].first_name) + 1) * sizeof(char));
                    jucatori->juc1->last_name = malloc((strlen(aux1.players[i].last_name) + 1) * sizeof(char));

                    strcpy(jucatori->juc1->first_name, aux1.players[i].first_name);
                    strcpy(jucatori->juc1->last_name, aux1.players[i].last_name);
                    jucatori->juc1->score = aux1.players[i].score;

                    jucatori->juc2 = malloc(sizeof(Player));

                    jucatori->juc2->first_name = malloc((strlen(aux2.players[j].first_name) + 1) * sizeof(char));
                    jucatori->juc2->last_name = malloc((strlen(aux2.players[j].last_name) + 1) * sizeof(char));

                    strcpy(jucatori->juc2->first_name, aux2.players[j].first_name);
                    strcpy(jucatori->juc2->last_name, aux2.players[j].last_name);
                    jucatori->juc2->score = aux2.players[j].score;

                    enQueue((*q), jucatori);                                                ///Se adauga in coada fiecare meci dintre jucatori pe rand

                    if (aux1.players[i].score > aux2.players[j].score)                     ///Fiecare jucator castigator si tara sa primesc puncte
                    {
                        aux1.players[i].score = aux1.players[i].score + 5;
                        aux1.global_score = aux1.global_score + 3;
                        scor_local1 = scor_local1 + 3;
                    } else if (aux1.players[i].score < aux2.players[j].score)
                    {
                        aux2.players[j].score = aux2.players[j].score + 5;
                        aux2.global_score = aux2.global_score + 3;
                        scor_local2 = scor_local2 + 3;
                    } else {
                        aux1.players[i].score = aux1.players[i].score + 2;
                        aux1.global_score = aux1.global_score + 1;
                        scor_local1 = scor_local1 + 1;

                        aux2.players[j].score = aux2.players[j].score + 2;
                        aux2.global_score = aux2.global_score + 1;
                        scor_local2 = scor_local2 + 1;
                    }
                }
            }
            while(!isEmptyQueue(*q))                                                        ///Cat timp coada nu e goala scot fiecare meci din ea
            {
                deQueue(*q, f);
            }

            int aux_nr_juc_ramasi = 1;

            for(i = 0; i < count_etapa; i++)                                                ///Auxiliar care ne ajuta sa vedem in ce etapa a competitiei suntem
            {
                aux_nr_juc_ramasi = aux_nr_juc_ramasi * 2;
            }

            if((nr_tari / aux_nr_juc_ramasi) == 1)                                          ///Daca suntem in finala adaugam intr-un vector de tari
            {                                                                               ///minim semifinaliste
                                                                                            ///Ambele finaliste cu jucatorii si scorurile finale
                (tari_BST[k]).players = malloc(aux2.nr_players * sizeof(Player));
                (tari_BST[k]).nr_players = aux2.nr_players;

                for(m = 0;  m < aux2.nr_players; m++)
                {

                    (tari_BST[k]).players[m].score = aux2.players[m].score;

                    (tari_BST[k]).players[m].first_name = malloc(((strlen(aux2.players[m].first_name)) + 1) * sizeof(char));
                    strcpy((tari_BST[k]).players[m].first_name, aux2.players[m].first_name);

                    (tari_BST[k]).players[m].last_name = malloc(((strlen(aux2.players[m].last_name)) + 1) * sizeof(char));
                    strcpy((tari_BST[k]).players[m].last_name, aux2.players[m].last_name);

                    (*nr_juc_BST)++;
                }
                k++;


                (tari_BST[k]).players = malloc(aux1.nr_players * sizeof(Player));
                (tari_BST[k]).nr_players = aux1.nr_players;

                for (m = 0; m < aux1.nr_players; m++) {

                    (tari_BST[k]).players[m].score = aux1.players[m].score;

                    (tari_BST[k]).players[m].first_name = malloc((strlen(aux1.players[m].first_name) + 1) * sizeof(char));
                    strcpy((tari_BST[k]).players[m].first_name, aux1.players[m].first_name);

                    (tari_BST[k]).players[m].last_name = malloc((strlen(aux1.players[m].last_name) + 1) * sizeof(char));
                    strcpy((tari_BST[k]).players[m].last_name, aux1.players[m].last_name);

                    (*nr_juc_BST)++;
                }

            }

            if (scor_local1 > scor_local2)                                             ///Daca prima echipa are dupa meci un scor local mai mare
            {                                                                          ///trece mai departe si este adaugata in stiva WINNER
                push(&(*WINNERTop), aux1);
                nr++;                                                                  ///Nr de echipe din stiva WINNER creste

                if((nr_tari / aux_nr_juc_ramasi) == 2)                                     ///Daca suntem in semifinale introducem echipele pierzatoare intr-un
                {                                                                          ///Vector de tari minim semifinaliste
                    (tari_BST[k]).players = malloc(aux2.nr_players * sizeof(Player));
                    (tari_BST[k]).nr_players = aux2.nr_players;

                    for(m = 0;  m < aux2.nr_players; m++)
                    {
                        (tari_BST[k]).players[m].score = aux2.players[m].score;

                        (tari_BST[k]).players[m].first_name = malloc(((strlen(aux2.players[m].first_name)) + 1) * sizeof(char));
                        strcpy((tari_BST[k]).players[m].first_name, aux2.players[m].first_name);

                        (tari_BST[k]).players[m].last_name = malloc(((strlen(aux2.players[m].last_name)) + 1) * sizeof(char));
                        strcpy((tari_BST[k]).players[m].last_name, aux2.players[m].last_name);

                        (*nr_juc_BST)++;
                    }
                    k++;
                }
            } else if (scor_local1 < scor_local2){                                          ///Daca echipa a doua are dupa meci un scor local mai maer
                push(&(*WINNERTop), aux2);                                                 ///Se repeta procesul doar ca se baga in stiva WINNER a doua echipa
                nr++;

                if((nr_tari / aux_nr_juc_ramasi) == 2) {                                    ///iar in vectorul de tari minim semifinaliste prima tara
                    (tari_BST[k]).players = malloc(aux1.nr_players * sizeof(Player));
                    (tari_BST[k]).nr_players = aux1.nr_players;

                    for (m = 0; m < aux1.nr_players; m++) {
                        (tari_BST[k]).players[m].score = aux1.players[m].score;

                        (tari_BST[k]).players[m].first_name = malloc((strlen(aux1.players[m].first_name) + 1) * sizeof(char));
                        strcpy((tari_BST[k]).players[m].first_name, aux1.players[m].first_name);

                        (tari_BST[k]).players[m].last_name = malloc((strlen(aux1.players[m].last_name) + 1) * sizeof(char));
                        strcpy((tari_BST[k]).players[m].last_name, aux1.players[m].last_name);

                        (*nr_juc_BST)++;
                    }
                    k++;
                }
            }else{
                if (MaxScorPersonal(aux1, aux2) == 33)                                        ///Daca cele doua echipa dupa meci au scoruri locale egale
                {                                                               ///se verifica ce jucator are scorul personal mai mare
                    push(&(*WINNERTop), aux1);                                  ///33 este un nr dat de mine sa stiu ca prima echipa are acel jucator
                    nr++;
                    if((nr_tari / aux_nr_juc_ramasi) == 2)
                    {
                        (tari_BST[k]).players = malloc(aux2.nr_players * sizeof(Player));
                        (tari_BST[k]).nr_players = aux2.nr_players;

                        for(m = 0;  m < aux2.nr_players; m++)
                        {
                            (tari_BST[k]).players[m].score = aux2.players[m].score;

                            (tari_BST[k]).players[m].first_name = malloc(((strlen(aux2.players[m].first_name)) + 1) * sizeof(char));
                            strcpy((tari_BST[k]).players[m].first_name, aux2.players[m].first_name);

                            (tari_BST[k]).players[m].last_name = malloc(((strlen(aux2.players[m].last_name)) + 1) * sizeof(char));
                            strcpy((tari_BST[k]).players[m].last_name, aux2.players[m].last_name);

                            (*nr_juc_BST)++;
                        }
                        k++;
                    }

                }
                if (MaxScorPersonal(aux1, aux2) == 44)                                              ///Daca cele doua echipa dupa meci au scoruri locale egale
                {                                                               ///se verifica ce jucator are scorul personal mai mare
                    push(&(*WINNERTop), aux2);                                  ///44 este un nr dat de mine sa stiu ca a doua echipa are acel jucator
                    nr++;
                    if((nr_tari / aux_nr_juc_ramasi) == 2) {
                        (tari_BST[k]).players = malloc(aux1.nr_players * sizeof(Player));
                        (tari_BST[k]).nr_players = aux1.nr_players;

                        for (m = 0; m < aux1.nr_players; m++) {
                            (tari_BST[k]).players[m].score = aux1.players[m].score;

                            (tari_BST[k]).players[m].first_name = malloc((strlen(aux1.players[m].first_name) + 1) * sizeof(char));
                            strcpy((tari_BST[k]).players[m].first_name, aux1.players[m].first_name);

                            (tari_BST[k]).players[m].last_name = malloc((strlen(aux1.players[m].last_name) + 1) * sizeof(char));
                            strcpy((tari_BST[k]).players[m].last_name, aux1.players[m].last_name);
                            (*nr_juc_BST)++;
                        }
                        k++;
                    }
                }
            }
        }
        fprintf(f,"\n=== WINNER ===\n");

        while(!isEmptyStack(*WINNERTop))                                                ///Cat timp stiva WINNER nu este goala
        {                                                                               ///Se adauga nodurile ei in stiva initiala
            Country winner;
            winner = pop(&(*WINNERTop));
            fprintf(f,"%s --- %d\n", winner.name, winner.global_score);         ///Si se afiseaza echipele care au trecut mai departe
            push(&(*stackTop), winner);
        }
        count_etapa++;                                                                      ///Trecem in etapa urmatoare!
    }

}
//// ex 3


void DeleteSameScorePlayer(Player *juc_semifinale, int *nr_juc)         ///Functie care sterge un jucator din cei doi semifinalisti cu scoruri egale
{
    int i,j,k;
    for(i = 0; i < ((*nr_juc) - 1); i++)
    {
        for(j = i+1; j < (*nr_juc); j++)
        {
            if(juc_semifinale[i].score == juc_semifinale[j].score)
            {
                if (strcmp(juc_semifinale[i].last_name, juc_semifinale[j].last_name) <= 0)          ///Se sterge jucatorul cu numele mai mic dpdv lexicografic
                {
                    for (k = j; k < (*nr_juc) - 1; k++) {
                        strcpy(juc_semifinale[k].first_name, juc_semifinale[k + 1].first_name);
                        strcpy(juc_semifinale[k].last_name, juc_semifinale[k + 1].last_name);
                        juc_semifinale[k].score = juc_semifinale[k + 1].score;
                    }
                    (*nr_juc)--;

                    j--;
                }
                else{
                    for (k = i; k < (*nr_juc) - 1; k++) {
                        strcpy(juc_semifinale[k].first_name, juc_semifinale[k + 1].first_name);
                        strcpy(juc_semifinale[k].last_name, juc_semifinale[k + 1].last_name);
                        juc_semifinale[k].score = juc_semifinale[k + 1].score;
                    }
                    (*nr_juc)--;

                    if(i > 0) i--;
                }
            }
        }
    }
    juc_semifinale = realloc(juc_semifinale, (*nr_juc) * sizeof(Player));
}

typedef struct nod_arbore
{
    Player juc;
    struct nod_arbore *left, *right;
}nod_arbore;                ///Un nod din arbore va arata astfel

nod_arbore* newNode(Player jucator)
{
    nod_arbore *nodNou = malloc(sizeof(nod_arbore));

    nodNou->juc.first_name = malloc((strlen(jucator.first_name) + 1) * sizeof(char));

    nodNou->juc.last_name = malloc((strlen(jucator.last_name) + 1) * sizeof(char));

    strcpy(nodNou->juc.first_name, jucator.first_name);
    strcpy(nodNou->juc.last_name, jucator.last_name);


    nodNou->juc.score = jucator.score;

    nodNou->left = nodNou->right = NULL;

    return nodNou;
}                   ///Functie de creare a unui nod pentru arbore

nod_arbore* insert(nod_arbore* nodNou, Player key)
{
    if(nodNou == NULL) return newNode(key);

    if(key.score < nodNou->juc.score)  nodNou->left = insert(nodNou->left, key);
    else if(key.score > nodNou->juc.score)   nodNou->right = insert(nodNou->right, key);


    return nodNou;
}       ///Functie de adaugare a unui nod in arbore


void AfisareDescrescator(nod_arbore* root, FILE *f)
{
    if(root)
    {
        AfisareDescrescator(root->right,f);
        fprintf(f,"%s %s %d\n",root->juc.last_name, root->juc.first_name, root->juc.score);
        AfisareDescrescator(root->left,f);

    }
}       /// Afisare jucatori semifinalisti in ordine descrescatoare a scorurilor personale

void CopiereJucatori(Country *tari_BST, Player *juc_semifinale)
{
    int i,j;
    int count = 0;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < tari_BST[i].nr_players; j++)
        {
            juc_semifinale[count].first_name = malloc((strlen(tari_BST[i].players[j].first_name) + 1) * sizeof(char));
            juc_semifinale[count].last_name = malloc((strlen(tari_BST[i].players[j].last_name) + 1) * sizeof(char));

            strcpy(juc_semifinale[count].first_name,tari_BST[i].players[j].first_name);
            strcpy(juc_semifinale[count].last_name, tari_BST[i].players[j].last_name);

            juc_semifinale[count].score = tari_BST[i].players[j].score;

            count++;
        }
    }
}   ///Se copiaza jucatorii dintr-un vecor de tip Country intr-unul de tip Player
/////ex4

void inorder_max1(nod_arbore *root_arbore, int scor1, int scor2, int *count)  ///Functie care verifica cate valori sunt intre doua valori din arbore
{                                                                             ///daca prima valoare este mai mare decat a doua
    if(root_arbore)
    {
        inorder_max1(root_arbore->left, scor1, scor2, count);
        inorder_max1(root_arbore->right, scor1, scor2, count);

        if((root_arbore->juc.score) < scor1 && (root_arbore->juc.score > scor2)) (*count)++;
    }
}

void inorder_max2(nod_arbore *root_arbore, int scor1, int scor2, int *count)     ///Functie care verifica cate valori sunt intre doua valori din arbore
                                                                                  ///daca a doua valoare este mai mare decat prima
{
    if(root_arbore)
    {
        inorder_max2(root_arbore->left, scor1, scor2, count);
        inorder_max2(root_arbore->right, scor1, scor2, count);

        if(((root_arbore->juc.score) > scor1) && ((root_arbore->juc.score < scor2)))     (*count)++;
    }
}

void JucatoriIntre(nod_arbore *root_arbore, FILE *f, Player *juc_semifinale, int nr_juc, int *nr_juc_intre, FILE *f_rez, int ex5) ///Functie de verificare cati juc sunt intre doi juc dati
{
    int i, OK1 = 0, OK2 = 0;
    int scor_juc1, scor_juc2;
    char *nume_juc1, *nume_juc2, *prenume_juc1, *prenume_juc2;          ///Se copiaza din fisierul cerinte.in cei doi jucatori
                                                                        ///pentru care se face compararea
    char buffer1[20], buffer2[20];

    fscanf(f,"%s %s %d", buffer1, buffer2, &scor_juc1);

    nume_juc1 = malloc((strlen(buffer1) + 1) * sizeof(char));
    strcpy(nume_juc1, buffer1);

    prenume_juc1 = malloc((strlen(buffer2) + 1) * sizeof(char));
    strcpy(prenume_juc1, buffer2);

    char buffer3[20], buffer4[20];
    fscanf(f,"%s %s %d", buffer3, buffer4, &scor_juc2);

    nume_juc2 = malloc((strlen(buffer3) + 1) * sizeof(char));
    strcpy(nume_juc2, buffer3);

    prenume_juc2 = malloc((strlen(buffer4) + 1) * sizeof(char));
    strcpy(prenume_juc2, buffer4);

    for(i = 0; i < nr_juc; i++)               ///Daca se gasesc jucatorii in arbore OK1 si OK2 iau valoarea 1
    {
       if((strcmp(juc_semifinale[i].first_name, prenume_juc1) == 0) && (strcmp(juc_semifinale[i].last_name, nume_juc1) == 0)) OK1 = 1;

       if((strcmp(juc_semifinale[i].first_name, prenume_juc2) == 0) && (strcmp(juc_semifinale[i].last_name, nume_juc2) == 0)) OK2 = 1;
    }
        if (OK1 == 0 && ex5 == 1)        fprintf(f_rez,"\n%s %s nu poate fi identificat!\n", nume_juc1, prenume_juc1);         ///Daca nu se gasesc juc1 sau juc2
        else if (OK2 == 0 && ex5 == 1)   fprintf(f_rez, "\n%s %s nu poate fi identificat!\n", nume_juc2, prenume_juc2);   ///Se afiseaza un mesaj lamuritor
        else if (OK1 == 1 && OK2 == 1)
        {                                                                                                                     ///Daca se gasesc se verifica cati
            if (scor_juc1 > scor_juc2) {                                                                                      ///jucatori au scoruri personale
                inorder_max1(root_arbore, scor_juc1, scor_juc2, nr_juc_intre);                                                 ///intre ei
            } else if (scor_juc1 < scor_juc2) {
                inorder_max2(root_arbore, scor_juc1, scor_juc2, nr_juc_intre);
            }
            fprintf(f_rez,"\n%d", *nr_juc_intre);
        }
}


int main(int argc, char* argv[])
{
    FILE *f_cerinte = fopen(argv[1], "r");
    int ex1, ex2, ex3, ex4, ex5;

    fscanf(f_cerinte,"%d %d %d %d %d", &ex1, &ex2, &ex3, &ex4, &ex5);               ///Variabile care ne arata daca se executa o anumita cerinta


    nod *root = malloc(sizeof(nod));                                                    ///Un nod root care pointeaza catre primul nod
    nod *dummy = malloc(sizeof(nod));                                                   ///O santinele pusa la finalul listei

    root->next = dummy;
    root->prev = dummy;

    dummy->prev = root;
    dummy->next = root;

    int nr_tari, i, j, tari_ramase;

    FILE *f_date = fopen(argv[2], "r");

    FILE *f_rez = fopen(argv[3], "w");

    fscanf(f_date, "%d", &nr_tari);                                         ///Se ia nr de tari initiale din fisierul date.in

    for (i = 0; i < nr_tari; i++) {                                                 ///Si se adauga toate tarile in lista
        insert_after(&root, dummy, f_date);
    }

    if(ex1 == 1 && ex2 == 0)
    {
        print_list(root, f_rez);
    }
///////////////ex1



    float *scor_initial = (float *) calloc(nr_tari, sizeof(float));           ///Initializam un vector de scoruri initiale si atribuim fiecarui element val 0

    float *scor_sortat = malloc(nr_tari * sizeof(float));                 ///Initializam un vector de scoruri sortate

    float val_min = 0;                                                          ///Valoarea minima a scorurilor pentru a nu fi eliminata inainte de prima runda

    InitialScoreArray(root, nr_tari, &scor_initial);                        ///Se completeaza vectorul de scor initial


    tari_ramase = Tari_concurs(nr_tari);                                ///Nr de tari care trebuie sa ramana inainte de prima etapa

    for (i = 0; i < nr_tari; i++) {
        scor_sortat[i] = scor_initial[i];                                   ///Se copiaza elementele din cei doi vectori
    }

    SortArray(&scor_sortat, nr_tari);                               ///Se sorteaza vectorul scor_sortat

    if (nr_tari != tari_ramase) {                                   ///Daca mai trebuie tari scoase din competitie inainte de prima runda
        val_min = scor_sortat[nr_tari - tari_ramase - 1];           ///se stocheaza valoarea minima care permite trecerea mai departe
    }


    if (val_min != 0) {                                             ///Daca mai trebuie scoase tari, acestea se elimina
        DeleteElem(&root, dummy, &nr_tari, tari_ramase, scor_sortat);
    }

    if(ex2 == 1)
    {
        print_list(root, f_rez);                                    ///Si se afiseaza lista finala
    }
    ////////ex2


    nod_stiva *stackTop = NULL;

    PushFromList(&stackTop, dummy);                                  ///Se adauga in stiva toate elementele din lista

    Queue *q;
    q = createQueue();                                              ///Se creeaza coada

    nod_stiva *WINNERTop = NULL;

    Country *tari_BST = malloc(4 * sizeof(Country));                ///Se initializeaza un vector de tari care retine tarile semifinaliste
    int nr_juc_tari_BST = 0;                                            ///Cati jucatori au participat in semifinale

    if(ex3 == 1) {
        etape_turneu(&stackTop, &q, &WINNERTop, tari_BST, nr_tari, &nr_juc_tari_BST, f_rez);
    }
    //// ex 3


    Player* juc_semifinale = malloc((nr_juc_tari_BST) * sizeof(Player));

    CopiereJucatori(tari_BST, juc_semifinale);                                  ///Se adauga toti jucatorii semifinalisti intr-un vector Player

    DeleteSameScorePlayer(juc_semifinale, &nr_juc_tari_BST);                        ///Se sterge un jucator semifinalist din cei doi cu scoruri egale daca exista

    nod_arbore *root_arbore = NULL;

    for(i = 0; i < nr_juc_tari_BST; i++)
    {
        root_arbore = insert(root_arbore, juc_semifinale[i]);                       ///si se adauga in arbore jucatorii semifinalisti ramasi
    }

    if(ex4 == 1)
    {
        fprintf(f_rez,"\n====== CLASAMENT JUCATORI ======\n");

        AfisareDescrescator(root_arbore, f_rez);                                ///Se afiseaza descrescator dupa scorurile lor jucatorii semifinalisti
    }
    //////ex4


    int nr_juc_intre = 0;

    if(ex5 == 1) {
        JucatoriIntre(root_arbore, f_cerinte, juc_semifinale, nr_juc_tari_BST, &nr_juc_intre, f_rez, ex5);  ///Se afiseaza cati jucatori sunt intre cei doi
    }                                                                                                       ///la scor personal
    /////ex5


    fclose(f_cerinte);                                                                                      ///Se inchid toate fisierele deschise
    fclose(f_date);
    fclose(f_rez);

    return 0;
}
