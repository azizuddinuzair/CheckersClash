#include "ai_checkers.h"
#include <iostream>
#include <string>

void displayGameInstructions() {
    std::cout << "\nWelcome to AI Checkers!\n";
    std::cout << "Instructions:\n";
    std::cout << "- You play as red (r/R), AI plays as black (b/B)\n";
    std::cout << "- Uppercase letters (R/B) represent kings\n";
    std::cout << "- Enter moves in algebraic notation (e.g., 'b6' for position)\n";
    std::cout << "- Type 'quit' to end the game\n\n";
}

int getDifficultyLevel() {
    int difficulty;
    do {
        std::cout << "Select difficulty level:\n";
        std::cout << "1. Easy\n";
        std::cout << "2. Medium\n";
        std::cout << "3. Hard\n";
        std::cout << "Enter choice (1-3): ";
        std::cin >> difficulty;
    } while (difficulty < 1 || difficulty > 3);
    return difficulty;
}

bool getPlayerMove(CheckersGame& game) {
    std::string input;
    while (true) {
        std::cout << "Select a piece to move (or 'quit'): ";
        std::cin >> input;

        if (input == "quit") {
            return false;
        }

        if (!game.isValidPosition(input)) {
            std::cout << "Invalid position. Use format like 'b6'.\n";
            continue;
        }

        auto [row, col] = game.convertPosition(input);
        auto moves = game.getValidMoves(row, col);

        if (moves.empty()) {
            std::cout << "No valid moves for this piece. Choose another.\n";
            continue;
        }

        // Display possible moves
        std::cout << "Possible moves (type 'x' to pick another piece):\n";
        for (const auto& move : moves) {
            std::cout << game.convertToNotation(move.endRow, move.endCol) << " ";
        }
        std::cout << "\nEnter move: ";
        std::cin >> input;

        if (input == "x") {
            continue;
        }

        if (!game.isValidPosition(input)) {
            std::cout << "Invalid move. Try again.\n";
            continue;
        }

        auto [endRow, endCol] = game.convertPosition(input);
        Move selectedMove;
        bool validMove = false;

        for (const auto& move : moves) {
            if (move.endRow == endRow && move.endCol == endCol) {
                selectedMove = move;
                validMove = true;
                break;
            }
        }

        if (!validMove) {
            std::cout << "Invalid move. Try again.\n";
            continue;
        }

        if (game.makeMove(selectedMove)) {
            break;
        }
    }
    return true;
}

int main() {
    displayGameInstructions();
    int difficulty = getDifficultyLevel();
    CheckersGame game;
    
    while (true) {
        game.printBoard();
        
        if (game.isGameOver()) {
            std::cout << (game.isBlackTurn() ? "Red" : "Black") << " wins!\n";
            break;
        }
        
        if (!game.isBlackTurn()) {  // Player's turn (Red)
            std::cout << "\nYour turn (Red)\n";
            if (!getPlayerMove(game)) {
                break;
            }
        } else {  // AI's turn (Black)
            std::cout << "\nAI's turn (Black)\n";
            Move aiMove = game.getBestMove(difficulty);
            std::cout << "AI moves from " 
                     << game.convertToNotation(aiMove.startRow, aiMove.startCol)
                     << " to "
                     << game.convertToNotation(aiMove.endRow, aiMove.endCol)
                     << std::endl;
            game.makeMove(aiMove);
        }
    }
    
    return 0;
}