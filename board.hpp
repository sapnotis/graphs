#ifndef _BOARD_LOGIC_
#define _BOARD_LOGIC_

#include <vector>
using std::vector;

struct Piece {
    int x, y, w, h;
};

class Board {
private:
    int w, h;
    vector<Piece> pieces;
public:
    Board(int w, int h, vector<Piece> pieces) : w(w), h(h), pieces(pieces) { };
    // bool isValid(const vector<int>& coords);
    bool isValid(const vector<int> coords);
};

bool select_board(int& width, int& height, vector<Piece>& pieces);

#endif