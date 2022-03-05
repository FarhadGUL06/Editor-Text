// GUL FARHAD ALI IRINEL - SD - TEMA 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuri.h"

// 1. Functii pentru implementarea blocului de liste:
// a) Initializeaza blocul de liste
void init(BlocList **list);
// b) Elibereaza memoria alocata din tot blocul
void free_list(BlocList **list);
// c) Afiseaza in fisierul dat ca parametru blocul de liste
void print_list(BlocList *list,FILE *output);
// d) Adauga un rand nou
int add_rand (TList *rand, BlocList *list, int n);
// e) Sterge randul n din bloc
char *stergere_rand(BlocList *list,int n);

// 2. Functii pentru implementarea listelor (randurilor):
// a) Functia de initializare a randului
void init_rand(TList **list);
// b) Functia de eliberare a memoriei alocate pe rand
void free_rand(TList **list);
// c) Adauga un element in rand pe pozitia indicata
int add (TList *list, int n, char data);
// d) Sterge nodul de pe pozitia din randul dat
node *stergere_nod(TList *list,int n);

// 3. Functii pentru implementarea stivelor de undo si redo:
void init_s(Stack **s);
void free_s(Stack **s);
int add_stack(Stack *s,node_s *nou);
node_s *pop_stack(Stack *s);
// Functia in care se opereaza undo-ul pe diferitele comenzi:
Stack *undo_text(BlocList *bloc,Stack *undo,Stack *redo);
// Functia in care se efectueaza redo-ul (se reface comanda):
Stack *redo_text(BlocList *bloc, Stack *undo, Stack *redo);

// 4. Functii de baza pentru operarea in bloc:
// a) Sterge un nod dintr-un rand indicat si imi intoarce valoarea acestuia
char del_nod_din_rand(BlocList *bloc,int nod_x,int rand_x);
// b) Imi adauga un nod in rand dat prin parametrii functiei
void add_nod_in_rand(BlocList *bloc,char val,int nod_x,int rand_x);

// FUNCTII PENTRU IMPLEMENTAREA COMENZILOR DATE:
// 1. Functii de operare pe liste / noduri:
/* a) Sterge linia indicata si imi returneaza un pointer
la vectorul alocat dinamic in care am stocat
randul sters (pentru operatia de redo):*/
char *delete_line(BlocList *bloc,int linie);
/* b) Sterge toate cuvintele "cuvant" si returneaza
un pointer la vectorul in care am salvat toate aparitiile:*/
aparitie *delete_all_words(BlocList *bloc, char cuvant[]);
// c) Sterge un cuvant din bloc si imi returneaza nodul:
node_s *delete_word(BlocList *bloc,char cuvant[]);
// Comenzile de jos asemanatoare cu a si b:
aparitie *replace_all_words(BlocList *bloc, char cuvant_vechi[],char cuvant_nou[]);
node_s *replace_word(BlocList *bloc,char cuvant_vechi[],char cuvant_nou[]);

// 2. Functii de modificare a parametrilor cursorului:
void goto_line(int linie);
void goto_char(int linie,int nod);

// 3. Functii pentru prelucrarea fisierului:
void prelucrare_fisier(FILE *input,BlocList *bloc);
void inversare_valoare(int *ok);

// Initializarea cursorului:
int nr_rand;
int nr_nod;

// Programul de prelucrare:
void init_s(Stack **s)
{
    (*s)=(Stack*)malloc(sizeof(Stack));
    (*s)->top=NULL;
    (*s)->len=0;
}

void free_s(Stack **s)
{
    node_s *nod=(*s)->top;
    while (nod!=NULL)
    {
        (*s)->top=(*s)->top->next;
        if ((strcmp(nod->comanda,"ra")==0)||(strcmp(nod->comanda,"da")==0))
        {
            free(nod->indici);
        }
        free(nod);
        nod=(*s)->top;
    }
    free(*s);
}

int add_stack(Stack *s,node_s *nou)
{
    s->len = s->len+1;
    if (s->len==1)
    {
        nou->next=NULL;
        s->top=nou;
        return 1;
    }
    nou->next=s->top;
    s->top=nou;
    return 1;
}

node_s *pop_stack(Stack *s)
{
    if(s->len==0)
    {
        return 0;
    }
    node_s *vf,*sters;
    s->len=s->len-1;
    vf = s->top;
    if (s->top->next!=NULL)
    {
        s->top=s->top->next;
    }
    else
    {
        s->top=NULL;
    }
    sters=vf;
    vf=vf->next;
    return sters;
}

void init_rand(TList **list)
{
    // Initializam un rand
    (*list)=(TList*)malloc(sizeof(TList));
    (*list)->head=NULL;
    (*list)->len=0;
    (*list)->tail=NULL;
}

void init(BlocList **list)
{
    // Initializam blocul de liste
    (*list)=(BlocList*)malloc(sizeof(BlocList));
    (*list)->inceput=NULL;
    (*list)->len=0;

}

void free_rand(TList **list)
{
    // Eliberam un rand din bloc
    node *nod=(*list)->head;
    while (nod!=NULL)
    {
        (*list)->head=(*list)->head->next;
        free(nod);
        nod=(*list)->head;
    }
    free(*list);
}

void free_list(BlocList **list)
{
    // Eliberam intregul bloc de liste
    TList *rand=(*list)->inceput;
    while (rand!=NULL)
    {
        (*list)->inceput=(*list)->inceput->down;
        if (rand!=NULL)
        {
            free_rand(&rand);
        }
        rand=(*list)->inceput;
    }
    free(*list);
}

void print_list(BlocList *list,FILE *output)
{
    // Afisam lista in in fisierul dat ca parametru
    TList *rand;
    node *parcurgere;
    rand=list->inceput;
    while (rand!=NULL)
    {
        parcurgere=rand->head;
        while (parcurgere->next!=NULL)
        {
            fputc(parcurgere->data,output);
            parcurgere=parcurgere->next;
        }
        rand=rand->down;
        fputc('\n',output);
    }
}

int add (TList *list, int n, char data)
{
    // Adaugam un nod pe pozitia indicata
    list->len = list->len+1;

    if ((n<0)||(n>list->len))
    {
        return -1;
    }
    if (n==0)
    {
        if (list->len==1)
        {
            node *nou=(node*)malloc(sizeof(node));
            nou->data=data;
            nou->next=NULL;
            nou->prev=NULL;
            list->head=nou;
            list->tail=nou;
            return 1;
        }
        else
        {
            node *nod;
            node *nou =malloc(sizeof(node));
            nod=list->head;
            nod->prev=nou;
            nod->next=list->head->next;
            nou->next=nod;
            nou->prev=NULL;
            nou->data=data;
            list->head=nou;
            list->head->next=nod;
            return 1;
        }
    }
    if (n+1==list->len)
    {
        node*nou=malloc(sizeof(node));
        node *ultim;
        ultim=list->tail;
        nou->prev=ultim;
        nou->next=NULL;
        nou->data=data;
        ultim->next=nou;
        list->tail=nou;
        return 1;

    }
    node *elem_prev;
    int i=0;
    elem_prev=list->head;
    while (i<n-1)
    {
        elem_prev=elem_prev->next;
        i++;
    }
    node *elem_next;
    elem_next=elem_prev->next;
    node *nou=malloc(sizeof(node));
    nou->prev=elem_prev;
    nou->next=elem_next;
    elem_next->prev=nou;
    elem_prev->next=nou;
    nou->data=data;
    return -1;
}

