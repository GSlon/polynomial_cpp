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
	fraction<float> ifr(4,4);

	
	cout << ifr << endl;	//не пашет
	cin >> ifr;
	cout << ifr.getnmr() << endl;

	cout << ifr + ifr;

    return 0;
}

