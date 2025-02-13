/**
 * This file contains the implementation of the CheckersGame class, which provides
 * the functionality for initializing the game board, printing the board, validating
 * positions and moves, generating valid moves, making moves, evaluating the board,
 * and determining the best move using the minimax algorithm with alpha-beta pruning.
 * 
 * The CheckersGame class supports both normal and jump moves, king promotion, and
 * game over detection. It also includes a transposition table to store evaluated
 * board states for optimization.
 * 
 * The main methods include:
 * - CheckersGame(): Constructor to initialize the game board.
 * - printBoard(): Prints the current state of the game board.
 * - isValidPosition(): Checks if a given position is valid on the board.
 * - convertPosition(): Converts a position from notation to row and column indices.
 * - convertToNotation(): Converts row and column indices to board notation.
 * - getJumpMoves(): Generates all possible jump moves for a piece.
 * - getNormalMoves(): Generates all possible normal moves for a piece.
 * - getValidMoves(): Returns all valid moves for a piece at a given position.
 * - getAllValidMoves(): Returns all valid moves for the current player.
 * - makeMove(): Executes a move on the board.
 * - evaluateBoard(): Evaluates the board and returns a score.
 * - orderMoves(): Orders moves based on priority (e.g., jump moves first).
 * - minimax(): Implements the minimax algorithm with alpha-beta pruning.
 * - getBestMove(): Determines the best move for the current player based on difficulty.
 * - getPiece(): Returns the piece at a given position.
 * - isGameOver(): Checks if the game is over.
 * - isValidMove(): Checks if a given move is valid.
 * - boardToString(): Converts the board to a string representation for the transposition table.
 * - isBlackTurn(): Returns whether it is the black player's turn.
 */
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

void CheckersGame::undoMove(const Move& move) {
    // Move the piece back to its original position
    board[move.startRow][move.startCol] = board[move.endRow][move.endCol];
    board[move.endRow][move.endCol] = PieceType::EMPTY;

    // Restore captured pieces
    if (move.isJump) {
        for (const auto& capture : move.capturedPieces) {
            board[capture.first][capture.second] = (blackTurn ? PieceType::RED : PieceType::BLACK);
        }
    }

    // Undo king promotion
    if (move.endRow == 0 && board[move.startRow][move.startCol] == PieceType::BLACK_KING) {
        board[move.startRow][move.startCol] = PieceType::BLACK;
    } else if (move.endRow == BOARD_SIZE - 1 && board[move.startRow][move.startCol] == PieceType::RED_KING) {
        board[move.startRow][move.startCol] = PieceType::RED;
    }

    // Revert the turn
    blackTurn = !blackTurn;
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
            makeMove(move);
            int eval = minimax(depth - 1, false, alpha, beta);
            undoMove(move);

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
            makeMove(move);
            int eval = minimax(depth - 1, true, alpha, beta);
            undoMove(move);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;
        }
        transpositionTable[boardStr] = minEval;
        return minEval;
    }
}


Move CheckersGame::getBestMove(int difficulty) {
    int depth;
    switch (difficulty) {
        case 1: depth = 2; break;  // Easy
        case 2: depth = 4; break;  // Medium
        case 3: depth = 6; break;  // Hard
        default: depth = 4; break;
    }
    
    std::vector<Move> allMoves = getAllValidMoves(blackTurn);
    Move bestMove = allMoves[0];
    int bestValue = INT_MIN;
    
    for (const Move& move : allMoves) {
        // Make move
        PieceType tempBoard[BOARD_SIZE][BOARD_SIZE];
        std::copy(&board[0][0], &board[0][0] + BOARD_SIZE * BOARD_SIZE, &tempBoard[0][0]);
        bool tempTurn = blackTurn;
        
        makeMove(move);
        int moveValue = minimax(depth - 1, false, INT_MIN, INT_MAX);
        
        // Undo move
        std::copy(&tempBoard[0][0], &tempBoard[0][0] + BOARD_SIZE * BOARD_SIZE, &board[0][0]);
        blackTurn = tempTurn;
        
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    
    return bestMove;
}

PieceType CheckersGame::getPiece(int row, int col) const {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return PieceType::EMPTY;
    }
    return board[row][col];
}

bool CheckersGame::isGameOver() const {
    bool redHasMoves = false;
    bool blackHasMoves = false;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            PieceType piece = getPiece(row, col);
            if (piece == PieceType::RED || piece == PieceType::RED_KING) {
                if (!getValidMoves(row, col).empty()) {
                    redHasMoves = true;
                }
            } else if (piece == PieceType::BLACK || piece == PieceType::BLACK_KING) {
                if (!getValidMoves(row, col).empty()) {
                    blackHasMoves = true;
                }
            }
        }
    }

    return !redHasMoves || !blackHasMoves;
}

bool CheckersGame::isValidMove(const Move& move) const {
    std::vector<Move> validMoves = getValidMoves(move.startRow, move.startCol);
    for (const Move& validMove : validMoves) {
        if (validMove.startRow == move.startRow && validMove.startCol == move.startCol &&
            validMove.endRow == move.endRow && validMove.endCol == move.endCol) {
            return true;
        }
    }
    return false;
}

std::string CheckersGame::boardToString() const {
    std::string boardStr;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            boardStr += std::to_string(static_cast<int>(board[row][col]));
        }
    }
    return boardStr;
}

bool CheckersGame::isBlackTurn() const {
    return blackTurn;
}
