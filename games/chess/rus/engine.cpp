#include <cassert>

#include <iostream>
#include <string>
#include <regex>

#include "engine.hpp"
#include "bit_scan.hpp"
#include "fill.hpp"
#include "enums.hpp"

namespace rus {

    Engine::Engine() {

    }

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
        auto player_e = state.player;
        auto player = to_i(state.player);
        auto enemy = to_i(!state.player);
        auto from_bb = board::position(move.from);
        auto to_bb = board::position(move.from);
        assert(from_bb & state.any_piece[player] > 0); // piece is owned by player

        const auto pawn_i = to_i(Piece::pawn);
        const auto rook_i = to_i(Piece::rook);
        const auto king_i = to_i(Piece::king);

        auto piece_e = new_state.find_piece_type(move.from);
        auto piece = to_i(piece_e);

        bool clear_ep = true;

        switch(move.flag) {
            case Move::Flag::quiet: {
                new_state.boards[player][piece] &= ~from_bb;
                new_state.boards[player][piece] |= to_bb;
            } break;

            case Move::Flag::dbl_pawn: {
                assert(piece_e == Piece::pawn);
                new_state.boards[player][pawn_i] &= ~from_bb;
                new_state.boards[player][pawn_i] |= to_bb;

                clear_ep = false;
                new_state.ep_able = true;
                new_state.ep_pos = move.to;
            } break;

            case Move::Flag::king_castle: {
                assert(piece_e == Piece::king);
                assert(new_state.castling_ability[player][to_i(Castle::king)]);
                auto rook_from_bb = board::position(player_e == Player::white ? Position::h1 : Position::h8);
                auto rook_to_bb = board::position(player_e == Player::white ? Position::f1 : Position::f8);
                assert(new_state.boards[player][to_i(Piece::rook)] & rook_from_bb > 0); // Assert rook is there

                new_state.boards[player][king_i] &= ~from_bb;
                new_state.boards[player][king_i] |= to_bb;

                new_state.boards[player][rook_i] &= ~rook_from_bb;
                new_state.boards[player][rook_i] |= rook_to_bb;

                new_state.castling_ability[player][to_i(Castle::king)] = false;
            } break;

            case Move::Flag::queen_castle: {
                assert(piece_e == Piece::king);
                assert(new_state.castling_ability[player][to_i(Castle::queen)]);
                auto rook_from_bb = board::position(player_e == Player::white ? Position::a1 : Position::a8);
                auto rook_to_bb = board::position(player_e == Player::white ? Position::d1 : Position::d8);
                assert(new_state.boards[player][rook_i] & rook_from_bb > 0); // Assert rook is there

                new_state.boards[player][king_i] &= ~from_bb;
                new_state.boards[player][king_i] |= to_bb;

                new_state.boards[player][rook_i] &= ~rook_from_bb;
                new_state.boards[player][rook_i] |= rook_to_bb;

                new_state.castling_ability[player][to_i(Castle::queen)] = false;
            } break;

            case Move::Flag::capture: {
                auto captured_type = to_i(new_state.find_piece_type(move.to));
                new_state.boards[player][piece] &= ~from_bb;
                new_state.boards[player][piece] |= to_bb;
                new_state.boards[enemy][captured_type] &= ~to_bb;
            } break;

            case Move::Flag::ep_capture: {
                assert(piece_e == Piece::pawn);
                assert(state.ep_able);
                auto captured_bb = board::position(state.ep_pos);
                assert(state.boards[enemy][pawn_i] & captured_bb > 0);

                new_state.boards[player][pawn_i] &= ~from_bb;
                new_state.boards[player][pawn_i] |= to_bb;
                new_state.boards[enemy][pawn_i] &= ~captured_bb;
            } break;

            case Move::Flag::knight_promo: {
                assert(piece_e == Piece::pawn);
                new_state.boards[player][piece] &= ~from_bb;
                new_state.boards[player][to_i(Piece::knight)] |= to_bb;
            } break;

            case Move::Flag::bishop_promo: {
                assert(piece_e == Piece::pawn);
                new_state.boards[player][piece] &= ~from_bb;
                new_state.boards[player][to_i(Piece::bishop)] |= to_bb;
            } break;

            case Move::Flag::rook_promo: {
                assert(piece_e == Piece::pawn);
                new_state.boards[player][piece] &= ~from_bb;
                new_state.boards[player][to_i(Piece::rook)] |= to_bb;
            } break;

            case Move::Flag::queen_promo: {
                assert(piece_e == Piece::pawn);
                new_state.boards[player][piece] &= ~from_bb;
                new_state.boards[player][to_i(Piece::queen)] |= to_bb;
            } break;

            default:
                assert(false); // Invalid flag
        }

        if(clear_ep) {
            new_state.ep_able = false;
            new_state.ep_pos = Position::a1;
        }

