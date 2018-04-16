#include "cbuffer.h"
#include <iostream>
#include <cassert>
#include <cctype>


/**
  @brief Funtore intero pari.

  Funtore che controlla se un intero è pari

  @return true se intero pari, false altrimenti
*/
struct isEven {

  bool operator()(int a) const{

    if(a%2==0)
      return true;
    return false;

  }

};

/**
  @brief Funtore intero pari.

  Funtore che controlla se un intero è dispari

  @return true se intero dispari, false altrimenti
*/
struct isOdd {

  bool operator()(int a) const{

    if(a%2!=0)
      return true;
    return false;

  }

};

/**
  @brief Funtore è un numero naturale.

  Funtore che controlla se una stringa è un numero naturale

  @return true se numero naturale, false altrimenti
*/
struct n_natural {

  bool operator()(const std::string &str) const{

    const char *str_array = str.c_str();
    for(int i = 0; i<str.length(); i++) {
      if(!isdigit(str[i]))
        return false;
    }
    str_array = 0;
    return true;

  }

};

/**
  @brief Struct nominativo.

  Struct nominativo che implementa un nominativo (nome e cognome).
*/

struct nominativo {

  std::string Nome;
  std::string Cognome;

  nominativo() : Nome(""), Cognome("") {}
  nominativo(const std::string &n, const std::string &c) : Nome(n), Cognome(c) {}

};

/**
  @brief Funtore validità nominativo.

  Funtore per il controllo della validità di un nominativo. Un nominativo è valido
  se contine sia il nome che il cognome

  @return true se nominativo valido, false altrimenti
*/
struct valid_nominativo {

  bool operator()(const nominativo &n) const {

    return(!(n.Nome=="" || n.Cognome==""));

  }

};

/**
  @brief Metodo test su interi.
*/
void test_cbuffer_int() {

  std::cout<<"******** Test cbuffer di interi ********"<<std::endl;

  cbuffer<int> cb(5);
  cb.insert(1);
  cb.insert(20);
  cb.insert(99);
  cb.insert(34);
  cb.insert(71);
  cb.insert(12);
  assert(cb[0] == 20);
  cb.insert(55);
  assert(cb[0] == 99);

  std::cout<<std::endl<<"Contenuto cbuffer<int> cb:"<<std::endl;
  std::cout << cb << std::endl;

  cbuffer<int> ck(cb);

  std::cout<<std::endl<<"Contenuto cbuffer<int> ck(cb):"<<std::endl;
  std::cout << ck << std::endl;

  cb.delete_head();
  assert(cb[0] == 34);
  cb[0] = 33;
  cb[3] = 11;

  std::cout<<std::endl<<"Contenuto cbuffer<int> cb dopo modifiche:"<<std::endl;
  std::cout << cb << std::endl;

  cbuffer<int> ch;
  ch = cb;

  std::cout<<std::endl<<"Contenuto cbuffer<int> ch = cb:"<<std::endl;
  std::cout << ch << std::endl;

  isEven pari;
  std::cout<<std::endl<<"Test evaluate_if(cb, isEven)"<<std::endl;
  evaluate_if(cb, pari);

  isOdd dispari;
  std::cout<<std::endl<<"Test evaluate_if(ck, isOdd)"<<std::endl;
  evaluate_if(ck, dispari);

  std::cout<<std::endl<<"Test iterator (stampa ck)"<<std::endl;
	cbuffer<int>::iterator i, ie;
	for(i=ck.begin(), ie=ck.end(); i!=ie; ++i)
		std::cout<<*i<<std::endl;

  std::cout<<std::endl<<"Test const_iterator (stampa cb)"<<std::endl;
  cbuffer<int>::const_iterator ci, cie;
  for(ci=cb.begin(), cie=cb.end(); ci!=cie; ++ci)
  		std::cout<<*ci<<std::endl;

}

