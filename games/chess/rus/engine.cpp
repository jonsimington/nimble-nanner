#include <cassert>

#include <iostream>
#include <string>
#include <regex>

#include "engine.hpp"
#include "bit_scan.hpp"
#include "fill.hpp"
#include "position.hpp"

namespace rus {

    void Engine::initialize(const Engine_options & options) {

        id_depth = options.id_depth.value_or(3);

        std::string fen = options.fen.value_or("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        std::cout << "Initializing with fen: " << fen << "\n";

        std::regex re("(.*) (.*) (.*) (.*) (.*) (.*)");
        std::smatch m;
        std::regex_match(fen, m, re);

        assert(m.size() == 6+1); // 1 + num_fen_sections

        initialize_fen_piece_placement(m[1]);
        initialize_fen_side_to_move(m[2]);
        initialize_fen_castling_ability(m[3]);
        initialize_fen_ep_target(m[4]);

        this->knight_pre_process();
        this->ray_pre_process();
        this->king_pre_process();
    }

    void Engine::update(const std::string &san) {
        std::cout << "Updating with san: " << san << "\n";
    }

    State Engine::apply_move(const State &state, const Move move) const {
        auto new_state = state;
        auto player = state.player;
        auto enemy = Player::opponent(state.player);
        auto from_bb = board::position(move.from);
        auto to_bb = board::position(move.from);
        auto piece = state.find_piece_type(move.from);

        assert(from_bb & state.any_piece[player] > 0); // piece is owned by player



        switch(move.flag) {
            case Move::Flag::quiet: {

            } break;
        }

        new_state.pre_process();
        return new_state;
    }

    std::vector<Move> Engine::piece_moves(const State state, const Position::Type sq) const {
        std::vector<Move> moves;

        auto curr = board::position(sq);

        auto player = state.player;
        auto enemy = Player::opponent(state.player);

        assert(curr & state.any_piece[state.player] > 0); // assert square is valid and owned by current player

        auto type = Piece::size;
        for(int piece = 0; piece < Piece::size; piece++) {
            if(state.boards[state.player][piece] & curr) {
                type = piece;
                break;
            }
        }
        assert(type != Piece::size);

        switch(type) {
            case Piece::pawn: {
                auto bb = pawn_moves1(player, curr, state.empty);
                bit_scan::for_each_bit(bb, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::quiet};
                    moves.push_back(m);
                });
                bb = pawn_moves1(player, curr, state.empty);
                bit_scan::for_each_bit(bb, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::dbl_pawn};
                    moves.push_back(m);
                });
                bb = pawn_captures(player, curr, state.any_piece[enemy]);
                bit_scan::for_each_bit(bb, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::capture};
                    moves.push_back(m);
                });
            } break;

            case Piece::knight: {
                auto bb = knight_moves(curr, state.any_piece[player]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::quiet};
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::capture};
                    moves.push_back(m);
                });
            } break;

            case Piece::bishop: {
                auto bb = bishop_moves(curr, state.any_piece[player], state.any_piece[enemy]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::quiet};
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::capture};
                    moves.push_back(m);
                });
            } break;

            case Piece::rook: {
                auto bb = rook_moves(curr, state.any_piece[player], state.any_piece[enemy]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::quiet};
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::capture};
                    moves.push_back(m);
                });
            } break;

            case Piece::queen: {
                auto bb = queen_moves(curr, state.any_piece[player], state.any_piece[enemy]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::quiet};
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::capture};
                    moves.push_back(m);
                });
            } break;

            case Piece::king: {
                auto bb = king_moves(curr, state.any_piece[player]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::quiet};
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [this, &moves, &state](Position::Type to){
                    Move m{.from=sq, .to=to, .flag=Move::Flag::capture};
                    moves.push_back(m);
                });
            } break;

            default:
                assert(false); // Unknown piece

        }


        return moves;
    }

    std::vector<Move> Engine::valid_piece_moves(const State state, const Position::Type sq) const {
        auto moves = piece_moves(state, sq);
        return filter_out_check(state, moves);
    }

    std::vector<Move> Engine::moves(const State state) const {
        std::vector<Move> moves;

        bit_scan::for_each_bit(state.any_piece[state.player], [this, &moves, &state](Position::Type sq){
            auto ms = piece_moves(state, sq);
            moves.insert(moves.begin(), ms.begin(), ms.end());
        });

        return moves;
    }

    std::vector<Move> Engine::valid_moves(const State state) const {
        auto mvs = moves(state);
        return filter_out_check(state, mvs);
    }

    board::Board Engine::pawn_moves1(const Player::Type player, const board::Board pawns, const board::Board empty) const {
        if(player == Player::white) {
            return board::step_n(pawns) & empty;
        }
        else {
            return board::step_s(pawns) & empty;
        }
    }
    board::Board Engine::pawn_moves2(const Player::Type player, const board::Board pawns, const board::Board empty) const {
        board::Board m1 = pawn_moves1(player, pawns, empty);

        if(player == Player::white) {
            return board::step_n(m1) & empty & board::rank(4);
        }
        else {
            return board::step_s(m1) & empty & board::rank(5);
        }
    }
    board::Board Engine::pawn_attacks(const Player::Type player, const board::Board pawns) const {
        if(player == Player::white) {
            return board::step_ne(pawns) | board::step_nw(pawns);
        }
        else {
            return board::step_se(pawns) | board::step_sw(pawns);
        }
    }

    board::Board Engine::pawn_captures(const Player::Type player, const board::Board pawns, const board::Board any_enemy) const {
        return pawn_attacks(player, pawns) & any_enemy;
    }

    board::Board Engine::knight_moves(board::Board knights, board::Board any_friendly) const {
        board::Board ms = board::empty;
        if(knights) {
            do {
                int idx = bit_scan::scan_fwd(knights);
                ms |= knight_tbl[idx];
            } while(knights &= knights-1);
        }
        ms &= ~any_friendly;
        return ms;
    }

    board::Board Engine::rook_moves(board::Board rooks, const board::Board any_friendly, const board::Board any_enemy) const {
        board::Board occupied = any_friendly | any_enemy;
        board::Board mvs = board::empty;
        if(rooks) {
            do {
                int idx = bit_scan::scan_fwd(rooks);
                mvs |= calc_pos_ray(idx, nort, occupied);
                mvs |= calc_pos_ray(idx, east, occupied);
                mvs |= calc_neg_ray(idx, west, occupied);
                mvs |= calc_neg_ray(idx, sout, occupied);
            } while(rooks &= rooks-1);
        }
        mvs &= ~any_friendly;
        return mvs;
    }

    board::Board Engine::bishop_moves(board::Board bishops, const board::Board any_friendly, const board::Board any_enemy) const {
        board::Board occupied = any_friendly | any_enemy;
        board::Board mvs = board::empty;
        if(bishops) {
            do {
                int idx = bit_scan::scan_fwd(bishops);
                mvs |= calc_pos_ray(idx, nort_east, occupied);
                mvs |= calc_pos_ray(idx, nort_west, occupied);
                mvs |= calc_neg_ray(idx, sout_east, occupied);
                mvs |= calc_neg_ray(idx, sout_west, occupied);
            } while(bishops &= bishops-1);
        }
        mvs &= ~any_friendly;
        return mvs;
    }

    board::Board Engine::queen_moves(board::Board queens, const board::Board any_friendly, const board::Board any_enemy) const {
        board::Board occupied = any_friendly | any_enemy;
        board::Board mvs = board::empty;
        if(queens) {
            do {
                int idx = bit_scan::scan_fwd(queens);
                mvs |= calc_pos_ray(idx, nort, occupied);
                mvs |= calc_pos_ray(idx, east, occupied);
                mvs |= calc_neg_ray(idx, west, occupied);
                mvs |= calc_neg_ray(idx, sout, occupied);
                mvs |= calc_pos_ray(idx, nort_east, occupied);
                mvs |= calc_pos_ray(idx, nort_west, occupied);
                mvs |= calc_neg_ray(idx, sout_east, occupied);
                mvs |= calc_neg_ray(idx, sout_west, occupied);
            } while(queens &= queens-1);
        }
        mvs &= ~any_friendly;
        return mvs;
    }

    board::Board Engine::king_moves(board::Board kings, board::Board any_friendly) const {
        board::Board mvs = board::empty;
        if(kings) {
            do {
                int idx = bit_scan::scan_fwd(kings);
                mvs |= king_tbl[idx];
            } while(kings &= kings - 1);
        }
        mvs &= ~any_friendly;
        return mvs;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Private
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Engine::initialize_fen_piece_placement(const std::string &str) {
        std::regex re("(.*)\\/(.*)\\/(.*)\\/(.*)\\/(.*)\\/(.*)\\/(.*)\\/(.*)");
        std::smatch m;
        std::regex_match(str, m, re);

        assert(m.size() == 8 + 1); // 1 + num_ranks

        State state;
        state.clear();

        int current_rank = 7;
        while(current_rank >= 0) {
            std::string rank_str = m[7-current_rank];

            int current_idx = 0;
            int current_file = 0;
            while(current_file < 8) {
                char p = rank_str[current_idx];

                // Skip number
                if(p >= '1' && p <= '8') {
                    current_file += (p - '0');
                    current_idx++;
                    continue;
                }

                auto bb = board::position(current_rank * 8 + current_file);

                switch(p) {
                    // black
                    case 'p': state.boards[Player::black][Piece::pawn]     |= bb; break;
                    case 'n': state.boards[Player::black][Piece::knight]   |= bb; break;
                    case 'b': state.boards[Player::black][Piece::bishop]   |= bb; break;
                    case 'r': state.boards[Player::black][Piece::rook]     |= bb; break;
                    case 'q': state.boards[Player::black][Piece::queen]    |= bb; break;
                    case 'k': state.boards[Player::black][Piece::king]     |= bb; break;

                    // white
                    case 'P': state.boards[Player::white][Piece::pawn]     |= bb; break;
                    case 'N': state.boards[Player::white][Piece::knight]   |= bb; break;
                    case 'B': state.boards[Player::white][Piece::bishop]   |= bb; break;
                    case 'R': state.boards[Player::white][Piece::rook]     |= bb; break;
                    case 'Q': state.boards[Player::white][Piece::queen]    |= bb; break;
                    case 'K': state.boards[Player::white][Piece::king]     |= bb; break;
                    default:
                        assert(false); // Invalid syntax
                }

                current_idx++;
            }
            current_rank --;
        }
    }

    void Engine::initialize_fen_side_to_move(const std::string &str) {
        switch(str[0]) {
            case 'w': state.player = Player::white; break;
            case 'b': state.player = Player::black; break;
            default:
                assert(false); // Got invalid side to move
        }
    }

    void Engine::initialize_fen_castling_ability(const std::string &str) {
        state.castling_ability[Player::white][king_side]  = (str.find("K") != std::string::npos);
        state.castling_ability[Player::white][queen_side] = (str.find("Q") != std::string::npos);
        state.castling_ability[Player::black][king_side]  = (str.find("k") != std::string::npos);
        state.castling_ability[Player::black][queen_side] = (str.find("q") != std::string::npos);
    }

    void Engine::initialize_fen_ep_target(const std::string &str) {
        state.ep_able = true;
        if(str == "-") {
            state.ep_able = false;
            return;
        }

        state.ep_pos = Position::from_rf(str[1] - '0', str[0]);
    }

    board::Board Engine::calc_knight_moves(const board::Board knights) {
        board::Board l1 = (knights >> 1) & ~board::file('h');
        board::Board l2 = (knights >> 2) & ~board::file('g') & ~board::file('h');
        board::Board r1 = (knights << 1) & ~board::file('a');
        board::Board r2 = (knights << 2) & ~board::file('a') & ~board::file('b');
        board::Board h1 = l1 | r1;
        board::Board h2 = l2 | r2;
        return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
    }

    board::Board Engine::calc_ray(const int sq, const int dir) {
        board::Board me = board::Board(0x1) << sq;
        switch(dir) {
            case nort: return fill::nortFill(me) & ~me;
            case east: return fill::eastFill(me) & ~me;
            case sout: return fill::soutFill(me) & ~me;
            case west: return fill::westFill(me) & ~me;
            case nort_east: return fill::noEaFill(me) & ~me;
            case sout_east: return fill::soEaFill(me) & ~me;
            case nort_west: return fill::noWeFill(me) & ~me;
            case sout_west: return fill::soWeFill(me) & ~me;
            default: assert(false);
        }
        return 0x0;
    }

    board::Board Engine::calc_pos_ray(int sq, const int dir, board::Board occupied) const {
        assert(dir != sout);
        assert(dir != west);
        assert(dir != sout_east);
        assert(dir != sout_west);

        board::Board attacks = rays_tbl[dir][sq];
        board::Board blocker = attacks & occupied;
        if ( blocker ) {
            sq = bit_scan::scan_fwd(blocker);
            attacks ^= rays_tbl[dir][sq];
        }
        return attacks;
    }

    board::Board Engine::calc_neg_ray(int sq, const int dir, board::Board occupied) const {
        assert(dir != nort);
        assert(dir != east);
        assert(dir != nort_east);
        assert(dir != nort_west);

        board::Board attacks = rays_tbl[dir][sq];
        board::Board blocker = attacks & occupied;
        if ( blocker ) {
            sq = bit_scan::scan_rev(blocker);
            attacks ^= rays_tbl[dir][sq];
        }
        return attacks;
    }

    board::Board Engine::calc_king_moves(board::Board kings) {
        board::Board atks = board::step_e(kings) | board::step_w(kings);
        kings |= atks;
        atks |= board::step_n(kings) | board::step_s(kings);
        return atks;
    }

    void Engine::knight_pre_process() {
        board::Board bb = 0x1;
        for(int i = 0; i < 64; i++) {
            knight_tbl[i] = calc_knight_moves(bb);
            bb <<= 0x1;
        }
    }

    void Engine::ray_pre_process() {
        for (int i = 0; i < Position::size; i++) {
            for (int dir = 0; dir < ray_direction_size; dir++) {
                rays_tbl[dir][i] = calc_ray(i, dir);
            }
        }
    }

    void Engine::king_pre_process() {
        board::Board bb = 0x1;
        for(int i = 0; i < 64; i++) {
            king_tbl[i] = calc_king_moves(bb);
            bb <<= 0x1;
        }
    }

}