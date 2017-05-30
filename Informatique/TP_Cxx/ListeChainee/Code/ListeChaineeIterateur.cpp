#include <iostream>

class Noeud
{
	int _valeur;
	Noeud* _suivant;
public:
	inline int getValeur() {return _valeur;}
	inline Noeud* getSuivant() {return _suivant;}
	inline void setSuivant(Noeud* suivant) {_suivant = suivant;}
	Noeud () : _suivant(0) {}
	Noeud (int valeur) : _valeur(valeur), _suivant(0) {}
	Noeud (int valeur, Noeud* suivant) : _valeur(valeur), _suivant(suivant){}
	Noeud (Noeud* suivant) : _suivant(suivant) {}
};

class Iterateur
{
	Noeud* pNoeud;
public:
	Iterateur(Noeud* _pNoeud) : pNoeud(_pNoeud) {}
	void operator++(){ pNoeud = pNoeud->getSuivant(); }
	bool operator!=(Iterateur rval){ return !(pNoeud == rval.pNoeud); }
	bool operator==(Iterateur rval){ return (pNoeud == rval.pNoeud); }
	int operator*(){	return pNoeud->getValeur(); }
};



class ListeChainee
{
	Noeud* debut;

public:
	ListeChainee () : debut(0) {}
	~ListeChainee()
	{
		if (debut)
		{
			Noeud *iter = debut;
			while (iter != 0)
			{
				Noeud* tmp = iter;
				iter = iter->getSuivant();
				delete tmp;
			}
		}
	}

	void push_back(int valeur)
	{
		if (debut)
		{
			Noeud *iter = debut;
			for (; iter->getSuivant() != 0; iter = iter->getSuivant()); //parcours de la liste
			iter->setSuivant(new Noeud(valeur));
		}
		else
			debut = new Noeud(valeur);
	};

	void push_front(int valeur)
	{
		if (debut)
		{
			Noeud * tmp = new Noeud(valeur);
			tmp->setSuivant(debut);
			debut = tmp;
		}
		else
			debut = new Noeud(valeur);
	}

	Iterateur begin(){ return Iterateur(debut); }
	Iterateur end(){ return Iterateur(0); }
};

int main(void)
{
	ListeChainee liste;
	liste.push_back(3);
	liste.push_back(4);
	liste.push_front(2);
	liste.push_front(1);




	for (Iterateur iter = liste.begin();
	     iter != liste.end(); ++iter)
	{
		std::cout << (*iter) << std::endl;
	}

	return 0;
}
