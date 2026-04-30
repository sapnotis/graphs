#include "board.hpp"

#include <iostream>
using std::cin, std::cout, std::endl;
#include <vector>
using std::vector;

bool Board::isValid(vector<int> coords) {

    // safety check
    if ( coords.size() != 2*pieces.size() ) {
        cout << "(!) incorrect vector size at IsValid" << endl;
        return false;
    }

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