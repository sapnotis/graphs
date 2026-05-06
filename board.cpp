#include "board.hpp"

#include <algorithm>

#include <iostream>
using std::cin, std::cout, std::endl;
#include <vector>
using std::vector;

#include <fstream>
#include <string>
#include <sstream>

vector<int> Board::result_state_of(const vector<int> state) const {

    vector<int> result_state;

    // check if state is valid, return {} if not

    if ( state.size() != 2*pieces.size() ) {
        cout << "(!) incorrect vector size at IsValid()" << endl;
        return result_state;
    }

    for  ( size_t i = 0; i < pieces.size(); i++ ) {
        
        int x1 = state[2 * i];
        int y1 = state[2 * i + 1];

        if (x1 < 0 or y1 < 0 or (x1 + pieces[i].w > w) or (y1 + pieces[i].h > h))
            return result_state;

        for ( size_t j = 0; j < i; j++ ) {

            int x2 = state[2 * j];
            int y2 = state[2 * j + 1];
            
            if (x1 < x2 + pieces[j].w and x1 + pieces[i].w > x2 and y1 < y2 + pieces[j].h and y1 + pieces[i].h > y2)
                return result_state;
        }
    }

    // find result state

    vector<bool> is_used(pieces.size(), false);

    for ( size_t i = 0; i < pieces.size(); ++i ) {

        int best_suitable_piece = -1;

        for ( size_t j = 0; j < pieces.size(); ++j ) {

            if ( !is_used[j] )
                if ( pieces[j]==pieces[i] )
                    if ( best_suitable_piece == -1
                    || ( h * state[2*j] + state[2*j+1] ) <= ( h * state[2*best_suitable_piece] + state[2*best_suitable_piece+1] ) )
                        best_suitable_piece = j;
        }

        if ( best_suitable_piece == -1 ) {
            std::cout << "(!) no best_suitable_piece" << std::endl;
        }

        result_state.push_back( state[2*best_suitable_piece] );
        result_state.push_back( state[2*best_suitable_piece+1] );
        is_used[best_suitable_piece] = true;
    }

    return result_state;
}

bool select_board(int& width, int& height, vector<Piece>& pieces) {

    cout << "\nSelect board:" << endl;

    int board_selector;

    cout << "  0: custom board from config" << endl;
    cout << endl;
    cout << "  1: almost flat graph" << endl;
    cout << "  2: bird-like graph" << endl;
    cout << "  3: 8-like graph" << endl;
    cout << "  4: coronavirus-like graph" << endl;
    cout << "  5: \"blown cilider\" graph" << endl;
    cout << "  6: rugby ball-like graph" << endl;
    cout << "  7: rose-like graph" << endl;
    cout << "  8: :O" << endl;

    cin >> board_selector;

    switch (board_selector)
    {
    case 0: {
        std::ifstream config("config.txt");
        if (!config.is_open()) {
            std::cout << "Couldn't find config! exit" << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(config, line)) {
            if ( line[0]=='#' || line.size()==1 ) continue;
            if (!width) {
                width = std::stoi(line);
                continue;
            }
            if (!height) {
                height = std::stoi(line);
                continue;
            }

            std::stringstream ss(line);
            Piece piece;
            ss >> piece.x >> piece.y >> piece.w >> piece.h;
            pieces.push_back(piece);
        }

        config.close();
        break;
    } case 1: {
        width = 6;
        height = 6;
        pieces = { {0, 0, 3, 3}, {3, 3, 3, 3}, {0, 3, 1, 3}, {3, 0, 2, 3} };
        break;
    } case 2: {
        width = 12;
        height = 2;
        pieces = { {0, 0, 4, 1}, {0, 1, 4, 1}, {4, 0, 1, 2} };
        break;
    } case 3: {
        width = 4;
        height = 4;
        pieces = { {0, 0, 1, 3}, {1, 0, 1, 3}, {0, 3, 1, 1} };
        break;
    } case 4: {
        width = 4;
        height = 4;
        pieces = { {0, 0, 2, 2}, {2, 0, 2, 2}, {0, 2, 1, 2}, {1, 2, 1, 1}, {1, 3, 1, 1}, {2, 2, 1, 1}, {2, 3, 1, 1} };
        break;
    } case 5: {
        width = 4;
        height = 4;
        pieces = { {0, 0, 2, 2}, {2, 0, 2, 2}, {0, 2, 1, 2}, {1, 2, 1, 1}, {1, 3, 1, 1}, {2, 2, 1, 1} };
        break;
    } case 6: {
        width = 4;
        height = 4;
        pieces = { {0, 0, 2, 2}, {2, 0, 2, 2}, {0, 2, 2, 1}, {2, 2, 2, 1} };
        break;
    } case 7: {
        width = 2;
        height = 3;
        pieces = { {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}, {0, 2, 1, 1} };
        break;
    } case 8: {
        width = 5;
        height = 5;
        pieces = { {0, 0, 3, 3}, {0, 4, 3, 1}, {4, 0, 1, 3}, {4, 4, 1, 1} };
        break;
    } default: {
        return false;
    }
    }

    return true;
}