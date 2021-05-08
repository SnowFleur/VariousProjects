#include<chrono>
#include<future>
#include<iostream>

using namespace std::chrono;

bool TerminateTime() {
    auto startTime = high_resolution_clock::now();

    while (true) {
        if (high_resolution_clock::now() - startTime >3s) {
            return true;
        }
    }
}


int main() {

    //호출을 원할 때
    auto async = std::async(TerminateTime);

    int time{};
    while (true) {
    
        if (time > 1000) {

            if (async.wait_for(0s) == std::future_status::ready) {
                int result = async.get();
                std::cout << std::boolalpha << result << "\n";
                break;
            }
            else {
                std::cout << "Wait For result\n";
            }
        }
        else {
            std::cout << "else\n";
        }
        ++time;
    
    }

}

