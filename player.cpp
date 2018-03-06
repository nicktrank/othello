#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* CHANGE 1 NICK TRANK */
    /* CHANGE 2 ED CHU*/
    curr_side = side; 
    if (curr_side == WHITE)
        opp_side = BLACK;
    else
        opp_side = WHITE;
    game = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete game;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    
    game->doMove(opponentsMove, opp_side);
    if (! game->hasMoves(curr_side))
        return nullptr;
    Move *move;
    int best_x;
    int best_y;
    Board *temp_board;
    int move_score = 0;
    int curr_max = -9999;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            temp_board = game->copy();
            move = new Move(i, j);
            if (temp_board->checkMove(move, curr_side))
            {
                temp_board->doMove(move, curr_side);
                move_score = temp_board->count(curr_side) - game->count(curr_side);
                /* ADD THESE LINES */
                if ((i == 0 && (j == 0 || j == 7)) || (i == 7 && (j == 0 && j == 7)))
                    move_score = move_score * 3;
                else if (i+j == 1 || i+j == 13 || (i == 0 && j == 6) || (i == 6 && j == 0) || (i == 7 && j == 1) || (i == 1 && j == 7))
                    move_score = move_score * -1;
                else if ((i == j && (i == 1 || i == 6)) || (i == 6 && j == 1) || (i == 1 && j == 6))
                    move_score = move_score * -2;
                else if (i == 0 || i == 7 || j == 0 || j == 7)
                    move_score = move_score * 2;
                /* END NEW PORTION */
                if (move_score > curr_max)
                {
                    best_x = i;
                    best_y = j;
                }
            }
            delete temp_board;
            delete move;
        }
    }
    move = new Move(best_x, best_y);
    game->doMove(move, curr_side);
    return move;
}
