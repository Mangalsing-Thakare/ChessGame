#include "ChessGame.h"
#include <iostream>


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

    // Files: a-h
    if (file < 'a' || file > 'h') {
        return false;
    }

    // Ranks: 1-8
    if (rank < '1' || rank > '8') {
        return false;
    }

    // Convert chess notation to array coordinates
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

    // No piece at starting position
    if (piece == nullptr) {

        std::cout
            << "No piece at that position.\n";

        return false;
    }


    // Check whether the correct player is moving
    if (piece->isWhite() != whiteTurn) {

        std::cout
            << "It is "
            << (whiteTurn ? "White" : "Black")
            << "'s turn.\n";

        return false;
    }


    // Ask Board to perform the move
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

        // Display current board
        board.display();


        // Display current player
        std::cout
            << (whiteTurn ? "White" : "Black")
            << "'s turn > ";


        std::string from;
        std::string to;

        std::cin >> from;


        // Exit game
        if (from == "quit") {
            break;
        }


        std::cin >> to;


        int startX;
        int startY;
        int endX;
        int endY;


        // Convert positions such as e2 -> x,y
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


        // Try to make the move
        if (!makeMove(
                startX,
                startY,
                endX,
                endY)) {

            continue;
        }


        // =============================================
        // Move was successful
        // =============================================

        // Change turn
        whiteTurn = !whiteTurn;


        // =============================================
        // Checkmate
        // =============================================

        if (board.isCheckmate(whiteTurn)) {

            board.display();

            std::cout
                << "CHECKMATE! "
                << (whiteTurn ? "White" : "Black")
                << " loses.\n";

            break;
        }


        // =============================================
        // Stalemate
        // =============================================

        if (board.isStalemate(whiteTurn)) {

            board.display();

            std::cout
                << "STALEMATE! "
                << "Game is a draw.\n";

            break;
        }


        // =============================================
        // Check
        // =============================================

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