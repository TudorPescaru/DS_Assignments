Copyright 2020 Pescaru Tudor-Mihai 315CA

# Music Playlist

Solutia aleasa implementeaza trei tipuri de structuri: o structura de tip 
Header ce va stoca toate informatiile necesare pentru fiecare melodie (TAG, 
titlu, artist, album si an de aparitie); o structura de tip Doubly Linked List 
de tip liniara ce va stoca un pointer catre inceput, unul catre final, un 
cursor si numarul de noduri; o structura de tip Node ce va contine o structura
de tip header pentru a stoca informatiile melodiei aferente si cate un pointer 
catre nodul urmator, respectiv anterior.

Pentru partea de header am definit doua functii, una pentru citirea din fisier 
a header-ului si pentru stocarea acestuia si una pentru citirea rapida a 
titlului unei melodi, functie ce este utilizata ulterior in cautarea prin 
lista.

Pentru partea de lista, in plus fata de functiile cerute am implementat si 
unele functii auxiliare. O functie ce va initializa lista, apelata la 
inceputul programului, o functie ce va elibera memoria ocupata de lista, de 
nodurile aferente ei si de header-ul fiecarui nod si o functie ce va realiza 
cautarea prin lista pentru a determina daca o melodie exista deja sau nu. In 
cazul in care melodia se afla deja in playlist se va returna un pointer catre 
aceasta pentru a putea fi stearsa in mod eficient.

Pentru a eficientiza codul, unele functii se folosesc de altele. Functiile de 
adaugare se folosesc de functia de DELETE_SONG pentru a sterge melodia adaugata 
in cazul in care ea deja exista. Functia de DELETE_SONG se foloseste, la randul
ei de functia search_dup pentru a gasii locatia melodiei ce trebuie stearsa si 
de functiile DELETE_FIRST si respectiv DELETE_LAST pentru a trata cazurile 
speciale.

Functiile aferente listei au fost separate in fisierele dllist.c/.h iar cele 
aferente header-ului in fisierele header.c/.h.
