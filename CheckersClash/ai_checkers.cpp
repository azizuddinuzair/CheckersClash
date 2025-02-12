#include "ai_checkers.h"
#include <iostream>
#include <algorithm>
#include <climits>


CheckersGame::CheckersGame() : blackTurn(false) {
    // Initialize empty board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = PieceType::EMPTY;
        }
    }
    
    // Place initial pieces
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((row + col) % 2 == 1) {
                board[row][col] = PieceType::RED;
            }
        }
    }
    
    for (int row = 5; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((row + col) % 2 == 1) {
                board[row][col] = PieceType::BLACK;
            }
        }
    }
}


void CheckersGame::printBoard() const {
    std::cout << "  a b c d e f g h\n";
    std::cout << "  -----------------\n";
    
    for (int row = BOARD_SIZE - 1; row >= 0; row--) {
        std::cout << row + 1 << " |";
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = '.';
            switch (board[row][col]) {
                case PieceType::RED: piece = 'r'; break;
                case PieceType::BLACK: piece = 'b'; break;
                case PieceType::RED_KING: piece = 'R'; break;
                case PieceType::BLACK_KING: piece = 'B'; break;
                default: break;
            }
            std::cout << " " << piece;
        }
        std::cout << " | " << row + 1 << "\n";
    }
    
    std::cout << "  -----------------\n";
    std::cout << "  a b c d e f g h\n";
}
