#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<random>>
#include<chrono>

using namespace std::chrono;
using DWORD = unsigned int;

constexpr DWORD MAX_DATA = 100000;

std::random_device g_rn;
std::mt19937_64 rnd(g_rn());


void TestOnlySearch() {
    std::vector<DWORD>       v;
    std::map<DWORD, DWORD>   map;
    std::uniform_int_distribution<DWORD> range(0, MAX_DATA);

    v.reserve(MAX_DATA);
    for (DWORD i = 0; i < MAX_DATA; ++i) {
        DWORD val = range(rnd);
        v.emplace_back(val);
        map[val] = val;
    }
    std::sort(v.begin(), v.end());

    {
        auto startTime = std::chrono::system_clock::now();
        for (DWORD i = 0; i < MAX_DATA; ++i) {
            DWORD val = range(rnd);
            std::lower_bound(v.begin(), v.end(), val);
        }

        auto endTime = std::chrono::system_clock::now() - startTime;
        int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime).count();

        std::cout << "Sort Vector----> Only Search ElapsedTime: " << elapsedTime << "ms\n";
    }

    {
        auto startTime = std::chrono::system_clock::now();
        for (DWORD i = 0; i < MAX_DATA; ++i) {
            DWORD val = range(rnd);

            map.find(val);
        }
        auto endTime = std::chrono::system_clock::now() - startTime;
        int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime).count();

        std::cout << "Map-----> Only Search ElapsedTime: " << elapsedTime << "ms\n";
    }
}

void TestInsertAndSearch() {

    std::vector<DWORD>       v;
    std::map<DWORD, DWORD>   map;
    std::uniform_int_distribution<DWORD> range(0, MAX_DATA);
    std::uniform_int_distribution<DWORD> Switch(0, 1);

    v.reserve(MAX_DATA);

    {
        auto startTime = std::chrono::system_clock::now();
        for (DWORD i = 0; i < MAX_DATA; ++i) {
            DWORD val = range(rnd);

            switch (Switch(rnd)) {
            case 0: {
                v.emplace_back(val);
                std::sort(v.begin(), v.end());
                break;
            }
            case 1: {
                std::lower_bound(v.begin(), v.end(), val);
                break;
            }
            }

        }

        auto endTime = std::chrono::system_clock::now() - startTime;
        int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime).count();

        std::cout << "Sort Vector----> Insert And Search ElapsedTime: " << elapsedTime << "ms\n";
    }

    {
        auto startTime = std::chrono::system_clock::now();
        for (DWORD i = 0; i < MAX_DATA; ++i) {
            DWORD val = range(rnd);

            switch (Switch(rnd)) {
            case 0: {
                map[val] = val;
                break;
            }
            case 1: {
                map.find(val);
                break;
            }
            }

        }
        auto endTime = std::chrono::system_clock::now() - startTime;
        int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime).count();

        std::cout << "Map-----> Only Insert And Search ElapsedTime: " << elapsedTime << "ms\n";
    }
}

void TestInsertDeletedAndSearch() {

    std::vector<DWORD>       v;
    std::map<DWORD, DWORD>   map;
    std::uniform_int_distribution<DWORD> range(0, MAX_DATA);
    std::uniform_int_distribution<DWORD> Switch(0, 2);

    v.reserve(MAX_DATA);

    {
        auto startTime = std::chrono::system_clock::now();
        for (DWORD i = 0; i < MAX_DATA; ++i) {
            DWORD val = range(rnd);

            switch (Switch(rnd)) {
            case 0: {
                v.emplace_back(val);
                std::sort(v.begin(), v.end());
                break;
            }
            case 1: {
                std::remove(v.begin(), v.end(), val);
                std::sort(v.begin(), v.end());
                break;
            }
            case 2: {
                std::lower_bound(v.begin(), v.end(), val);
                break;
            }
            }
        }

        auto endTime = std::chrono::system_clock::now() - startTime;
        int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime).count();

        std::cout << "Sort Vector----> Insert, Deleted And Search ElapsedTime: " << elapsedTime << "ms\n";
    }

    {
        auto startTime = std::chrono::system_clock::now();
        for (DWORD i = 0; i < MAX_DATA; ++i) {
            DWORD val = range(rnd);

            switch (Switch(rnd)) {
            case 0: {
                map[val] = val;
                break;
            }
            case 1: {
                map.erase(val);
                break;
            }
            case 2: {
                map.find(val);
                break;
            }
            }
        }
        auto endTime = std::chrono::system_clock::now() - startTime;
        int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime).count();

        std::cout << "Map-----> Only Insert, Deleted And Search ElapsedTime: " << elapsedTime << "ms\n";
    }
}



int main() {
    TestOnlySearch();
    std::cout << "\n";
    TestInsertAndSearch();
    std::cout << "\n";
    TestInsertDeletedAndSearch();
}