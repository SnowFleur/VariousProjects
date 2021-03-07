#pragma once
#include<vector>
#include<random>

#define DEFAULT_ALLOCATE_MEMORY 10000
#define MIN 0
#define MAX 100

class CTable {
    struct Candidate {
        int     id_;
        int     weight_;
    };
private:
    std::vector<Candidate> candidates_;
    std::mt19937_64 rnd_;
    std::uniform_int_distribution<int>range_;
public:
    CTable() :range_(MIN, MAX) {
        candidates_.reserve(DEFAULT_ALLOCATE_MEMORY);
        std::random_device rd;
        rnd_.seed(rd());
    }

    inline void InsertCoard(const int id, const int weight) {
        candidates_.emplace_back(Candidate{ id,weight });
    }

    inline int SelectCard() {

        int value = range_(rnd_);
        int sum = 0;

        for (const auto& i : candidates_) {
            sum += i.weight_;
            if (sum >= value) {
                return i.id_;
            }
        }
        return candidates_[0].id_;
    }

};