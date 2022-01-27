#ifndef COMPLEX_H
#define COMPLEX_H

class Complex
{
public:
	double Re;
	double Im;

	Complex();
	Complex(double re);
	Complex(double re, double im);

	Complex operator+(Complex x);
	Complex operator+=(Complex x);
	Complex operator-(Complex x);
	Complex operator*(Complex x);
	Complex operator*=(Complex x);
};

#endif // COMPLEX_H