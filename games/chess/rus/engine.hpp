#ifndef RUS_ENGINE_HPP
#define RUS_ENGINE_HPP

#include <cstdint>

#include <vector>
#include <limits>

#include "board.hpp"
#include "move.hpp"
#include "state.hpp"
#include "engine_options.hpp"

namespace rus {

    struct Minimax_node {
        Move move;
        int value;
    };

    class Engine {
    public:

        Engine();

        Player me;
        State current_state;

        int id_depth;

        void initialize(const Engine_options & options);

        void update(const Move move);
        State apply_move(const State & state, const Move move) const;

        std::vector<Move> filter_out_check(const State state, const std::vector<Move> & moves) const;

        Minimax_node id_minimax(const State state, const bool maximize) {
            Minimax_node best;
            for(int i = 1; i <= id_depth; i++) {
                best = minimax(state, i, maximize);
            }
            return best;
        }

        Minimax_node minimax(const State state, const int depth, const bool maximize) {

            const int checkmate_val = 1000000;
            const int draw_val = -1000;

            Minimax_node best;

            // if depth = 0 or node is a terminal node
            if(depth == 0) {
                // return the heuristic value of node
                best.value = state.heuristic();
                best.move = state.last_moves.back();
                return best;
            }

            if(state.is_draw()) {
                best.value = -1;
                best.move = state.last_moves.back();
            }

            auto moves = valid_moves(state);
            if(moves.size() == 0) {
                best.value = checkmate_val;
                best.move = state.last_moves.back();
            }

            std::cout << "Eval at depth: " << depth << std::endl;

            // Print out moves for depth
            std::cout << "    Valid moves: ";
            for(auto& move: moves) { std::cout << move << " "; }
            std::cout << std::endl;

            best.value = (maximize ? std::numeric_limits<int>::lowest() : std::numeric_limits<int>::max());
            for(auto& move: moves) {
                auto child = apply_move(state, move);
                auto v = minimax(child, depth-1, !maximize);

                if(maximize ? (v.value > best.value) : (v.value < best.value)) {
                    best.value = v.value;
                    best.move = move;
                }
            }
            return best;
        }

        Move move_from_framework(const State state,
                                 const std::string& from_file, const int& from_rank,
                                 const std::string& to_file, const int& to_rank,
                                 const std::string& promotion) const;
        bool is_in_check(const State state) const;
        std::vector<Move> piece_moves(const State state, const Position sq) const;
        std::vector<Move> valid_piece_moves(const State state, const Position sq) const;
        std::vector<Move> moves(const State state) const;
        std::vector<Move> valid_moves(const State state) const;

        board::Board pawn_moves1(const Player player, const board::Board pawns, const board::Board empty) const;
        board::Board pawn_moves2(const Player player, const board::Board pawns, const board::Board empty) const;
        board::Board pawn_attacks(const Player player, const board::Board pawns) const;
        board::Board pawn_captures(const Player player, const board::Board pawns, const board::Board any_enemy) const;
        board::Board knight_moves(board::Board knights, board::Board any_friendly) const;
        board::Board rook_moves(board::Board rooks, const board::Board any_friendly, const board::Board any_enemy) const;
        board::Board bishop_moves(board::Board bishops, const board::Board any_friendly, const board::Board any_enemy) const;
        board::Board queen_moves(board::Board queens, const board::Board any_friendly, const board::Board any_enemy) const;
        board::Board king_moves(board::Board kings, board::Board any_friendly) const;

    private:
        board::Board knight_tbl[Position_size];
        board::Board rays_tbl[Ray_size][Position_size];
        board::Board king_tbl[Position_size];

        void initialize_fen_piece_placement(const std::string & str);
        void initialize_fen_side_to_move(const std::string & str);
        void initialize_fen_castling_ability(const std::string & str);
        void initialize_fen_ep_target(const std::string & str);

        board::Board calc_knight_moves(const board::Board knights);
        board::Board calc_ray(const Position sq, const Ray dir);
        board::Board calc_king_moves(board::Board kings);
        board::Board calc_pos_ray(const Position sq, const Ray dir, board::Board occupied) const;
        board::Board calc_neg_ray(const Position sq, const Ray dir, board::Board occupied) const;

        void knight_pre_process();
        void ray_pre_process();
        void king_pre_process();

    };

}

#endif