int add_rand (TList *rand, BlocList *list, int n)
{
    // Adaugare rand pe o pozitie data in blocul de liste
    list->len++;
    if (n==1)
    {
        if (list->len==1)
        {
            list->inceput=rand;
            list->inceput->down=NULL;
            list->inceput->up=NULL;
            list->sfarsit=rand;
            list->sfarsit->up=NULL;
            list->sfarsit->down=NULL;
            return 1;
        }
        TList *retinut;
        retinut=list->inceput;
        retinut->up=rand;
        rand->down=retinut;
        rand->up=NULL;
        list->inceput=rand;
        list->inceput->down=retinut;
        return 1;
    }
    if (n==list->len)
    {
        TList *parcurgere;
        parcurgere=list->sfarsit;
        rand->up=parcurgere;
        rand->down=NULL;
        parcurgere->down=rand;
        list->sfarsit=rand;
        list->sfarsit->down=NULL;
        list->inceput->up=NULL;
        return 1;
    }
    if (n<list->len)
    {
        TList *list_prev;
        list_prev=list->inceput;
        int i=1;
        while (i<n-1)
        {
            list_prev=list_prev->down;
            i++;
        }
        TList *list_next;
        list_next=list_prev->down;
        rand->up=list_prev;
        rand->down=list_next;
        list_next->up=rand;
        list_prev->down=rand;
        return 1;
    }
    return -1;
}

node *stergere_nod(TList *list,int n)
{
    // Sterge nodul indicat ca parametru din lista
    if ((n>list->len-1)||(n<0))
    {
        return NULL;
    }
    node *nod,*nod_modif;
    nod = list->head;
    int i=0;
    if (n==0)
    {
        list->head=list->head->next;
        nod_modif=list->head;
        nod_modif->prev=NULL;
        list->len=list->len-1;
        return nod;
    }
    else
    {
        if (n==list->len-1)
        {
            nod=list->tail;
            list->tail=list->tail->prev;
            nod_modif=list->tail;
            nod_modif->next=NULL;
            list->len=list->len-1;
            return nod;
        }
        while (i<n-1)
        {
            nod=nod->next;
            i++;
        }
        node *retinut, *elem_next;
        retinut=nod->next;
        elem_next=nod->next->next;
        elem_next->prev=nod;
        nod ->next = nod -> next -> next;
        list->len=list->len-1;
        return retinut;
    }
}

char *stergere_rand(BlocList *list,int n)
{
    // Sterge randul dat ca parametru din bloc
    char *rand_st=malloc(100*sizeof(char));
    if ((n>list->len-1)||(n<0))
    {
        return NULL;
    }
    TList *rand,*rand_modif;
    rand = list->inceput;
    int i=0;
    if (n==0)
    {
        node *p;
        int i=0;
        p=rand->head;
        while (p!=NULL)
        {
            rand_st[i]=p->data;
            p=p->next;
            i++;
        }
        rand_st[list->inceput->len]='\0';
        list->inceput=list->inceput->down;
        rand_modif=list->inceput;
        rand_modif->up=NULL;
        list->len=list->len-1;
        free_rand(&rand);
        return rand_st;
    }
    else
    {
        if (n==list->len-1)
        {
            rand=list->sfarsit;
            node *p;
            int i=0;
            p=rand->head;
            while (p!=NULL)
            {
                rand_st[i]=p->data;
                p=p->next;
                i++;
            }
            rand_st[list->sfarsit->len]='\0';
            list->sfarsit=list->sfarsit->up;
            rand_modif=list->sfarsit;
            rand_modif->down=NULL;
            list->len=list->len-1;
            free_rand(&rand);
            return rand_st;
        }
        while (i<n-1)
        {
            rand=rand->down;
            i++;
        }
        TList *retinut, *rand_next;
        retinut=rand->down;
        node *p;
        int i=0;
        p=retinut->head;
        while (p!=NULL)
        {
            rand_st[i]=p->data;
            p=p->next;
            i++;
        }
        rand_st[list->sfarsit->len]='\0';
        rand_next=rand->down->down;
        rand_next->up=rand;
        rand->down=rand->down->down;
        list->len=list->len-1;
        free_rand(&retinut);
        return rand_st;
    }
}

char del_nod_din_rand(BlocList *bloc,int nod_x,int rand_x)
{
    // Sterge nodul dat de pe randul dat din bloc
    int i;
    char val;
    node *sters;
    TList *rand;
    rand=bloc->inceput;
    for (i=0; i<rand_x-1; i++)
    {
        rand=rand->down;
    }
    sters=stergere_nod(rand,nod_x-1);
    val=sters->data;
    free(sters);
    return val;

}

void add_nod_in_rand(BlocList *bloc,char val,int nod_x,int rand_x)
{
    // Adauga nodul dat pe pozitia data pe randul dat din bloc
    int i;
    TList *rand;
    rand=bloc->inceput;
    for (i=0; i<rand_x-1; i++)
    {
        rand=rand->down;
    }
    add(rand,nod_x-1,val);
}

char *delete_line(BlocList *bloc,int linie)
{
    // Sterge o linie din bloc
    linie--;
    char *rand_sters;
    rand_sters=stergere_rand(bloc,linie);
    return rand_sters;
}

void goto_line(int linie)
{
    // Muta cursorul pe pozitia indicata
    nr_rand=linie;
    nr_nod=1;
}

void goto_char(int linie,int nod)
{
    // Muta cursorul pe pozitia indicata
    nr_rand=linie;
    nr_nod=nod;
}

