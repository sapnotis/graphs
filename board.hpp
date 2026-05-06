#ifndef _BOARD_LOGIC_
#define _BOARD_LOGIC_

#include <vector>
using std::vector;

struct Piece {
    int x, y, w, h;

    bool operator==(const Piece& other) const {
        return ( w==other.w && h==other.h );
    }
};

class Board {
private:
    int w, h;
    vector<Piece> pieces;
public:
    Board(int w, int h, vector<Piece> pieces) : w(w), h(h), pieces(pieces) { };
    
    // bad name, can't come up with good one
    vector<int> result_state_of(const vector<int> state) const;
};

bool select_board(int& width, int& height, vector<Piece>& pieces);

#endif