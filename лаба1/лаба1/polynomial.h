#pragma once

#include <vector>
#include <iterator>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "fraction.h"
#include "matrix.h"
#include <type_traits>
#include <iosfwd>
#include <map>
#include <utility>
#include <cmath>

using std::vector;
using std::ostream;
using std::istream;
using std::iterator;
using std::swap;
using std::map;
using std::pair;
using std::make_pair;

template <class T>
class polynomial
{
	/*static_assert(std::is_same<T, int>::value || std::is_same<T, char>::value || std::is_same<T, unsigned char>::value || std::is_same<T, fraction<int>>::value
		|| std::is_same<T, fraction<char>>::value || std::is_same<T, fraction<unsigned char>>::value 
		|| std::is_same<T, matrix<int , int>>::value || std::is_same<T, matrix<int , char>>::value || std::is_same<T, matrix<int , unsigned char>>::value 
		|| std::is_same<T, matrix<int cdim, fraction<int>>>::value || std::is_same<T, matrix<int, fraction<char>>>::value || std::is_same<T, matrix<int, fraction<unsigned char>>>::value, L"недопустимый тип");
	*/
private:
	map<int, T> m_pol;
	void zeroChecked();

public:
	polynomial();
	polynomial(int);
	polynomial(int, T);
	polynomial(const polynomial<T>& );
	~polynomial() = default;

	polynomial<T> operator+ (const polynomial<T>& ) const;
	polynomial<T> operator- (const polynomial<T>& ) const;
	polynomial<T> operator* (const polynomial<T>& ) const;
	polynomial<T> operator* (const T) const;
	polynomial<T> operator/ (const polynomial<T>& ) const;
	polynomial<T> operator% (const polynomial<T>& ) const;
	polynomial<T>& operator= (const polynomial<T>& );


	void add(int , const T& );	// перезапись элемента
	void addP(int, const T& );	// добавление к текущему
	polynomial<T> differ() const;
	T solve(T x) const;
	polynomial<T> suppos(const polynomial<T>& ) const;

	template<int cdim, class U>
	friend matrix<cdim, U> pow(const matrix<cdim, U>& , int);
	
	template<class U>
	friend fraction<U> pow(const fraction<U>& , int);
	
	template<class U>
	friend polynomial<U> pow(const polynomial<U>&, int);
	

	template<class T>
	friend polynomial<T> operator* (const T& , const polynomial<T>& );

	template<class U> 
	friend ostream& operator<< (ostream& , const polynomial<U>& );
	
	template<int cdim, class U>
	friend ostream& operator<< (ostream&, const polynomial<matrix<cdim, U>>& );
	
	template<class U>
	friend istream& operator>> (istream&, polynomial<U>& );
};

//реализация
template<class T>
inline polynomial<T>::polynomial()
{
	m_pol.insert(pair<int, T> (0, T(0)));
}

template<class T>
inline polynomial<T>::polynomial(int degree)
{
	m_pol.insert(pair<int, T>(degree, T(0)));
}

template<class T>
inline polynomial<T>::polynomial(int degree, T koef)
{
	m_pol.insert(pair<int, T>(degree, koef));
}

template<class T>
inline polynomial<T>::polynomial(const polynomial<T>& pol)
{
	this->m_pol = pol.m_pol;
}

template<class T>
inline polynomial<T> polynomial<T>::operator+ (const polynomial<T>& pol) const
{
	polynomial<T> newpol;
	newpol = *this;		//кладём первый полином

	for (auto iter = pol.m_pol.begin(); iter != pol.m_pol.end(); ++iter)
	{
		auto itfind = this->m_pol.find(iter->first);
		if (itfind != this->m_pol.end())		//ищем одинаковые степени
			newpol.m_pol[iter->first] = iter->second + itfind->second;
		else
			newpol.m_pol.insert(pair<int, T>(iter->first, iter->second));
	}

	newpol.zeroChecked();
	return newpol;
}

template<class T>
inline polynomial<T> polynomial<T>::operator- (const polynomial<T>& pol) const
{
	polynomial<T> newpol;
	newpol = *this;		//кладём первый полином

	for (auto iter = pol.m_pol.begin(); iter != pol.m_pol.end(); ++iter)
	{
		auto itfind = this->m_pol.find(iter->first);
		if (itfind != this->m_pol.end())		//ищем одинаковые степени
			newpol.m_pol[iter->first] = itfind->second - iter->second;
		else
			newpol.m_pol.insert(pair<int, T>(iter->first, iter->second * (-1) ));
	}

	newpol.zeroChecked();
	return newpol;
}

template<class T>
inline polynomial<T> polynomial<T>::operator* (const polynomial<T>& pol) const
{
	polynomial<T> newpol;

	for (auto iter = this->m_pol.begin(); iter != this->m_pol.end(); ++iter)
		for (auto iter2 = pol.m_pol.begin(); iter2 != pol.m_pol.end(); ++iter2)
		{
			newpol.addP(iter->first + iter2->first, iter->second * iter2->second);
		}

	newpol.zeroChecked();
	return newpol;
}

