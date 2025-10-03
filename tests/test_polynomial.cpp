#include "polynomial.h"
#include <cassert>
#include <iostream>


using namespace std;


// Main program to run polynomial manager
int main() {

// Forming a polynomial
Polynomial p1;
p1.insertTerm(3, 4);
p1.insertTerm(2, 2);
p1.insertTerm(-1, 1);
p1.insertTerm(5, 0);

// Performing sum op on polynomials
Polynomial p2;
p2.insertTerm(1, 4);
p2.insertTerm(1, 0);
Polynomial sum = p1.add(p2);

// Performing multiplication and
// derivation op on polynomials
Polynomial p3;
p3.insertTerm(2, 1);
Polynomial prod = sum.multiply(p3);
Polynomial deriv = p1.derivative();

cout << "p1: " << p1.toString();
cout << "\nsum: " << sum.toString();
cout << "\nprod: " << prod.toString();
cout << "\nderiv: " << deriv.toString() << endl;

return 0;
}

