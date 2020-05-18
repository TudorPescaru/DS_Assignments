Copyright 2020 Pescaru Tudor-Mihai 
# Tema 3 - Academia Network

## Structuri

Pentru implementarea acestei teme am ales sa utilizam mai multe structuri,
fiecare fiind definita cu un anumit scop, pentru a rezolva un anumit task.
De asemenea, avem si cateva structuri implementate generic pentru a facilita 
un numar mare de intrebuintari.
Structuri generice:
 * Node - contine pointer la nodul urmator s la cel precedent; data este 
 reprezentata prin `void*` pentru a permite stocare diferitelor elemente;
 * LinkedList - contine pointeri la primul si ultimul nod si tine cont de 
 dimensiune;
 * HashTable - contine un array de LinkedList, buckets si dimensiunea acestuia;
Structuri personalizate:
 * PaperData - contine toate datele aferente unui paper;
 * CitData - este utilizat intr-un Hashtable pe id-uri de paper-uri pentru 
 a mentine un contor de citari al fiecarui paper;
 * VenData - este utilizat intr-un Hashtable pe nume de jurnale pentru a 
 realiza corespondenta dintre un paper si jurnalul in care a fost publicat;
 * FieldData - este utilizat intr-un Hashtable pe domenii pentru a realiza 
 corespondenta dintre un paper si domeniul sau;
 * InflData - este utilizat intr-un Hashtable pe id-uri de paper-uri pentru a
 realiza corespondente intre paper-uri si paper-urile influentate;
 * AuthorData - este utilizat intr-un Hashtable pe id-uri de autori pentru a 
 realiza corespondente intre un autor si articolele publicate de acesta, 
 stocate prin LinkedList de id-uri;
 * CollabData - este utilizat intr-un Hashtable pe id-uri de autori pentru a 
 realiza corespondente intre un autor si autorii cu care acesta a colaborat,
 stocati prin LinkedList de id-uri;
 * **PublData** - structura principala pnetru acest API, contine toate 
 Hashtable-urile mentionate anterior si un array ce stocheaza numarul de 
 articole publicate in fiecare an;

## Functii auxiliare

* hash - functie de hash pe string-uri, luata din laboratorul 4 si utilizata 
pentru hashtable-urile care folosesc string-uri drept cheie;
* init_list - functie ce initializeaza un LinkedList;
* add_node - functia generica de adaugare de nod in lista, foloseste `void*` 
pentru data;
* add_cit - functie ce adauga structuri de tip CitData in Hashtable-ul de 
citari;
* add_ven - functie ce adauga structuri de tip VenData in Hashtable-ul de 
jurnale de publicate;
* add_field - functie ce adauga structuri de tip FieldData in Hashtable-ul
de field-uri;
* add_infl - functie ce adauga structuri de tip InflData in Hashtable-ul de 
influente pentru paper-uri;
* add_author - functie ce adauga o structura de tip AuthorData in Hashtable-ul
de paper-uri publicate de autori sau updateaza o structura deja existenta de 
acest tip, prin adaugarea id-ului unui paper realizat de autorul respectiv;
* add_collab - functie ce adauga o structura de tip CollabData in Hashtable-ul
de colaboratori ai unui autor sau updateaza o structura deja existenta de 
acest tip, prin adaugarea id-ului unui nou colaborator;
* remove_front - sterge primul nod dintr-o lista, functie utilizata pentru 
emularea unei cozi;
* remove_back - sterge ultimul nod dintr-o lista, functie utilizata pentru
emularea unei stive;
* in_list - functie generica de verificare a existentei unui element intr-o 
lista;
* in_list_id - functie ce verifica existenta unui id intr-o lista de id-uri;
* has_citations - functie ce returneaza o structura de tip CitData pentru un 
id dat, din Hashtable-ul de citari;
* find_paper - functie ce returneaza o structura de tip PaperData pentru un 
id dat, din Hashtable-ul principal;
* find_collabs - functie ce returneaza o structura de tip CollabData pentru 
un id de autor dat, din Hashtable-ul de colaboratori;
* free_list - functie ce elibereaza memoria alocata unei liste, pentru acele 
liste in care nu este necesara eliberarea memoriei pentru data din noduri;
* free_list_data - functie cu acelasi rol ca si precedenta, dar care elibereaza
 si memoria pentru data din noduri;
