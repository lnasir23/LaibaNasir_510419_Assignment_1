#include "polynomial.h"
#include <iostream>
#include <memory>

// Main program to run polynomial manager
int main() {

// Forming a polynomial
std::unique_ptr<Polynomial> p1 = Polynomial::create();
p1->insertTerm(3, 4);
p1->insertTerm(2, 2);
p1->insertTerm(-1, 1);
p1->insertTerm(5, 0);

// Performing sum op on polynomials
std::unique_ptr<Polynomial> p2 = Polynomial::create();
p2->insertTerm(1, 4);
p2->insertTerm(1, 0);
std::unique_ptr<Polynomial> sum = p1->add(*p2);

// Performing multiplication and
// derivation op on polynomials
std::unique_ptr<Polynomial> p3 = Polynomial::create();
p3->insertTerm(2, 1);
std::unique_ptr<Polynomial> prod = sum->multiply(*p3);
std::unique_ptr<Polynomial> deriv = p1->derivative();

std::cout << "p1: " << p1->toString();
std::cout << "\nsum: " << sum->toString();
std::cout << "\nprod: " << prod->toString();
std::cout << "\nderiv: " << deriv->toString() << std::endl;

return 0;
}

