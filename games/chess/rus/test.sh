#!/bin/bash

echorun() { echo "$@" ; "$@" ; }

CXX=g++
CXXFLAGS="-std=c++14"

echo "Removing previous tests..."
echorun rm -f test_*.exe

echo "Building tests..."
echorun ${CXX} ${CXXFLAGS} -DRUS_PAWN_TEST      -o test_pawn.exe    piece_board.cpp pawn.cpp
echorun ${CXX} ${CXXFLAGS} -DRUS_KNIGHT_TEST    -o test_knight.exe  piece_board.cpp knight.cpp
echorun ${CXX} ${CXXFLAGS} -DRUS_KING_TEST      -o test_king.exe    piece_board.cpp king.cpp
echorun ${CXX} ${CXXFLAGS} -DRUS_FILL_TEST      -o test_fill.exe    piece_board.cpp fill.cpp
echorun ${CXX} ${CXXFLAGS} -DRUS_BISHOP_TEST    -o test_bishop.exe  piece_board.cpp fill.cpp bishop.cpp
echorun ${CXX} ${CXXFLAGS} -DRUS_ROOK_TEST      -o test_rook.exe    piece_board.cpp fill.cpp rook.cpp
echorun ${CXX} ${CXXFLAGS} -DRUS_QUEEN_TEST     -o test_queen.exe   piece_board.cpp fill.cpp queen.cpp

echo "Running tests..."
echorun ./test_pawn.exe
echorun ./test_knight.exe
echorun ./test_king.exe
echorun ./test_fill.exe
echorun ./test_bishop.exe
echorun ./test_rook.exe
echorun ./test_queen.exe

echo "Done."