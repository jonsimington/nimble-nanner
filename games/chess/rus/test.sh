#!/bin/bash

echorun() { echo "$@" ; "$@" ; }

CXX=g++
CXXFLAGS="-std=c++14"
SOURCES="bishop.cpp bit_scan.cpp fill.cpp king.cpp knight.cpp pawn.cpp piece_board.cpp queen.cpp rook.cpp"

echo "Removing previous tests..."
echorun rm -f test_*.exe

echo "Building tests..."
echorun ${CXX} ${CXXFLAGS} -DRUS_PAWN_TEST      -o test_pawn.exe    ${SOURCES}
echorun ${CXX} ${CXXFLAGS} -DRUS_KNIGHT_TEST    -o test_knight.exe  ${SOURCES}
echorun ${CXX} ${CXXFLAGS} -DRUS_KING_TEST      -o test_king.exe    ${SOURCES}
echorun ${CXX} ${CXXFLAGS} -DRUS_FILL_TEST      -o test_fill.exe    ${SOURCES}
echorun ${CXX} ${CXXFLAGS} -DRUS_BISHOP_TEST    -o test_bishop.exe  ${SOURCES}
echorun ${CXX} ${CXXFLAGS} -DRUS_ROOK_TEST      -o test_rook.exe    ${SOURCES}
echorun ${CXX} ${CXXFLAGS} -DRUS_QUEEN_TEST     -o test_queen.exe   ${SOURCES}

echo "Running tests..."
echorun ./test_pawn.exe
echorun ./test_knight.exe
echorun ./test_king.exe
echorun ./test_fill.exe
echorun ./test_bishop.exe
echorun ./test_rook.exe
echorun ./test_queen.exe

echo "Done."