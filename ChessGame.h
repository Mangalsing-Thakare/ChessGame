#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Board.h"

class ChessGame {
private:
    Board board;
    bool whiteTurn;

public:
    ChessGame();

    void play();

private:
    bool makeMove(
        int startX,
        int startY,
        int endX,
        int endY
    );

    bool parsePosition(
        const std::string& position,
        int& x,
        int& y
    ) const;
};

#endif