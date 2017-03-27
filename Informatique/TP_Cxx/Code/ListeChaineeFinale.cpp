// example inspired by http://www.cplusplus.com/articles/Lw6AC542/
#include <iostream>
#include <iterator>

template <typename T>
class ListeChainee;

template <class TNoeud>
class Iterator
{
	/* Helper class to provide pointer like facilities around a Noeud */
	friend class ListeChainee<typename TNoeud::valeur_type>;
	TNoeud* pNoeud; //The Noeud oriented with this instance of iterator.

	Iterator(TNoeud* _pNoeud) : pNoeud(_pNoeud) {}
public:
	void operator++(){ pNoeud = pNoeud->_suivant; }
	void operator++(int){ pNoeud = pNoeud->_suivant; }
	bool operator!=(Iterator<TNoeud> rval){ return !(pNoeud == rval.pNoeud); }
	bool operator==(Iterator<TNoeud> rval){ return (pNoeud == rval.pNoeud); }
	typename TNoeud::valeur_type operator*(){	return pNoeud->_valeur; }
	Iterator<TNoeud> operator+(int _i)
	{
		Iterator<TNoeud> iter = *this;
		for (int i = 0; i < _i; ++i)
		{
			if (iter.pNoeud) //If there's something to move onto...
				++iter;
			else
				break;
		}
		return iter; //Return regardless of whether its valid...
	}
};

template <typename T>
class Noeud
{
	friend class ListeChainee<T>;
	friend class Iterator<Noeud<T> >;
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
	Noeud<T>* first;

public:
	typedef Iterator<Noeud<T> > iterator;
	typedef T 		  valeur_type;

	ListeChainee () : first(0) {}
	~ListeChainee()
	{
		if (first)
		{
			Noeud<T> *iter = first;
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
		if (first)
		{
			Noeud<T> *iter = first;
			for (; iter->_suivant != 0; iter = iter->_suivant); //Iterate until we reach the end of our linked ListeChainee.
			iter->_suivant = new Noeud<T>(valeur);
		}
		else
			first = new Noeud<T>(valeur);
	};

	void push_front(T valeur)
	{
		if (first)
		{
			Noeud<T> * tmp = new Noeud<T>(valeur);
			tmp->_suivant = first;
			first = tmp;
		}
		else
			first = new Noeud<T>(valeur);
	}

	iterator begin(){ return iterator(first); }
	iterator end(){ return iterator(0); }

	bool erase(iterator& _iNoeud) //True for success, vice versa
	{
		/* This is rather inneffecient. Maybe a better way to do this? */
		/* Even then, it's *still* more effecient than a contiguous container */
		if (_iNoeud.pNoeud == first)
		{
			first = first->_suivant;
			delete _iNoeud.pNoeud;
			return true;
		}
		else
		{
			for (Noeud<T>* iter = first; iter->_suivant; iter = iter->_suivant)
			{
				if (iter->_suivant == _iNoeud.pNoeud) //Find our Noeud.
				{
					iter->_suivant = _iNoeud.pNoeud->_suivant;
					delete _iNoeud.pNoeud;
					return true;
				}
			}
		}

		return false;
	}
};

int main(void)
{
	ListeChainee<int> ListeChainee;
	ListeChainee.push_back(3);
	ListeChainee.push_back(4);
	ListeChainee.push_front(2);
	ListeChainee.push_front(1);

	/*Print all elements*/
	for (ListeChainee<int>::iterator iter = ListeChainee.begin();
	     iter != ListeChainee.end(); ++iter)
	{
		std::cout << (*iter) << std::endl;
	}

	return 0;
}
