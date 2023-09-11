# Prova Finale di Algoritmi e Strutture Dati 2022-2023

#### ***Lorenzo Bardelli***

**performance**

| memoria max a runtime | tempo di esecuzione |
| --------------------- | ------------------- |
|  40,2 mB               | ~ 5,7s             |

*N.B. le performance sono basate sui test non pubblici eseguiti dal server verificatore*

**commenti**

Per il salvataggio delle stazioni di servizio necessitavo di una struttura dati abbastanza efficiente per gli inserimenti/ cancellazioni e al contempo per la ricerca, ho optato quindi per un albero red-black. 

per l'algoritmo di pianifica percorso ho utilizzato soluzioni differenti per i due casi (verso sx e verso dx):

per i percorsi classici verso dx, procedendo a ritroso considero sempre la stazione raggiungibile più lontana, e se riesco a giungere alla stazione iniziale il percorso è già quello ottimo.

per i percorsi verso sx ho implementato un algoritmo in stile BFS partendo anche qui dalla stazione finale, così facendo garantisco al contempo il percorso minore e ottimo in termini di distanza dall' inizio dell' autostrada, per costruzione infatti il primo percorso trovato è quello ottimo.

Un ottimizzazione ulteriore sul meccanismo di aggiunta/ demolizione auto avrebbe migliorato ulteriormente il punteggio, essendo quello il bottleneck principale.
