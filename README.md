# LaibaNasir_510419_Assignment_1
Data Structures and Algorithms (CS250) Assignment 1 - Polynomial, Text Editor, and Uno Game

## Approach:
### Polynomial ADT:
Implemented using a sorted singly linked list where terms are stored in descending order of exponents. This allows efficient insertion while automatically combining like terms and ignoring zero coefficients. The implementation ensures mathematical accuracy and correctness for addition, multiplication and differentiation while producing human-readable output.
### Text Editor:
Used a singly linked list to represent the text, with each node storing one character. A cursor position is tracked internally to support insertion, deletion and navigation operations. the design allows efficient modifications at any cursor position while maintaining correct test representation.
### UNO Game:
Modeled the game using object_oriented principles with:
- Player hands as vectors of Card objects
- Draw and discard piles as deques for efficient front/back operations
- Rule-based turn management with special card handling (SKip, Reverse, Draw Two)
- Fixed random seed for reproducible gameplay
- Stalemate detection when no valid moves remain

## GitHub Link
https://github.com/lnasir23/LaibaNasir_510419_Assignment_1

## Challenges Faced
### Polynomial:
- Handling sign formatting for negative coefficients and ensuring proper mathematical notation.
- Managing term combination when coefficients cancel out to zero
- Implementing efficient polynomial multiplication without duplicate term creation
- Ensuring the descending exponent order during inserts
### Text Editor:
- Managing the cursor boundary conditions
- Handling where the insertion happens with respect to the cursor
- Ensuring proper text representation with cursor symbol placement
### UNO Game:
- Implementing card matching priorities
- Handing different action card results to give proper outcome
- Making direction transitions when conditions are met
