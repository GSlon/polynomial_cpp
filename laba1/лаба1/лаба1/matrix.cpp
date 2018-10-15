#include "stdafx.h"

#ifndef MATRIX_H
#define MATRIX_H
#include "matrix.h"

template <class T>
matrix<T>::matrix()
{
	m_dim = 1;
	//m_mtr.push_back(0);
}

template <class T>
matrix<T>::matrix(int dim)
{
	m_dim = dim;
	
	for (int i = 0; i < dim; i++)
	{
		m_mtr.push_back(vector<int>());

		for (int j = 0; j < dim; j++)
			m_mtr[i].push_back(0);

	}
}

template <class T>
matrix<T>::~matrix()
{
}





#endif // !MATRIX_H