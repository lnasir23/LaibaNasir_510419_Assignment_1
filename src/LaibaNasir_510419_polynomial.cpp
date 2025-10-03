#include "polynomial.h"
#include <sstream>
#include <cstdlib>

using namespace std;

// Constructor
Polynomial::Polynomial() : head(nullptr) {}

// Destructor
Polynomial::~Polynomial(){
    clear();
}

// Copy constructor
Polynomial::Polynomial(const Polynomial& other) {
    copy(other);
}

// Assognment operator
Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this != &other) {
        clear();
        copy(other);
    }

    return *this;
}

// Clear helper method
void Polynomial::clear() {
    while (head) {

    Node* temp = head;
    head = head->next;
    delete temp;
}
}

// Copy helper method
void Polynomial::copy(const Polynomial& other) {
    head = nullptr;
    Node* tail = nullptr;
    for (Node* cur = other.head; cur; cur = cur->next) {
        if (cur->exp>=0) {
            Node* newNode = new Node(cur->coeff, cur->exp);
            if (!head) {
                head = tail = newNode;
            }
            else {
                tail -> next = newNode;
                tail = newNode;
            }
        }
    }
}

// Insert a term into the polynomial
void Polynomial::insertTerm(int coefficient, int exponent) {

    // Ignore zero coeff and non-positive exp
    if (coefficient == 0 || exponent < 0) return;

    Node* newNode = new Node(coefficient, exponent);

    // Front insertion if empty or exp highest
    if (!head || head->exp < exponent) {

        newNode->next = head;
        head = newNode;

        return;
    }

    // Insertion point (exp desc)
    Node* prev = nullptr;
    Node* cur = head;

    while (cur && cur->exp > exponent) {

        prev = cur;
        cur = cur->next;

    }


    // Like terms combine
    if (cur && cur->exp == exponent) {

        cur->coeff += coefficient;
        delete newNode;

        if(cur->coeff == 0) {

            if (prev) {
                prev-> next = cur->next;
            } else {

                head = cur->next;
            }
            delete cur;
        }


        return;
    }


    // Inserting new term
    newNode->next = cur;
    if (prev) {
        prev->next = newNode;
    } else {


        head = newNode;
    }
}


// Return polynomial as a human-readable string
std::string Polynomial::toString() const {

    // Empty
    if (!head) return "0";

    ostringstream oss;
    bool first = true;


    for (Node* cur = head; cur; cur = cur->next) {
        // In case of non-positie exp
        if (cur->exp< 0) continue;

        int abs_coeff = abs(cur->coeff);
        bool negative = cur->coeff < 0;


        // Sign handling
        if (!first) {
            oss << (negative ? " - " : " + ");

        } else if (negative) {
            oss << "-";
        }


        // Showing coeff > 1 and exp > 0
        bool show_coeff = (abs_coeff != 1) || (cur->exp == 0);
        if (show_coeff) oss << abs_coeff;

        // x and exp
        if (cur->exp > 0){
            oss << "x";
            if (cur->exp>1) {
            oss << "^" << cur->exp;
            }
        }
        first = false;
    }

    if (oss.str().empty()) return "0";

    return oss.str();

}


// Return a new polynomial that is the sum of this and other
Polynomial Polynomial::add(const Polynomial& other) const{

    Polynomial result;
    Node* p1 = head;
    Node* p2 = other.head;


    // Merge terms in desc order
    while (p1 || p2) {

        if (p1 && (!p2 || p1->exp > p2->exp)) {
            if (p1->exp >=0) {
            result.insertTerm(p1->coeff, p1->exp);
            }
            p1 = p1->next;
        } else if (p2 && (!p1 || p2->exp > p1->exp)) {
            if (p1->exp >=0) {
            result.insertTerm(p2->coeff, p2->exp);
            }
            p2 = p2->next;

        } else {

            int sum_coeff = p1->coeff + p2->coeff;
            if (sum_coeff != 0 && p1->exp >=0) {
                result.insertTerm(sum_coeff, p1->exp);

            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }

    return result;
}


// Return a new polynomial that is the product of this and other
Polynomial Polynomial::multiply(const Polynomial& other) const {
    Polynomial result;

    for (Node* cur1 = head; cur1; cur1 = cur1->next) {
        for (Node* cur2 = other.head; cur2; cur2 = cur2->next) {
            int newCoeff = cur1->coeff * cur2->coeff;
            int newExp = cur1->exp + cur2->exp;

            if (newExp >=0) {
            result.insertTerm(newCoeff, newExp);
            }
        }
        
    }

    return result;
}


// Return a new polynomial that is the derivative of this polynomial
Polynomial Polynomial::derivative() const {

    Polynomial result;
    for (Node* cur = head; cur; cur = cur->next){
        if (cur->exp > 0) {
            result.insertTerm(cur->coeff * cur->exp, cur->exp -1);
        }
    }

    return result;
}


