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


bool CheckersGame::isValidPosition(const std::string& pos) const {
    if (pos.length() != 2) return false;
    char col = pos[0];
    char row = pos[1];
    return (col >= 'a' && col <= 'h' && row >= '1' && row <= '8');
}


std::pair<int, int> CheckersGame::convertPosition(const std::string& pos) const {
    int col = pos[0] - 'a';
    int row = pos[1] - '1';
    return {row, col};
}


std::string CheckersGame::convertToNotation(int row, int col) const {
    return std::string(1, 'a' + col) + std::string(1, '1' + row);
}


void CheckersGame::getJumpMoves(int row, int col, std::vector<Move>& moves) const {
    PieceType currentPiece = board[row][col];
    bool isKing = (currentPiece == PieceType::RED_KING || currentPiece == PieceType::BLACK_KING);
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    for (auto [dr, dc] : directions) {
        if (!isKing && ((currentPiece == PieceType::RED && dr < 0) || 
                       (currentPiece == PieceType::BLACK && dr > 0))) {
            continue;
        }
        
        int newRow = row + dr * 2;
        int newCol = col + dc * 2;
        int jumpRow = row + dr;
        int jumpCol = col + dc;
        
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE &&
            board[newRow][newCol] == PieceType::EMPTY) {
            
            PieceType jumpedPiece = board[jumpRow][jumpCol];
            bool isOpponent = (currentPiece == PieceType::RED || currentPiece == PieceType::RED_KING) ?
                            (jumpedPiece == PieceType::BLACK || jumpedPiece == PieceType::BLACK_KING) :
                            (jumpedPiece == PieceType::RED || jumpedPiece == PieceType::RED_KING);
            
            if (isOpponent) {
                Move move = {row, col, newRow, newCol, true, {{jumpRow, jumpCol}}};
                moves.push_back(move);
            }
        }
    }
}


void CheckersGame::getNormalMoves(int row, int col, std::vector<Move>& moves) const {
    PieceType currentPiece = board[row][col];
    bool isKing = (currentPiece == PieceType::RED_KING || currentPiece == PieceType::BLACK_KING);
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    for (auto [dr, dc] : directions) {
        if (!isKing && ((currentPiece == PieceType::RED && dr < 0) || 
                       (currentPiece == PieceType::BLACK && dr > 0))) {
            continue;
        }
        
        int newRow = row + dr;
        int newCol = col + dc;
        
        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE &&
            board[newRow][newCol] == PieceType::EMPTY) {
            Move move = {row, col, newRow, newCol, false, {}};
            moves.push_back(move);
        }
    }
}


std::vector<Move> CheckersGame::getValidMoves(int row, int col) const {
    std::vector<Move> moves;
    
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return moves;
    }
    
    PieceType currentPiece = board[row][col];
    bool isCurrentPlayerPiece = (blackTurn && (currentPiece == PieceType::BLACK || 
                                              currentPiece == PieceType::BLACK_KING)) ||
                               (!blackTurn && (currentPiece == PieceType::RED || 
                                             currentPiece == PieceType::RED_KING));
    
    if (!isCurrentPlayerPiece) {
        return moves;
    }
    
    getJumpMoves(row, col, moves);
    if (moves.empty()) {
        getNormalMoves(row, col, moves);
    }
    
    return moves;
}


std::vector<Move> CheckersGame::getAllValidMoves(bool isBlackTurn) const {
    std::vector<Move> allMoves;
    std::vector<Move> jumpMoves;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            PieceType piece = getPiece(row, col);
            bool isCurrentPlayerPiece = (isBlackTurn && (piece == PieceType::BLACK || 
                piece == PieceType::BLACK_KING)) || (!isBlackTurn && (piece == PieceType::RED || piece == PieceType::RED_KING));

            if (isCurrentPlayerPiece) {
                std::vector<Move> moves = getValidMoves(row, col);
                for (const Move& move : moves) {
                    if (move.isJump) {
                        jumpMoves.push_back(move);
                    } else {
                        allMoves.push_back(move);
                    }
                }
            }
        }
    }

    // If there are jump moves available, they must be taken
    return jumpMoves.empty() ? allMoves : jumpMoves;
}


