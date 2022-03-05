//STRUCTURILE PENTRU LISTE (RANDURI):
typedef struct node
{
    char data;
    struct node * next;
    struct node * prev;
} node;

typedef struct TList
{
    node *head;
    node *tail;
    struct TList *down;
    struct TList *up;
    int len;
}  TList;

//STRUCTURA PENTRU BLOCUL DE LISTE:
typedef struct BlocList
{
    TList *inceput;
    TList *sfarsit;
    int len;

} BlocList;

//STRUCTURILE PENTRU STIVE:
typedef struct Stack
{
    struct node_s *top;
    int len;
} Stack;

//Structura suplimentara pentru rezolvarea unor functii
typedef struct aparitie
{
    int val;
    int verif;
    int randul;
    int nodul;
} aparitie;

typedef struct node_s
{
    int data;
    char comanda[10],caractere_memorate[100],caractere_memorate_noi[100];
    int cursor_vechi_rand,cursor_vechi_nod,cursor_nou_rand,cursor_nou_nod;
    char *rand_memorat,rand_memorat1[100];
    char nod_memorat;
    int nr_noduri,lungime_rand;
    struct node_s *next;
    aparitie *indici;
} node_s;
