#include "Complex.h"

Complex::Complex()
{
	Re = 0;
	Im = 0;
}

Complex ::Complex(double re)
{
	Re = re;
	Im = 0;
}

Complex::Complex(double re, double im)
{
	Re = re;
	Im = im;
}

Complex Complex::operator+(Complex x)
{
	return Complex(Re + x.Re, Im + x.Im);
}

Complex Complex::operator+=(Complex x)
{
	return Complex(Re + x.Re, Im + x.Im);
}

Complex Complex::operator-(Complex x)
{
	return Complex(Re - x.Re, Im - x.Im);
}

Complex Complex::operator*(Complex x)
{
	return Complex(Re * x.Re - Im * x.Im, Re * x.Im + Im * x.Re);
}

Complex Complex::operator*=(Complex x)
{
	return Complex(Re * x.Re - Im * x.Im, Re * x.Im + Im * x.Re);
}
