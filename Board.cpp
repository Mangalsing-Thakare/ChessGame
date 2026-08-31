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

    int direction = white ? 1 : -1;

    Piece* destination =
        board.getPiece(newX, newY);

    // One square forward
    if (dx == 0 && dy == direction) {
        return destination == nullptr;
    }

    // Two squares forward on first move
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


// -------------------- KNIGHT --------------------

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

    Piece* destination =
        board.getPiece(newX, newY);

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

    if (dx == 0 || dx != dy) {
        return false;
    }

    Piece* destination =
        board.getPiece(newX, newY);

    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    return board.isPathClear(
        x,
        y,
        newX,
        newY
    );
}


// -------------------- ROOK --------------------

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

    Piece* destination =
        board.getPiece(newX, newY);

    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    return board.isPathClear(
        x,
        y,
        newX,
        newY
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

    bool straight =
        (newX == x || newY == y);

    bool diagonal =
        (dx == dy && dx != 0);

    if (!straight && !diagonal) {
        return false;
    }

    Piece* destination =
        board.getPiece(newX, newY);

    if (destination != nullptr &&
        destination->isWhite() == white) {
        return false;
    }

    return board.isPathClear(
        x,
        y,
        newX,
        newY
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

    if (dx > 1 ||
        dy > 1 ||
        (dx == 0 && dy == 0)) {
        return false;
    }

    Piece* destination =
        board.getPiece(newX, newY);

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

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = nullptr;
        }
    }


    // ---------------- WHITE ----------------

    for (int x = 0; x < BOARD_SIZE; x++) {
        board[1][x] =
            std::make_unique<Pawn>(
                true, x, 1
            );
    }

    board[0][0] =
        std::make_unique<Rook>(
            true, 0, 0
        );

    board[0][7] =
        std::make_unique<Rook>(
            true, 7, 0
        );

    board[0][1] =
        std::make_unique<Knight>(
            true, 1, 0
        );

    board[0][6] =
        std::make_unique<Knight>(
            true, 6, 0
        );

    board[0][2] =
        std::make_unique<Bishop>(
            true, 2, 0
        );

    board[0][5] =
        std::make_unique<Bishop>(
            true, 5, 0
        );

    board[0][3] =
        std::make_unique<Queen>(
            true, 3, 0
        );

    board[0][4] =
        std::make_unique<King>(
            true, 4, 0
        );


    // ---------------- BLACK ----------------

    for (int x = 0; x < BOARD_SIZE; x++) {
        board[6][x] =
            std::make_unique<Pawn>(
                false, x, 6
            );
    }

    board[7][0] =
        std::make_unique<Rook>(
            false, 0, 7
        );

    board[7][7] =
        std::make_unique<Rook>(
            false, 7, 7
        );

    board[7][1] =
        std::make_unique<Knight>(
            false, 1, 7
        );

    board[7][6] =
        std::make_unique<Knight>(
            false, 6, 7
        );

    board[7][2] =
        std::make_unique<Bishop>(
            false, 2, 7
        );

    board[7][5] =
        std::make_unique<Bishop>(
            false, 5, 7
        );

    board[7][3] =
        std::make_unique<Queen>(
            false, 3, 7
        );

    board[7][4] =
        std::make_unique<King>(
            false, 4, 7
        );
}


// -------------------- GET PIECE --------------------

