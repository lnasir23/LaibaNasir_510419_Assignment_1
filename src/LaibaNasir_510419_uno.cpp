#include "uno.h"
#include <vector>
#include <deque>
#include <random>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <memory>

// Concrete UNOGame implementation
class ConcreteUNOGame : public UNOGame {
private:


    int numPlayers;
    bool clockwise;
    int currentPlayer;

    std::vector<std::vector<Card>> playersHands;
    std::deque<Card> drawPile;
    std::deque<Card> discardPile;
    std::mt19937 rng;

    bool gameOver;
    int winner;

public:
    ConcreteUNOGame(int numPlayers) : UNOGame(numPlayers),
        numPlayers(numPlayers), clockwise(true), currentPlayer(0),
        gameOver(false), winner(-1), rng(1234) {
        
        if (numPlayers < 2 || numPlayers > 4) {
            throw std::invalid_argument("Number of players must be between 2 and 4");
        }
    }

    void initialize() override {
        // Reset game state
        playersHands.clear();
        drawPile.clear();
        discardPile.clear();
        clockwise = true;
        currentPlayer = 0;
        gameOver = false;
        winner = -1;

        // Initialize players' hands
        playersHands.resize(numPlayers);


        // Create the deck (76 cards as specified)
        createDeck();

        // Shuffle the deck
        std::shuffle(drawPile.begin(), drawPile.end(), rng);


        // Deal 7 cards to each player
        for (int i = 0; i < numPlayers; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (!drawPile.empty()) {
                    playersHands[i].push_back(drawPile.front());
                    drawPile.pop_front();
                }


            }

        }

        // Start with top card from draw pile
        if (!drawPile.empty()) {
            discardPile.push_back(drawPile.front());
            drawPile.pop_front();
        }


    }

    void playTurn() override {
        if (gameOver) return;

        Card topCard = discardPile.back();
        bool playedCard = false;


        // Check if current player has any playable cards
        std::vector<int> playableIndices;
        for (int i = 0; i < playersHands[currentPlayer].size(); ++i) {

            if (playersHands[currentPlayer][i].matches(topCard)) {
                playableIndices.push_back(i);
            }
        }

        if (!playableIndices.empty()) {
            // Play the first matching card according to priority
            int cardIndex = selectCardToPlay(playableIndices, topCard);

            Card played = playersHands[currentPlayer][cardIndex];
            
            // Remove card from player's hand
            playersHands[currentPlayer].erase(playersHands[currentPlayer].begin() + cardIndex);
            
            // Add to discard pile
            discardPile.push_back(played);
            
            // Handle action cards

            handleActionCard(played);
            
            playedCard = true;
            
            // Check for UNO and win condition
            if (playersHands[currentPlayer].size() == 1) {
                // UNO condition - in simulation we just note it
            }
            
            if (playersHands[currentPlayer].empty()) {
                gameOver = true;
                winner = currentPlayer;
                return;
            }
        } else {


            // Draw one card
            if (!drawPile.empty()) {
                Card drawn = drawPile.front();

                drawPile.pop_front();
                playersHands[currentPlayer].push_back(drawn);
                
                // If drawn card is playable, play it immediately
                if (drawn.matches(topCard)) {
                    playersHands[currentPlayer].pop_back(); // Remove the card we just added
                    discardPile.push_back(drawn);
                    handleActionCard(drawn);
                    playedCard = true;
                    
                    if (playersHands[currentPlayer].empty()) {
                        gameOver = true;
                        winner = currentPlayer;
                        return;
                    }
                }
            }
        }

        // Move to next player if no action card changed the turn
        if (playedCard) {
            // Action cards already handled the turn change in handleActionCard
            // For normal play, just move to next player

            if (discardPile.back().type != Card::SKIP &&
                discardPile.back().type != Card::REVERSE &&
                discardPile.back().type != Card::DRAW_TWO) {
                moveToNextPlayer();
            }
        } else {
            moveToNextPlayer();
        }

        // Check for stalemate (empty deck and no plays possible)
        checkStalemate();
    }

    bool isGameOver() const override {
        return gameOver;
    }

    int getWinner() const override {
        return winner;
    }

    std::string getState() const override {
        std::stringstream ss;

        ss << "Player " << currentPlayer << "'s turn, Direction: "
        << (clockwise ? "Clockwise" : "Counter-clockwise")
        << ", Top: " << discardPile.back().toString()
        << ", Players cards: ";
        
        for (int i = 0; i < numPlayers; ++i) {
            ss << "P" << i << ":" << playersHands[i].size();
            if (i < numPlayers - 1) {
                ss << ", ";
            }
        }
        
        return ss.str();
    }

