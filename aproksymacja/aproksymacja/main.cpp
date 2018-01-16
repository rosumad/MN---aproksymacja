#include<math.h>
#include<iostream>
#include<vector>
using namespace std;

double f(double x) { return sin(2 * x)*sin(x); }

struct Przedzial 
{
	double min;
	double max;
};

vector<double> rownoodlegleWezly(int n, Przedzial przedzial)
{
	vector<double> wezly;
	double wezel;
	double q = (przedzial.max - przedzial.min) / n;
	for (int i = 0; i <= n; i++) 
	{
		wezel = przedzial.min + q * i;
		wezly.push_back(wezel);
	}
	return wezly;
}

vector<double> wartosciAproksymowanej(vector<double> wezly) 
{
	vector<double> wartosciAproksymowanej;
	double wartosc;
	int n = wezly.size() - 1;
	for (int i = 0; i <= n; i++) 
	{
		wartosc = f(wezly[i]);
		wartosciAproksymowanej.push_back(wartosc);
	}
	return wartosciAproksymowanej;
}

double wielomianCzynnikowy(int q, int k)
{
	double wynik = 1;
	for (int i = 0; i <= k + 1; i++)
		wynik *= q - i;

	return wynik;
}

double symbolNewtona(int n, int k)
{
	double wynik = 1;
	for (int i = 1; i <= k; i++)
		wynik = wynik * (n - i + 1) / i;

	return wynik;
}

double wielomianGrama(int n, int k, int q) 
{
	double wynikSumy = 0.0;
	for (int s = 0; s <= k; s++)
		wynikSumy += pow(-1, s) * symbolNewtona(k, s) * symbolNewtona(k+s, s) * (wielomianCzynnikowy(q, s) / wielomianCzynnikowy(n, s));

	return wynikSumy;
}

double wspolczynnik(int n, int j, vector<double> wezly, vector<double> wartosciAproksymowanej) 
{
	double wynikSumyLicznik = 0.0;
	double wynikSumyMianownik = 0.0;
	double wynikGrama;
	for (int i = 0; i <= n; i++) 
	{
		wynikGrama = wielomianGrama(n, j, wezly[i]);
		wynikSumyLicznik += wartosciAproksymowanej[i] * wynikGrama;
		wynikSumyMianownik += pow(wynikGrama, 2);
	}

	return wynikSumyLicznik / wynikSumyMianownik;
}

vector<double> wartosciAproksymujacej(int m, vector<double> wezly, vector<double> wartosciAproksymowanej) 
{
	vector<double> wartosciAproksymujacej;
	double wynikSumy;
	int n = wezly.size() - 1;
	for (int i = 0; i <= n; i++)
	{
		wynikSumy = 0.0;
		for (int j = 0; j <= m; j++) 
			wynikSumy += wspolczynnik(n, j, wezly, wartosciAproksymowanej) * wielomianGrama(n, j, wezly[j]);
	
		wartosciAproksymowanej.push_back(wynikSumy);
	}
	return wartosciAproksymujacej;
}

int main() 
{
	Przedzial przedzial;
	przedzial.min = -2;
	przedzial.max = 2;
	vector<double> wezly;
	wezly = rownoodlegleWezly(50, przedzial);
	vector<double> aproksymowana;
	aproksymowana = wartosciAproksymowanej(wezly);
	vector<double> aproksymujaca;
	aproksymujaca = wartosciAproksymujacej(20, wezly, aproksymowana);

	for (int i = 0; i <= 50; i++)
		cout << wezly[i] << " " << aproksymowana[i] << " " << aproksymujaca[i] << endl;

	return 0;
}

/*
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>
using namespace std;

double funkcja(double x)
{
	return cos(x) * sin(x) * sin(x) * sin(x);
}



int newton(int n, int k)
{
	double rtrn = 1;
	for (int i = 1; i <= k; i++)
		rtrn *= (n - i + 1) / i;
	rtrn += 0.2;
	return (int)rtrn;
}

void przygotuj(double a, double b, int n, vector<double> & tablica, vector<double> & x)
{
	tablica.clear();
	x.clear();
	double tmp = (b - a) / n;
	for (int i = 0; i <= n; i++) {
		double arg = a + (tmp * i);
		x.push_back(arg);
		tablica.push_back(funkcja(arg));
	}
}

double mianA(int n, int j)
{
	double suma = .0;
	for (int q = 0; q <= n; ++q) {
		double gram = Gram(n, j, q);
		suma += gram * gram;
	}
	return suma;
}

double licznikA(int n, int j, const vector<double>& fwart)
{
	double suma = .0;
	for (int q = 0; q <= n; ++q)
		suma += fwart[q] * Gram(n, j, q);
	return suma;
}

double liczP(int n, int m, int q, const vector<double>& fwart)
{
	double suma = .0;
	for (int j = 0; j <= m; ++j)
		suma += licznikA(n, j, fwart) / mianA(n, j) * Gram(n, j, q);
	return suma;
}

void aproksymuj(int wezly, int stopien, double a, double b);
int main()
{
	for (int i = 1; i < 21; i++)
		aproksymuj(50, i, -8, 8);
	return 0;
}

void aproksymuj(int n, int m, double a, double b)
{
	vector<double> punkty;
	vector<double> org;
	vector<double> wynik;
	przygotuj(a, b, n, org, punkty);
	for (int q = 0; q <= n; ++q)
		wynik.push_back(liczP(n, m, q, org));
	//obliczamy blad
	double S = .0;
	for (int q = 0; q <= n; ++q) {
		double diff = org[q] - wynik[q];
		S += diff * diff;
	}
	S /= n + 1;
	S = sqrt(S);
	fstream raport, bledy;
	ostringstream ss;
	ss << "wezly " << n << " stopien " << m << "\n";
	raport.open("raport.txt", ios::out);
	raport << ss.str();
	raport.setf(ios::fixed);
	raport.precision(20);
	//output values;
	for (int q = 0; q <= n; ++q)
		raport << punkty[q] << "\t" << org[q] << "\t" << wynik[q] << "\n";
	raport.close();
	bledy.open("bledy.txt", ios::app);
	bledy.setf(ios::fixed);
	bledy.precision(20);
	ss.str(""), ss.clear();
	ss << "wezly " << n << " stopien " << m << "; (" << a << ", " << b << ")\t";
	bledy << "\n" << ss.str() << S;
	bledy.close();
}
*/