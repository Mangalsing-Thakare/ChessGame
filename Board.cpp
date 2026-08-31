#include "Board.h"

// =====================================================
//                    PIECES
// =====================================================

bool Pawn::canMoveTo(int newX, int newY) const {
    int dx = newX - x;
    int dy = newY - y;

    int direction = white ? 1 : -1;

    // Move one square forward
    if (dx == 0 && dy == direction) {
        return true;
    }

    // Move two squares forward from starting position
    if (!hasMoved &&
        dx == 0 &&
        dy == 2 * direction) {
        return true;
    }

    // Diagonal capture
    if (std::abs(dx) == 1 && dy == direction) {
        return true;
    }

    return false;
}


bool Knight::canMoveTo(int newX, int newY) const {
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    return (dx == 2 && dy == 1) ||
           (dx == 1 && dy == 2);
}


bool Bishop::canMoveTo(int newX, int newY) const {
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    return dx == dy && dx != 0;
}


bool Rook::canMoveTo(int newX, int newY) const {
    if (newX == x && newY == y) {
        return false;
    }

    return newX == x || newY == y;
}


bool Queen::canMoveTo(int newX, int newY) const {
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    if (newX == x && newY == y) {
        return false;
    }

    return newX == x ||
           newY == y ||
           dx == dy;
}


bool King::canMoveTo(int newX, int newY) const {
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    return dx <= 1 &&
           dy <= 1 &&
           (dx != 0 || dy != 0);
}


// =====================================================
//                      BOARD
// =====================================================

Board::Board() {
    initialize();
}


void Board::initialize() {

    // Clear board
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = nullptr;
        }
    }

    // ---------------- WHITE ----------------

    // Pawns
    for (int x = 0; x < BOARD_SIZE; x++) {
        board[1][x] =
            std::make_unique<Pawn>(true, x, 1);
    }

    // Rooks
    board[0][0] =
        std::make_unique<Rook>(true, 0, 0);

    board[0][7] =
        std::make_unique<Rook>(true, 7, 0);

    // Knights
    board[0][1] =
        std::make_unique<Knight>(true, 1, 0);

    board[0][6] =
        std::make_unique<Knight>(true, 6, 0);

    // Bishops
    board[0][2] =
        std::make_unique<Bishop>(true, 2, 0);

    board[0][5] =
        std::make_unique<Bishop>(true, 5, 0);

    // Queen
    board[0][3] =
        std::make_unique<Queen>(true, 3, 0);

    // King
    board[0][4] =
        std::make_unique<King>(true, 4, 0);


    // ---------------- BLACK ----------------

    // Pawns
    for (int x = 0; x < BOARD_SIZE; x++) {
        board[6][x] =
            std::make_unique<Pawn>(false, x, 6);
    }

    // Rooks
    board[7][0] =
        std::make_unique<Rook>(false, 0, 7);

    board[7][7] =
        std::make_unique<Rook>(false, 7, 7);

    // Knights
    board[7][1] =
        std::make_unique<Knight>(false, 1, 7);

    board[7][6] =
        std::make_unique<Knight>(false, 6, 7);

    // Bishops
    board[7][2] =
        std::make_unique<Bishop>(false, 2, 7);

    board[7][5] =
        std::make_unique<Bishop>(false, 5, 7);

    // Queen
    board[7][3] =
        std::make_unique<Queen>(false, 3, 7);

    // King
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


std::unique_ptr<Piece> Board::removePiece(int x, int y) {

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

    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int currentX = startX + stepX;
    int currentY = startY + stepY;

    while (currentX != endX ||
           currentY != endY) {

        if (getPiece(currentX, currentY) != nullptr) {
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

    Piece* piece = getPiece(startX, startY);

    if (piece == nullptr) {
        return false;
    }

    if (!piece->canMoveTo(endX, endY)) {
        return false;
    }

    Piece* destination = getPiece(endX, endY);

    // Cannot capture your own piece
    if (destination != nullptr &&
        destination->isWhite() == piece->isWhite()) {
        return false;
    }

    // Sliding pieces need clear paths
    PieceType type = piece->getType();

    if (type == PieceType::BISHOP ||
        type == PieceType::ROOK ||
        type == PieceType::QUEEN) {

        if (!isPathClear(startX, startY,
                         endX, endY)) {
            return false;
        }
    }

    // Pawn special handling
    if (type == PieceType::PAWN) {

        int dx = std::abs(endX - startX);
        int dy = std::abs(endY - startY);

        // Straight pawn movement cannot capture
        if (dx == 0 && destination != nullptr) {
            return false;
        }

        // Diagonal pawn movement MUST capture
        if (dx == 1 && destination == nullptr) {
            return false;
        }

        // Two-square pawn movement requires empty middle square
        if (dy == 2) {

            int direction =
                piece->isWhite() ? 1 : -1;

            int middleY =
                startY + direction;

            if (getPiece(startX, middleY) != nullptr) {
                return false;
            }
        }
    }

    // Move the piece
    auto movingPiece = removePiece(startX, startY);

    movingPiece->setPosition(endX, endY);

    // Pawn has now moved
    if (movingPiece->getType() == PieceType::PAWN) {

        Pawn* pawn =
            dynamic_cast<Pawn*>(movingPiece.get());

        if (pawn != nullptr) {
            pawn->setHasMoved(true);
        }
    }

    // Destination piece gets automatically destroyed
    // when overwritten.
    setPiece(endX, endY, std::move(movingPiece));

    return true;
}


void Board::display() const {

    std::cout << "\n";

    std::cout << "   a b c d e f g h\n";

    std::cout << "  -----------------\n";

    for (int y = BOARD_SIZE - 1; y >= 0; y--) {

        std::cout << y + 1 << "| ";

        for (int x = 0; x < BOARD_SIZE; x++) {

            Piece* piece = getPiece(x, y);

            if (piece == nullptr) {
                std::cout << ". ";
            }
            else {
                std::cout << piece->getSymbol()
                          << " ";
            }
        }

        std::cout << "|"
                  << y + 1
                  << "\n";
    }

    std::cout << "  -----------------\n";
    std::cout << "   a b c d e f g h\n\n";
}