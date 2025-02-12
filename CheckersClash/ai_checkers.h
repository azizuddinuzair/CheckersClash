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