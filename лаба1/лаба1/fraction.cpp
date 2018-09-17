#include "stdafx.h"

#ifndef FRAC_H
#define FRAC_H
#include "fraction.h"

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
fraction<T> fraction<T>::operator+ (const fraction& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_dnm + fr2.m_nmr * this->m_dnm;
	newfr.m_dnm = this->m_dnm * fr2.m_dnm;
		
	return newfr;
}

template <class T>
fraction<T> fraction<T>::operator- (const fraction& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_dnm - fr2.m_nmr * this->m_dnm;
	newfr.m_dnm = this->m_dnm * fr2.m_dnm;

	return newfr;
}

template <class T>
fraction<T> fraction<T>::operator* (const fraction& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_nmr;
	newfr.m_dnm = this->m_dnm * fr2.m_dnm;

	return newfr;
}

template <class T>
fraction<T> fraction<T>::operator/ (const fraction& fr2) const
{
	fraction newfr;
	newfr.m_nmr = this->m_nmr * fr2.m_dnm;
	newfr.m_dnm = this->m_dnm * fr2.m_nmr;

	return newfr;
}



#endif // !FRAC_H