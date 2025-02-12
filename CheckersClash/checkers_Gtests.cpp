#include "ai_checkers.h"
#include <gtest/gtest.h>

class CheckersGameTest : public ::testing::Test {
protected:
    CheckersGame game;
};

TEST_F(CheckersGameTest, IsValidPosition) {
    EXPECT_TRUE(game.isValidPosition("a1"));
    EXPECT_TRUE(game.isValidPosition("h8"));
    EXPECT_FALSE(game.isValidPosition("i1"));
    EXPECT_FALSE(game.isValidPosition("a9"));
}

TEST_F(CheckersGameTest, ConvertPosition) {
    auto pos = game.convertPosition("a1");
    EXPECT_EQ(pos.first, 0);
    EXPECT_EQ(pos.second, 0);

    pos = game.convertPosition("h8");
    EXPECT_EQ(pos.first, 7);
    EXPECT_EQ(pos.second, 7);
}

TEST_F(CheckersGameTest, GetPiece) {
    EXPECT_EQ(game.getPiece(0, 0), PieceType::RED);
    EXPECT_EQ(game.getPiece(7, 7), PieceType::BLACK);
    EXPECT_EQ(game.getPiece(3, 3), PieceType::EMPTY);
}

TEST_F(CheckersGameTest, MakeMove) {
    Move move = {2, 1, 3, 2, false, {}};
    EXPECT_TRUE(game.makeMove(move));
    EXPECT_EQ(game.getPiece(3, 2), PieceType::RED);
    EXPECT_EQ(game.getPiece(2, 1), PieceType::EMPTY);
}

TEST_F(CheckersGameTest, IsGameOver) {
    EXPECT_FALSE(game.isGameOver());

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            game.board[row][col] = PieceType::EMPTY;
        }
    }
    EXPECT_TRUE(game.isGameOver());
}

TEST_F(CheckersGameTest, GetAllValidMoves) {
    auto moves = game.getAllValidMoves(true);
    EXPECT_FALSE(moves.empty());

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            game.board[row][col] = PieceType::EMPTY;
        }
    }
    moves = game.getAllValidMoves(true);
    EXPECT_TRUE(moves.empty());
}

TEST_F(CheckersGameTest, EvaluateBoard) {
    int score = game.evaluateBoard();
    EXPECT_EQ(score, 0);

    game.board[0][0] = PieceType::BLACK;
    game.board[7][7] = PieceType::EMPTY;
    score = game.evaluateBoard();
    EXPECT_GT(score, 0);
}

TEST_F(CheckersGameTest, Minimax) {
    int score = game.minimax(1, true, INT_MIN, INT_MAX);
    EXPECT_NE(score, 0);
}

TEST_F(CheckersGameTest, GetBestMove) {
    Move bestMove = game.getBestMove(1);
    EXPECT_NE(bestMove.startRow, -1);
}

TEST(CheckersGameTest, InitialBoardSetup) {
    CheckersGame game;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                EXPECT_EQ(game.getPiece(row, col), PieceType::RED);
            }
        }
    }
    for (int row = 5; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                EXPECT_EQ(game.getPiece(row, col), PieceType::BLACK);
            }
        }
    }
    for (int row = 3; row < 5; row++) {
        for (int col = 0; col < 8; col++) {
            EXPECT_EQ(game.getPiece(row, col), PieceType::EMPTY);
        }
    }
}

TEST(CheckersGameTest, ValidMove) {
    CheckersGame game;
    Move move = {2, 1, 3, 2, false, {}};
    EXPECT_TRUE(game.makeMove(move));
    EXPECT_EQ(game.getPiece(3, 2), PieceType::RED);
    EXPECT_EQ(game.getPiece(2, 1), PieceType::EMPTY);
}

TEST(CheckersGameTest, InvalidMove) {
    CheckersGame game;
    Move move = {2, 1, 4, 2, false, {}};
    EXPECT_FALSE(game.makeMove(move));
    EXPECT_EQ(game.getPiece(2, 1), PieceType::RED);
    EXPECT_EQ(game.getPiece(4, 2), PieceType::EMPTY);
}

TEST(CheckersGameTest, JumpMove) {
    CheckersGame game;
    game.makeMove({2, 1, 3, 2, false, {}});
    game.makeMove({5, 2, 4, 3, false, {}});
    Move jumpMove = {3, 2, 5, 4, true, {{4, 3}}};
    EXPECT_TRUE(game.makeMove(jumpMove));
    EXPECT_EQ(game.getPiece(5, 4), PieceType::RED);
    EXPECT_EQ(game.getPiece(3, 2), PieceType::EMPTY);
    EXPECT_EQ(game.getPiece(4, 3), PieceType::EMPTY);
}

TEST(CheckersGameTest, KingPromotion) {
    CheckersGame game;
    game.makeMove({2, 1, 3, 2, false, {}});
    game.makeMove({5, 2, 4, 3, false, {}});
    game.makeMove({3, 2, 5, 4, true, {{4, 3}}});
    game.makeMove({6, 1, 5, 2, false, {}});
    game.makeMove({5, 4, 6, 5, false, {}});
    game.makeMove({7, 0, 6, 1, false, {}});
    game.makeMove({6, 5, 7, 6, false, {}});
    game.makeMove({7, 6, 5, 4, true, {{6, 5}}});
    EXPECT_EQ(game.getPiece(5, 4), PieceType::BLACK_KING);
}

TEST(CheckersGameTest, GameOver) {
    CheckersGame game;
    game.makeMove({2, 1, 3, 2, false, {}});
    game.makeMove({5, 2, 4, 3, false, {}});
    game.makeMove({3, 2, 5, 4, true, {{4, 3}}});
    game.makeMove({6, 1, 5, 2, false, {}});
    game.makeMove({5, 4, 6, 5, false, {}});
    game.makeMove({7, 0, 6, 1, false, {}});
    game.makeMove({6, 5, 7, 6, false, {}});
    game.makeMove({7, 6, 5, 4, true, {{6, 5}}});
    EXPECT_TRUE(game.isGameOver());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
