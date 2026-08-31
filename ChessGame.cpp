#include "ChessGame.h"
#include <iostream>


ChessGame::ChessGame()
    : whiteTurn(true) {
}


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


bool ChessGame::makeMove(
    int startX,
    int startY,
    int endX,
    int endY
) {

    Piece* piece =
        board.getPiece(startX, startY);

    if (piece == nullptr) {
        std::cout
            << "No piece at that position.\n";
        return false;
    }

    if (piece->isWhite() != whiteTurn) {

        std::cout
            << "It is "
            << (whiteTurn ? "White" : "Black")
            << "'s turn.\n";

        return false;
    }

    if (!board.movePiece(
            startX,
            startY,
            endX,
            endY)) {

        std::cout << "Invalid move.\n";
        return false;
    }

    // Change turn
    whiteTurn = !whiteTurn;

    // Check the new player's king
    if (board.isInCheck(whiteTurn)) {

        std::cout
            << "CHECK! "
            << (whiteTurn ? "White" : "Black")
            << " king is in check.\n";
    }

    return true;
}


void ChessGame::play() {

    std::cout << "=========================\n";
    std::cout << "       CHESS GAME\n";
    std::cout << "=========================\n";

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

        makeMove(
            startX,
            startY,
            endX,
            endY
        );
    }

    std::cout << "Game ended.\n";
}