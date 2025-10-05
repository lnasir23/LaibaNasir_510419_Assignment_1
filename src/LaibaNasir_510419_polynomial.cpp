#include "polynomial.h"
#include <sstream>
#include <memory>
#include <stdexcept>

using namespace std;

class Term{
public:
    int coefficient;
    int exponent;
    std::unique_ptr<Term> next;

    Term(int coeff, int exp) : coefficient(coeff), exponent(exp), next(nullptr) {}

};



class PolynomialImpl : public Polynomial {
private:
    std::unique_ptr<Term> head;

public:
    PolynomialImpl() : head(nullptr) {}

    PolynomialImpl(const PolynomialImpl& other) {

        if (other.head) {
            head = std::make_unique<Term>(other.head->coefficient, other.head->exponent);
            Term* current = head.get();
            Term* otherCurrent = other.head->next.get();

            while (otherCurrent) {
                current->next = std::make_unique<Term>(otherCurrent->coefficient, otherCurrent->exponent);
                current = current->next.get();
                otherCurrent = otherCurrent->next.get();
            }

        }

    }


    // Insert a term into the polynomial
    void insertTerm(int coefficient, int exponent) override {

        // Ignore zero coeff and non-positive exp
        if (coefficient == 0) return;

        if (exponent < 0) throw std::invalid_argument("Exponent must be non-negative");

        // If empty or exp higher than head
        if (!head || head->exponent < exponent) {

            auto newTerm = std::make_unique<Term>(coefficient, exponent);
            newTerm->next = std::move(head);
            head = std::move(newTerm);
            return;
        }

        // Insertion point
        Term* current = head.get();
        Term* prev = nullptr;

        while (current && current->exponent > exponent) {

            prev = current;
            current = current->next.get();

        }


        // Like terms combine
        if (current && current->exponent == exponent) {

            current->coefficient += coefficient;

            if(current->coefficient == 0) {

                if (prev) {

                    prev-> next = std::move(current->next);
                } else {

                    head = std::move(head->next);
                }
            }

        } else {

            // New term insert
            auto newTerm = std::make_unique<Term>(coefficient, exponent);
            if (prev) {
                newTerm->next = std::move(prev->next);
                prev->next = std::move(newTerm);

            } else {
                newTerm->next = std::move(head);
                head = std::move(newTerm);

            }
        }
    }


    // Return polynomial as a human-readable string
    std::string toString() const override {

        // Empty
        if (!head) return "0";

        ostringstream oss;
        Term* current = head.get();
        bool firstTerm = true;

        while(current) {
            int coeff = current->coefficient;
            int exp = current->exponent;

            if (!firstTerm) {
                if (coeff > 0) {
                    oss << " + ";

                } else {
                    oss << " - ";
                    coeff = -coeff;
                }
            } else {
                if (coeff < 0) {
                    oss << "-";
                    coeff = -coeff;
                }
            }
            
            if (exp == 0) {
                oss << coeff;

            } else if (exp == 1) {
                if (coeff ==1) {
                    oss << "x";

                } else {
                    oss << coeff << "x";

                }
            } else {
                if (coeff == 1){
                    oss << "x^" << exp;

                } else {
                    oss << coeff << "x^" << exp;
                }
            }

            firstTerm = false;
            current = current->next.get();

            }

        return oss.str();

    }



    // Return a new polynomial that is the sum of this and other
    std::unique_ptr<Polynomial> add(const Polynomial& other) const override {

        const PolynomialImpl* otherImpl = dynamic_cast<const PolynomialImpl*>(&other);
        if (!otherImpl) throw std::invalid_argument("Invalid polynomial type");


        auto result = std::make_unique<PolynomialImpl>();


            // Add all termd from this one polynomial
    Term* current = head.get();
        while (current) {
            result->insertTerm(current->coefficient, current->exponent);
            current = current->next.get();

    
        }


        // Add all other terms from other polynomial
        current = otherImpl->head.get();
        while (current) {
            result->insertTerm(current->coefficient, current->exponent);
            current = current->next.get();
        }

        return result;
    }


    // Return a new polynomial that is the product of this and other
    std::unique_ptr<Polynomial> multiply(const Polynomial& other) const override {
        const PolynomialImpl* otherImpl = dynamic_cast<const PolynomialImpl*>(&other);
        if (!otherImpl) throw std::invalid_argument("Invalid polynomial type");


        auto result = std::make_unique<PolynomialImpl>();

        Term* thisCurrent = head.get();
        while (thisCurrent) {

            Term* otherCurrent = otherImpl->head.get();
            while (otherCurrent) {
                int newCoeff = thisCurrent->coefficient * otherCurrent->coefficient;
                int newExp = thisCurrent->exponent + otherCurrent->exponent;
                result->insertTerm(newCoeff, newExp);
                otherCurrent = otherCurrent->next.get();

            }

            thisCurrent = thisCurrent->next.get();

        }

        return result;

    }

    // Return a new polynomial that is the derivative of this polynomial
    std::unique_ptr<Polynomial> derivative() const override {

        auto result = std::make_unique<PolynomialImpl>();

        Term* current = head.get();
        while (current) {

            if (current->exponent > 0) {
                int newCoeff = current->coefficient * current->exponent;
                int newExp = current->exponent - 1;
                result->insertTerm(newCoeff, newExp);

            }
            current= current->next.get();
        }

        return result;

    }
};

std::unique_ptr<Polynomial> Polynomial::create() {
    return std::make_unique<PolynomialImpl>();
}
