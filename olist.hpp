/**
  @file olist.hpp

  @brief File header della classe ordered_list templata

  File di dichiarazioni/definizioni della classe ordered_list templata
*/

#ifndef OLIST_HPP
#define OLIST_HPP

#include <algorithm> // std::swap
#include <iostream> // std::ostream
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
  @brief classe lista ordinata

  La classe implementa una generica lista ordinata di oggetti T.
  L'ordinamento è realizzato tramite il funtore Compare che prende
  due valori a e b, e ritorna vero se a viene prima di b.
  La valutazione se due dati di tipo T sono uguali è realizzata
  tramite un secondo funtore Equal. 
*/
template <typename T, typename Compare, typename Equal>
class ordered_list {

  /**
    @brief struttura nodo

    Struttura dati nodo interna che viene usata per creare
    la lista.  
  */
  struct node {
    T value;    ///< valore da memorizzare 
    node *next; ///< puntatore al prossimo nodo della lista

    /**
      Costruttore di default
      @post next == nullptr
    */
    node() : next(nullptr) {}

    /**
      Costruttore secondario

      @param v valore da colpiare
      @param n puntatore a next 

      @post next == n
      @post value == v
    */
    node(const T &v, node *n) : value(v), next(n) {}

    // NOTA: Gli altri metodi fondamentali possiamo non implementarli
    // perchè non sono usati e quelli di default vanno bene.
  }; // fine node

  node *_head;  ///< puntatore al primo nodo della lista
  Compare _cmp; ///< funtore per il confronto tra dati T
  Equal _eql;   ///< funtore per l'uguaglianza tra dati T

public:

  /**
    Costruttore di default.

    @post _head == nullptr
  */
  ordered_list() : _head(nullptr) {}

  /**
    Copy constructor

    @param other lista da copiare

    @throw eccezione di allocazione
  */
  ordered_list(const ordered_list &other) : _head(nullptr) {
    node *o = other._head;

    // Cicliamo sui nodi di other e mano a mano
    // aggiungiamo un valore dentro this. La add
    // prevede una new e quindi può fallire. Per questo
    // dobbiamo gestire l'eventuale eccezione che si 
    // genera con un blocco try/catch
    try {
      while(o!=nullptr) {
        add(o->value); 
        o = o->next;
      }
    }
    catch(...) {
      // Se c'e' un problema, svuotiamo la lista e rilanciamo
      // l'eccezione
      empty(); 
      throw;
    }
  }

  /**
    Operatore di assegnamento

    @param other lista da copiare

    @return reference alla lista this
    
    @throw eccezione di allocazione
  */
  ordered_list& operator=(const ordered_list &other) {

    if (&other != this) {
      ordered_list tmp(other);
      this->swap(tmp); 
    }

    return *this;
  }

  /**
    Distruttore 

    @post _head == nullptr
  */
  ~ordered_list() {
    empty(); 
  }

  /**
    Costruttore che crea una ordered_list riempita con dati
    presi da una sequenza identificata da uniteratore di 
    inizio e uno di fine. Gli iteratori possono essere di
    qualunque tipo. 

    @param begin iteratore di inizio sequenza
    @param end iteratore di fine sequenza

    @throw std::bad_alloc possibile eccezione di allocazione
  */
  template <typename Iter>
  ordered_list(Iter begin, Iter end) : _head(nullptr) {
    Iter curr = begin;
    try {
      for(; curr!=end; ++curr) {
        add(static_cast<T>(*curr));
      }
    }
    catch(...) {
      empty();
      throw;
    }
  }

  /**
    Aggiunge un elemento nella lista mantenendo l'ordinamento
    indotto dal funtore di tipo Compare.

    @param value valore da inserire nella lista

    @throw std::bad_alloc possibile eccezione di allocazione
  */
  void add(const T &value) {
    node *temp = new node(value,nullptr);

    if (_head==nullptr) {
      _head = temp;
    }
    else {
      node *curr = _head;
      node *prev = _head;
      while(curr!=nullptr && _cmp(curr->value, value)) {
        prev = curr;
        curr = curr->next;
      }

      if (curr!=nullptr) {
        if (curr == _head) {
          temp->next = _head;
          _head = temp;
        }
        else {
          prev->next = temp;
          temp->next = curr;
        }
      }
      else {
        prev->next = temp;
      }
    }
  }

