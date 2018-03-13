#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
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
    //return doHeuristicMove(opponentsMove, msLeft);
    //if (opponentsMove != nullptr)
        //std::cerr << "OPPONENT DOES "<< opponentsMove->x << " " << opponentsMove->y << std::endl;
    game->doMove(opponentsMove, opp_side);
    if (! game->hasMoves(curr_side))
        return nullptr;
    Move *move;
    Board *temp_board = game->copy();
    int best_x = 0;
    int best_y = 0;
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
                move_score = moveScore(temp_board, move, curr_side);
                //temp_board->doMove(move, curr_side);
                //Move *opp_move = findBestMove(temp_board, opp_side);
                //temp_board->doMove(opp_move, opp_side);
                //move_score = temp_board->count(curr_side)-game->count(curr_side);
                //std::cerr << "score would be: " << move_score << std::endl;
                if (move_score > curr_max)
                {
                    curr_max = move_score;
                    best_x = i;
                    best_y = j;
                }
            }
            delete move;
            delete temp_board;
        }
    }
    move = new Move(best_x, best_y);
    game->doMove(move, curr_side);
    return move;
}

//find best move based on immediate score
Move *Player::findBestMove(Board *curr_board, Side s)
{
    Move *move;
    int best_x=0;
    int best_y=0;
    Board *temp_board = curr_board->copy();
    int move_score = 0;
    int curr_max = temp_board->count(s) - curr_board->count(s);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            temp_board = curr_board->copy();
            move = new Move(i,j);
            if (temp_board->checkMove(move, s))
            {
                temp_board->doMove(move, s);
                move_score = temp_board->count(s) - curr_board->count(s);
                if (move_score > curr_max)
                {
                    curr_max = move_score;
                    best_x = i;
                    best_y = j;
                }
            }
            //delete temp_board;
            delete move;
        }
    }
    move = new Move(best_x, best_y);
    return move;
}

//gives the overall score of a move accounting for all implemented factors
int Player::moveScore(Board *board, Move *move, Side s)
{
    int score = 0;
    if(!board->checkMove(move, s))
    {
        return -9999;
    }
    else
    {
        if ((move->getX()==0 && (move->getY()==0 || move->getY()==7)) || (move->getX()==7 && (move->getY()==0 || move->getY()==7)))
        {
            score = score + 20;
        }
        else if ((move->getX()<2 && (move->getY()<2 || move->getY()>5)) || (move->getX()>5 && (move->getY()<2 || move->getY()>5)))
        {
            score = score - 15;
        }
        else if(move->getY()==0 || move->getY()==7 || move->getX()==0 || move->getX()==7)
        {
            score = score + 5;
        }
        score = score - mobilityScore(board, move, curr_side, opp_side) + minmaxScore(board, move, curr_side, opp_side);
        return score;
    }
}

//checks the worst case from doing 'move' when assumed the opposite side
//only looks at the current board to decide its best move (no depth)
int Player::minmaxScore(Board *board, Move *move, Side mover, Side opp)
{
    Board *temp_board = board->copy();
    if (!temp_board->checkMove(move, mover))
        return -9999;
    else
    {
        temp_board->doMove(move, mover);
        Move *best_opp_move = findBestMove(temp_board, opp);
        temp_board->doMove(best_opp_move, opp);
        return temp_board->count(mover)-board->count(mover);
    }
}

//checks number of available moves side "opp" has after side "mover" does *move and returns
//returns -1 if move not valid
int Player::mobilityScore(Board *board, Move *move, Side mover, Side opp)
{
    int score = 0;
    Board *temp_board = board->copy();
    if (!temp_board->checkMove(move, mover))
    {
        return -9999;
    }
    else
    {
        Move *testMove;
        temp_board->doMove(move, mover);
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                testMove = new Move(i,j);
                if(temp_board->checkMove(testMove, opp))
                {
                    score++;
                }
                delete testMove;
            }
        }
    }
    return score;
}

//do move based on immediate score
Move *Player::doHeuristicMove(Move *opponentsMove, int msLeft) {
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
                move_score = temp_board->count(curr_side)-game->count(curr_side);
                if (move_score > curr_max)
                {
                    curr_max = move_score;
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
