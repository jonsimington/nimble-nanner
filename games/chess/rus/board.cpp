#include <cassert>

#include "board.hpp"

namespace rus {
    namespace board {

        int idxFromRankFile(const int rank, const std::string file) {
            int file_offset = file[0] - 'a';
            int rank_offset = rank - 1;
            return file_offset + (8 * rank_offset);
        }

        void rankFileFromIdx(int idx, int & rank, std::string & file) {
            const std::string files[] = { "a", "b", "c", "d", "e", "f", "g", "h" };

            file = files[idx % 8];
            rank = (idx / 8) + 1;
        }

    }
}

