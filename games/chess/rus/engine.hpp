#ifndef RUS_ENGINE_HPP
#define RUS_ENGINE_HPP

#include <vector>

#include "board.hpp"
#include "move.hpp"
#include "state.hpp"
#include "engine_options.hpp"

namespace rus {

    class Engine {
    public:
        enum Ray_direction {
            nort,
            nort_east,
            sout_east,
            east,
            sout,
            nort_west,
            sout_west,
            west,

            ray_direction_size,
        };

        State state;

        int id_depth;

        enum Castling_enum {
            queen_side,
            king_side,

            castling_enum_size,
        };

        void initialize(const Engine_options & options);
        void update(const std::string & san);

        State apply_move(const State & state, const Move move) const;

        std::vector<Move> filter_out_check(const State state, std::vector<Move> moves) const;

        std::vector<Move> piece_moves(const State state, const Position::Type sq) const;
        std::vector<Move> valid_piece_moves(const State state, const Position::Type sq) const;
        std::vector<Move> moves(const State state) const;
        std::vector<Move> valid_moves(const State state) const;

        board::Board pawn_moves1(const Player::Type player, const board::Board pawns, const board::Board empty) const;
        board::Board pawn_moves2(const Player::Type player, const board::Board pawns, const board::Board empty) const;
        board::Board pawn_attacks(const Player::Type player, const board::Board pawns) const;
        board::Board pawn_captures(const Player::Type player, const board::Board pawns, const board::Board any_enemy) const;
        board::Board knight_moves(board::Board knights, board::Board any_friendly) const;
        board::Board rook_moves(board::Board rooks, const board::Board any_friendly, const board::Board any_enemy) const;
        board::Board bishop_moves(board::Board bishops, const board::Board any_friendly, const board::Board any_enemy) const;
        board::Board queen_moves(board::Board queens, const board::Board any_friendly, const board::Board any_enemy) const;
        board::Board king_moves(board::Board kings, board::Board any_friendly) const;

    private:
        board::Board knight_tbl[Position::size];
        board::Board rays_tbl[ray_direction_size][Position::size];
        board::Board king_tbl[Position::size];

        void initialize_fen_piece_placement(const std::string & str);
        void initialize_fen_side_to_move(const std::string & str);
        void initialize_fen_castling_ability(const std::string & str);
        void initialize_fen_ep_target(const std::string & str);

        board::Board calc_knight_moves(const board::Board knights);
        board::Board calc_ray(const int sq, const int dir);
        board::Board calc_king_moves(board::Board kings);
        board::Board calc_pos_ray(int sq, const int dir, board::Board occupied) const;
        board::Board calc_neg_ray(int sq, const int dir, board::Board occupied) const;

        void knight_pre_process();
        void ray_pre_process();
        void king_pre_process();

    };

}

#endif