Piece* Board::getPiece(
    int x,
    int y
) const {

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

bool Board::isInside(
    int x,
    int y
) const {

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


    // Find the King
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


    if (kingX == -1) {
        return false;
    }


    // Check opponent pieces
    for (int y = 0; y < BOARD_SIZE; y++) {

        for (int x = 0; x < BOARD_SIZE; x++) {

            Piece* piece =
                getPiece(x, y);

            if (piece == nullptr) {
                continue;
            }

            if (piece->isWhite() == white) {
                continue;
            }

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
//                  LEGAL MOVE CHECK
// =====================================================

bool Board::hasLegalMove(bool white) const {

    for (int startY = 0;
         startY < BOARD_SIZE;
         startY++) {

        for (int startX = 0;
             startX < BOARD_SIZE;
             startX++) {

            Piece* piece =
                getPiece(startX, startY);

            if (piece == nullptr ||
                piece->isWhite() != white) {
                continue;
            }


            for (int endY = 0;
                 endY < BOARD_SIZE;
                 endY++) {

                for (int endX = 0;
                     endX < BOARD_SIZE;
                     endX++) {

                    if (!piece->canMoveTo(
                            endX,
                            endY,
                            *this)) {
                        continue;
                    }


                    Piece* destination =
                        getPiece(endX, endY);

                    if (destination != nullptr &&
                        destination->isWhite() == white) {
                        continue;
                    }


                    Board* mutableBoard =
                        const_cast<Board*>(this);


                    auto capturedPiece =
                        mutableBoard->removePiece(
                            endX,
                            endY
                        );

                    auto movingPiece =
                        mutableBoard->removePiece(
                            startX,
                            startY
                        );


                    Pawn* pawn = nullptr;
                    bool pawnHadMoved = false;

                    if (movingPiece->getType() ==
                        PieceType::PAWN) {

                        pawn =
                            dynamic_cast<Pawn*>(
                                movingPiece.get()
                            );

                        if (pawn != nullptr) {
                            pawnHadMoved =
                                pawn->getHasMoved();

                            pawn->setHasMoved(true);
                        }
                    }


                    movingPiece->setPosition(
                        endX,
                        endY
                    );


                    mutableBoard->setPiece(
                        endX,
                        endY,
                        std::move(movingPiece)
                    );


                    bool stillInCheck =
                        isInCheck(white);


                    auto restoredPiece =
                        mutableBoard->removePiece(
                            endX,
                            endY
                        );


                    restoredPiece->setPosition(
                        startX,
                        startY
                    );


                    if (pawn != nullptr) {

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


                    mutableBoard->setPiece(
                        startX,
                        startY,
                        std::move(restoredPiece)
                    );


                    mutableBoard->setPiece(
                        endX,
                        endY,
                        std::move(capturedPiece)
                    );


                    if (!stillInCheck) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


// =====================================================
//                    CHECKMATE
// =====================================================

bool Board::isCheckmate(bool white) const {

    return isInCheck(white) &&
           !hasLegalMove(white);
}


// =====================================================
//                    STALEMATE
// =====================================================

bool Board::isStalemate(bool white) const {

    return !isInCheck(white) &&
           !hasLegalMove(white);
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


    if (!piece->canMoveTo(
            endX,
            endY,
            *this)) {

        return false;
    }


    auto capturedPiece =
        removePiece(endX, endY);

    auto movingPiece =
        removePiece(startX, startY);


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

            pawn->setHasMoved(true);
        }
    }


    bool wasKing = false;
    bool kingHadMoved = false;

    King* king = nullptr;


    if (movingPiece->getType() ==
        PieceType::KING) {

        wasKing = true;

        king =
            dynamic_cast<King*>(
                movingPiece.get()
            );

        if (king != nullptr) {

            kingHadMoved =
                king->getHasMoved();

            king->setHasMoved(true);
        }
    }


    bool wasRook = false;
    bool rookHadMoved = false;

    Rook* rook = nullptr;


    if (movingPiece->getType() ==
        PieceType::ROOK) {

        wasRook = true;

        rook =
            dynamic_cast<Rook*>(
                movingPiece.get()
            );

        if (rook != nullptr) {

            rookHadMoved =
                rook->getHasMoved();

            rook->setHasMoved(true);
        }
    }


    movingPiece->setPosition(
        endX,
        endY
    );


    setPiece(
        endX,
        endY,
        std::move(movingPiece)
    );


    Piece* movedPiece =
        getPiece(endX, endY);


    bool ownKingInCheck =
        isInCheck(
            movedPiece->isWhite()
        );


    if (ownKingInCheck) {

        auto restoredPiece =
            removePiece(
                endX,
                endY
            );


        restoredPiece->setPosition(
            startX,
            startY
        );


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


        if (wasKing) {

            King* restoredKing =
                dynamic_cast<King*>(
                    restoredPiece.get()
                );

            if (restoredKing != nullptr) {

                restoredKing->setHasMoved(
                    kingHadMoved
                );
            }
        }


        if (wasRook) {

            Rook* restoredRook =
                dynamic_cast<Rook*>(
                    restoredPiece.get()
                );

            if (restoredRook != nullptr) {

                restoredRook->setHasMoved(
                    rookHadMoved
                );
            }
        }


        setPiece(
            startX,
            startY,
            std::move(restoredPiece)
        );


        setPiece(
            endX,
            endY,
            std::move(capturedPiece)
        );


        return false;
    }

    // =====================================================
    //                  PAWN PROMOTION
    // =====================================================

    movedPiece = getPiece(endX, endY);

if (movedPiece != nullptr &&
    movedPiece->getType() == PieceType::PAWN) {

    bool white = movedPiece->isWhite();

    // White reaches rank 8
    // Black reaches rank 1
    if ((white && endY == 7) ||
        (!white && endY == 0)) {

        // Remove the pawn
        auto pawn = removePiece(
            endX,
            endY
        );

        // Replace it with a Queen
        setPiece(
            endX,
            endY,
            std::make_unique<Queen>(
                white,
                endX,
                endY
            )
        );
    }
}

    return true;
}


// =====================================================
//                      CASTLING
// =====================================================

bool Board::castle(
    int kingY,
    bool kingSide
) {

    int kingX = 4;

    int rookX =
        kingSide ? 7 : 0;

    int kingDestinationX =
        kingSide ? 6 : 2;

    int rookDestinationX =
        kingSide ? 5 : 3;


    Piece* king =
        getPiece(kingX, kingY);

    Piece* rook =
        getPiece(rookX, kingY);


    // King and rook must exist
    if (king == nullptr ||
        rook == nullptr) {

        return false;
    }


    // Correct piece types
    if (king->getType() != PieceType::KING ||
        rook->getType() != PieceType::ROOK) {

        return false;
    }


    // Must have same color
    if (king->isWhite() != rook->isWhite()) {
        return false;
    }


    King* kingPiece =
        dynamic_cast<King*>(king);

    Rook* rookPiece =
        dynamic_cast<Rook*>(rook);


    if (kingPiece == nullptr ||
        rookPiece == nullptr) {

        return false;
    }


    // Neither can have moved
    if (kingPiece->getHasMoved() ||
        rookPiece->getHasMoved()) {

        return false;
    }


    // Squares between King and Rook
    int start =
        std::min(kingX, rookX) + 1;

    int end =
        std::max(kingX, rookX);


    for (int x = start; x < end; x++) {

        if (getPiece(x, kingY) != nullptr) {
            return false;
        }
    }


    // King cannot castle while in check
    if (isInCheck(king->isWhite())) {
        return false;
    }


    // Check square King passes through
    int direction =
        kingSide ? 1 : -1;

    int middleX =
        kingX + direction;


    auto kingObject =
        removePiece(
            kingX,
            kingY
        );


    kingObject->setPosition(
        middleX,
        kingY
    );


    setPiece(
        middleX,
        kingY,
        std::move(kingObject)
    );


    bool middleIsSafe =
        !isInCheck(
            king->isWhite()
        );


    auto restoredKing =
        removePiece(
            middleX,
            kingY
        );


    restoredKing->setPosition(
        kingX,
        kingY
    );


    setPiece(
        kingX,
        kingY,
        std::move(restoredKing)
    );


    if (!middleIsSafe) {
        return false;
    }


    // Move King
    auto finalKing =
        removePiece(
            kingX,
            kingY
        );


    finalKing->setPosition(
        kingDestinationX,
        kingY
    );


    King* finalKingPtr =
        dynamic_cast<King*>(
            finalKing.get()
        );


    if (finalKingPtr != nullptr) {
        finalKingPtr->setHasMoved(true);
    }


    setPiece(
        kingDestinationX,
        kingY,
        std::move(finalKing)
    );


    // Move Rook
    auto rookObject =
        removePiece(
            rookX,
            kingY
        );


    rookObject->setPosition(
        rookDestinationX,
        kingY
    );


    Rook* finalRookPtr =
        dynamic_cast<Rook*>(
            rookObject.get()
        );


    if (finalRookPtr != nullptr) {
        finalRookPtr->setHasMoved(true);
    }


    setPiece(
        rookDestinationX,
        kingY,
        std::move(rookObject)
    );


    return true;
}


// =====================================================
//                      DISPLAY
// =====================================================

void Board::display() const {

    std::cout << "\n";

    std::cout
        << "   a b c d e f g h\n";

    std::cout
        << "  -----------------\n";


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