#include <iostream>

class Noeud
{
	friend class ListeChainee;
	friend class Iterateur;
	Noeud () : _suivant(0) {}
	Noeud (int valeur) : _valeur(valeur), _suivant(0) {}
	Noeud (int valeur, Noeud* suivant) : _valeur(valeur), _suivant(suivant){}
	Noeud (Noeud* suivant) : _suivant(suivant) {}
	int _valeur;
	Noeud* _suivant;
};

class Iterateur
{
	friend class ListeChainee;
	Noeud* pNoeud;
	Iterateur(Noeud* _pNoeud) : pNoeud(_pNoeud) {}
public:
	void operator++(){ pNoeud = pNoeud->_suivant; }
	bool operator!=(Iterateur rval){ return !(pNoeud == rval.pNoeud); }
	bool operator==(Iterateur rval){ return (pNoeud == rval.pNoeud); }
	int operator*(){	return pNoeud->_valeur; }
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
				iter = iter->_suivant;
				delete tmp;
			}
		}
	}

	void push_back(int valeur)
	{
		if (debut)
		{
			Noeud *iter = debut;
			for (; iter->_suivant != 0; iter = iter->_suivant); //parcours de la liste
			iter->_suivant = new Noeud(valeur);
		}
		else
			debut = new Noeud(valeur);
	};

	void push_front(int valeur)
	{
		if (debut)
		{
			Noeud * tmp = new Noeud(valeur);
			tmp->_suivant = debut;
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
