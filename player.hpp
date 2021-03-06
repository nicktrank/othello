#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *doHeuristicMove(Move *opponentsMove, int msLeft);
    Move *findBestMove(Board *curr_board, Side s);
    int mobilityScore(Board *board, Move *move, Side mover, Side opp);
    int moveScore(Board *board, Move *move, Side mover);
    int minmaxScore(Board *board, Move *move, Side mover, Side opp);
    Side curr_side;
    Side opp_side;
    Board *game;
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