private:
    void createDeck() {
        Card::Color colors[] = {Card::RED, Card::GREEN, Card::BLUE, Card::YELLOW};
        
        // Create number cards (1-9: two each, 0: one each)
        for (auto color : colors) {
            // One zero card per color
            drawPile.push_back(Card(color, Card::NUMBER, 0));
            
            // Two of each number 1-9 per color
            for (int num = 1; num <= 9; ++num) {
                drawPile.push_back(Card(color, Card::NUMBER, num));
                drawPile.push_back(Card(color, Card::NUMBER, num));
            }
            
            // Action cards (two of each per color)
            for (int i = 0; i < 2; ++i) {
                drawPile.push_back(Card(color, Card::SKIP));
                drawPile.push_back(Card(color, Card::REVERSE));
                drawPile.push_back(Card(color, Card::DRAW_TWO));
            }
        }
    }

    int selectCardToPlay(const std::vector<int>& playableIndices, const Card& topCard) {
        // Priority order: color match > value match > action cards (Skip > Reverse > Draw Two)
        
        // First, check for color matches
        for (int idx : playableIndices) {
            if (playersHands[currentPlayer][idx].color == topCard.color) {
                return idx;


            }
        }
        
        // Then check for value/number matches
        for (int idx : playableIndices) {

            if (playersHands[currentPlayer][idx].type == Card::NUMBER && 
                topCard.type == Card::NUMBER &&
                playersHands[currentPlayer][idx].number == topCard.number) {
                return idx;
            }
        }
        
        // Finally, prioritize action cards: Skip > Reverse > Draw Two
        int bestIndex = playableIndices[0];
        Card::Type bestType = playersHands[currentPlayer][bestIndex].type;


        
        for (int i = 1; i < playableIndices.size(); ++i) {
            Card::Type currentType = playersHands[currentPlayer][playableIndices[i]].type;
            
            if (currentType == Card::SKIP) {

                return playableIndices[i]; // Skip has highest priority

            } else if (currentType == Card::REVERSE && bestType != Card::SKIP) {
                bestIndex = playableIndices[i];
                bestType = currentType;
            } else if (currentType == Card::DRAW_TWO &&
                    bestType != Card::SKIP && bestType != Card::REVERSE) {
                bestIndex = playableIndices[i];
                bestType = currentType;
            }
        }
        
        return bestIndex;
    }

    void handleActionCard(const Card& played) {
        switch (played.type) {
            case Card::SKIP:

                moveToNextPlayer(); // Skip next player
                break;
                
            case Card::REVERSE:
                clockwise = !clockwise;
                // In 2-player game, Reverse acts like Skip
                if (numPlayers == 2) {

                    moveToNextPlayer();
                }
                break;
                
            case Card::DRAW_TWO: {
                int nextPlayer = getNextPlayer();


                // Make next player draw 2 cards
                for (int i = 0; i < 2 && !drawPile.empty(); ++i) {
                    playersHands[nextPlayer].push_back(drawPile.front());
                    drawPile.pop_front();


                }
                moveToNextPlayer(); // Skip the player who had to draw
                break;
            }
                
            default:
                // Number card - no special action
                break;
        }
    }

    void moveToNextPlayer() {
        if (clockwise) {

            currentPlayer = (currentPlayer + 1) % numPlayers;
        } else {

            currentPlayer = (currentPlayer - 1 + numPlayers) % numPlayers;
        }
    }

    int getNextPlayer() const {
        if (clockwise) {
            return (currentPlayer + 1) % numPlayers;
        } else {


            return (currentPlayer - 1 + numPlayers) % numPlayers;
        }
    }

    void checkStalemate() {

        // Check if deck is empty and no player can play
        if (drawPile.empty()) {

            bool anyPlayable = false;
            Card topCard = discardPile.back();
            
            for (int i = 0; i < numPlayers && !anyPlayable; ++i) {
                for (const auto& card : playersHands[i]) {
                    if (card.matches(topCard)) {
                        anyPlayable = true;
                        break;
                    }
                }
            }
            
            if (!anyPlayable) {
                gameOver = true;
                
                // No winner in stalemate
                winner = -1;
            }
        }
    }
};

// Factory method
std::unique_ptr<UNOGame> UNOGame::create(int numPlayers) {
    return std::make_unique<ConcreteUNOGame>(numPlayers);
}