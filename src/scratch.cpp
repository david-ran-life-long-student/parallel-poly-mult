//
// Created by dran on 9/8/25.
//

#include <iostream>
#include <vector>

int main(int argc, const char * argv[]) {
    int i, j, d;
    d = 2;

    for(i=0; i <= d; i++) {
        for(j=0; j <= i; j++) {
            std::cout << i - j << "," << j << "  ";
        }
        std::cout << std::endl;
    }

    for(i=d+1; i <= 2*d; i++) {
        for(j=d; j >= i-d; j-=1) {
            std::cout << i - j << "," << j << "  ";
        }
        std::cout << std::endl;
    }
    return 0;
}
