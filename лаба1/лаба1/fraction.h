#pragma once

#include <fstream>
#include <type_traits>
#include <iosfwd>


using std::istream;
using std::ostream;

template <class T = int>
class fraction
{
	//отметаем неподходящие типы
	static_assert(std::is_same<T, int>::value || std::is_same<T, char>::value || std::is_same<T, unsigned char>::value, L"недопустимый тип");

private:
	T m_nmr;	//числитель
	T m_dnm;	//знаменатель
	void Nod();
	void NegCheck();

public:
	fraction();
	fraction(T);
	fraction(T nmr, T dnm);
	fraction(const fraction<T>& );
	~fraction() = default;

	fraction<T> operator+ (const fraction<T>& ) const;
	fraction<T> operator- (const fraction<T>& ) const;
	fraction<T> operator* (const fraction<T>& ) const;
	fraction<T> operator* (const T) const;
	fraction<T> operator/ (const fraction<T>& ) const;
	fraction<T> operator/ (const T) const;
	fraction<T>& operator= (const fraction<T>& );
	fraction<T>& operator= (const T);
	fraction<T>& operator+= (const fraction<T>& );
	fraction<T>& operator*= (const T);
	bool operator== (int) const;
	bool operator!= (int) const;

	template <class U>
	friend ostream& operator<< (ostream& , const fraction<U>& );

	template <class U>
	friend istream& operator>> (istream& , fraction<U>& );

	template <class U>
	friend fraction<U> operator/ (int, const fraction<U>& );

	template <class U>
	friend fraction<T> operator* (int, const fraction<U>& );


	void setnmr(T newnmr) { m_nmr = newnmr; }
	void setdnm(T newdnm) { m_dmn = newdnm; }
	T getnmr() { return m_nmr; }
	T getdnm() { return m_dnm; }
	
	// операторы преобразования типа
	/*
	operator int() 	{ 
		int num = m_nmr / m_dnm;
		return num;
	}
	operator float() {
		float num = (float)m_nmr / (float)m_dnm;
		return num;
	}
	*/
};


// реализация друзей
template<class T>
ostream& operator<< (ostream& out, const fraction<T>& fr)
{
	if (fr.m_nmr != 0)
	{
		if (fr.m_dnm == 1)
			out << (int)fr.m_nmr;
		else
			if ((int)fr.m_nmr == (int)fr.m_dnm)		// сокращение не прошло
				out << 1;
			else
				out << (int)fr.m_nmr << '/' << (int)fr.m_dnm;
	}
	else
		out << 0;

	return out;
}

template<class T>
istream& operator>> (istream& in, fraction<T>& fr)
{
	in >> fr.m_nmr >> fr.m_dnm;

	if (fr.m_dnm == 0)
	{
		throw std::invalid_argument("division by zero");
	}

	fr.Nod();
	fr.NegCheck();
	return in;
}

template<class T>
inline fraction<T> operator/ (int num , const fraction<T>& fr)
{
	if (fr.m_nmr == 0)
		throw std::invalid_argument("division by zero");

	fraction<T> newfr;
	newfr.m_nmr = fr.m_dnm * num;
	newfr.m_dnm = fr.m_nmr;

	newfr.Nod();
	newfr.NegCheck();

	return newfr;
}

template<class T>
inline fraction<T> operator*(int num, const fraction<T>& fr)
{
	fraction<T> newnum;
	newnum.m_nmr = num * fr.m_nmr;
	newnum.m_dnm = fr.m_dnm;

	newnum.Nod();
	newnum.NegCheck();

	return newnum;
}


// реализация функций-членов
template <class T>
fraction<T>::fraction()
{
	m_nmr = 1;
	m_dnm = 1;
}

template<class T>
inline fraction<T>::fraction(T num)
{
	m_nmr = num;
	m_dnm = 1;
}

template <class T>
fraction<T>::fraction(T nmr, T dnm)
{
	if (dnm == 0)
	{
		throw std::invalid_argument("division by zero");
	}

	m_nmr = nmr;
	m_dnm = dnm;
}

