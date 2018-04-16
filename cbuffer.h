#ifndef CBUFFER_H
#define CBUFFER_H

#include <algorithm>
#include <iostream>
#include <ostream>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <cassert>

/**
	@brief Buffer circolare

	Classe che implementa un buffer circolare di dati generici T.

	@param T tipo di dato
*/
template <typename T>
class cbuffer {

	private:

		/**
			@brief Elemento del buffer

			Struttura che implementa un elemento del cbuffer.
		*/
		struct item {

			T value;			///< Dato inserito nel buffer
			item *next;		///< Puntatore all'item successivo del buffer

			/**
				@brief Costruttore di default
			*/
			item() : value(0) {}

			/**
				@brief Costruttore secondario

				Costruttore secondario che inizializza un item dato un valore

				@param v valore del dato
			*/
			item(const T &v) : value(v), next(0) {}

			/**
				@brief Costruttore secondario

				Costruttore secondario che istanzia un item dato un valore e puntatore
				all'item successivo

				@param v valore del dato
				@param i puntatore all'elemento successivo
			*/
			item(const T &v, item *i) : value(v) {
				next = i;
			}

			/**
				@brief Costruttore secondario

				Costruttore secondario che istanzia un item dato un puntatore
				all'item successivo.

				@param i puntatore all'elemento successivo
			*/
			item(item *i) {
				next = i;
			}

		};


		item *_head;											///< Puntatore alla testa del cbuffer
		item *_end;			///< Puntatore alla fine del cbuffer (elemento fittizio)
		unsigned int _size;								///< Dimensione del cbuffer

	public:

		/**
			@brief Costruttore di default

			Costruttore di default per istanziare un cbuffer vuoto.
		*/
		cbuffer() : _head(0), _size(0), _end(0) {}

		/**
			@brief Copy constructor

			Costruttore di copia. Permette di istanziare un cbuffer con i valori
			presi da un altro cbuffer.

			@param other buffer da copiare

			@throw Eccezione di allocazione di memoria
		*/
		cbuffer(const cbuffer &other) : _head(0), _size(other._size), _end(0) {

			const item *tmp = other._head;

			if(tmp == 0) return;

			try {
				while(tmp != other._end) {
					insert(tmp->value);
					tmp = tmp->next;
				}
			} catch(...) {
				clear();
				throw;
			}

		}

		/**
			@brief Costruttore secondario

			Costruttore secondario. Permette di istanziare un cbuffer con una data dimensione.

			@param int size capacita' del cbuffer
		*/
		explicit cbuffer(unsigned int size) : _head(0), _size(size), _end(0) {}

		/**
			@brief Costruttore secondario

			Costruttore secondario che costruisce la lista a partire
			da una sequenza generica di dati identificata da due
			iteratori.

			@param begin iteratore di inizio della sequenza
			@param end iteratore di fine della sequenza
			@param size intero capacita' buffer

			@throw eccezione di allocazione di memoria
		*/
		template <typename IterT>
		cbuffer(unsigned int size, IterT begin, IterT end) : _head(0), _size(size), _end(0) {
			try {
				for(; begin!=end; ++begin) {
					insert(static_cast<T>(*begin));
				}
			}
			catch(...) {
				clear();
				throw;
			}
		}

		/**
			@brief Operatore di assegnamento

			Operatore di assegnamento. Permette la copia tra cbuffer.

			@param other cbuffer da copiare

			@return reference a this

			@throw Eccezione di allocazione di memoria
		*/
		cbuffer& operator=(const cbuffer &other) {

				if(this != &other) {
					cbuffer tmp(other);
					std::swap(tmp._head, this->_head);
					std::swap(tmp._end, this->_end);
					std::swap(tmp._size, this->_size);
				}

				return *this;

		}

		/**
			@brief Distruttore

			Distruttore. Rimuove la memoria allocata da cbuffer.
		*/
		~cbuffer() {

			clear();
			if(_end!= 0) {
				delete _end;
				_end = 0;
			}
			_size = 0;

		}

		/**
			@brief Inserimento dati

			Inserisce un elemento in coda al cbuffer

			@param value valore da inserire

			@throw eccezione di allocazione di memoria
		*/
		void insert(const T &value) {

			try {

				item *i = new item(value, _end);

				// Buffer vuoto
				if(_head == 0 && _size > 0) {
					_head = i;
					_end = new item(_head);
					i->next = _end;
					return;
				}

				item *tmp = _head;
				int n = 1;
				while(tmp->next != _end && n<_size) {
					tmp = tmp->next;
					n++;
				}


				if(n<_size) {

					tmp->next = i;
					return;

				} else if(n==_size) {

					item *new_head = _head->next;
					delete _head;
					_head = new_head;

					_end->next = new_head;
					tmp->next = i;

				}

			} catch(...) {

				throw;

			}



		}

