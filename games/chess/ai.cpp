// AI
// This is where you build your AI

#include "ai.hpp"

// You can add #includes here for your AI.
#include <string>
#include "rus/rus.hpp"

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
    return "Nimble Nanner";
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    srand(time(NULL));

    rus::Engine_options options;
    std::string str;

    options.fen = game->fen;
    if((str = get_setting("id_depth")) != "") {
        options.id_depth = rus::util::from_string<int>(str);
    }

    engine.initialize(options);
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
    print_current_board();

    // Update from opponent's move
    if(game->moves.size() > 0) {
        auto ai_move = game->moves.back();
        auto move = engine.move_from_framework(
                engine.current_state,
                ai_move->from_file, ai_move->from_rank,
                ai_move->to_file, ai_move->to_rank,
                ai_move->promotion
        );
        engine.update(move);
    }

    //auto all_moves = engine.valid_moves(engine.current_state);
    //std::cout << "All moves: " << all_moves.size() << std::endl;

    // Make random move
    // make_move(all_moves[rand() % all_moves.size()]);

    auto good_state = engine.id_minimax(engine.current_state, engine.current_state.player == rus::Player::white);
    make_move(good_state.move);

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
void AI::make_move(const rus::Move & move) {
    std::cout << move << std::endl;

    char from_file, to_file;
    int from_rank, to_rank;

    rus::fr_from_pos(move.from, from_file, from_rank);
    rus::fr_from_pos(move.to, to_file, to_rank);

    std::cout <<  "From " << from_file << from_rank << std::endl;
    std::cout <<  "To   " << to_file << to_rank << std::endl;
    for(auto& piece: game->pieces) {
        if(piece->file[0] == from_file && piece->rank == from_rank) {
            piece->move(std::string(1, to_file), to_rank);
            engine.update(move);
            return;
        }
    }
    std::cerr << "UNABLE TO MAKE MOVE" << std::endl;
}


} // chess

} // cpp_client
