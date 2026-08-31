#include "Board.h"

// =====================================================
//                    PIECES
// =====================================================

// -------------------- PAWN --------------------

bool Pawn::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = newX - x;
    int dy = newY - y;

    // White moves upward (+y)
    // Black moves downward (-y)
    int direction = white ? 1 : -1;

    Piece* destination = board.getPiece(newX, newY);

    // Move one square forward
    if (dx == 0 && dy == direction) {
        return destination == nullptr;
    }

    // Move two squares forward on first move
    if (dx == 0 &&
        dy == 2 * direction &&
        !hasMoved) {

        int middleY = y + direction;

        return destination == nullptr &&
               board.getPiece(x, middleY) == nullptr;
    }

    // Capture diagonally
    if (std::abs(dx) == 1 &&
        dy == direction) {

        return destination != nullptr &&
               destination->isWhite() != white;
    }

    return false;
}


// -------------------- KNIGHT --------------------

bool Knight::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    // Knight must move in an L shape
    if (!((dx == 2 && dy == 1) ||
          (dx == 1 && dy == 2))) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    // Empty square OR opponent piece
    return destination == nullptr ||
           destination->isWhite() != white;
}


// -------------------- BISHOP --------------------

bool Bishop::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    // Bishop moves diagonally
    if (dx == 0 || dx != dy) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    // Cannot capture own piece
    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    // No piece can be between start and destination
    return board.isPathClear(
        x, y, newX, newY
    );
}


// -------------------- ROOK --------------------

bool Rook::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    // Cannot stay on same square
    if (newX == x && newY == y) {
        return false;
    }

    // Rook moves horizontally OR vertically
    if (newX != x && newY != y) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    // Cannot capture own piece
    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    // Check for pieces blocking the path
    return board.isPathClear(
        x, y, newX, newY
    );
}


// -------------------- QUEEN --------------------

bool Queen::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    // Queen can move horizontally/vertically
    bool straight =
        (newX == x || newY == y);

    // Or diagonally
    bool diagonal =
        (dx == dy && dx != 0);

    if (!straight && !diagonal) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    // Cannot capture own piece
    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    // Check for blocking pieces
    return board.isPathClear(
        x, y, newX, newY
    );
}


// -------------------- KING --------------------

bool King::canMoveTo(
    int newX,
    int newY,
    const Board& board
) const {

    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    // King moves exactly one square
    if (dx > 1 ||
        dy > 1 ||
        (dx == 0 && dy == 0)) {
        return false;
    }

    Piece* destination = board.getPiece(newX, newY);

    // Empty square OR opponent piece
    return destination == nullptr ||
           destination->isWhite() != white;
}


// =====================================================
//                      BOARD
// =====================================================

Board::Board() {
    initialize();
}


// -------------------- INITIALIZE --------------------

void Board::initialize() {

    // Clear board first
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = nullptr;
        }
    }


    // =================================================
    //                    WHITE
    // =================================================

    // Pawns
    for (int x = 0; x < BOARD_SIZE; x++) {
        board[1][x] =
            std::make_unique<Pawn>(
                true, x, 1
            );
    }

    // Rooks
    board[0][0] =
        std::make_unique<Rook>(
            true, 0, 0
        );

    board[0][7] =
        std::make_unique<Rook>(
            true, 7, 0
        );

    // Knights
    board[0][1] =
        std::make_unique<Knight>(
            true, 1, 0
        );

    board[0][6] =
        std::make_unique<Knight>(
            true, 6, 0
        );

    // Bishops
    board[0][2] =
        std::make_unique<Bishop>(
            true, 2, 0
        );

    board[0][5] =
        std::make_unique<Bishop>(
            true, 5, 0
        );

    // Queen
    board[0][3] =
        std::make_unique<Queen>(
            true, 3, 0
        );

    // King
    board[0][4] =
        std::make_unique<King>(
            true, 4, 0
        );


    // =================================================
    //                    BLACK
    // =================================================

    // Pawns
    for (int x = 0; x < BOARD_SIZE; x++) {
        board[6][x] =
            std::make_unique<Pawn>(
                false, x, 6
            );
    }

    // Rooks
    board[7][0] =
        std::make_unique<Rook>(
            false, 0, 7
        );

    board[7][7] =
        std::make_unique<Rook>(
            false, 7, 7
        );

    // Knights
    board[7][1] =
        std::make_unique<Knight>(
            false, 1, 7
        );

    board[7][6] =
        std::make_unique<Knight>(
            false, 6, 7
        );

    // Bishops
    board[7][2] =
        std::make_unique<Bishop>(
            false, 2, 7
        );

    board[7][5] =
        std::make_unique<Bishop>(
            false, 5, 7
        );

    // Queen
    board[7][3] =
        std::make_unique<Queen>(
            false, 3, 7
        );

    // King
    board[7][4] =
        std::make_unique<King>(
            false, 4, 7
        );
}


// -------------------- GET PIECE --------------------

Piece* Board::getPiece(int x, int y) const {

    if (!isInside(x, y)) {
        return nullptr;
    }

    return board[y][x].get();
}


// -------------------- SET PIECE --------------------

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


// -------------------- REMOVE PIECE --------------------

std::unique_ptr<Piece> Board::removePiece(
    int x,
    int y
) {

    if (!isInside(x, y)) {
        return nullptr;
    }

    return std::move(board[y][x]);
}


// -------------------- INSIDE BOARD --------------------

bool Board::isInside(int x, int y) const {

    return x >= 0 &&
           x < BOARD_SIZE &&
           y >= 0 &&
           y < BOARD_SIZE;
}


