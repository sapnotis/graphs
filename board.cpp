class Board {

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

}
