//Creators: Uzair Azizuddin Firas Al Halaq
#ifndef AI_CHECKERS_H
#define AI_CHECKERS_H

#include <vector>
#include <string>
#include <unordered_map>


enum class PieceType {
    EMPTY, RED, BLACK, RED_KING, BLACK_KING
};


struct Move {
    int startRow, startCol;
    int endRow, endCol;
    bool isJump;
    std::vector<std::pair<int, int>> capturedPieces;
};

class CheckersGame {
    public:
        CheckersGame();
        void printBoard() const;
        bool makeMove(const Move& move);
        std::vector<Move> getValidMoves(int row, int col) const;
        std::vector<Move> getAllValidMoves(bool isBlackTurn) const;
        bool isGameOver() const;
        Move getBestMove(int difficulty);
        bool isValidPosition(const std::string& pos) const;
        std::pair<int, int> convertPosition(const std::string& pos) const;
        std::string convertToNotation(int row, int col) const;
        PieceType getPiece(int row, int col) const;
        bool isBlackTurn() const;
    
    private:
        static const int BOARD_SIZE = 8;
        PieceType board[BOARD_SIZE][BOARD_SIZE];
        bool blackTurn;
        std::unordered_map<std::string, int> transpositionTable;
        std::string boardToString() const;
        
        int minimax(int depth, bool maximizingPlayer, int alpha, int beta);
        void undoMove(const Move& move);
        int evaluateBoard() const;
        void getJumpMoves(int row, int col, std::vector<Move>& moves) const;
        void getNormalMoves(int row, int col, std::vector<Move>& moves) const;
        bool isValidMove(const Move& move) const;
        std::vector<Move> orderMoves(const std::vector<Move>& moves) const; 
};
 
#endif