template<class T>
inline fraction<T>::fraction(const fraction<T> &pol)
{
	this->m_nmr = pol.m_nmr;
	this->m_dnm = pol.m_dnm;
}

template <class T>
fraction<T> fraction<T>::operator+ (const fraction<T>& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_dnm + fr2.m_nmr * this->m_dnm;
	newfr.m_dnm = this->m_dnm * fr2.m_dnm;

	newfr.Nod();
	newfr.NegCheck();

	return newfr;
}

template <class T>
fraction<T> fraction<T>::operator- (const fraction<T>& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_dnm - fr2.m_nmr * this->m_dnm;
	newfr.m_dnm = this->m_dnm * fr2.m_dnm;

	newfr.Nod();
	newfr.NegCheck();

	return newfr;
}

template <class T>
fraction<T> fraction<T>::operator* (const fraction<T>& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_nmr;
	newfr.m_dnm = this->m_dnm * fr2.m_dnm;

	newfr.Nod();
	newfr.NegCheck();

	return newfr;
}

template<class T>
inline fraction<T> fraction<T>::operator* (const T num) const
{
	fraction<T> newnum;
	newnum.m_nmr = num * this->m_nmr;
	newnum.m_dnm = this->m_dnm;

	newnum.Nod();
	newnum.NegCheck();

	return newnum;
}

template <class T>
fraction<T> fraction<T>::operator/ (const fraction<T>& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_dnm;
	newfr.m_dnm = this->m_dnm * fr2.m_nmr;

	if (newfr.m_dnm == 0)
		throw std::invalid_argument("division by zero");

	newfr.Nod();
	newfr.NegCheck();

	return newfr;
}

template<class T>
inline fraction<T> fraction<T>::operator/ (const T num) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr;
	newfr.m_dnm = this->m_dnm * num;

	if (newfr.m_dnm == 0)
		throw std::invalid_argument("division by zero");

	newfr.Nod();
	newfr.NegCheck();

	return newfr;
}

template <class T>
inline fraction<T>& fraction<T>::operator= (const fraction<T>& fr2)
{
	this->m_nmr = fr2.m_nmr;
	this->m_dnm = fr2.m_dnm;

	this->Nod();
	this->NegCheck();

	return *this;
}

template<class T>
inline fraction<T>& fraction<T>::operator=(const T num)
{
	this->m_nmr = num;
	this->m_dnm = 1;

	return *this;
}

template<class T>
inline fraction<T>& fraction<T>::operator+= (const fraction<T> &fr2)
{
	this->m_nmr = this->m_nmr * fr2.m_dnm + fr2.m_nmr * this->m_dnm;
	this->m_dnm = this->m_dnm * fr2.m_dnm;

	this->Nod();
	this->NegCheck();

	return *this;
}

template<class T>
inline fraction<T>& fraction<T>::operator*= (T num)
{
	this->m_nmr *= num;

	this->Nod();
	this->NegCheck();

	return *this;
}

template<class T>
inline bool fraction<T>::operator== (int num) const
{
	if ( (m_nmr % m_dnm == 0) && ((m_nmr / m_dnm) == num)  )
		return true;
	else
		return false;
}

template<class T>
inline bool fraction<T>::operator!= (int num) const
{
	if ((m_nmr % m_dnm == 0) && ((m_nmr / m_dnm) == num))
		return false;
	else
		return true;
}

template<class T>
inline void fraction<T>::Nod()
{
	int c = 0, a = (int)this->m_nmr, b = (int)this->m_dnm;

	while (b)
	{
		c = a % b;
		a = b;
		b = c;
	}

	a = abs(a);		//теперь a - общий множитель числителя и знаменателя

	this->m_nmr /= (T)a;
	this->m_dnm /= (T)a;

	return;
}

template<class T>
inline void fraction<T>::NegCheck()
{
	if ((this->m_nmr < int(-1)) && (this->m_dnm < int(-1)))
	{
		this->m_nmr *= (int)-1;
		this->m_dnm *= (int)-1;
	}
}
