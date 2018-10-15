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
		|| std::is_same<T, matrix<int cdim, fraction<int>>>::value || std::is_same<T, matrix<int, fraction<char>>>::value || std::is_same<T, matrix<int, fraction<unsigned char>>>::value, "недопустимый тип");
	*/
private:
	map<int, T> m_pol;
	void zeroChecked();

public:
	polynomial();
	polynomial(int, T);
	polynomial(const polynomial<T>& );
	~polynomial() = default;

	polynomial<T> operator+ (const polynomial<T>& ) const;
	polynomial<T> operator- (const polynomial<T>& ) const;
	polynomial<T> operator* (const polynomial<T>& ) const;
	polynomial<T> operator* (int) const;
	polynomial<T> operator/ (const polynomial<T>& ) const;
	polynomial<T> operator% (const polynomial<T>& ) const;
	polynomial<T>& operator= (const polynomial<T>& );

	// void add(int , T );
	polynomial<T> differ() const;

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
	T koef;
	koef = 0;
	m_pol.insert(pair<int, T> (0, koef));
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
inline polynomial<T> polynomial<T>::operator+(const polynomial<T>& pol) const
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
			auto iterProv = newpol.m_pol.find(iter->first + iter2->first);
			if (iterProv != newpol.m_pol.end())	  //такой элемент есть - надо перезаписать (не забыв про уже лежащее значение)
				newpol.m_pol[iterProv->first] += iter->second * iter2->second;
			else
				newpol.m_pol.insert(pair<int, T>(iter->first + iter2->first, iter->second * iter2->second));
		}

	newpol.zeroChecked();
	return newpol;
}

template<class T>
inline polynomial<T> polynomial<T>::operator* (int num) const
{
	polynomial<T> newpol(*this);

	for (auto iter = newpol.m_pol.begin(); iter != newpol.m_pol.end(); ++iter)
		newpol.m_pol[iter->first] = iter->second * num;

	return newpol;
}

/*
template<class T>
inline polynomial<T> polynomial<T>::operator/(const polynomial<T>& pol) const
{
	polynomial<T> newpol;

	for (auto iter = this->m_pol.begin(); iter != this->m_pol.end(); ++iter)
		for (auto iter2 = pol.m_pol.begin(); iter2 != pol.m_pol.end(); ++iter2)
		{
			auto iterProv = newpol.m_pol.find(iter->first - iter2->first);
			if (iterProv != newpol.m_pol.end())	  //такой элемент есть - надо перезаписать (в противном случае - добавить)
			{
				try
				{
					newpol.m_pol[iterProv->first] += iter->second / iter2->second;
				}
				catch (...)
				{
					throw std::invalid_argument("division by zero");
				}
			}
			else
				newpol.m_pol.insert(pair<int, T>(iter->first - iter2->first, iter->second / iter2->second));
		}

	newpol.zeroChecked();
	return newpol;
}
*/


template<class T>
inline polynomial<T> polynomial<T>::operator/(const polynomial<T>& pol) const
{
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
				
				auto itfind = temp.m_pol.find(iter->first + itTemp->first - itPol->first);
				if (itfind != temp.m_pol.end())		// есть такой элемент
					temp.m_pol[iter->first + itTemp->first - itPol->first] = num * iter->second * itTemp->second / itPol->second;
				else
					temp.m_pol.insert(pair<int, T> (iter->first + itTemp->first - itPol->first, num * iter->second * itTemp->second / itPol->second));

			}
			
			auto itfind = result.m_pol.find(itTemp->first - itPol->first);
			if (itfind != result.m_pol.end())
				result.m_pol[itTemp->first - itPol->first] = itTemp->second / itPol->second;
			else
				result.m_pol.insert(pair<int, T> (itTemp->first - itPol->first, itTemp->second / itPol->second));

			temp.m_pol.erase(itTemp->first);
		}
	}
}

template<class T>
inline polynomial<T> polynomial<T>::operator%(const polynomial<T>& pol) const
{
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
			for (auto iter = pol.m_pol.begin(); iter != itPol; iter++)
			{
				T num;
				num = -1;
				
				auto itfind = result.m_pol.find(iter->first + itRes->first - itPol->first);
				if (itfind != result.m_pol.end())
					result.m_pol[iter->first + itRes->first - itPol->first] = num * iter->second * itRes->second / itPol->second;
				else
					result.m_pol.insert(pair<int, T> (iter->first + itRes->first - itPol->first, num * iter->second * itRes->second / itPol->second));
			}
			result.m_pol.erase(itRes->first);
		}
	}
}


template<class T>
inline polynomial<T>& polynomial<T>::operator=(const polynomial<T> &pol)
{
	this->m_pol = pol.m_pol;

	return *this;
}

// добавляем новый или дописываем в имеющийся
/* template<class T>
inline void polynomial<T>::add(int, T)
{

}
*/

template<class T>
inline polynomial<T> polynomial<T>::differ() const
{
	polynomial<T> newpol;
	for (auto iter = this->m_pol.begin(); iter != this->m_pol.end(); ++iter)
	{
		if (iter->first == 0)	// степень = 0
			continue;

		newpol.m_pol.insert(pair<int, T> (iter->first - 1, iter->second * iter->first));
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
			out << iter->second << "*x^" << iter->first;
			return out;
		}

	polynomial<T> copy(pol);
	auto itfind = copy.m_pol.find(0);
	if (itfind != copy.m_pol.end())  //есть такой элемент
		if (itfind->second == 0)
			copy.m_pol.erase(0);

	int counter = copy.m_pol.size();
	for (auto iter = copy.m_pol.rbegin(); iter != copy.m_pol.rend(); ++iter)
	{
		if (iter->second != 0)
			out << iter->second << "*x^" << iter->first;

		if (counter > 1)
		{
			out << " + ";
		}
		
		counter--;
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
			out << iter->second << "*x^" << iter->first << endl;
			return out;
		}

	int counter = pol.m_pol.size();
	for (auto iter = pol.m_pol.rbegin(); iter != pol.m_pol.rend(); ++iter)
	{
		if ((iter->first == 0) && (iter->second == 0))	// пропускаем 0*x^0
		{
			counter--;
			continue;
		}

		if (iter->second != 0)
			out << iter->second << "*x^" << iter->first;

		if (counter > 1)
		{
			out << " + " << endl;
		}

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
	pol.m_pol.insert(pair<int, T> (degree, koef));

	while (true)
	{
		in >> koef >> degree;

		if (in.fail())	// "плохой символ"
			break;

		if ((koef != 0) || (degree == 0))
		{
			auto itfind = pol.m_pol.find(degree);
			
			if (itfind != pol.m_pol.end())
				pol.m_pol[degree] = koef;
			else
				pol.m_pol.insert(pair<int, T>(degree, koef));
		}

		if (degree == 0)	// нулевая степень - остановка ввода
			break;
	}

	return in;
}