template<class T>
inline polynomial<T> polynomial<T>::operator* (const T num) const
{
	polynomial<T> newpol(*this);

	for (auto iter = newpol.m_pol.begin(); iter != newpol.m_pol.end(); ++iter)
		newpol.m_pol[iter->first] = iter->second * num;

	newpol.zeroChecked();
	return newpol;
}

template<class T>
inline polynomial<T> operator* (const T &num, const polynomial<T> &pol)
{
	polynomial<T> newpol(pol);

	for (auto iter = newpol.m_pol.begin(); iter != newpol.m_pol.end(); ++iter)
		newpol.m_pol[iter->first] = num * iter->second;

	newpol.zeroChecked();
	return newpol;
}


template<class T>
inline polynomial<T> polynomial<T>::operator/ (const polynomial<T>& pol) const
{
	if ( (pol.m_pol.size() == 1) && (pol.m_pol.find(0)->second == 0) )	//проверка, что второй полином не равен 0 (состоит из 0*x^0)
		throw std::invalid_argument("division by zero");

	polynomial<T> temp(*this), result;

	auto itPol = pol.m_pol.end();
	--itPol;	// последний элемент

	while (true)
	{
		auto itTemp = temp.m_pol.end();
		--itTemp;	// последний элемент

		if (itTemp->first < itPol->first)	// знаменатель более высокого порядка
			return result;	// возвращаем 0 (в случае, если true сразу)
		else
		{
			for (auto iter = pol.m_pol.begin(); iter != itPol; iter++)
			{
				T num;
				num = -1;
				
				temp.addP(iter->first + itTemp->first - itPol->first, num * iter->second * itTemp->second / itPol->second);
			}
			
			result.addP(itTemp->first - itPol->first, itTemp->second / itPol->second);
			temp.m_pol.erase(itTemp->first);
		}
	}
}

template<class T>
inline polynomial<T> polynomial<T>::operator% (const polynomial<T>& pol) const
{
	if ((pol.m_pol.size() == 1) && (pol.m_pol.find(0)->second == 0))	//проверка, что второй полином не равен 0 (состоит из 0*x^0)
		throw std::invalid_argument("division by zero");

	polynomial<T> result(*this);

	auto itPol = pol.m_pol.end();
	--itPol;

	while (true)
	{
		auto itRes = result.m_pol.end();
		--itRes;

		if (itRes->first < itPol->first)
			return result;	// возвращаем исходный полином (в случае, если true сразу)
		else
		{
			for (auto iter = pol.m_pol.begin(); iter != itPol; ++iter)
			{
				T num;
				num = -1;
				
				result.addP(iter->first + itRes->first - itPol->first, num * iter->second * itRes->second / itPol->second);
			}
			result.m_pol.erase(itRes->first);
		}
	}
}

template<class T>
inline polynomial<T>& polynomial<T>::operator= (const polynomial<T> &pol)
{
	this->m_pol = pol.m_pol;

	return *this;
}

// добавляем новый или переписываем имеющийся
template<class T>
inline void polynomial<T>::add(int key, const T &num)
{
	auto itfind = this->m_pol.find(key);
	if (itfind != this->m_pol.end())
		this->m_pol[key] = num;
	else
		this->m_pol.insert(pair<int, T>(key, num));
	
	return;
}

template<class T>
inline void polynomial<T>::addP(int key, const T &num)
{
	auto itfind = this->m_pol.find(key);
	if (itfind != this->m_pol.end())
		this->m_pol[key] += num;
	else
		this->m_pol.insert(pair<int, T>(key, num));

	return;
}

template<class T>
inline polynomial<T> polynomial<T>::differ() const
{
	polynomial<T> newpol;
	for (auto iter = this->m_pol.begin(); iter != this->m_pol.end(); ++iter)
	{
		if (iter->first == 0)	// степень = 0
			continue;

		newpol.addP(iter->first - 1, iter->second * iter->first);
	}

	return newpol;
}

template<class T>
inline T polynomial<T>::solve(T x) const
{
	T result;
	result = 0;

	for (auto iter = this->m_pol.begin(); iter != this->m_pol.end(); ++iter)
		result +=  iter->second * pow(x, iter->first);

	return result;
}

template<class T>
inline polynomial<T> polynomial<T>::suppos(const polynomial<T> &pol) const
{
	polynomial<T> newpol, temp;
	for (auto iter = this->m_pol.begin(); iter != this->m_pol.end(); ++iter)
	{
		temp = iter->second * pow(pol, iter->first);	// новый моном

		newpol = newpol + temp;		// добавляем новый моном
		temp.m_pol.clear();
	}

	return newpol;
}