        new_state.pre_process();
        return new_state;
    }

    std::vector<Move> Engine::piece_moves(const State state, const Position sq) const {
        std::vector<Move> moves;

        auto curr = board::position(sq);

        auto player = to_i(state.player);
        auto player_e = state.player;
        auto enemy = to_i(!state.player);

        assert(curr & state.any_piece[player] > 0); // assert square is valid and owned by current player

        auto type = Piece::end;
        for(auto piece = Piece::start; piece != Piece::end; ++piece) {
            if(state.boards[player][to_i(piece)] & curr) {
                type = piece;
                break;
            }
        }
        assert(type != Piece::end);

        switch(type) {
            case Piece::pawn: {
                auto bb = pawn_moves1(player_e, curr, state.empty);
                bit_scan::for_each_bit(bb, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from = sq;
                    m.to = to;
                    m.flag = Move::Flag::quiet;
                    moves.push_back(m);
                });
                bb = pawn_moves1(player_e, curr, state.empty);
                bit_scan::for_each_bit(bb, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::dbl_pawn;
                    moves.push_back(m);
                });
                bb = pawn_captures(player_e, curr, state.any_piece[enemy]);
                bit_scan::for_each_bit(bb, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from = sq;
                    m.to = to;
                    m.flag=Move::Flag::capture;
                    moves.push_back(m);
                });
            } break;

            case Piece::knight: {
                auto bb = knight_moves(curr, state.any_piece[player]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from = sq;
                    m.to = to;
                    m.flag=Move::Flag::quiet;
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from = sq;
                    m.to = to;
                    m.flag=Move::Flag::capture;
                    moves.push_back(m);
                });
            } break;

            case Piece::bishop: {
                auto bb = bishop_moves(curr, state.any_piece[player], state.any_piece[enemy]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::quiet;
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::capture;
                    moves.push_back(m);
                });
            } break;

            case Piece::rook: {
                auto bb = rook_moves(curr, state.any_piece[player], state.any_piece[enemy]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::quiet;
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::capture;
                    moves.push_back(m);
                });
            } break;

            case Piece::queen: {
                auto bb = queen_moves(curr, state.any_piece[player], state.any_piece[enemy]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::quiet;
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::capture;
                    moves.push_back(m);
                });
            } break;

            case Piece::king: {
                auto bb = king_moves(curr, state.any_piece[player]);
                auto quiet = bb & ~state.any_piece[enemy];
                bit_scan::for_each_bit(quiet, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::quiet;
                    moves.push_back(m);
                });

                auto captures = bb & state.any_piece[enemy];
                bit_scan::for_each_bit(captures, [sq, this, &moves, &state](Position to){
                    Move m;
                    m.from=sq;
                    m.to=to;
                    m.flag=Move::Flag::capture;
                    moves.push_back(m);
                });
            } break;

            default:
                assert(false); // Unknown piece

        }


        return moves;
    }

    std::vector<Move> Engine::valid_piece_moves(const State state, const Position sq) const {
        auto moves = piece_moves(state, sq);
        return filter_out_check(state, moves);
    }

    std::vector<Move> Engine::moves(const State state) const {
        std::vector<Move> moves;

        bit_scan::for_each_bit(state.any_piece[to_i(state.player)], [this, &moves, &state](Position sq){
            auto ms = piece_moves(state, sq);
            moves.insert(moves.begin(), ms.begin(), ms.end());
        });

        return moves;
    }

    std::vector<Move> Engine::valid_moves(const State state) const {
        auto mvs = moves(state);
        return filter_out_check(state, mvs);
    }

    board::Board Engine::pawn_moves1(const Player player, const board::Board pawns, const board::Board empty) const {
        if(player == Player::white) {
            return board::step_n(pawns) & empty;
        }
        else {
            return board::step_s(pawns) & empty;
        }
    }
    board::Board Engine::pawn_moves2(const Player player, const board::Board pawns, const board::Board empty) const {
        board::Board m1 = pawn_moves1(player, pawns, empty);

        if(player == Player::white) {
            return board::step_n(m1) & empty & board::rank(4);
        }
        else {
            return board::step_s(m1) & empty & board::rank(5);
        }
    }
    board::Board Engine::pawn_attacks(const Player player, const board::Board pawns) const {
        if(player == Player::white) {
            return board::step_ne(pawns) | board::step_nw(pawns);
        }
        else {
            return board::step_se(pawns) | board::step_sw(pawns);
        }
    }

    board::Board Engine::pawn_captures(const Player player, const board::Board pawns, const board::Board any_enemy) const {
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
                auto idx = static_cast<Position>(bit_scan::scan_fwd(rooks));
                mvs |= calc_pos_ray(idx, Ray::nort, occupied);
                mvs |= calc_pos_ray(idx, Ray::east, occupied);
                mvs |= calc_neg_ray(idx, Ray::west, occupied);
                mvs |= calc_neg_ray(idx, Ray::sout, occupied);
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
                auto idx = static_cast<Position>(bit_scan::scan_fwd(bishops));
                mvs |= calc_pos_ray(idx, Ray::nort_east, occupied);
                mvs |= calc_pos_ray(idx, Ray::nort_west, occupied);
                mvs |= calc_neg_ray(idx, Ray::sout_east, occupied);
                mvs |= calc_neg_ray(idx, Ray::sout_west, occupied);
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
                auto idx = static_cast<Position>(bit_scan::scan_fwd(queens));
                mvs |= calc_pos_ray(idx, Ray::nort, occupied);
                mvs |= calc_pos_ray(idx, Ray::east, occupied);
                mvs |= calc_neg_ray(idx, Ray::west, occupied);
                mvs |= calc_neg_ray(idx, Ray::sout, occupied);
                mvs |= calc_pos_ray(idx, Ray::nort_east, occupied);
                mvs |= calc_pos_ray(idx, Ray::nort_west, occupied);
                mvs |= calc_neg_ray(idx, Ray::sout_east, occupied);
                mvs |= calc_neg_ray(idx, Ray::sout_west, occupied);
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

                auto bb = board::position(static_cast<Position>(current_rank * 8 + current_file));

                const auto white = to_i(Player::white);
                const auto black = to_i(Player::black);

                switch(p) {
                    // black
                    case 'p': state.boards[black][to_i(Piece::pawn)]     |= bb; break;
                    case 'n': state.boards[black][to_i(Piece::knight)]   |= bb; break;
                    case 'b': state.boards[black][to_i(Piece::bishop)]   |= bb; break;
                    case 'r': state.boards[black][to_i(Piece::rook)]     |= bb; break;
                    case 'q': state.boards[black][to_i(Piece::queen)]    |= bb; break;
                    case 'k': state.boards[black][to_i(Piece::king)]     |= bb; break;

                    // white
                    case 'P': state.boards[white][to_i(Piece::pawn)]     |= bb; break;
                    case 'N': state.boards[white][to_i(Piece::knight)]   |= bb; break;
                    case 'B': state.boards[white][to_i(Piece::bishop)]   |= bb; break;
                    case 'R': state.boards[white][to_i(Piece::rook)]     |= bb; break;
                    case 'Q': state.boards[white][to_i(Piece::queen)]    |= bb; break;
                    case 'K': state.boards[white][to_i(Piece::king)]     |= bb; break;
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
        state.castling_ability[to_i(Player::white)][to_i(Castle::king)]  = (str.find("K") != std::string::npos);
        state.castling_ability[to_i(Player::white)][to_i(Castle::queen)] = (str.find("Q") != std::string::npos);
        state.castling_ability[to_i(Player::black)][to_i(Castle::king)]  = (str.find("k") != std::string::npos);
        state.castling_ability[to_i(Player::black)][to_i(Castle::queen)] = (str.find("q") != std::string::npos);
    }

    void Engine::initialize_fen_ep_target(const std::string &str) {
        state.ep_able = true;
        if(str == "-") {
            state.ep_able = false;
            return;
        }

        state.ep_pos = pos_from_rf(str[1] - '0', str[0]);
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

    board::Board Engine::calc_ray(const Position sq, const Ray dir) {
        board::Board me = board::position(sq);
        switch(dir) {
            case Ray::nort: return fill::nortFill(me) & ~me;
            case Ray::east: return fill::eastFill(me) & ~me;
            case Ray::sout: return fill::soutFill(me) & ~me;
            case Ray::west: return fill::westFill(me) & ~me;
            case Ray::nort_east: return fill::noEaFill(me) & ~me;
            case Ray::sout_east: return fill::soEaFill(me) & ~me;
            case Ray::nort_west: return fill::noWeFill(me) & ~me;
            case Ray::sout_west: return fill::soWeFill(me) & ~me;
            default: assert(false);
        }
        return 0x0;
    }

    board::Board Engine::calc_pos_ray(const Position sq, const Ray dir, board::Board occupied) const {
        assert(dir != Ray::sout);
        assert(dir != Ray::west);
        assert(dir != Ray::sout_east);
        assert(dir != Ray::sout_west);

        board::Board attacks = rays_tbl[static_cast<Ray_type>(dir)][static_cast<Position_type>(sq)];
        board::Board blocker = attacks & occupied;
        if ( blocker ) {
            auto s = bit_scan::scan_fwd(blocker);
            attacks ^= rays_tbl[static_cast<Ray_type>(dir)][static_cast<Position_type>(s)];
        }
        return attacks;
    }

    board::Board Engine::calc_neg_ray(const Position sq, const Ray dir, board::Board occupied) const {
        assert(dir != Ray::nort);
        assert(dir != Ray::east);
        assert(dir != Ray::nort_east);
        assert(dir != Ray::nort_west);

        board::Board attacks = rays_tbl[(Ray_type)dir][(Position_type)sq];
        board::Board blocker = attacks & occupied;
        if ( blocker ) {
            auto s = bit_scan::scan_rev(blocker);
            attacks ^= rays_tbl[(Ray_type)dir][(Position_type)s];
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
        for(auto i = Position::start; i != Position::end; ++i) {
            for(auto dir = Ray::start; dir != Ray::end; ++dir) {
                rays_tbl[(Ray_type)dir][(Position_type)i] = calc_ray(i, dir);
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