		/**
			@brief Rimozione dati

			Cancella il contenuto del cbuffer (non la fine)
		*/
		void clear() {

			item *tmp = _head;
			if(tmp==0)
				return;

			while (tmp != _end) {
				item *n = tmp->next;
				delete tmp;
				tmp = n;
			}

			_head = 0;

		}

		/**
			@brief Rimozione testa

			Cancella l'elemento in testa al cbuffer
		*/
		void delete_head() {

			if(_head==0)
				return;
			item *tmp = _head->next;
			delete _head;
			_head = tmp;
			_end->next = tmp;

		}

		/**
			@brief Capacita' cbuffer

			Ritorna la capacita' del cbuffer

			@return int capacita'
		*/
		int max_size() const {
			return _size;
		}

		/**
			@brief Numero elementi presenti

			Ritorna il numero di elementi presenti nel buffer

			@return int numero elementi inseriti
		*/
		int get_size() const {
			if(_head==0)
				return 0;
			int csize = 0;
			item *tmp = _head;
			while(tmp != _end) {
				tmp = tmp->next;
				csize++;
			}
			return csize;
		}

		/**
			@brief Accesso ai dati in lettura/scrittura

			Metodo per leggere/scrivere il valore index-esimo del cbuffer.

			@pre index < get_size()

			@param int posizione elemento

			@return reference al valore dell'elemento

			@throw eccezione di accesso fuori dai dati
		*/
		T &operator[](unsigned int index) {
			assert(index < get_size());
			item *tmp = _head;
			for(int i=0; i<index; i++) {
				tmp = tmp->next;
			}
			return tmp->value;
		}

		/**
			@brief Accesso ai dati in lettura

			Metodo per leggere il valore index-esimo del cbuffer.

			@pre index < get_size()

			@param index posizione elemento

			@return const reference al valore dell'elemento

			@throw eccezione di accesso fuori dai dati
		*/
		const T &operator[](unsigned int index) const {
			assert(index < get_size());
			item *tmp = _head;
			for(int i=0; i<index; i++) {
				tmp = tmp->next;
			}
			return tmp->value;
		}

		class const_iterator; // forward declaration

		/**
			@brief Iteratore della lista

			Iteratore della lista
		*/
		class iterator {

		  item *i;
			item *i_end;

		  public:
		    typedef std::forward_iterator_tag iterator_category;
		    typedef T                         value_type;
		    typedef ptrdiff_t                 difference_type;
		    typedef T*                        pointer;
		    typedef T&                        reference;


		    iterator() : i(0), i_end(0) {}

		    iterator(const iterator &other) : i(other.i), i_end(other.i_end) {}

		    iterator& operator=(const iterator &other) {

		      i = other.i;
					i_end = other.i_end;
		      return *this;

		    }

		    ~iterator() {

		      i = 0;
					i_end = 0;

		    }

		    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
		    reference operator*() const {

					assert(i!=i_end);
		    	return i->value;

		    }

		    // Ritorna il puntatore al dato riferito dall'iteratore
		    pointer operator->() const {

					assert(i!=i_end);
					return &(i->value);

		    }

		    // Operatore di iterazione post-incremento
		    iterator operator++(int) {

		      iterator tmp(*this);
		      i = i->next;
		      return tmp;

		    }

		    // Operatore di iterazione pre-incremento
		    iterator& operator++() {

		      i = i ->next;
		      return *this;

		    }

		    // Uguaglianza
		    bool operator==(const iterator &other) const {

		      return i == other.i;

		    }

		    // Diversita'
		    bool operator!=(const iterator &other) const {

		      return i != other.i;

		    }

		    friend class const_iterator;

		    // Uguaglianza
		    bool operator==(const const_iterator &other) const {

		      return i == other.i;

		    }

		    // Diversita'
		    bool operator!=(const const_iterator &other) const {

		      return i != other.i;

		    }

		  private:

				/**
					La classe container deve essere messa friend dell'iteratore per poter
					usare il costruttore di inizializzazione.
				*/
		    friend class cbuffer;

				/**
					Costruttore privato di inizializzazione usato dalla classe container
					tipicamente nei metodi begin e end
				*/
		    iterator(item *ii, item *ii_end) : i(ii), i_end(ii_end) {}

		}; // classe iterator

