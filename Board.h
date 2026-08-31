#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <memory>
#include <cmath>

const int BOARD_SIZE = 8;

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

class Board;

// ==================== PIECE ====================

class Piece {
protected:
    PieceType type;
    bool white;
    int x;
    int y;

public:
    Piece(PieceType type, bool white, int x, int y)
        : type(type), white(white), x(x), y(y) {}

    virtual ~Piece() = default;

    virtual bool canMoveTo(
        int newX,
        int newY,
        const Board& board
    ) const = 0;

    PieceType getType() const {
        return type;
    }

    bool isWhite() const {
        return white;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    virtual char getSymbol() const = 0;
};


// ==================== PAWN ====================

class Pawn : public Piece {
private:
    bool hasMoved;

public:
    Pawn(bool white, int x, int y)
        : Piece(PieceType::PAWN, white, x, y),
          hasMoved(false) {}

    bool canMoveTo(
        int newX,
        int newY,
        const Board& board
    ) const override;

    bool getHasMoved() const {
        return hasMoved;
    }

    void setHasMoved(bool value) {
        hasMoved = value;
    }

    char getSymbol() const override {
        return white ? 'P' : 'p';
    }
};


// ==================== KNIGHT ====================

class Knight : public Piece {
public:
    Knight(bool white, int x, int y)
        : Piece(PieceType::KNIGHT, white, x, y) {}

    bool canMoveTo(
        int newX,
        int newY,
        const Board& board
    ) const override;

    char getSymbol() const override {
        return white ? 'N' : 'n';
    }
};


// ==================== BISHOP ====================

class Bishop : public Piece {
public:
    Bishop(bool white, int x, int y)
        : Piece(PieceType::BISHOP, white, x, y) {}

    bool canMoveTo(
        int newX,
        int newY,
        const Board& board
    ) const override;

    char getSymbol() const override {
        return white ? 'B' : 'b';
    }
};


// ==================== ROOK ====================

class Rook : public Piece {
public:
    Rook(bool white, int x, int y)
        : Piece(PieceType::ROOK, white, x, y) {}

    bool canMoveTo(
        int newX,
        int newY,
        const Board& board
    ) const override;

    char getSymbol() const override {
        return white ? 'R' : 'r';
    }
};


// ==================== QUEEN ====================

class Queen : public Piece {
public:
    Queen(bool white, int x, int y)
        : Piece(PieceType::QUEEN, white, x, y) {}

    bool canMoveTo(
        int newX,
        int newY,
        const Board& board
    ) const override;

    char getSymbol() const override {
        return white ? 'Q' : 'q';
    }
};


// ==================== KING ====================

class King : public Piece {
public:
    King(bool white, int x, int y)
        : Piece(PieceType::KING, white, x, y) {}

    bool canMoveTo(
        int newX,
        int newY,
        const Board& board
    ) const override;

    char getSymbol() const override {
        return white ? 'K' : 'k';
    }
};


// ==================== BOARD ====================

class Board {
private:
    std::unique_ptr<Piece> board[BOARD_SIZE][BOARD_SIZE];

public:
    Board();

    void initialize();

    Piece* getPiece(int x, int y) const;

    void setPiece(
        int x,
        int y,
        std::unique_ptr<Piece> piece
    );

    std::unique_ptr<Piece> removePiece(
        int x,
        int y
    );

    bool isInside(int x, int y) const;

    bool isPathClear(
        int startX,
        int startY,
        int endX,
        int endY
    ) const;

    bool movePiece(
        int startX,
        int startY,
        int endX,
        int endY
    );

    bool isInCheck(bool white) const;

    bool hasLegalMove(bool white) const;

    bool isCheckmate(bool white) const;

    bool isStalemate(bool white) const;
    void display() const;
};

#endif