aparitie *delete_all_words(BlocList *bloc, char cuvant[])
{
    // Functia pentru sters toate cuvintele
    TList *rand;
    node *nod;
    rand=bloc->inceput;
    nod=rand->head;
    int ok=1,i,noduri=1,randuri=1,verif,lungime,j=0;
    lungime=strlen(cuvant);
    // Alocam dinamic vectorul de pozitii
    aparitie *aparitii=malloc(sizeof(aparitie)*20);
    while(ok==1)
    {
        i=0;
        verif=0;
        // Cat timp primele litere (cea a cuvantului de sters si cea din nod) nu corespund
        if(cuvant[i]!=nod->data)
        {
            nod=nod->next;
            if (nod==NULL)
            {
                noduri=0;
                rand=rand->down;
                if (rand==NULL)
                {
                    ok=0;
                    break;
                }
                randuri++;
                nod=rand->head;
            }
            noduri++;
        }
        // Cat timp literele cuvantului = cu cele din lista
        while ((cuvant[i]==nod->data)&&(i<lungime))
        {
            verif++;
            nod=nod->next;
            i++;
        }
        // Daca cuvantul de sters are aceeasi lungime cu cuvantul gasit
        if (verif==lungime)
        {
            aparitii[j].nodul=noduri;
            aparitii[j].randul=randuri;
            aparitii[j].verif=verif;
            j++;
            noduri=noduri+verif;
        }

    }
    // Stergerea am ales sa o fac de la capat la inceput spre a pastra pozitia constanta
    int lungime_aparitii=j;
    for (i=lungime_aparitii-1; i>=0; i--)
    {
        if (aparitii[i].verif==lungime)
        {
            j=0;
            while (j<lungime)
            {
                del_nod_din_rand(bloc,aparitii[i].nodul,aparitii[i].randul);
                j++;
            }
        }

    }
    return aparitii;
}

node_s *delete_word(BlocList *bloc,char cuvant[])
{
    // Functia pentru sters un cuvant
    node_s *comanda_noua=malloc(sizeof(node_s));
    strcpy(comanda_noua->comanda,"dw");
    strcpy(comanda_noua->caractere_memorate,cuvant);
    int lungime,ok=1,verif,noduri=1,i,j,pozitie,rand_curent=1;
    lungime=strlen(cuvant);
    TList *rand;
    node *nod;
    // Ne deplasam spre pozitia cursorului
    rand=bloc->inceput;
    for (i=0; i<nr_rand-1; i++)
    {
        rand=rand->down;
        rand_curent++;
    }
    nod=rand->head;
    for (i=0; i<nr_nod-1; i++)
    {
        nod=nod->next;
        noduri++;
    }
    while(ok==1)
    {
        i=0;
        verif=0;
        // Cat timp literele initiale nu corespund
        if(cuvant[i]!=nod->data)
        {
            nod=nod->next;
            if (nod==NULL)
            {
                noduri=0;
                rand=rand->down;
                rand_curent++;
                if (rand==NULL)
                {
                    rand=bloc->inceput;
                    rand_curent=1;
                    noduri=1;
                }
                nod=rand->head;
            }
            noduri++;
        }
        // Cat timp literele corespund
        while ((cuvant[i]==nod->data)&&(i<lungime))
        {
            verif++;
            nod=nod->next;
            i++;
        }
        if (verif==lungime)
        {
            ok=0;
            pozitie=noduri;
        }
        noduri=noduri+i;

    }
    // Daca lungimea cuvantului gasit = cuvantului de sters
    if (verif==lungime)
    {
        j=0;
        comanda_noua->cursor_vechi_nod=pozitie-1;
        comanda_noua->cursor_vechi_rand=rand_curent;
        node *po;
        while (j<lungime)
        {
            po=stergere_nod(rand,pozitie-1);
            free(po);
            j++;
        }
    }
    comanda_noua->rand_memorat=NULL;
    comanda_noua->nod_memorat='\0';
    return comanda_noua;
}

aparitie *replace_all_words(BlocList *bloc, char cuvant_vechi[],char cuvant_nou[])
{
    // Functia pentru inlocuit toate cuvintele vechi cu cele noi
    TList *rand;
    node *nod;
    rand=bloc->inceput;
    nod=rand->head;
    int ok=1,i,noduri=1,randuri=1,verif,lungime,j=0,lungime_nou;
    lungime=strlen(cuvant_vechi);
    lungime_nou=strlen(cuvant_nou);
    aparitie *aparitii=malloc(sizeof(aparitie)*20);
    while(ok==1)
    {
        i=0;
        verif=0;
        // Deplasam cursorul pana corespund primele litere
        if(cuvant_vechi[i]!=nod->data)
        {
            nod=nod->next;
            if (nod==NULL)
            {
                noduri=0;
                rand=rand->down;
                if (rand==NULL)
                {
                    ok=0;
                    break;
                }
                randuri++;
                nod=rand->head;
            }
            noduri++;
        }
        // Cat timp corespund literele
        while ((cuvant_vechi[i]==nod->data)&&(i<lungime))
        {
            verif++;
            nod=nod->next;
            i++;
        }
        // Daca lungimea cuvantului de sters = cuvantului gasit
        if (verif==lungime)
        {
            aparitii[j].val=1;
            aparitii[j].nodul=noduri;
            aparitii[j].randul=randuri;
            aparitii[j].verif=verif;
            j++;
            noduri=noduri+verif;
        }
    }
    // Stergem nodurile cuvantului vechi descrescator pentru a sterge fix ce trebuie
    int lungime_aparitii=j;
    for (i=lungime_aparitii-1; i>=0; i--)
    {
        if (aparitii[i].verif==lungime)
        {
            j=0;
            while (j<lungime)
            {
                del_nod_din_rand(bloc,aparitii[i].nodul,aparitii[i].randul);
                j++;
            }
        }
    }
    // Inseram nodurile cuvantului nod crescator pentru a pastra pozitia exacta
    for (i=0; i<lungime_aparitii; i++)
    {
        if (aparitii[i].verif==lungime)
        {
            j=0;
            while (j<lungime_nou)
            {
                add_nod_in_rand(bloc,cuvant_nou[j],aparitii[i].nodul+j,aparitii[i].randul);
                j++;
            }
        }
    }
    return aparitii;
}

node_s *replace_word(BlocList *bloc,char cuvant_vechi[],char cuvant_nou[])
{
    // Functia de inlocuit un cuvant vechi cu unul nou
    node_s *comanda_noua=malloc(sizeof(node_s));
    strcpy(comanda_noua->comanda,"re");
    strcpy(comanda_noua->caractere_memorate,cuvant_vechi);
    strcpy(comanda_noua->caractere_memorate_noi,cuvant_nou);
    int lungime,ok=1,verif,noduri=1,i,j,pozitie,rand_curent=1;
    lungime=strlen(cuvant_vechi);
    TList *rand;
    node *nod;
    rand=bloc->inceput;
    for (i=0; i<nr_rand-1; i++)
    {
        rand=rand->down;
        rand_curent++;
    }
    nod=rand->head;
    for (i=0; i<nr_nod-1; i++)
    {
        nod=nod->next;
        noduri++;
    }
    while(ok==1)
    {
        i=0;
        verif=0;
        if(cuvant_vechi[i]!=nod->data)
        {
            nod=nod->next;
            if (nod==NULL)
            {
                noduri=0;
                rand=rand->down;
                rand_curent++;
                nod=rand->head;
            }
            noduri++;
        }

        while ((cuvant_vechi[i]==nod->data)&&(i<lungime))
        {
            verif++;
            nod=nod->next;
            i++;
        }
        if (verif==lungime)
        {
            ok=0;
            pozitie=noduri;
        }
        noduri=noduri+i;

    }
    // Daca acesta e cuvantul de schimbat, il scoate pe cel vechi din lista
    if (verif==lungime)
    {
        j=0;
        comanda_noua->cursor_vechi_nod=pozitie-1;
        comanda_noua->cursor_vechi_rand=rand_curent;
        node *po;
        while (j<lungime)
        {
            po=stergere_nod(rand,pozitie-1);
            free(po);
            j++;
        }
    }
    // Inseram nodurile cuvantului nod crescator pentru a pastra pozitia exacta
    int lungime2=strlen(cuvant_nou);
    for (i=0; i<lungime2; i++)
    {
        add(rand,pozitie-1,cuvant_nou[i]);
        pozitie++;

    }
    return comanda_noua;
}

