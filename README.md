Gul Farhad Ali Irinel | Grupa 314CC
Structuri de date - Tema 1

<=================================<STRUCTURI>=================================>
In realizarea acestei teme am decis sa pun intr-un fisier separat numit
"structuri.h" toate structurile folosite. Am sa explic in cateva randuri 
semnificatia fiecarei structuri in parte:
0. Nodul (unitatea de baza) contine un singur caracter. O lista (rand) contine 
mai multe noduri (caractere). Un bloc de liste contine mai multe liste.
1. typedef struct node - structura de baza a nodului de lista care contine 
data in format char, pointer la nodul urmator si pointer la nodul din urma.
2. typedef struct TList - contine pointeri la nodul cap (de inceput), la 
nodul de final (tail), la lista care este pozitionata pe randul urmator,
la lista care se afla deasupra (voi explica mai jos logica aceasta) si
lungimea listei respective (numarul de noduri din lista).
3. typedef struct BlocList - aici o sa fie nevoie de o reprezentare
spre o intelegere mai eficienta. Ideea de baza ar fi ca am folosit o
structura care sa aiba in componenta pointer la prima lista (primul rand)
din toate listele mele (din blocul meu de liste), pointer la ultimul rand
si lungimea blocului (numarul de randuri.

"abcdefg" - o lista (un rand) care contine 7 noduri.

"abcdefg
jklmnopq" - un bloc de liste (de randuri) care contine 2 randuri / liste.

Ideea blocului e putin diferita de cea a unei liste de liste. O lista de
liste ar avea in componenta noduri care ar indica (pointa) la o lista
tip rand. Blocul de liste are in componenta directa pointer la lista de
tip rand. Oarecum asemanatoare cele doua tipuri de implementari.
4. typedef struct Stack - are in componenta un pointer special pe care il
explic mai jos care pointeaza la primul nod din stiva (cel de deasupra)
si numarul de elemente din stiva.
5. typedef struct aparitie - folosit pentru functiile de replace all 
si delete all pentru a stii exact urmatoarele date: val - daca exista
date in pozitia indicata (spre a elibera memoria in functia de free_stack),
verif - numara numarul de caractere al cuvantului de inlocuit / sters
pentru a putea verifica daca corespunde cu cel curent gasit in text,
randul - indica randul si nodul - indica nodul in care este gasit cuvantul.
6. typedef struct node_s - cea mai plina structura din cod, nod de baza
pentru implementarea stivelor de undo si redo. Are in componenta urmatoarele:
data - verifica ce tip de inserare text am(initiala, in mijlocul randului
de un singur rand, in mijlocul randului dar mai multe randuri de inserat
sau la finalul randurilor, comanda - retine comanda executata, 
caractere_memorare / caractere_memorare_noi - retin cuvintele in cazul
replace all / delete all / replace / delete, cursor_vechi_rand si
cursor_vechi_nod - retin pozitiile vechi ale cursorului inainte de operatie,
cursor_nou_rand si cursor_nou_nod - retin pozitiile noi ale cursorului 
dupa comanda efectuata, rand_memorat - folosit in cazul delete line, 
rand_memorat1 - asemanator celui de dinainte doar ca folosit pentru alte
tipuri de functii, nod_memorat - retine un caracter, lungime_rand - retine
lungimea randului operat, next - pointer la urmatorul nod din stiva si
indici - retine indicii cuvintelor in cazul replace all si delete word.