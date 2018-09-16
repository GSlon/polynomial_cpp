#pragma once

#include <fstream>

using std::istream;
using std::ostream;

template <class T>
class fraction
{
static_assert(std::is_same<T, int>::value || std::is_same<T, double>::value || std::is_same<T, float>::value, "недопустимый тип");	//отметаем неподходящие типы

private:
	T m_nmr;	//числитель
	T m_dnm;	//знаменатель

public:
	fraction();
	fraction(T nmr, T dnm);
	~fraction();

	fraction<T> operator+(const fraction& fr2) const;
	fraction<T> operator-(const fraction& fr2) const;
	fraction<T> operator*(const fraction& fr2) const;
	fraction<T> operator/(const fraction& fr2) const;

	friend ostream& operator<< (ostream& out, fraction<T> fr)
	{
		out << fr.m_nmr << '/' << fr.m_dnm << endl;
		return out;
	}

	friend istream& operator>> (istream& in, fraction<T>& fr)	//вывод и в виде обыкновенной дроби, и в десятичной
	{
		in >> fr.m_nmr >> fr.m_dnm;
		return in;
	}


	void setnmr(T newnmr) { m_nmr = newnmr; }
	void setdnm(T newdnm) { m_dmn = newdnm; }
	T getnmr() { return m_nmr; }
	T getdnm() { return m_dnm; }
	
};

/*
template <class T>
ostream& operator<< (ostream& out, fraction<T> fr)
{
	out << fr.m_nmr << '/' << fr.m_dnm << endl;
	return out;
}

template <class T>
istream& operator<< <T> (istream& in, fraction<T>& fr)
{
	in >> fr.m_nmr >> fr.m_dnm;
	return in;
}
*/

#include "fraction.cpp"
// напишите #include "fraction.cpp" - это как реализация в инлайн файлах
