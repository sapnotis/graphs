
//----------------------------------------------------newest version-----------------------------------------------------

#include <vector>
#include <iostream>
using namespace std;

struct Piece {
    int x, y, w, h;
};

class Board {
private:
    int w, h;
    vector<Piece> pieces;  // block's sizes for the board

public:
    Board(int w, int h, vector<Piece> pieces) : w(w), h(h), pieces(pieces) {}

    bool isValid(vector<int>& coords) {
        // safety check
        if (coords.size() != pieces.size() * 2)
            return false;

        for (int i = 0; i < pieces.size(); i++) {
            int x = coords[2 * i];
            int y = coords[2 * i + 1];

            // limits
            if (x < 0 or y < 0 or (x + pieces[i].w > w) or (y + pieces[i].h > h))
                return false;

            // nalozheniya
            for (int j = 0; j < i; j++) {
                int x2 = coords[2 * j];
                int y2 = coords[2 * j + 1];

                if (x < x2 + pieces[j].w and x + pieces[i].w > x2 and y < y2 + pieces[j].h and y + pieces[i].h > y2)
                    return false;
            }
        }
        return true;
    }
};




int main() {
    vector<Piece> pieces = { {0, 0, 2, 2}, {0, 0, 1, 2}, {0, 0, 1, 2}, {0, 0, 2, 1}, {0, 0, 2, 1}, {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 1, 1} };

    vector<int> coords = { 0, 0, 0, 2, 3, 2, 0, 4, 2, 4, 2, 0, 3, 0, 2, 1, 3, 1 };

    Board board(4, 5, pieces);

    if (board.isValid(coords)) {
        cout << "Board is valid!" << endl;
    }
    else {
        cout << "Board is invalid!" << endl;
    }

    return 0;
}