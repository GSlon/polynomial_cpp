#pragma once	//вместо ifdef MATRIX.H

#include <vector>
#include <iterator>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "fraction.h"
#include <type_traits>
#include <iosfwd>


using std::vector;
using std::ostream;
using std::istream;
using std::iterator;
using std::swap;

template <int cdim = 1, class T = int>
class matrix
{
	//отметаем неподходящие типы
	static_assert(std::is_same<T, int>::value || std::is_same<T,char>::value || std::is_same<T, unsigned char>::value || std::is_same<T, fraction<int>>::value
		|| std::is_same<T, fraction<char>>::value || std::is_same<T, fraction<unsigned char>>::value, L"недопустимый тип");

private:
	vector<vector<T>> m_mtr;
	void minor(const vector<vector<T>>& , vector<vector<T>>& , int, int, int) const;
	T determinant(vector<vector<T>> , int) const;
	void transp(matrix<cdim, T>& ) const;
	void transp(vector<vector<float>>& ) const;

public:
	matrix();
	matrix(T);
	matrix(const matrix<cdim, T> &mtr);
	~matrix() = default;

	matrix<cdim, T> operator+ (const matrix<cdim, T>& ) const;
	matrix<cdim, T> operator- (const matrix<cdim, T>& ) const;
	matrix<cdim, T> operator* (const matrix<cdim, T>& ) const;
	matrix<cdim, T> operator= (const matrix<cdim, T>& );
	matrix<cdim, T> operator* (const T& ) const;
	matrix<cdim, T> operator= (const T& );
	matrix<cdim, T>& operator+= (const matrix<cdim, T>& );
	matrix<cdim, T> operator/ (const matrix<cdim, T>& ) const;
	bool operator== (int) const;
	bool operator!= (int) const;
	
	T det() ;		//определитель
	matrix<cdim, T> rev() const;	//обратная (возвращает matrix<cdim, T>)
	vector<vector<float>> revf() const;	//обратная (возврщает vector)
	void toE();


	template<int cdim, class U>
	friend ostream& operator<< (ostream& out, const matrix<cdim, U> &mtr);
	
	template<int cdim, class U>
	friend ostream& operator<< (ostream& out, const matrix<cdim, fraction<U>> &mtr);
	
	template<int cdim, class U>
	friend istream& operator>> (istream& in, matrix<cdim, U> &mtr);

	int getdim() { return cdim; };
	vector<vector<T>> getmtr() { return m_mtr; };
};


//реализация друзей
template <int cdim, class T>
ostream& operator<< (ostream& out, const matrix<cdim, T> &mtr)
{
	for (int i = 0; i < cdim; i++)
	{
		for (int j = 0; j < cdim; j++)
			out << (int)mtr.m_mtr[i][j] << ' ';		// для вывода char приведение

		out << '\n';
	}

	return out;
}

//для вывода дроби
template <int cdim, class T>
ostream& operator<< (ostream& out, const matrix<cdim, fraction<T>> &mtr)
{
	for (int i = 0; i < cdim; i++)
	{
		for (int j = 0; j < cdim; j++)
			out << mtr.m_mtr[i][j] << ' ';

		out << '\n';
	}

	return out;
}

template <int cdim, class T>
istream& operator>> (istream& in, matrix<cdim, T> &mtr)
{
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
		{
			in >> mtr.m_mtr[i][j];
		}

	return in;
}
// // //

// конструктор по умолчанию
template <int cdim, class T>
matrix<cdim, T>::matrix()
{
	//cdim = dim;
	m_mtr.resize(cdim);
	for (int i = 0; i < cdim; i++)
		m_mtr[i].resize(cdim);

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			m_mtr[i][j] = 1;
}

template<int cdim, class T>
inline matrix<cdim, T>::matrix(T num)
{
	m_mtr.resize(cdim);
	for (int i = 0; i < cdim; i++)
		m_mtr[i].resize(cdim);

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			m_mtr[i][j] = num;
}

template<int cdim, class T>
matrix<cdim, T>::matrix(const matrix<cdim, T>& mtr)
{
	//cdim = mtr.cdim;
	/*m_mtr.resize(cdim);
	for (int i = 0; i < cdim; i++)
		m_mtr[i].resize(cdim);
	*/

	this->m_mtr = mtr.m_mtr;
}

template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::operator+ (const matrix<cdim, T> &mtr2) const
{
	matrix<cdim, T> newmtr;

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			newmtr.m_mtr[i][j] = this->m_mtr[i][j] + mtr2.m_mtr[i][j];

	return newmtr;
}

template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::operator- (const matrix<cdim, T> &mtr2) const
{
	matrix<cdim, T> newmtr;

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			newmtr.m_mtr[i][j] = this->m_mtr[i][j] - mtr2.m_mtr[i][j];

	return newmtr;
}

template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::operator* (const matrix<cdim, T> &mtr2) const
{
	matrix<cdim, T> newmtr;
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			newmtr.m_mtr[i][j] = 0;

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			for (int k = 0; k < cdim; k++)
				newmtr.m_mtr[i][j] += this->m_mtr[i][k] * mtr2.m_mtr[k][j];

	return newmtr;
}

template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::operator/ (const matrix<cdim, T>& mtr2) const
{
	matrix<cdim, T> newmtr;
	newmtr = mtr2.rev();
	newmtr = *this * newmtr;	// матрица 1 на обратную матрицу 2

	return newmtr;
}

template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::operator= (const matrix<cdim, T> &mtr2)
{
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			this->m_mtr[i][j] = mtr2.m_mtr[i][j];

	return *this;
}

template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::operator* (const T &num) const
{
	matrix<cdim, T> newmatrix;
	newmatrix = *this;

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			newmatrix.m_mtr[i][j] = newmatrix.m_mtr[i][j] * num;

	return newmatrix;
}

