#include <iostream>
#include <stdexcept>

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
	Noeud (int valeur, Noeud* suivant) : _valeur(valeur),
			_suivant(suivant){}
	Noeud (Noeud* suivant) : _suivant(suivant) {}
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
			for (; iter->getSuivant() != 0;
				iter = iter->getSuivant());
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

	int get_value(int n) {
		Noeud *iter = debut;
		for (int i = 0; i < n; i++) {
				iter = iter->getSuivant();
				if (!iter->getSuivant())
					throw std::runtime_error("Erreur: le noeud n'existe pas !");
		}
		return iter->getValeur();
	}
};




int main(void)
{
	ListeChainee liste;
	liste.push_back(3);
	liste.push_back(4);
	liste.push_front(2);
	liste.push_front(1);

	std::cout << liste.get_value(1) << std::endl;

	return 0;
}
