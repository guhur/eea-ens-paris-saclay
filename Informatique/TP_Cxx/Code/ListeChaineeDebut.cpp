#include <iostream>


class Noeud
{
	friend class ListeChainee;
	Noeud () : _suivant(0) {}
	Noeud (int valeur) : _valeur(valeur), _suivant(0) {}
	Noeud (int valeur, Noeud* suivant) : _valeur(valeur),
			_suivant(suivant){}
	Noeud (Noeud* suivant) : _suivant(suivant) {}
	int _valeur;
	Noeud* _suivant;
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
			for (; iter->_suivant != 0;
				iter = iter->_suivant);
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

	int get_value(int n) {
		Noeud *iter = debut;
		for (int i = 0; i < n; i++)
			try {
				iter = iter->_suivant;
				if (!iter->_suivant)
					throw 1;
			}
			catch (int e) {
				std::cerr <<
					"Erreur: le noeud n'existe pas !"
					<< std::endl;
			}
		return iter->_valeur;
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