		/**
			@brief Iteratore begin

			Ritorna l'iteratore all'inizio della sequenza dati

			@return iteratore all'inizio della sequenza
		*/
		iterator begin() {
		  return iterator(_head, _end);
		}

		/**
			@brief Iteratore const end

			Ritorna l'iteratore alla fine della sequenza dati

			@return iteratore alla fine della sequenza
		*/
		iterator end() {
		  return iterator(_end, _end);
		}

		/**
			@brief Iteratore costante della lista

			Iteratore costande della lista
		*/
		class const_iterator {

		  const item *i;
			const item *i_end;

		  public:
		    typedef std::forward_iterator_tag iterator_category;
		    typedef T                         value_type;
		    typedef ptrdiff_t                 difference_type;
		    typedef const T*                  pointer;
		    typedef const T&                  reference;


		    const_iterator() : i(0), i_end(0) {}

		    const_iterator(const const_iterator &other) : i(other.i), i_end(other.i_end) {}

				const_iterator(const iterator &other) : i(other.i), i_end(other.i_end) {}

		    const_iterator& operator=(const const_iterator &other) {

		      i = other.i;
					i_end = other.i_end;
		      return *this;

		    }

				const_iterator& operator=(const iterator &other) {

		      i = other.i;
					i_end = other.i_end;
		      return *this;

		    }

		    ~const_iterator() {

		      i = 0;
					i_end = 0;

		    }

		    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
		    reference operator*() const {

					assert(i!=i_end);
					return i->value;

		    }

		    // Ritorna il puntatore al dato riferito dall'iteratore
		    pointer operator->() const {

					assert(i!=i_end);
				  return &(i->value);

		    }

		    // Operatore di iterazione post-incremento
		    const_iterator operator++(int) {

		      const_iterator tmp(*this);
					i = i->next;
		      return tmp;

		    }

		    // Operatore di iterazione pre-incremento
		    const_iterator& operator++() {

		      i = i->next;
		      return *this;

		    }

		    // Uguaglianza
		    bool operator==(const const_iterator &other) const {

		      return i == other.i;

		    }

		    // Diversita'
		    bool operator!=(const const_iterator &other) const {

		      return i != other.i;

		    }

		    friend class iterator;

		    // Uguaglianza
		    bool operator==(const iterator &other) const {

		      return i == other.i;

		    }

		    // Diversita'
		    bool operator!=(const iterator &other) const {

		      return i != other.i;

		    }

		  private:

				/**
					La classe container deve essere messa friend dell'iteratore per poter
					usare il costruttore di inizializzazione.
				*/
		    friend class cbuffer;

				/**
 				 Costruttore privato di inizializzazione usato dalla classe container
 				 tipicamente nei metodi begin e end
 			 */
		    const_iterator(const item *ii, const item *ii_end) : i(ii), i_end(ii_end) {}

		  }; // classe const_iterator

		/**
			@brief Iteratore const begin

			Ritorna l'iteratore costante all'inizio della sequenza dati

			@return iteratore costante all'inizio della sequenza
		*/
		const_iterator begin() const {
		  return const_iterator(_head, _end);
		}

		/**
			@brief Iteratore const end

			Ritorna l'iteratore costante alla fine della sequenza dati

			@return iteratore costante alla fine della sequenza
		*/
		const_iterator end() const {
		  return const_iterator(_end, _end);
		}

}; // Classe cbuffer

/**
	@brief Operatore di stream

	Ridefinizione dell'operatore di stream per la stampa del contenuto
	della lista

	@param os oggetto stream di output
	@param ol cbuffer da stampare

	@return reference allo stream di output
*/
template <typename T>
std::ostream &operator<<(std::ostream &os, const cbuffer<T> &ol) {

	typename cbuffer<T>::const_iterator i, ie;

	for(i = ol.begin(), ie = ol.end(); i!=ie; i++)
		os << *i << " ";

	return os;
}

/**
	@brief Funzione di valutazione

	Funzione templata che stampa a console il risultato di un predicato
	unario applicato ad ogni elemento di un cbuffer

	@param cb cbuffer da valutare
	@param P predicato
*/
template <typename T, typename Predicate>
void evaluate_if(const cbuffer<T> &cb,const Predicate &P) {

	int h = cb.get_size();
	for(int i=0; i<h; i++) {
		if(P(cb[i]))	{	std::cout << "[" << i << "]: true " << std::endl;		}
		else 					{	std::cout << "[" << i << "]: false " << std::endl;	}
	}

}

#endif
