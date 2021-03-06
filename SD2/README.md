Copyright 2020 Pescaru Tudor-Mihai 315CA

# Count-distinct problem

## I. Vector de frecventa

Am alocat dinamic un vector de frecventa cu 2000002 elemente pentru a stoca 
frecventa fiecarui numar de la 0 la 2000000. Am citit input de la stdin pana 
am ajuns la EOF si am incrementat valoarea din vector de la indexul 
elementului. Am afisat fiecare element cu frecventa != 0 impreuna cu 
frecventa sa.

## II. Hashtable cu open addresing

Am declarat un tip de structura info ce contine o cheie de tip string si un 
intreg count ca valoare. Structura pentru Hashtable contine un nr maxim de
bucket-uri si un vector de structuri info ca vector de bucketuri. Am folosit 
functii pentru initializare ce aloca memoria pentru vectorul de bucketuri, 
functie de adaugare, functie de printare a elementelor, de eliberare a 
memoriei, o functie de hash si una de comparare a cheilor, luate din 
laboratorul de Hashtable. Functia de adaugare calculeaza index-ul bucket-ului 
ce ar trebui folosit si verifica daca acesta este liber, caz in care adauga 
elementul cheia si frecventa egala cu 1. In caz contrar verifica daca deja se 
afla acelasi element acolo comparand cheile si creste frecventa in caz 
afirmativ. Altfel cauta urmatorul bucket liber sau un bucket care deja contine 
elementul. Daca se ajunge la index maxim urmatoarea incrementare duce la index 
0 pentru a asigura continuitate. Fucntia de print afisieaza toate elementele 
si frecventa lor. Initial se face o trecere prin input-uri pentru a determina 
numarul de bucketuri cu care trebuie initializat Hashtable-ul. Mai apoi se 
reiau inputurile si se adauga in Hashtable. La final se afiseaza si se 
elibereaza memoria.

## III. Estimatori probabilistici

Pentru o acuratete cat mai buna am ales o impartire de 11 cu 21 biti intre 
portiunea pentru index si cea pentru calcularea valorii din bucket. Am 
utilizat documentatia oficiala a algoritmului HyperLogLog, si am considerat 
poztitia celui mai din stanga bit de 1 din portiunea de valoare drept valoarea 
ce se va introduce in bucket. Aceasta pozitie este indexata incepand de la 1, 
incepand din partea stanga. Pentru a obtine partea de index am aplicata o 
masca pentru a pastra doar cei mai din stanga 11 biti si i-am shiftat pana 
pe pozitia cea mai din dreapta pentru a afla valoarea. Pentru a afla cealalta 
portiune am aplicat o maska pentru a elimina cei mai din stanga 11 biti. Apoi, 
facund un & intre numar si 2^(bits-1) pentru bits de la 21 la 0 am aflat 
pozitia celui mai din stanga bit de 1 si am adaugat-o la vector. In calcularea 
agregarii am folosit doar valorile bucket-urilor utilizate iar pentru factorul 
de atenuare si pentru rezultatul final am folosit doar numarul de bucket-uri 
utilizate.