// -------------------- PATH CLEAR --------------------

bool Board::isPathClear(
    int startX,
    int startY,
    int endX,
    int endY
) const {

    int dx = endX - startX;
    int dy = endY - startY;

    // Determine direction of movement
    int stepX =
        (dx == 0)
            ? 0
            : (dx > 0 ? 1 : -1);

    int stepY =
        (dy == 0)
            ? 0
            : (dy > 0 ? 1 : -1);

    int currentX = startX + stepX;
    int currentY = startY + stepY;

    // Check every square between start and end
    while (currentX != endX ||
           currentY != endY) {

        if (getPiece(
                currentX,
                currentY
            ) != nullptr) {

            return false;
        }

        currentX += stepX;
        currentY += stepY;
    }

    return true;
}


// =====================================================
//                    CHECK DETECTION
// =====================================================

bool Board::isInCheck(bool white) const {

    int kingX = -1;
    int kingY = -1;


    // -------------------------------------------------
    // Find the King
    // -------------------------------------------------

    for (int y = 0; y < BOARD_SIZE; y++) {

        for (int x = 0; x < BOARD_SIZE; x++) {

            Piece* piece =
                getPiece(x, y);

            if (piece != nullptr &&
                piece->getType() ==
                    PieceType::KING &&
                piece->isWhite() == white) {

                kingX = x;
                kingY = y;

                break;
            }
        }

        if (kingX != -1) {
            break;
        }
    }


    // King not found
    if (kingX == -1) {
        return false;
    }


    // -------------------------------------------------
    // Look for opponent attacking the King
    // -------------------------------------------------

    for (int y = 0; y < BOARD_SIZE; y++) {

        for (int x = 0; x < BOARD_SIZE; x++) {

            Piece* piece =
                getPiece(x, y);

            if (piece == nullptr) {
                continue;
            }

            // Ignore pieces belonging to same player
            if (piece->isWhite() == white) {
                continue;
            }

            // Can opponent piece attack King?
            if (piece->canMoveTo(
                    kingX,
                    kingY,
                    *this)) {

                return true;
            }
        }
    }

    return false;
}


// =====================================================
//                     MOVE PIECE
// =====================================================

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


    // -------------------------------------------------
    // Check whether piece can make this move
    // -------------------------------------------------

    if (!piece->canMoveTo(
            endX,
            endY,
            *this)) {

        return false;
    }


    // -------------------------------------------------
    // Save destination piece
    // -------------------------------------------------

    auto capturedPiece =
        removePiece(endX, endY);


    // -------------------------------------------------
    // Remove moving piece
    // -------------------------------------------------

    auto movingPiece =
        removePiece(startX, startY);


    // -------------------------------------------------
    // Remember Pawn state
    // -------------------------------------------------

    bool wasPawn = false;
    bool pawnHadMoved = false;

    Pawn* pawn = nullptr;

    if (movingPiece->getType() ==
        PieceType::PAWN) {

        wasPawn = true;

        pawn =
            dynamic_cast<Pawn*>(
                movingPiece.get()
            );

        if (pawn != nullptr) {
            pawnHadMoved =
                pawn->getHasMoved();
        }
    }


    // -------------------------------------------------
    // Temporarily make the move
    // -------------------------------------------------

    movingPiece->setPosition(
        endX,
        endY
    );

    if (pawn != nullptr) {
        pawn->setHasMoved(true);
    }

    setPiece(
        endX,
        endY,
        std::move(movingPiece)
    );


    // -------------------------------------------------
    // Check if our King is now in check
    // -------------------------------------------------

    Piece* movedPiece =
        getPiece(endX, endY);

    bool ownKingInCheck =
        isInCheck(
            movedPiece->isWhite()
        );


    // -------------------------------------------------
    // Illegal move
    // -------------------------------------------------

    if (ownKingInCheck) {

        // Remove temporarily moved piece
        auto restoredPiece =
            removePiece(
                endX,
                endY
            );

        // Put it back at original position
        restoredPiece->setPosition(
            startX,
            startY
        );

        // Restore Pawn state
        if (wasPawn) {

            Pawn* restoredPawn =
                dynamic_cast<Pawn*>(
                    restoredPiece.get()
                );

            if (restoredPawn != nullptr) {

                restoredPawn->setHasMoved(
                    pawnHadMoved
                );
            }
        }

        setPiece(
            startX,
            startY,
            std::move(restoredPiece)
        );


        // Restore captured piece
        setPiece(
            endX,
            endY,
            std::move(capturedPiece)
        );

        return false;
    }


    // -------------------------------------------------
    // Move is legal
    // -------------------------------------------------

    return true;
}


// =====================================================
//                     DISPLAY
// =====================================================

void Board::display() const {

    std::cout << "\n";

    std::cout
        << "   a b c d e f g h\n";

    std::cout
        << "  -----------------\n";


    // Display rank 8 → 1
    for (int y = BOARD_SIZE - 1;
         y >= 0;
         y--) {

        std::cout
            << y + 1
            << "| ";

        for (int x = 0;
             x < BOARD_SIZE;
             x++) {

            Piece* piece =
                getPiece(x, y);

            if (piece == nullptr) {

                std::cout << ". ";

            } else {

                std::cout
                    << piece->getSymbol()
                    << " ";
            }
        }

        std::cout
            << "| "
            << y + 1
            << "\n";
    }


    std::cout
        << "  -----------------\n";

    std::cout
        << "   a b c d e f g h\n\n";
}