#include "ChessGame.h"
#include <iostream>
#include <cmath>


ChessGame::ChessGame()
    : whiteTurn(true) {
}


// =====================================================
//                  PARSE POSITION
// =====================================================

bool ChessGame::parsePosition(
    const std::string& position,
    int& x,
    int& y
) const {

    if (position.length() != 2) {
        return false;
    }

    char file = position[0];
    char rank = position[1];

    if (file < 'a' || file > 'h') {
        return false;
    }

    if (rank < '1' || rank > '8') {
        return false;
    }

    x = file - 'a';
    y = rank - '1';

    return true;
}


// =====================================================
//                     MAKE MOVE
// =====================================================

bool ChessGame::makeMove(
    int startX,
    int startY,
    int endX,
    int endY
) {

    Piece* piece =
        board.getPiece(startX, startY);


    // No piece
    if (piece == nullptr) {

        std::cout
            << "No piece at that position.\n";

        return false;
    }


    // Wrong player's piece
    if (piece->isWhite() != whiteTurn) {

        std::cout
            << "It is "
            << (whiteTurn ? "White" : "Black")
            << "'s turn.\n";

        return false;
    }


    // =================================================
    //                    CASTLING
    // =================================================

    if (piece->getType() ==
            PieceType::KING &&

        startX == 4 &&

        startY == endY &&

        std::abs(endX - startX) == 2) {

        bool kingSide =
            endX > startX;


        if (board.castle(
                startY,
                kingSide)) {

            return true;
        }


        std::cout
            << "Invalid castling.\n";

        return false;
    }


    // =================================================
    //                  NORMAL MOVE
    // =================================================

    if (!board.movePiece(
            startX,
            startY,
            endX,
            endY)) {

        std::cout
            << "Invalid move.\n";

        return false;
    }


    return true;
}


// =====================================================
//                       PLAY
// =====================================================

void ChessGame::play() {

    std::cout
        << "=========================\n";

    std::cout
        << "       CHESS GAME\n";

    std::cout
        << "=========================\n";


    std::cout
        << "\nEnter moves like: e2 e4\n";

    std::cout
        << "Enter 'quit' to exit.\n";


    while (true) {

        board.display();


        std::cout
            << (whiteTurn ? "White" : "Black")
            << "'s turn > ";


        std::string from;
        std::string to;


        std::cin >> from;


        if (from == "quit") {
            break;
        }


        std::cin >> to;


        int startX;
        int startY;
        int endX;
        int endY;


        // Convert chess notation
        if (!parsePosition(
                from,
                startX,
                startY) ||

            !parsePosition(
                to,
                endX,
                endY)) {

            std::cout
                << "Invalid position.\n";

            continue;
        }


        // Try move
        if (!makeMove(
                startX,
                startY,
                endX,
                endY)) {

            continue;
        }


        // Change turn
        whiteTurn = !whiteTurn;


        // =================================================
        //                    CHECKMATE
        // =================================================

        if (board.isCheckmate(whiteTurn)) {

            board.display();

            std::cout
                << "CHECKMATE! "
                << (whiteTurn ? "White" : "Black")
                << " loses.\n";

            break;
        }


        // =================================================
        //                    STALEMATE
        // =================================================

        if (board.isStalemate(whiteTurn)) {

            board.display();

            std::cout
                << "STALEMATE! "
                << "Game is a draw.\n";

            break;
        }


        // =================================================
        //                       CHECK
        // =================================================

        if (board.isInCheck(whiteTurn)) {

            std::cout
                << "CHECK! "
                << (whiteTurn ? "White" : "Black")
                << " king is in check.\n";
        }
    }


    std::cout
        << "Game ended.\n";
}