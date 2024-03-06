# oriented graph - febbraio 2024

This is my implementation of the oriented graph for the cpp course at unimib.

Points:
- 26/26
- +2 for custom exceptions
- +2 for extended code coverage

## utilizzo

- `make` per compilare il progetto
- `make docs` per generare la documentazione Doxygen

Il makefile include delle feature extra che semplificano la vita:

- `make test` compila ed esegue il programma usando [ASAN](https://www.cse.unsw.edu.au/%7Elearn/debugging/modules/asan/), una instrumentazione simile a valgrind che riconosce
 errori a runtime. Se il programma ha errori di memoria, verrà stampato il tipo di errore e il punto esatto del codice responsabile.

- `make coverage` (solo su linux, utilizza i programmi less, grep, e gcov) Mostra quali parti del codice della matrice non sono
  state eseguite durante i test. Le righe non eseguite iniziano con `#####`.
  Per chiudere il programma premere `q`. Per passare alla riga non eseguita successiva o precedente premere `n` o `N`.


## Note

La matrice di adiacenza per il grafo orientato è implementata tramite matrice di interi: `int **`.
Il prof si aspetta una implementazione diversa tramite una classe matrice, poichè è un sistema che porta a meno errori di memoria.

Tuttavia:

- Implementare la matrice tramite classe aumenta notevolmente le linee di codice, e quindi la "superfice di attacco" su cui il prof può trovare errori.
- implementare la matrice tramite `int **` richiede buona comprensione della gestione della memoria, e test manuali su errori di allocazione. vedi [questo esempio](https://github.com/robalb/cpp-matrix/blob/88d0a004d8a20a0ec19af3287682296c2559b9bf/ograph.hpp#L346) ma richiede anche meno codice


## penalizzazioni comuni progetto c++

- test insufficenti (-2)
- test solo su tipi standard (-2)
- totale o parziale mancanza di funtori di confronto nella classe (fino a -3)
- mancata gestione della memoria/eccezioni (-1 per occorrenza)
- dimenticanza di un metodo richiesto (-5)
- documentazione minimale/insufficente (-2)
- non è stata messa la MAIL dello studente nel pdf della consegna (-2)
- mancanza di const/ref nel passaggio di parametri generici (-0.5 per occorrenza)
- metodi che espongono la struttura interna della classe (-1 per occorrenza)




