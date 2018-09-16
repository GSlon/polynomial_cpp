#include "stdafx.h"

#ifndef FRAC_H
#define FRAC_H
#include "fraction.h"

//алгоритм Евклида (для избавления от сокращаемых дробей)
/*template <typename N>
N Nod(N a, N b) 
{
	while (a != b) 
	{
		if (a > b) 
		{
			a = a - b;
		}
		else 
		{
			b = b - a;
		}
	}
	return a;
}
*/

template <class T>
fraction<T>::fraction()
{
	m_nmr = 0;
	m_dnm = 1;
}

template <class T>
fraction<T>::fraction(T nmr, T dnm)
{
	m_nmr = nmr;
	m_dnm = dnm;
}

template <class T>
fraction<T>::~fraction()
{
}

template <class T>
fraction<T> fraction<T>::operator+(const fraction& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_dnm + fr2.m_nmr * this->m_dnm;
	newfr.m_dnm = this->m_dnm * fr2.m_dnm;
		
	//сделать сокращение int дробей
	return newfr;
}

/*
template <class T>
ostream& operator<< (ostream& out, fraction<T> fr)
{
	out << fr.m_nmr << '/' << fr.m_dnm << endl;
	return out;
}

template <class T>
istream& operator<< (istream& in, fraction<T> fr)
{
	in >> fr.m_nmr >> fr.m_dnm;
	return in;
}
*/


#endif // !FRAC_H