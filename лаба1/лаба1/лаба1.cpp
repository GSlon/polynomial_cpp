// лаба1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "fraction.h"
#include <iostream>
#include <cstdio>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
	fraction<double> ifr(4,7), ifr2(2,5);

	//cout << ifr << endl;	
	cin >> ifr;
	//cout << ifr2 << endl;
	cin >> ifr2;

	cout << ifr << " + " << ifr2 << " = " << ifr + ifr2 << endl;
	cout << ifr << " - " << ifr2 << " = " << ifr - ifr2 << endl;
	cout << ifr << " * " << ifr2 << " = " << ifr * ifr2 << endl;
	cout << ifr << " / " << ifr2 << " = " << ifr / ifr2 << endl;



    return 0;
}

