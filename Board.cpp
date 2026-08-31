#include "Board.h"

// =====================================================
//                    PIECES
// =====================================================

bool Pawn::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = newX - x;
    int dy = newY - y;

    int direction = white ? 1 : -1;

    Piece* destination = board.getPiece(newX, newY);

    // One square forward
    if (dx == 0 && dy == direction) {
        return destination == nullptr;
    }

    // Two squares forward from starting position
    if (dx == 0 &&
        dy == 2 * direction &&
        !hasMoved) {

        int middleY = y + direction;

        return destination == nullptr &&
               board.getPiece(x, middleY) == nullptr;
    }

    // Diagonal capture
    if (std::abs(dx) == 1 &&
        dy == direction) {

        return destination != nullptr &&
               destination->isWhite() != white;
    }

    return false;
}


bool Knight::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    if (!((dx == 2 && dy == 1) ||
          (dx == 1 && dy == 2))) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    return destination == nullptr ||
           destination->isWhite() != white;
}


bool Bishop::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    if (dx == 0 || dx != dy) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    return board.isPathClear(
        x, y, newX, newY
    );
}


bool Rook::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    if (newX == x && newY == y) {
        return false;
    }

    if (newX != x && newY != y) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    return board.isPathClear(
        x, y, newX, newY
    );
}


bool Queen::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    bool straight =
        (newX == x || newY == y);

    bool diagonal =
        (dx == dy && dx != 0);

    if (!straight && !diagonal) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    return board.isPathClear(
        x, y, newX, newY
    );
}


bool King::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    if (dx > 1 || dy > 1 ||
        (dx == 0 && dy == 0)) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    return destination == nullptr ||
           destination->isWhite() != white;
}


// =====================================================
//                      BOARD
// =====================================================

Board::Board() {
    initialize();
}


void Board::initialize() {

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = nullptr;
        }
    }

    // ---------------- WHITE ----------------

    for (int x = 0; x < BOARD_SIZE; x++) {
        board[1][x] =
            std::make_unique<Pawn>(true, x, 1);
    }

    board[0][0] =
        std::make_unique<Rook>(true, 0, 0);

    board[0][7] =
        std::make_unique<Rook>(true, 7, 0);

    board[0][1] =
        std::make_unique<Knight>(true, 1, 0);

    board[0][6] =
        std::make_unique<Knight>(true, 6, 0);

    board[0][2] =
        std::make_unique<Bishop>(true, 2, 0);

    board[0][5] =
        std::make_unique<Bishop>(true, 5, 0);

    board[0][3] =
        std::make_unique<Queen>(true, 3, 0);

    board[0][4] =
        std::make_unique<King>(true, 4, 0);


    // ---------------- BLACK ----------------

    for (int x = 0; x < BOARD_SIZE; x++) {
        board[6][x] =
            std::make_unique<Pawn>(false, x, 6);
    }

    board[7][0] =
        std::make_unique<Rook>(false, 0, 7);

    board[7][7] =
        std::make_unique<Rook>(false, 7, 7);

    board[7][1] =
        std::make_unique<Knight>(false, 1, 7);

    board[7][6] =
        std::make_unique<Knight>(false, 6, 7);

    board[7][2] =
        std::make_unique<Bishop>(false, 2, 7);

    board[7][5] =
        std::make_unique<Bishop>(false, 5, 7);

    board[7][3] =
        std::make_unique<Queen>(false, 3, 7);

    board[7][4] =
        std::make_unique<King>(false, 4, 7);
}


Piece* Board::getPiece(int x, int y) const {

    if (!isInside(x, y)) {
        return nullptr;
    }

    return board[y][x].get();
}


void Board::setPiece(
    int x,
    int y,
    std::unique_ptr<Piece> piece
) {

    if (!isInside(x, y)) {
        return;
    }

    board[y][x] = std::move(piece);
}


std::unique_ptr<Piece> Board::removePiece(
    int x,
    int y
) {

    if (!isInside(x, y)) {
        return nullptr;
    }

    return std::move(board[y][x]);
}


bool Board::isInside(int x, int y) const {

    return x >= 0 &&
           x < BOARD_SIZE &&
           y >= 0 &&
           y < BOARD_SIZE;
}


bool Board::isPathClear(
    int startX,
    int startY,
    int endX,
    int endY
) const {

    int dx = endX - startX;
    int dy = endY - startY;

    int stepX =
        (dx == 0) ? 0 : (dx > 0 ? 1 : -1);

    int stepY =
        (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int currentX = startX + stepX;
    int currentY = startY + stepY;

    while (currentX != endX ||
           currentY != endY) {

        if (getPiece(
                currentX,
                currentY) != nullptr) {

            return false;
        }

        currentX += stepX;
        currentY += stepY;
    }

    return true;
}


bool Board::movePiece(
    int startX,
    int startY,
    int endX,
    int endY
) {

    Piece* piece =
        getPiece(startX, startY);

    if (piece == nullptr) {
        return false;
    }

    if (!piece->canMoveTo(
            endX,
            endY,
            *this)) {

        return false;
    }

    auto movingPiece =
        removePiece(startX, startY);

    // Destination is automatically destroyed
    // when this unique_ptr is overwritten.
    movingPiece->setPosition(
        endX,
        endY
    );

    if (movingPiece->getType() ==
        PieceType::PAWN) {

        Pawn* pawn =
            dynamic_cast<Pawn*>(
                movingPiece.get()
            );

        if (pawn != nullptr) {
            pawn->setHasMoved(true);
        }
    }

    setPiece(
        endX,
        endY,
        std::move(movingPiece)
    );

    return true;
}


void Board::display() const {

    std::cout << "\n";

    std::cout << "   a b c d e f g h\n";
    std::cout << "  -----------------\n";

    for (int y = BOARD_SIZE - 1; y >= 0; y--) {

        std::cout << y + 1 << "| ";

        for (int x = 0; x < BOARD_SIZE; x++) {

            Piece* piece =
                getPiece(x, y);

            if (piece == nullptr) {
                std::cout << ". ";
            }
            else {
                std::cout
                    << piece->getSymbol()
                    << " ";
            }
        }

        std::cout << "| "
                  << y + 1
                  << "\n";
    }

    std::cout << "  -----------------\n";
    std::cout << "   a b c d e f g h\n\n";
}