bool CheckersGame::makeMove(const Move& move) {
    if (!isValidMove(move)) {
        return false;
    }
    
    // Move the piece
    board[move.endRow][move.endCol] = board[move.startRow][move.startCol];
    board[move.startRow][move.startCol] = PieceType::EMPTY;
    
    // Handle captures
    if (move.isJump) {
        for (const auto& capture : move.capturedPieces) {
            board[capture.first][capture.second] = PieceType::EMPTY;
        }
    }
    
    // King promotion
    if (move.endRow == 0 && board[move.endRow][move.endCol] == PieceType::BLACK) {
        board[move.endRow][move.endCol] = PieceType::BLACK_KING;
    }
    else if (move.endRow == BOARD_SIZE - 1 && board[move.endRow][move.endCol] == PieceType::RED) {
        board[move.endRow][move.endCol] = PieceType::RED_KING;
    }
    
    blackTurn = !blackTurn;
    return true;
}


int CheckersGame::evaluateBoard() const {
    int score = 0;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            switch (board[row][col]) {
                case PieceType::BLACK:
                    score += 10;
                    score += row; // Preference for advancing
                    break;
                case PieceType::BLACK_KING:
                    score += 15;
                    break;
                case PieceType::RED:
                    score -= 10;
                    score -= (BOARD_SIZE - 1 - row); // Preference for advancing
                    break;
                case PieceType::RED_KING:
                    score -= 15;
                    break;
                default:
                    break;
            }
        }
    }
    return score;
}


std::vector<Move> CheckersGame::orderMoves(const std::vector<Move>& moves) const {
    std::vector<Move> orderedMoves = moves;
    std::sort(orderedMoves.begin(), orderedMoves.end(), [this](const Move& a, const Move& b) {
        // Prioritize jump moves
        if (a.isJump != b.isJump) {
            return a.isJump > b.isJump;
        }
        // Further ordering criteria can be added here
        return false;
    });
    return orderedMoves;
}


int CheckersGame::minimax(int depth, bool maximizingPlayer, int alpha, int beta) {
    std::string boardStr = boardToString();
    if (transpositionTable.find(boardStr) != transpositionTable.end()) {
        return transpositionTable[boardStr];
    }

    if (depth == 0 || isGameOver()) {
        int eval = evaluateBoard();
        transpositionTable[boardStr] = eval;
        return eval;
    }

    std::vector<Move> allMoves = getAllValidMoves(maximizingPlayer);
    allMoves = orderMoves(allMoves); // Order the moves

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (const Move& move : allMoves) {
            // Make move
            PieceType tempBoard[BOARD_SIZE][BOARD_SIZE];
            std::copy(&board[0][0], &board[0][0] + BOARD_SIZE * BOARD_SIZE, &tempBoard[0][0]);
            bool tempTurn = blackTurn;

            makeMove(move);
            int eval = minimax(depth - 1, false, alpha, beta);

            // Undo move
            std::copy(&tempBoard[0][0], &tempBoard[0][0] + BOARD_SIZE * BOARD_SIZE, &board[0][0]);
            blackTurn = tempTurn;

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        transpositionTable[boardStr] = maxEval;
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const Move& move : allMoves) {
            // Make move
            PieceType tempBoard[BOARD_SIZE][BOARD_SIZE];
            std::copy(&board[0][0], &board[0][0] + BOARD_SIZE * BOARD_SIZE, &tempBoard[0][0]);
            bool tempTurn = blackTurn;

            makeMove(move);
            int eval = minimax(depth - 1, true, alpha, beta);

            // Undo move
            std::copy(&tempBoard[0][0], &tempBoard[0][0] + BOARD_SIZE * BOARD_SIZE, &board[0][0]);
            blackTurn = tempTurn;

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;
        }
        transpositionTable[boardStr] = minEval;
        return minEval;
    }
}
