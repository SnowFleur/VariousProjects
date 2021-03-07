#include<iostream>
#include<map>
#include"Table.h"

#define LOOP 1000000

enum RANK {
    S = 0, A = 1, B = 2, C = 3, D = 4
};

int main() {
    CTable cardTable;

    cardTable.InsertCoard(S, 1);
    cardTable.InsertCoard(A, 10);
    cardTable.InsertCoard(B, 20); 
    cardTable.InsertCoard(C, 30);
    cardTable.InsertCoard(D, 39);

    std::map<int, int>result{ {S,0},{A,0},{B,0},{C,0},{D,0} };

    for (int i = 0; i < LOOP; ++i) {

        switch (cardTable.SelectCard()) {
        case S: {
            result[S]++;
            break;
        }
        case A: {
            result[A]++;
            break;
        }
        case B: {
            result[B]++;
            break;
        }
        case C: {
            result[C]++;
            break;
        }
        case D: {
            result[D]++;
            break;
        }
        default:
            break;
        }
    }

    for (auto& i : result) {

        switch (i.first) {
        case S: {
            std::cout << "S:" << i.second << "\n";
            break;
        }
        case A: {
            std::cout << "A:" << i.second << "\n";
            break;
        }
        case B: {
            std::cout << "B:" << i.second << "\n";
            break;
        }
        case C: {
            std::cout << "C:" << i.second << "\n";
            break;
        }
        case D: {
            std::cout << "D:" << i.second << "\n";
            break;
        }
        default:
            break;
        }
    }

}