template<class T>
inline void polynomial<T>::zeroChecked()
{
	vector<int> save;
	for (auto iter = this->m_pol.begin(); iter != this->m_pol.end(); ++iter)
	{
		if ((iter->second == 0) && (iter->first != 0))
			save.push_back(iter->first);
	}

	for (int i = 0; i < save.size(); i++)
		this->m_pol.erase(save[i]);

	return;
}

template<class T>
inline ostream& operator<< (ostream& out, const polynomial<T>& pol)
{
	if (pol.m_pol.size() == 0)
	{
		throw std::invalid_argument("empty polynom");
	}
	else
		if (pol.m_pol.size() == 1)	// лежит только нулевая степень
		{
			auto iter = pol.m_pol.begin();
			out << iter->second;
			return out;
		}

	polynomial<T> copy(pol);	// копируем для вывода
	if ( (copy.m_pol.find(0) != copy.m_pol.end()) && (copy.m_pol.find(0)->second == 0) )  //есть 0*x^0
			copy.m_pol.erase(0);

	int counter = copy.m_pol.size();
	for (auto iter = copy.m_pol.rbegin(); iter != copy.m_pol.rend(); ++iter)
	{
		if (iter->second != 0)
		{
			if (iter->first == 0)
				out << iter->second;
			else
				out << iter->second << "*x^" << iter->first;
		}
		else
		{
			--counter;
			continue;
		}

		if (counter > 1)
			out << " + ";
		
		--counter;
	}

	return out;
}

template<int cdim, class T>
inline ostream& operator<< (ostream& out, const polynomial<matrix<cdim, T>>& pol)
{
	if (pol.m_pol.size() == 0)
	{
		throw std::invalid_argument("empty polynom");
	}
	else
		if (pol.m_pol.size() == 1)	// лежит только нулевая степень
		{
			auto iter = pol.m_pol.begin();
			out << iter->second << endl;
			return out;
		}


	polynomial<matrix<cdim, T>> copy(pol);
	if ( (copy.m_pol.find(0) != copy.m_pol.end()) && (copy.m_pol.find(0)->second == 0) )
			copy.m_pol.erase(0);	// удаляем 0*x^0

	int counter = copy.m_pol.size();
	for (auto iter = copy.m_pol.rbegin(); iter != copy.m_pol.rend(); ++iter)
	{
		if (iter->second != 0)
		{
			if (iter->first == 0)
				out << iter->second;
			else
				out << iter->second << "*x^" << iter->first;
		}
		else
		{
			--counter;
			continue;
		}

		if (counter > 1)
			out << " + " << endl;

		counter--;
	}

	out << endl;
	return out;
}

template<class T>
inline istream& operator>> (istream& in, polynomial<T>& pol)
{
	pol.m_pol.clear();
	T koef;
	koef = 0;
	int degree = 0;
	pol.m_pol.insert(pair<int, T> (degree, koef));	//всегда присутствует нулевая степень

	while (true)
	{
		in >> koef >> degree;

		if (in.fail())	// "плохой символ"
			break;

		if ((koef != 0) || (degree == 0))
			pol.add(degree, koef);

		if (degree == 0)	// нулевая степень - остановка ввода
			break;
	}

	return in;
}

template<class T, class U>
inline fraction<U> pow(const fraction<U> &arg , int degree)
{
	if (degree == 0)
		return fraction<U>(1);
	else
		if (degree < 0)
		{
			fraction<U> newarg(arg);
			newarg = 1 / newarg;
			for (int i = degree; i > 1; i--)
				newarg *= newarg;

			return newarg;
		}
		else
			if (degree > 0)
			{
				fraction<U> newarg(arg);

				for (int i = 1; i < degree; i++)
					newarg = newarg * arg;
	
				return newarg;
			}
}

template<int cdim, class U>
inline matrix<cdim, U> pow(const matrix<cdim, U> &arg, int degree)
{
	if (degree == 0)
	{
		matrix<cdim, U> newmatr(0);
		newmatr.toE();
		return newmatr;
	}
	else
		if (degree < 0)
		{
			matrix<cdim, U> newarg;
			newarg = arg.rev();
			for (int i = degree; i > 1; i--)
				newarg = newarg * newarg;

			return newarg;
		}
		else
			if (degree > 0)
			{
				matrix<cdim, U> newarg(arg);

				for (int i = 1; i < degree; i++)
					newarg = newarg * arg;

				return newarg;
			}
}

template<class T>
inline polynomial<T> pow(const polynomial<T> &arg, int degree)
{
	if (degree == 0)
		return polynomial<T>(0, pow(T(1), 0));
	else
		if (degree < 0)
		{
			throw std::invalid_argument("polynom to negative degree ");
			return polynomial<T>();
		}
		else
			if (degree > 0)
			{
				polynomial<T> newarg(arg);

				for (int i = 1; i < degree; i++)
					newarg = newarg * arg;

				return newarg;
			}
}
