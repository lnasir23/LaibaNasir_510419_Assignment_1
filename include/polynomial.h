#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>

class Polynomial {

private:
    // Node struct and head for linked list storage
    struct Node {
        int coeff, exp;
        Node* next;
        Node(int c, int e) : coeff(c), exp(e), next(nullptr) {}

    };

    // Singly Linked List head
    Node* head;

    // Helper method to free nodes
    void clear();

    // Helper method to copy nodes
    void copy(const Polynomial& other);

public:
    // Constructor
    Polynomial();

    // Destructor
    ~Polynomial();

    // Copy constructor
    Polynomial(const Polynomial& other);

    // Assignment operator
    Polynomial& operator=(const Polynomial& other);


    // Insert a term into the polynomial
    void insertTerm(int coefficient, int exponent);

    // Return polynomial as a human-readable string
    std::string toString() const;

    // Return a new polynomial that is the sum of this and other
    Polynomial add(const Polynomial& other) const;

    // Return a new polynomial that is the product of this and other
    Polynomial multiply(const Polynomial& other) const;

    // Return a new polynomial that is the derivative of this polynomial
    Polynomial derivative() const;
};

#endif