Stack *undo_text(BlocList *bloc,Stack *undo,Stack *redo)
{
    // Stiva de undo si prelucrarea acesteia
    node_s *ultima_comanda;
    ultima_comanda=pop_stack(undo);
    char u_com[100];
    int verificare;
    strcpy(u_com,ultima_comanda->comanda);
    // Daca ultima comanda este delete
    verificare=strcmp(u_com,"d");
    if (verificare==0)
    {
        // Daca am de pus inapoi doar un nod
        if(ultima_comanda->nr_noduri==1)
        {
            add_nod_in_rand(bloc,ultima_comanda->nod_memorat,ultima_comanda->cursor_vechi_nod,ultima_comanda->cursor_vechi_rand);
        }
        else
        {
            // Daca am mai multe noduri de pus inapoi
            int i,poz=ultima_comanda->cursor_vechi_nod;
            char rand_mem[100];
            strcpy(rand_mem,ultima_comanda->caractere_memorate);
            for (i=0; i<ultima_comanda->nr_noduri; i++)
            {
                add_nod_in_rand(bloc,rand_mem[i],poz,ultima_comanda->cursor_vechi_rand);
                poz++;
            }
        }
        // Adaugam comanda in stiva de redo
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este backspace
    verificare=strcmp(u_com,"b");
    if (verificare==0)
    {
        add_nod_in_rand(bloc,ultima_comanda->nod_memorat,ultima_comanda->cursor_vechi_nod,ultima_comanda->cursor_vechi_rand);
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este delete line
    verificare=strcmp(u_com,"dl");
    if (verificare==0)
    {
        TList *p;
        init_rand(&p);
        char cuv[100];
        strcpy(cuv,ultima_comanda->rand_memorat);
        int i=0, l=strlen(cuv);
        while (i<l)
        {
            add(p,i,cuv[i]);
            i++;
        }
        add_rand(p,bloc,ultima_comanda->cursor_vechi_rand);
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este goto line
    verificare=strcmp(u_com,"gl");
    if (verificare==0)
    {
        goto_char(ultima_comanda->cursor_vechi_rand,ultima_comanda->cursor_vechi_nod);
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este goto char
    verificare=strcmp(u_com,"gc");
    if (verificare==0)
    {
        goto_char(ultima_comanda->cursor_vechi_rand,ultima_comanda->cursor_vechi_nod);
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este delete word
    verificare=strcmp(u_com,"dw");
    if (verificare==0)
    {
        char cuvant[100];
        strcpy(cuvant,ultima_comanda->caractere_memorate);
        int lungime=strlen(cuvant),i,pozitie=ultima_comanda->cursor_vechi_nod+1;
        // Imi reinsereaza pe pozitia veche nodurile din cuvantul sters
        for (i=0; i<lungime; i++)
        {
            add_nod_in_rand(bloc,cuvant[i],pozitie,ultima_comanda->cursor_vechi_rand);
            pozitie++;
        }
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este delete all words
    verificare=strcmp(u_com,"da");
    if (verificare==0)
    {
        char cuvant[100];
        strcpy(cuvant,ultima_comanda->caractere_memorate);
        int lungime=strlen(cuvant),i,j;
        aparitie *ap;
        ap=ultima_comanda->indici;
        i=0;
        int r,n;
        // Cat timp am cuvinte de repus in bloc
        while (ap[i].verif!=0)
        {
            n=ap[i].nodul;
            r=ap[i].randul;
            lungime=ap[i].verif;
            // Readaug nod cu nod pe pozitia veche
            for (j=0; j<lungime; j++)
            {
                add_nod_in_rand(bloc,cuvant[j],n,r);
                n++;
            }
            i++;
        }
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este replace word
    verificare=strcmp(u_com,"re");
    if (verificare==0)
    {
        char cuvant_vechi[100],cuvant_nou[100];
        strcpy(cuvant_vechi,ultima_comanda->caractere_memorate);
        strcpy(cuvant_nou,ultima_comanda->caractere_memorate_noi);
        int lungime=strlen(cuvant_nou),i,pozitie=ultima_comanda->cursor_vechi_nod+1;
        int lungime_veche=strlen(cuvant_vechi);
        // Sterg nodurile cuvantului pe care l-am inserat in locul celui vechi
        for (i=0; i<lungime; i++)
        {
            del_nod_din_rand(bloc,pozitie,ultima_comanda->cursor_vechi_rand);;
        }
        // Adaug caracter cu caracter din cuvantul vechi
        for (i=0; i<lungime_veche; i++)
        {
            add_nod_in_rand(bloc,cuvant_vechi[i],pozitie,ultima_comanda->cursor_vechi_rand);
            pozitie++;
        }
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este replace all words
    verificare=strcmp(u_com,"ra");
    if (verificare==0)
    {
        char cuvant_vechi[100],cuvant_nou[100];
        strcpy(cuvant_vechi,ultima_comanda->caractere_memorate);
        strcpy(cuvant_nou,ultima_comanda->caractere_memorate_noi);
        int lungime_veche=strlen(cuvant_vechi),lungime_noua=strlen(cuvant_nou);
        int i,j;
        aparitie *ap;
        ap=ultima_comanda->indici;
        i=0;
        int r,n;
        // Cat timp am cuvinte de reinlocuit
        while (ap[i].verif!=0)
        {
            i++;
        }
        i--;
        // Cat timp am cuvinte puse de sters
        while (i>=0)
        {
            n=ap[i].nodul;
            r=ap[i].randul;
            for (j=0; j<lungime_noua; j++)
            {
                del_nod_din_rand(bloc,n,r);
            }
            i--;
        }
        i=0;
        // Readaug nod cu nod pe pozitia veche
        while (ap[i].verif!=0)
        {
            n=ap[i].nodul;
            r=ap[i].randul;
            for (j=0; j<lungime_veche; j++)
            {
                add_nod_in_rand(bloc,cuvant_vechi[j],n,r);
                n++;
            }
            i++;
        }
        add_stack(redo,ultima_comanda);
    }
    // Daca ultima comanda este inserare text
    verificare=strcmp(u_com,"::i");
    if (verificare==0)
    {
        // Daca este de tipul inserare un singur rand in mijlocul unui rand
        if (ultima_comanda->data==2)
        {
            int i=0;
            int lung=strlen(ultima_comanda->caractere_memorate);
            while (i<lung)
            {
                del_nod_din_rand(bloc,ultima_comanda->cursor_vechi_nod+1,ultima_comanda->cursor_vechi_rand);
                i++;
            }
        }
        else
        {
            // Daca a fost inserat un rand la finalul blocului
            if (ultima_comanda->data==1)
            {
                TList *randx;
                randx=bloc->inceput;
                int j=1;
                while ((j<nr_rand)&&(randx->down!=NULL))
                {
                    randx=randx->down;
                    j++;
                }
                char *po;
                if (ultima_comanda->nr_noduri==randx->len-1)
                {
                    po=stergere_rand(bloc,j-1);
                    ultima_comanda->cursor_nou_rand=j-1;
                    ultima_comanda->lungime_rand=j-1;
                    free(po);
                }
                else
                {
                    int i;
                    char rand_mem[100];
                    strcpy(rand_mem,ultima_comanda->caractere_memorate);
                    for (i=0; i<ultima_comanda->nr_noduri; i++)
                    {
                        del_nod_din_rand(bloc,ultima_comanda->cursor_vechi_nod,ultima_comanda->cursor_vechi_rand);
                    }
                }
            }
            else
            {
                // Daca am de scos inserarea la mijlocul unui rand a mai multor randuri
                if (ultima_comanda->data==3)
                {
                    int nr_r=ultima_comanda->cursor_vechi_rand;
                    char *p1,*p2,*p3;
                    p1=stergere_rand(bloc,nr_r);
                    p2=stergere_rand(bloc,nr_r);
                    p3=stergere_rand(bloc,nr_r-1);
                    free(p1);
                    free(p2);
                    free(p3);
                    TList *rand1;
                    init_rand(&rand1);
                    char text_r1[100];
                    strcpy(text_r1,ultima_comanda->caractere_memorate);
                    int i=0,l1=strlen(text_r1);
                    while (i<l1)
                    {
                        add(rand1,i,text_r1[i]);
                        i++;
                    }
                    add(rand1,i,'\0');
                    add_rand(rand1,bloc,nr_r);
                }
                else
                {
                    // Daca a fost inserat un rand undeva in bloc
                    if (ultima_comanda->data==4)
                    {
                        int rand_nr=ultima_comanda->cursor_vechi_rand;
                        stergere_rand(bloc,rand_nr-1);
                    }
                }
            }
        }
        add_stack(redo,ultima_comanda);
    }
    return 0;
}

Stack *redo_text(BlocList *bloc, Stack *undo, Stack *redo)
{
    // Stiva de redo si prelucrarea acesteia
    node_s *ultima_comanda;
    ultima_comanda=pop_stack(redo);
    char u_com[100];
    int verificare;
    strcpy(u_com,ultima_comanda->comanda);
    // Daca ultima comanda este backspace
    verificare=strcmp(u_com,"b");
    if (verificare==0)
    {
        del_nod_din_rand(bloc,ultima_comanda->cursor_vechi_nod,ultima_comanda->cursor_vechi_rand);
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este delete
    verificare=strcmp(u_com,"d");
    if (verificare==0)
    {
        // Daca am de sters inapoi doar un nod
        if(ultima_comanda->nr_noduri==1)
        {
            del_nod_din_rand(bloc,ultima_comanda->cursor_vechi_nod,ultima_comanda->cursor_vechi_rand);
        }
        else
        {
            // Daca am mai multe noduri de sters inapoi
            int i;
            char rand_mem[100];
            strcpy(rand_mem,ultima_comanda->caractere_memorate);
            for (i=0; i<ultima_comanda->nr_noduri; i++)
            {
                del_nod_din_rand(bloc,ultima_comanda->cursor_vechi_nod,ultima_comanda->cursor_vechi_rand);
            }
        }
        // Adaugam comanda in stiva de undo
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este delete line
    verificare=strcmp(u_com,"dl");
    if (verificare==0)
    {
        stergere_rand(bloc,ultima_comanda->cursor_vechi_rand-1);
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este goto line
    verificare=strcmp(u_com,"gl");
    if (verificare==0)
    {
        goto_char(ultima_comanda->cursor_nou_rand,ultima_comanda->cursor_nou_nod);
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este goto char
    verificare=strcmp(u_com,"gc");
    if (verificare==0)
    {
        goto_char(ultima_comanda->cursor_nou_rand,ultima_comanda->cursor_nou_nod);
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este delete word
    verificare=strcmp(u_com,"dw");
    if (verificare==0)
    {
        char cuvant[100];
        strcpy(cuvant,ultima_comanda->caractere_memorate);
        node_s *p;
        p=delete_word(bloc,cuvant);
        free(p);
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este delete all words
    verificare=strcmp(u_com,"da");
    if (verificare==0)
    {
        char cuvant[100];
        strcpy(cuvant,ultima_comanda->caractere_memorate);
        delete_all_words(bloc,cuvant);
        add_stack(undo,ultima_comanda);

    }
    verificare=strcmp(u_com,"re");
    if (verificare==0)
    {
        char cuvant_vechi[100],cuvant_nou[100];
        strcpy(cuvant_vechi,ultima_comanda->caractere_memorate);
        strcpy(cuvant_nou,ultima_comanda->caractere_memorate_noi);
        int lungime=strlen(cuvant_nou),i,pozitie=ultima_comanda->cursor_vechi_nod+1;
        int lungime_veche=strlen(cuvant_vechi);
        for (i=0; i<lungime_veche; i++)
        {
            del_nod_din_rand(bloc,pozitie,ultima_comanda->cursor_vechi_rand);;
        }
        for (i=0; i<lungime; i++)
        {
            add_nod_in_rand(bloc,cuvant_nou[i],pozitie,ultima_comanda->cursor_vechi_rand);
            pozitie++;
        }
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este replace word
    verificare=strcmp(u_com,"ra");
    if (verificare==0)
    {
        char cuvant_vechi[100],cuvant_nou[100];
        strcpy(cuvant_vechi,ultima_comanda->caractere_memorate);
        strcpy(cuvant_nou,ultima_comanda->caractere_memorate_noi);
        int lungime_noua=strlen(cuvant_nou),i;
        int lungime_veche=strlen(cuvant_vechi);
        int j;
        aparitie *ap;
        ap=ultima_comanda->indici;
        i=0;
        // Sterg nodurile cuvantului pe care l-am inserat in locul celui nou
        while (ap[i].verif!=0)
        {
            i++;
        }
        int limita=i;
        i--;
        for (i=limita-1; i>=0; i--)
        {
            j=0;
            while (j<lungime_veche)
            {
                del_nod_din_rand(bloc,ap[i].nodul,ap[i].randul);
                j++;
            }
        }
        // Adaug caracter cu caracter din cuvantul nou
        int retinere=0;
        for (i=0; i<limita; i++)
        {
            j=0;

            while (j<lungime_noua)
            {
                if (retinere!=ap[i].randul)
                {
                    add_nod_in_rand(bloc,cuvant_nou[j],ap[i].nodul+j,ap[i].randul);
                }
                if (retinere==ap[i].randul)
                {
                    add_nod_in_rand(bloc,cuvant_nou[j],ap[i].nodul+j+lungime_noua-lungime_veche,ap[i].randul); //
                }
                j++;
            }
            retinere=ap[i].randul;
        }
        add_stack(undo,ultima_comanda);
    }
    // Daca ultima comanda este inserare text
    verificare=strcmp(u_com,"::i");
    if (verificare==0)
    {
        // Daca este de tipul inserare un singur rand in mijlocul unui rand
        if (ultima_comanda->data==2)
        {
            TList *randx;
            randx=bloc->inceput;
            node *nodx;
            int i=0;
            while(i<(ultima_comanda->cursor_vechi_rand)-1)
            {
                randx=randx->down;
                i++;
            }
            i=0;
            nodx=randx->head;
            while(i<(ultima_comanda->cursor_vechi_nod)-1)
            {
                nodx=nodx->next;
                i++;
            }
            int lung=strlen(ultima_comanda->caractere_memorate);
            i=0;
            while (i<lung)
            {
                add_nod_in_rand(bloc,ultima_comanda->caractere_memorate[i],ultima_comanda->cursor_vechi_nod+i+1,ultima_comanda->cursor_vechi_rand);
                i++;
            }
        }
        else
        {
            // Daca a fost inserat un rand la finalul blocului
            if (ultima_comanda->data==1)
            {
                if ((ultima_comanda->cursor_nou_rand)==(ultima_comanda->lungime_rand))
                {
                    TList *randx;
                    int i;
                    init_rand(&randx);
                    nr_nod=0;
                    for (i=0; i<strlen(ultima_comanda->caractere_memorate); i++)
                    {
                        add(randx,i,ultima_comanda->caractere_memorate[i]);
                        nr_nod++;
                    }
                    add_rand(randx,bloc,ultima_comanda->cursor_vechi_rand);
                }
                else
                {
                    int i,poz=ultima_comanda->cursor_vechi_nod;
                    char rand_mem[100];
                    strcpy(rand_mem,ultima_comanda->caractere_memorate);
                    for (i=0; i<ultima_comanda->nr_noduri; i++)
                    {
                        add_nod_in_rand(bloc,rand_mem[i],poz,ultima_comanda->cursor_vechi_rand);
                        poz++;
                    }
                }
            }
            else
            {
                // Daca a fost inserat un rand undeva in bloc
                if (ultima_comanda->data==4)
                {
                    char text[100];
                    strcpy(text,ultima_comanda->rand_memorat);
                    TList *rand_nou;
                    init_rand(&rand_nou);
                    int l=strlen(text),i;
                    for (i=0; i<l; i++)
                    {
                        add(rand_nou,i,text[i]);
                    }
                    add_rand(rand_nou,bloc,ultima_comanda->cursor_vechi_rand);
                }
            }

        }
        add_stack(undo,ultima_comanda);
    }
    return 0;
}

void inversare_valoare(int *ok)
{
    // Functie menita sa-mi schimbe modul de lucru (insert text si comenzi)
    if (*ok==0)
    {
        *ok=1;
    }
    else *ok=0;
}

void prelucrare_fisier(FILE *input,BlocList *bloc)
{
    int i,j=1,ok=1,comanda,parametrii,verificare,lungime;
    int check_insert=0,sunt_final=0,de_inserat=0,verif_mijl=0,repoz_cursor=0;
    int nr_rand_i=0;
    char parametru[100],*par1,*par2,*par3;
    TList *rand, *rand_i, *rand_adaugat,*rand_ramas;
    FILE *output;
    // Initializam stivele de undo si redo
    Stack *undo,*redo;
    init_s(&undo);
    init_s(&redo);
    node_s *comanda_noua;
    char linie[100];
    nr_rand=1;
    init_rand(&rand_i);
    while (fgets(linie,100,input))
    {
        parametrii=0;
        if (linie[0]==':')
        {
            linie[3]='\0';
        }
        verificare=strcmp(linie,"::i");
        if (verificare==0)
        {
            check_insert=1;
            if (de_inserat==1)
            {
                j=0;
                if (sunt_final==1)
                {
                    /* Aici este cazul in care avem de inserat text
                    (dupa primirea altor comenzi) */
                    comanda_noua=(node_s*)malloc(sizeof(node_s));
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    strcpy(comanda_noua->comanda,"::i");
                    comanda_noua->data=1;
                    verif_mijl=1;
                    int lungime_linie=rand_i->len-1;
                    char text_r1[100],text_r2[100];
                    node *p;
                    // Parcurgem pentru a ajunge la pozitia de inserare
                    TList *r;
                    int i=0;
                    r=bloc->inceput;
                    while (i<nr_rand-1)
                    {
                        r=r->down;
                        i++;
                    }
                    i=0;
                    p=r->head;
                    while (i<r->len-1)
                    {
                        text_r1[i]=p->data;
                        p=p->next;
                        i++;
                    }
                    text_r1[r->len-1]='\0';
                    r=r->down;
                    i=0;
                    p=r->head;
                    while (i<r->len-1)
                    {
                        text_r2[i]=p->data;
                        p=p->next;
                        i++;
                    }
                    text_r2[r->len-1]='\0';
                    node *nod;
                    nod=rand_i->head;
                    int nr_r=nr_rand, nr_n=nr_nod;
                    int k=1;
                    char text[100];
                    for (j=1; j<=lungime_linie; j++)
                    {
                        if (nod->data!='\n')
                        {
                            add_nod_in_rand(bloc,nod->data,nr_n+k,nr_r);
                            text[k-1]=nod->data;
                            k++;
                            nod=nod->next;
                        }
                        else
                        {
                            /* Aici ajunge in cazul in care avem
                            mai mult de 1 rand de inserat */
                            verif_mijl=0;
                            add_nod_in_rand(bloc,'\n',nr_n+k,nr_r);
                            break;
                        }
                        text[k]='\0';
                    }
                    strcpy(comanda_noua->caractere_memorate,text);
                    if (verif_mijl==1)
                    {
                        // Comanda de tipul inserare un singur rand
                        comanda_noua->data=2;
                    }
                    if (verif_mijl==0)
                    {
                        // Comanda de tipul inserare mai mult de 1 rand
                        comanda_noua->data=3;
                        strcpy(comanda_noua->caractere_memorate,text_r1);
                        strcpy(comanda_noua->caractere_memorate_noi,text_r2);
                        int i;
                        TList *parcurgere;
                        parcurgere=bloc->inceput;
                        i=0;
                        while (i<nr_rand)
                        {
                            parcurgere=parcurgere->down;
                            i++;
                        }
                        i=0;
                        if (nod->data=='\n')
                        {
                            /* Cand ajunge la finalul unui rand,
                            cream un rand nou si pozitionam noile
                            randuri conform cerintei */
                            node *parc;
                            TList *parcurg=bloc->inceput;
                            i=0;
                            while (i<nr_r-1)
                            {
                                parcurg=parcurg->down;
                                i++;
                            }
                            parc=parcurg->head;
                            i=0;
                            while (i<nr_n+k-1)
                            {
                                parc=parc->next;
                                i++;
                            }
                            nod=nod->next;
                            init_rand(&rand_ramas);
                            int lung=rand_i->len-1;
                            int l=0;
                            while (l+k<lung)
                            {
                                add(rand_ramas,l,nod->data);
                                nod=nod->next;
                                l++;
                            }
                            add(rand_ramas,l,rand_i->tail->prev->data);
                            add_rand(rand_ramas,bloc,nr_r+1);
                            init_rand(&rand_adaugat);
                            l=0;
                            parc=parc->next;
                            while (parc!=NULL)
                            {
                                add(rand_adaugat,l,parc->data);
                                del_nod_din_rand(bloc,nr_n+k,nr_r);
                                parc=parc->next;
                                l++;
                            }
                            nr_r=nr_r+1;
                            add_rand(rand_adaugat,bloc,nr_r+1);
                            nod=nod->next;
                            nr_n=1;
                            k=0;
                            de_inserat=0;
                        }

                    }
                    add_stack(undo,comanda_noua);
                }
            }
            /* Schimbam modul de lucru
            (din insert text in insert comenzi si invers) */
            inversare_valoare(&ok);
        }
        if ((verificare!=0)&&(ok==1))
        {
            // Daca suntem in modul de scriere text
            init_rand(&rand);
            sunt_final=1;
            if (nr_rand>bloc->len)
            {
                // Inseram text la finalul celui deja existent
                if (check_insert==0)
                {
                    // Daca este prima inserare
                    nr_nod=0;
                    for (i=0; i<strlen(linie); i++)
                    {
                        if (linie[i]!='\0')
                        {
                            add(rand,i,linie[i]);
                        }
                        nr_nod++;
                    }
                    add_rand(rand,bloc,j);
                    nr_rand++;
                    j++;
                    sunt_final=0;
                }
                if (check_insert==1)
                {
                    /* Daca avem de inserat text la final dar
                    dupa primirea altor comenzi inainte */
                    comanda_noua=(node_s*)malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,"::i");
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    comanda_noua->rand_memorat=NULL;
                    comanda_noua->nod_memorat='\0';
                    comanda_noua->data=1;
                    lungime=strlen(linie)-1;
                    comanda_noua->nr_noduri=lungime;
                    strcpy(comanda_noua->caractere_memorate,linie);
                    linie[strlen(linie)]='\0';
                    nr_nod=0;
                    for (i=0; i<strlen(linie); i++)
                    {
                        add(rand,i,linie[i]);
                        nr_nod++;
                    }
                    add_rand(rand,bloc,j);
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                    nr_rand++;
                    j++;
                    sunt_final=0;
                }
            }
            else
            {
                // Eliberam memoria randului
                // Aici inseram textul intr-un rand
                free_rand(&rand);
                de_inserat=1;
                linie[strlen(linie)]='\0';
                int lung=strlen(linie);
                j=0;
                for (i=nr_rand_i; j<lung; i++)
                {
                    if (linie[j]!='\0')
                    {
                        add(rand_i,i,linie[j]);
                    }
                    j++;
                }
                nr_rand_i=i;
            }
        }
        if ((ok==0)&&(verificare!=0))
        {
            // Aici suntem in modul de primit comenzi
            /* Calculam numarul de parametrii primiti
            (cu tot cu comanda in sine) */
            for (i=0; i<strlen(linie); i++)
            {
                if ((linie[i]==' ')||(linie[i]=='\n'))
                {
                    parametrii++;
                }
            }
            if (parametrii==1)
            {
                comanda=strcmp(linie,"q\n");
                if (comanda==0)
                {
                    // QUIT
                    break;
                }
                comanda=strcmp(linie,"s\n");
                if (comanda==0)
                {
                    // SAVE
                    output=fopen("editor.out","w");
                    print_list(bloc,output);
                    fclose(output);
                }
                comanda=strcmp(linie,"dl\n");
                if (comanda==0)
                {
                    // DELETE LINE
                    linie[2]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,linie);
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    char *po;
                    po=delete_line(bloc,nr_rand);
                    char cuv[100];
                    TList *re;
                    re=bloc->inceput;
                    int pa=1;
                    while (pa<nr_rand-1)
                    {
                        re=re->down;
                        pa++;
                    }
                    po[pa-1]='\0';
                    strcpy(cuv,po);
                    free(po);
                    strcpy(comanda_noua->rand_memorat1,cuv);
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(linie,"b\n");
                if (comanda==0)
                {
                    // BACKSPACE
                    linie[1]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,linie);
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    comanda_noua->rand_memorat=NULL;
                    if (repoz_cursor==0)
                    {
                        // Daca suntem la finalul blocului ne repozitionam pe ultimul caracter
                        comanda_noua->nod_memorat=del_nod_din_rand(bloc,nr_nod,nr_rand-1);
                        comanda_noua->cursor_vechi_rand=nr_rand-1;
                        nr_nod--;
                        if (nr_nod<1)
                        {
                            nr_nod=1;
                        }
                    }
                    else
                    {
                        // Daca suntem in mijlocul textului se sterge efectiv.
                        comanda_noua->nod_memorat=del_nod_din_rand(bloc,nr_nod,nr_rand);
                        comanda_noua->cursor_vechi_rand=nr_rand;
                        // Ne repozitionam cursorul cu o pozitie mai la stanga
                        nr_nod--;
                        if (nr_nod<1)
                        {
                            nr_nod=1;
                        }
                    }
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(linie,"d\n");
                if (comanda==0)
                {
                    // DELETE (un singur caracter)
                    linie[1]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,linie);
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    comanda_noua->rand_memorat=NULL;
                    if (repoz_cursor==0)
                    {
                        // Daca suntem la finalul blocului ne repozitionam pe ultimul caracter
                        comanda_noua->nod_memorat=del_nod_din_rand(bloc,nr_nod+1,nr_rand-1);
                        comanda_noua->cursor_vechi_rand=nr_rand-1;
                    }
                    else
                    {
                        // Daca suntem in mijlocul textului se sterge efectiv.
                        comanda_noua->nod_memorat=del_nod_din_rand(bloc,nr_nod+1,nr_rand);
                        comanda_noua->cursor_vechi_rand=nr_rand;
                    }
                    comanda_noua->nr_noduri=1;
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(linie,"u\n");
                if (comanda==0)
                {
                    // UNDO
                    undo_text(bloc,undo,redo);
                }
                comanda=strcmp(linie,"r\n");
                if (comanda==0)
                {
                    // REDO
                    redo_text(bloc,undo,redo);
                }
            }
            if (parametrii==2)
            {
                // Daca comanda introdusa are textul comenzii + 1 parametru
                strcpy(parametru,linie);
                par1=strtok(parametru," ");
                par2=strtok(NULL,"\n");
                comanda=strcmp(par1,"gl");
                if (comanda==0)
                {
                    // Nu este necesara repozitionarea cursorului in comenzile b si d urmatoare
                    repoz_cursor=1;
                    // GOTO LINE
                    char comand[100];
                    strcpy(comand,linie);
                    comand[2]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,comand);
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    goto_line(*par2-'0');
                    comanda_noua->nod_memorat='\0';
                    comanda_noua->rand_memorat=NULL;
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(par1,"dl");
                if (comanda==0)
                {
                    // DELETE LINE
                    char comand[100];
                    strcpy(comand,linie);
                    comand[2]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,comand);
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    comanda_noua->nod_memorat='\0';
                    char *po;
                    char cuv[100];
                    int len=*par2-'0';
                    po=delete_line(bloc,len);
                    TList *re;
                    re=bloc->inceput;
                    int pa=1;
                    while (pa<*par2-'0'-1)
                    {
                        re=re->down;
                        pa++;
                    }
                    po[pa-1]='\0';
                    strcpy(cuv,po);
                    free(po);
                    strcpy(comanda_noua->rand_memorat1,cuv);
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(par1,"gc");
                if (comanda==0)
                {
                    // GOTO CHAR
                    repoz_cursor=1;
                    char comand[100];
                    strcpy(comand,linie);
                    comand[2]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,comand);
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    comanda_noua->nod_memorat='\0';
                    goto_char(nr_rand,*par2-'0');
                    comanda_noua->rand_memorat=NULL;
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(par1,"d");
                if (comanda==0)
                {
                    // DELETE N CARACTERE
                    char comand[100];
                    strcpy(comand,linie);
                    comand[1]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,comand);
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    int de_sters=0;
                    char rand_mem[100];
                    for (de_sters=0; de_sters<*par2-'0'; de_sters++)
                    {
                        rand_mem[de_sters]=del_nod_din_rand(bloc,nr_nod,nr_rand);
                    }
                    comanda_noua->nod_memorat='\0';
                    strcpy(comanda_noua->caractere_memorate,rand_mem);
                    comanda_noua->nr_noduri=*par2-'0';
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(par1,"dw");
                if (comanda==0)
                {
                    // DELETE WORD
                    node_s *comanda_noua;
                    comanda_noua=delete_word(bloc,par2);
                    add_stack(undo,comanda_noua);
                    TList *de_curatat;
                    de_curatat=bloc->inceput;
                    int i=0;
                    while (i<comanda_noua->cursor_vechi_rand-1)
                    {
                        de_curatat=de_curatat->down;
                        i++;
                    }
                    char *po;
                    if (de_curatat->head->data=='\n')
                    {
                        po=stergere_rand(bloc,i);
                        free(po);
                    }

                }
                comanda=strcmp(par1,"da");
                if (comanda==0)
                {
                    // DELETE ALL WORDS
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,"da");
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    strcpy(comanda_noua->caractere_memorate,par2);
                    aparitie *aparitii;
                    aparitii=delete_all_words(bloc,par2);
                    comanda_noua->indici=aparitii;
                    comanda_noua->nod_memorat='\0';
                    comanda_noua->rand_memorat=NULL;
                    add_stack(undo,comanda_noua);
                    TList *de_curatat;
                    de_curatat=bloc->inceput;
                    int i=0;
                    while (i<nr_rand+1)
                    {
                        de_curatat=de_curatat->down;
                        i++;
                    }
                    char *po;
                    if (de_curatat->head->data=='\n')
                    {
                        po=stergere_rand(bloc,i);
                        free(po);
                    }
                }
            }
            if (parametrii==3)
            {
                strcpy(parametru,linie);
                par1=strtok(parametru," ");
                par2=strtok(NULL," ");
                par3=strtok(NULL,"\n");
                comanda=strcmp(par1,"gc");
                if (comanda==0)
                {
                    // GOTO CHAR LINE
                    repoz_cursor=1;
                    char comand[100];
                    strcpy(comand,linie);
                    comand[2]='\0';
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,comand);
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    comanda_noua->nod_memorat='\0';
                    goto_char(*par3-'0',*par2-'0');
                    comanda_noua->rand_memorat=NULL;
                    comanda_noua->cursor_nou_rand=nr_rand;
                    comanda_noua->cursor_nou_nod=nr_nod;
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(par1,"re");
                if (comanda==0)
                {
                    // REPLACE
                    node_s *comanda_noua;
                    comanda_noua=replace_word(bloc,par2,par3);
                    add_stack(undo,comanda_noua);
                }
                comanda=strcmp(par1,"ra");
                if (comanda==0)
                {
                    // REPLACE ALL
                    node_s *comanda_noua=malloc(sizeof(node_s));
                    strcpy(comanda_noua->comanda,"ra");
                    comanda_noua->cursor_vechi_rand=nr_rand;
                    comanda_noua->cursor_vechi_nod=nr_nod;
                    strcpy(comanda_noua->caractere_memorate,par2);
                    strcpy(comanda_noua->caractere_memorate_noi,par3);
                    aparitie *aparitii;
                    aparitii=replace_all_words(bloc,par2,par3);
                    comanda_noua->indici=aparitii;
                    comanda_noua->nod_memorat='\0';
                    comanda_noua->rand_memorat=NULL;
                    add_stack(undo,comanda_noua);
                }
            }
        }
    }
    // Eliberam randul intermediar si stivele
    free_rand(&rand_i);
    free_s(&undo);
    free_s(&redo);
}

int main()
{
    FILE *input;
    // Initializam blocul de liste
    BlocList *bloc;
    init(&bloc);
    // Deschidem fisierul
    input=fopen("editor.in","r");
    // Functia de prelucrare a datelor
    prelucrare_fisier(input,bloc);
    // Eliberam memoria si inchidem fisierul de input
    free_list(&bloc);
    fclose(input);
    return 0;
}