  /**
    Svuota la lista

    @post _head == nullptr
  */
  void empty(void) {
    node *temp=nullptr;
    while(_head != nullptr) {
      temp = _head->next;
      delete _head;
      _head = temp;
    }
  }

  /**
    Funzione scambia lo stato tra l'istanza corrente di
    ordered_List e quella passata come parametro.

    @param other ordered_list con cui scambiare o stato
  */
  void swap(ordered_list &other) {
    std::swap(_head,other._head);
    std::swap(_cmp,other._cmp);
    std::swap(_eql,other._eql);
  }

  /**
    Ritorna il numero di elementi nella lista.

    @return il numero di elementi nella lista
  */
  unsigned int size(void) const {
    // Il conteggio è fatto ad ogni chiamata. 
    // Si poteva avere una variabile di stato che
    // manteneva il valore.
    unsigned int sz = 0;
    node *curr = _head;
    while (curr!=nullptr) {
      ++sz;
      curr = curr->next;
    }
    return sz;
  }

  /**
    Cerca se un valore passato è presente nella lista. Usa il
    funtore di uguaglianza di tipo Equal.

    @param value valore da cercare nella lista

    @return true se il valore è stato trovato
  */ 
  bool find(const T &value) const {
    node *curr = _head;

    while (curr!=nullptr && !_eql(curr->value,value)) { 
      curr = curr->next;
    }

    return (curr!=nullptr);
  }

  /**
    Funzione GLOBALE che implementa l'operatore di stream.
    La funzione è templata sull'ordered list ed è messa
    friend perchè accediamo ai dati privati di ordered_list.

    @param os stream di output
    @param ol ordered_list da spedire sullo stream

    @return lo stream di output
  */
  friend std::ostream& operator<<(std::ostream &os, const ordered_list &ol) {
    node *curr = ol._head;

    while (curr !=nullptr) {
      os << curr->value << " ";
      curr = curr->next;
    }

    return os;
  }

  /**
    Gli iteratori devono iterare sui dati inseriti nella classe
    principale. Siccome nella ordered_list mettiamo dei dati di 
    tipo T, l'iteratore deve iterare sui dati di tipo T. NON sui
    nodi della lista che sono un dettaglio implementativo interno.
  */
  class const_iterator {
    //  
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T                         value_type;
    typedef ptrdiff_t                 difference_type;
    typedef const T*                  pointer;
    typedef const T&                  reference;

  
    const_iterator() : n(nullptr) {}
    
    const_iterator(const const_iterator &other) : n(other.n) {}

    const_iterator& operator=(const const_iterator &other) {
      n = other.n;
      return *this;
    }

    ~const_iterator() {}

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return n->value;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return &(n->value);
    }
    
    // Operatore di iterazione post-incremento
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      n = n->next;
      return tmp;
    }

    // Operatore di iterazione pre-incremento
    const_iterator& operator++() {
      n = n->next;
      return *this;
    }

    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return n == other.n;
    }
    
    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return !(other == *this);
    }

    // Solo se serve anche iterator aggiungere le precedenti definizioni

  private:
    const node *n;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class ordered_list; 

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    const_iterator(const node *nn) : n(nn) { }
    
    
  }; // classe const_iterator
  
  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator begin() const {
    return const_iterator(_head);
  }
  
  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator end() const {
    return const_iterator(nullptr);
  }


}; // fine class ordered_List

/**
  Funzione GLOBALE che stampa a schermo i soli valori 
  contenuti in una generica ordered_list che soddisfano 
  un predicato generico di tipo P.

  @param OL ordered_list sorgente
  @param pred predicato
*/
template <typename T, typename Comp, typename Equal, typename P>
void checkif(const ordered_list<T,Comp,Equal> &OL, P pred) {
  typename ordered_list<T,Comp,Equal>::const_iterator i, ie;

  for(i=OL.begin(), ie = OL.end(); i!=ie; ++i) {
    if (pred(*i)) 
      std::cout << *i << std::endl;
  }
}


#endif