* destroy_paper_data - functie ce elibereaza memoria alocata unei structuri de 
tip PaperData;

## init_publ_data & destroy_publ_data

Aceste functii aloca si elibereaza memoria pentru fiecare componenta a 
structurii principale. Functia de eliberare se foloseste si de functiile 
auxiliare de eliberare a memoriei pentru a elibera memoria utilizata de 
diferitele structuri folosite.

## 0 - add_paper

In prinicpal, functia consta in preprocesarile necesare realizarii task-urilor 
prin popularea componentelor structurii principale cu elemente precum datele 
generale despre paper, incrementarea numarului de citari pentru fiecare paper, 
realizarea unei evidente pentru influentele fiecarui paper si respectiv pentru 
colaboratorii unui autor sau pentru paper-urile publicate de un autor. De 
asemenea aici sunt categorizate paper-urile in functie de field si venue si 
este realizata o histograma a numarului de articole publicate pe an.

## 1 - get_oldest_influence

Pentru acest task am implementat un algoritm de tip BFS pentru a realiza o 
parcurgere prin graful de referinte a unui paper dat. Am utilizat 
Hashtable-urile de paper-uri si de citari. Primul pentru a avea acces rapid la 
referintele fiecarui paper iar al doilea pentru a avea acces rapid la numarul 
de citari al fiecarui articol pentru a realiza departajarea intre acestea.

## 2 - get_venue_impact_factor

Am utilizat Hashtable-urile de venue-uri si citari. Primul pentru a avea 
acces rapid la toate paper-urile publicate intr-un jurnal dat iar cel de-al 
doilea pentru a putea numara usor numarul de citari pentru fiecare din aceste 
articole

## 3 - get_number_of_influenced_papers

Am aplicat un algoritm de tip DFS pentru a realiza o parcurgere prin graful de 
paper-uri influentate. Am utilizat Hashtable-ul de influente pentru a gasii 
rapid, pentru fiecare paper, paper-urile influentate de acesta. De asemenea, 
pentru cazul in care distanta ceruta este 1 am returnat direct numarul de 
citari ale paper-ului dat din Hashtable-ul de citari.

## 4 - get_erdos_distance

Am utilizat un algoritm de tip BFS pentru a parcurge graful de colaboratori, 
pentru un autor dat, utilizand Hashtable-ul de colaboratori. De asemenea am 
folosit si un array distante pentru a mentine evidenta distantei parcurse in 
graf.

Probleme performanta:
* get_erdos_distance_task_0 obtine un timp de 0.6 sec
* get_erdos_distance_task_1 obtine un timp de 0.9 sec
* get_erdos_distance_task_2 prezinta o discrepanta uriasa fata de celelalte 
doua teste printr-un timp de 1 min 20 sec

## 5 - get_most_cited_papers_by_field

Am folosit Hashtable-urile de field-uri si de citari. Primul pentru a avea 
acces rapid la toate paper-urile publicate in field-ul dat iar al doilea 
pentru a putea realiza departajarea si sortarea celor mai citate.

## 6 - get_number_of_papers_between_dates

Am utilizat array-ul in care am numarat paper-urile pe ani si am adunat 
numarul de paper-uri pentru fiecare an intre anii dati.

## 7 - get_number_of_authors_with_field

Am utilizat Hashtable-ulrile de field-uri si cel principal de paper-uri. 
Primul a fost utilizat pentru a accesa rapid toate paper-urile publicate in 
field-ul dat iar celalat pentru a obtine informatiile despre autorii fiecarui 
paper. Am utilizat si o lista de autori pentru a marca autorii numarati deja 
pentru a nu numara duplicate.

## 8 - get_histogram_of_citations

Am utilizat Hashtable-ul de paper-uri publicate de catre un autor pentru a 
avea posibilitatea de a parcurge in mod rapid toate paper-urile publicate de 
un autor dat. Prin aceasta parcurgere am determinat si cel mai vechi articol 
al autorului, pentru a putea determina dimensiunea histogramei. De asemenea am 
utilizat si Hashtable-ul de citari pentru a putea obtine usor numarul de 
citari pentru fiecare paper. Folosind acestea si anul de publicatie al 
fiecarui paper, am realizat histograma.

## 9 - get_reading_order

## 10 - find_best_coordinator