/**
  @brief Metodo test su stringhe.
*/
void test_cbuffer_string() {

  std::cout<<"******** Test cbuffer di stringhe ********"<<std::endl;

  cbuffer<std::string> cb(4);
  cb.insert("Topolino");
  cb.insert("Pippo");
  cb.insert("Pluto");
  cb.insert("Paperino");
  cb.insert("Gancio");
  cb.insert("Minnie");
  assert(cb[0] == "Pluto");
  cb.insert("Paperone");
  assert(cb[0] == "Paperino");

  std::cout<<std::endl<<"Contenuto cbuffer<std::string> cb:"<<std::endl;
  std::cout << cb << std::endl;

  cbuffer<std::string> ck(cb);

  std::cout<<std::endl<<"Contenuto cbuffer<std::string> ck(cb):"<<std::endl;
  std::cout << ck << std::endl;

  cb.delete_head();
  assert(cb[0] == "Gancio");
  cb[0] = "Qui";
  cb[2] = "Quo";

  std::cout<<std::endl<<"Contenuto cbuffer<std::string> cb dopo modifiche:"<<std::endl;
  std::cout << cb << std::endl;

  ck[0] = "453879";
  ck[2] = "34f342";

  std::cout<<std::endl<<"Test iterator (stampa ck)"<<std::endl;
	cbuffer<std::string>::iterator i, ie;
	for(i=ck.begin(), ie=ck.end(); i!=ie; ++i)
		std::cout<<*i<<std::endl;

  n_natural numero;
  std::cout<<std::endl<<"Test evaluate_if(ck, n_natural)"<<std::endl;
  evaluate_if(ck, numero);

  cbuffer<std::string>::const_iterator ci, cie;
	cbuffer<std::string> ch(2, ci=cb.begin(), cie=cb.end());

  std::cout<<std::endl<<"Contenuto cbuffer<std::string> ch(int, cb.begin(), cb.end()):"<<std::endl;
  std::cout << ch << std::endl;

}

/**
	Ridefinizione dell'operatore di stream << per un nominativo.
*/
std::ostream &operator<<(std::ostream &os, const nominativo &n) {
	std::cout<<"("<<n.Nome<< " " <<n.Cognome<<")";
	return os;
}

/**
  @brief Metodo test su nominativi.
*/
void test_cbuffer_nominativo() {

  std::cout<<"******** Test cbuffer di nominativi ********"<<std::endl;

  cbuffer<nominativo> cb(7);
  cb.insert(nominativo("Bruce","Dickinson"));
  cb.insert(nominativo("Dave","Murray"));
  cb.insert(nominativo("Dave","Mustaine"));
  cb.insert(nominativo("James","Hetfield"));
  cb.insert(nominativo("Kirk","Hammett"));
  cb.insert(nominativo("Paul","Gilbert"));

  std::cout<<std::endl<<"Contenuto cbuffer<nominativo> cb:"<<std::endl;
  std::cout << cb << std::endl;

  std::cout<<std::endl<<"Dimensione cbuffer<nominativo> cb: "<< cb.get_size() << "/" << cb.max_size() <<std::endl;


  cb.insert(nominativo("","Dickinson"));
  cb[2] = nominativo("James","");

  std::cout<<std::endl<<"Test const_iterator (stampa cb)"<<std::endl;
  cbuffer<nominativo>::const_iterator ci, cie;
  for(ci=cb.begin(), cie=cb.end(); ci!=cie; ++ci)
  		std::cout<<*ci<<std::endl;

  valid_nominativo isvalid;
  std::cout<<std::endl<<"Test evaluate_if(cbS, valid_nominativo)"<<std::endl;
  evaluate_if(cb, isvalid);

}

int main() {

  std::cout<<std::endl;
  test_cbuffer_int();

  std::cout<<std::endl;
  test_cbuffer_string();

  std::cout<<std::endl;
  test_cbuffer_nominativo();

  std::cout<<std::endl;
  return 0;

}
