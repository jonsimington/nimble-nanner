// AI
// This is where you build your AI

#include "ai.hpp"
#include "rus/rus.hpp"

// You can add #includes here for your AI.
//#include "rus/rus.hpp"

namespace cpp_client
{

namespace chess
{

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // REPLACE WITH YOUR TEAM NAME!
    return "Russley Shaw";
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // This is a good place to initialize any variables
    srand(time(NULL));

    rus::knight::pre_process();
    rus::bishop::pre_process();
    rus::rook::pre_process();
    rus::queen::pre_process();
    rus::king::pre_process();

}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // If a function you call triggers an update this will be called before it returns.
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    // You can do any cleanup of your AI here.  The program ends when this function returns.
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
//    // Here is where you'll want to code your AI.
//
//    // We've provided sample code that:
//    //    1) prints the board to the console
//    //    2) prints the opponent's last move to the console
//    //    3) prints how much time remaining this AI has to calculate moves
//    //    4) makes a random (and probably invalid) move.
//
    // 1) print the board to the console
    print_current_board();

    // 2) print the opponent's last move to the console
    if(game->moves.size() > 0) {
        std::cout << "Opponent's Last Move: '" << game->moves[game->moves.size() - 1]->san << "'" << std::endl;
    }

    // 3) print how much time remaining this AI has to calculate moves
    std::cout << "Time Remaining: " << player->time_remaining << " ns" << std::endl;

    // Build current chess state
    rus::State_helper state;
    state.construct_from(this);

    rus::Player_enum me = this->player->color == "White" ? rus::white_idx : rus::black_idx;

    std::cout << "Chosing move..." << std::endl;

    auto player_moves = state.playerMoves(me);

    auto& move = player_moves[rand() % player_moves.size()];

    int moveRank;
    std::string moveFile;

    rus::board::rankFileFromIdx(move.to, moveRank, moveFile);

    auto toBB = rus::board::from_idx(move.to);

    auto piece = this->getPieceByIdx(move.from);

    if((toBB & rus::board::rank18) && move.piece == rus::pawn_idx) {
        piece->move(moveFile, moveRank, "Queen");
    }
    else {
        piece->move(moveFile, moveRank);
    }

    return true; // to signify we are done with our turn.
}

/// <summary>
///  Prints the current board using pretty ASCII art
/// </summary>
/// <remarks>
/// Note: you can delete this function if you wish
/// </remarks>
void AI::print_current_board()
{
    for(int rank = 9; rank >= -1; rank--)
    {
        std::string str = "";
        if(rank == 9 || rank == 0) // then the top or bottom of the board
        {
            str = "   +------------------------+";
        }
        else if(rank == -1) // then show the ranks
        {
            str = "     a  b  c  d  e  f  g  h";
        }
        else // board
        {
            str += " ";
            str += std::to_string(rank);
            str += " |";
            // fill in all the files with pieces at the current rank
            for(int file_offset = 0; file_offset < 8; file_offset++)
            {
                std::string file(1, 'a' + file_offset); // start at a, with with file offset increasing the char;
                chess::Piece current_piece = nullptr;
                for(const auto& piece : game->pieces)
                {
                    if(piece->file == file && piece->rank == rank) // then we found the piece at (file, rank)
                    {
                        current_piece = piece;
                        break;
                    }
                }

                char code = '.'; // default "no piece";
                if(current_piece != nullptr)
                {
                    code = current_piece->type[0];

                    if(current_piece->type == "Knight") // 'K' is for "King", we use 'N' for "Knights"
                    {
                        code = 'N';
                    }

                    if(current_piece->owner->id == "1") // the second player (black) is lower case. Otherwise it's upppercase already
                    {
                        code = tolower(code);
                    }
                }

                str += " ";
                str += code;
                str += " ";
            }

            str += "|";
        }

        std::cout << str << std::endl;
    }
}

// You can add additional methods here for your AI to call
Piece AI::getPieceByIdx(const int idx) {
    int rank;
    std::string file;

    rus::board::rankFileFromIdx(idx, rank, file);

    for(auto& p: this->game->pieces) {
        if(p->rank == rank && p->file == file) {
            return p;
        }
    }
}

} // chess

} // cpp_client
