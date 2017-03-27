// example inspired by http://www.cplusplus.com/articles/Lw6AC542/
#include <iostream>

template <typename T>
class ListeChainee;

template <class TNoeud>
class Iterateur
{
	friend class ListeChainee<typename TNoeud::valeur_type>;
	TNoeud* pNoeud;
	Iterateur(TNoeud* _pNoeud) : pNoeud(_pNoeud) {}
public:
	void operator++(){ pNoeud = pNoeud->_suivant; }
	void operator++(int){ pNoeud = pNoeud->_suivant; }
	bool operator!=(Iterateur<TNoeud> rval){ return !(pNoeud == rval.pNoeud); }
	bool operator==(Iterateur<TNoeud> rval){ return (pNoeud == rval.pNoeud); }
	typename TNoeud::valeur_type operator*(){	return pNoeud->_valeur; }
};

template <typename T>
class Noeud
{
	friend class ListeChainee<T>;
	friend class Iterateur<Noeud<T> >;
	Noeud () : _suivant(0) {}
	Noeud (T valeur) : _valeur(valeur), _suivant(0) {}
	Noeud (T valeur, Noeud<T>* suivant) : _valeur(valeur), _suivant(suivant){}
	Noeud (Noeud<T>* suivant) : _suivant(suivant) {}
	T _valeur;
	Noeud<T>* _suivant;
public:
	typedef T valeur_type;
};

template <typename T>
class ListeChainee
{
	Noeud<T>* debut;

public:
	typedef Iterateur<Noeud<T> > iterateur;
	typedef T 		  valeur_type;

	ListeChainee () : debut(0) {}
	~ListeChainee()
	{
		if (debut)
		{
			Noeud<T> *iter = debut;
			while (iter != 0)
			{
				Noeud<T>* tmp = iter;
				iter = iter->_suivant;
				delete tmp;
			}
		}
	}

	void push_back(T valeur)
	{
		if (debut)
		{
			Noeud<T> *iter = debut;
			for (; iter->_suivant != 0; iter = iter->_suivant); //parcours de la liste
			iter->_suivant = new Noeud<T>(valeur);
		}
		else
			debut = new Noeud<T>(valeur);
	};

	void push_front(T valeur)
	{
		if (debut)
		{
			Noeud<T> * tmp = new Noeud<T>(valeur);
			tmp->_suivant = debut;
			debut = tmp;
		}
		else
			debut = new Noeud<T>(valeur);
	}

	iterateur begin(){ return iterateur(debut); }
	iterateur end(){ return iterateur(0); }
};

int main(void)
{
	ListeChainee<int> liste;
	liste.push_back(3);
	liste.push_back(4);
	liste.push_front(2);
	liste.push_front(1);

	for (ListeChainee<int>::iterateur iter = liste.begin();
	     iter != liste.end(); ++iter)
	{
		std::cout << (*iter) << std::endl;
	}

	return 0;
}
