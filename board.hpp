#ifndef _BOARD_
#define _BOARD_

struct Piece {
    int x, y, w, h;
};

class Board {
private:
    int w, h;
    vector<Piece> pieces; // block's sizes for the board

public:
    Board(int w, int h, vector<Piece> pieces) : w(w), h(h), pieces(pieces) { };
    bool isValid(vector<int>& coords);
};

#endif