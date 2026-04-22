/*class Board {

private:
    int w;
    int h;
    int n;


public:
    Board(int w, int h, int n) : w(w), h(h), n(n) {}

    ~Board() {}

    bool isPositionValid(int* coords) {
        int sizes[][2] = { {2,2},{1,2},{1,2},{2,1},{2,1},{1,1},{1,1},{1,1},{1,1} };

        for (int i = 0; i < n; i++) {
            int x = coords[2 * i];
            int y = coords[2 * i + 1];
            // togda figuri dolzhni ukazivatsa v takom zhe poryadke chto i v sizes
            int i_w = sizes[i][0];
            int i_h = sizes[i][1];

            // granitsi
            if (x < 0 or y < 0 or (x + i_w > w) or (y + i_h > h)) {
                return false;
            }
            // nalozhenia
            for (int j = 0; j < i; j++) {
                int x2 = coords[2 * j];
                int y2 = coords[2 * j + 1];
                int j_w = sizes[j][0];
                int j_h = sizes[j][1];

                if ((x < x2 + j_w) and (x + i_w > x2) and (y < y2 + j_h) and (y + i_h > y2)) {
                    return false;
                }
            }
        }
        return true;
    }
*/

//---------------------------------actual version!!!-----------------------------------------

#include <vector>
#include <iostream>
using namespace std;

struct Piece {
    int x, y, w, h;
};

class Board {
private:
    int w, h;
    vector<Piece> pieces;

public:
    Board(int w, int h, vector<Piece> pieces) : w(w), h(h), pieces(pieces) {}

    bool isValid() {
        for (int i = 0; i < pieces.size(); i++) {
            auto& p = pieces[i];

            //granitsi
            if (p.x < 0 or p.y < 0 or p.x + p.w > w or p.y + p.h > h)
                return false;

            //nalozheniya
            for (int j = 0; j < i; j++) {
                auto& q = pieces[j];
                if (p.x < q.x + q.w and p.x + p.w > q.x and
                    p.y < q.y + q.h and p.y + p.h > q.y)
                    return false;
            }
        }
        return true;
    }
};


/*int main() {
    vector<Piece> p = {{0, 0, 2, 2}, {1, 1, 2, 2}};

    Board board(4, 5, p);

    if (board.isValid()) {
        cout << "Board is valid!" << endl;
    }
    else {
        cout << "Board is invalid!" << endl;
    }

    return 0;
}*/