template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::operator= (const T &num)
{
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			this->m_mtr[i][j] = num;

	return *this;
}

template<int cdim, class T>
inline matrix<cdim, T>& matrix<cdim, T>::operator+=(const matrix<cdim, T>& mtr2)
{
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			this->m_mtr[i][j] += mtr2.m_mtr[i][j];

	return *this;
}

template<int cdim, class T>
inline bool matrix<cdim, T>::operator==(int num) const
{
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
		{
			if (this->m_mtr[i][j] != num)
				return false;
		}

	return true;
}

template<int cdim, class T>
inline bool matrix<cdim, T>::operator!=(int num) const
{
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
		{
			if (this->m_mtr[i][j] != num)
				return true;
		}

	return false;
}

// определитель
template<int cdim, class T>
inline T matrix<cdim, T>::det() 
{
	return determinant(m_mtr, cdim);
}

// обратная матрица
template<int cdim, class T>
inline matrix<cdim, T> matrix<cdim, T>::rev() const
{
	if (cdim == 0)
		throw std::invalid_argument("error: dimension = 0");
	else 
		if (cdim == 1)
			return matrix<cdim, T>(); //обратный на int всё равно будет 1


	int a = -1;
	T det;

	det = determinant(this->m_mtr, cdim);

	if (det == 0)
		throw std::invalid_argument("determinator = 0");

	vector<vector<T>> newmatrix(cdim-1);
	for (int i = 0; i < cdim - 1; i++)
		newmatrix[i].resize(cdim-1);

	matrix<cdim, T> revmatrix;

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++) 
		{
			this->minor(this->m_mtr, newmatrix, cdim, j, i);

			if ((i + j) % 2 == 0)
				a = 1;
			else 
				a = -1;
			
			revmatrix.m_mtr[i][j] = determinant(newmatrix, cdim - 1) * a;
		}

	this->transp(revmatrix);

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			revmatrix.m_mtr[i][j] = revmatrix.m_mtr[i][j] / det;

	
	return revmatrix;
}

// улучшенная версия обратной матрицы (выводит float) (проигрывает дробям, помогает в int)
template<int cdim, class T>
inline vector<vector<float>> matrix<cdim, T>::revf() const
{
	if (cdim == 0)
		throw std::invalid_argument("error: dimension = 0");
	else
		if (cdim == 1)
		{
			vector<vector<float>> revmatrix(cdim);
			revmatrix[0].resize(cdim);
			revmatrix[0][0] = (float)1 / (float)this->m_mtr[0][0];
			return revmatrix; 
		}


	int a = -1;
	T det;

	det = determinant(this->m_mtr, cdim);

	if (det == 0)
		throw std::invalid_argument("determinator = 0");

	vector<vector<T>> newmatrix(cdim - 1);
	for (int i = 0; i < cdim - 1; i++)
		newmatrix[i].resize(cdim - 1);

	vector<vector<float>> revmatrix(cdim);
	for (int i = 0; i < cdim; i++)
		revmatrix[i].resize(cdim);

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
		{
			minor(this->m_mtr, newmatrix, cdim, j, i);

			if ((i + j) % 2 == 0)
				a = 1;
			else
				a = -1;

			revmatrix[i][j] = determinant(newmatrix, cdim - 1) * a;
		}

	transp(revmatrix);

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			revmatrix[i][j] = revmatrix[i][j] / (float)det;


	return revmatrix;
}

template<int cdim, class T>
inline void matrix<cdim, T>::toE()
{
	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
		{
			if (i == j)
				this->m_mtr[i][j] = 1;
			else
				this->m_mtr[i][j] = 0;
		}

}

// реализация функций-членов
template<int cdim, class T>
inline void matrix<cdim, T>::minor(const vector<vector<T>> &matrix, vector<vector<T>> &newmatrix, int size, int column, int string) const
{
	int col;
	int str = 0;
	for (int i = 0; i < size - 1; i++)
	{

		if (i == string)
			str = 1;

		col = 0;
		for (int j = 0; j < size - 1; j++)
		{

			if (j == column)
				col = 1;

			newmatrix[i][j] = matrix[i + str][j + col];
		}

	}
}

template<int cdim, class T>
inline T matrix<cdim, T>::determinant(vector<vector<T>> matrix, int size) const
{
	int a = 1;
	T det;
	det = 0;

	if (size == 0)
		throw std::invalid_argument("error: dimension = 0");

	else if (size == 1)
		return matrix[0][0];

	else if (size == 2)
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

	else
	{
		vector<vector<T>> newmatrix(size - 1);
		for (int i = 0; i < size - 1; i++)
			newmatrix[i].resize(size - 1);

		for (int i = 0; i < size; i++)
		{
			minor(matrix, newmatrix, size, i, 0);
			det = det + matrix[0][i] * determinant(newmatrix, size - 1) * a;

			a = -a;
		}

		return det;
	}
}

template<int cdim, class T>
inline void matrix<cdim, T>::transp(matrix<cdim, T> &matr) const
{
	vector<vector<T>> newmatrix(cdim);
	for (int i = 0; i < cdim; i++)
		newmatrix[i].resize(cdim);

	newmatrix = matr.m_mtr;

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			matr.m_mtr[i][j] = newmatrix[j][i];
}

// для revf
template<int cdim, class T>
inline void matrix<cdim, T>::transp(vector<vector<float>> &matr) const
{
	vector<vector<float>> newmatrix(cdim);
	for (int i = 0; i < cdim; i++)
		newmatrix[i].resize(cdim);

	newmatrix = matr;

	for (int i = 0; i < cdim; i++)
		for (int j = 0; j < cdim; j++)
			matr[i][j] = newmatrix[j][i];
}

