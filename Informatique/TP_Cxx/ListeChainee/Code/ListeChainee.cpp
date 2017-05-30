#include <iostream>

template <typename T>
class ListeChainee;

template <class TNoeud>
class Iterateur
{
	TNoeud* pNoeud;
public:
	Iterateur(TNoeud* _pNoeud) : pNoeud(_pNoeud) {}
	void operator++(){ pNoeud = pNoeud->getSuivant(); }
	void operator++(int){ pNoeud = pNoeud->getSuivant(); }
	bool operator!=(Iterateur<TNoeud> rval){ return !(pNoeud == rval.pNoeud); }
	bool operator==(Iterateur<TNoeud> rval){ return (pNoeud == rval.pNoeud); }
	typename TNoeud::valeur_type operator*(){ return pNoeud->getValeur(); }
}	;

template <typename T>
class Noeud
{
	T _valeur;
	Noeud<T>* _suivant;
public:
	Noeud () : _suivant(0) {}
	Noeud (T valeur) : _valeur(valeur), _suivant(0) {}
	Noeud (T valeur, Noeud<T>* suivant) : _valeur(valeur), _suivant(suivant){}
	Noeud (Noeud<T>* suivant) : _suivant(suivant) {}
	typedef T valeur_type;
	inline T getValeur() {return _valeur;}
	inline Noeud<T>* getSuivant() {return _suivant;}
	inline void setSuivant(Noeud<T>* suivant) {_suivant = suivant;}
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
				iter = iter->getSuivant();
				delete tmp;
			}
		}
	}

	void push_back(T valeur)
	{
		if (debut)
		{
			Noeud<T> *iter = debut;
			for (; iter->getSuivant() != 0; iter = iter->getSuivant()){};
			iter->setSuivant(new Noeud<T>(valeur));
		}
		else
			debut = new Noeud<T>(valeur);
	};

	void push_front(T valeur)
	{
		if (debut)
		{
			Noeud<T> * tmp = new Noeud<T>(valeur);
			tmp->setSuivant(debut);
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
