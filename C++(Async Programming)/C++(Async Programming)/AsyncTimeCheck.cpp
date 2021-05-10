#include<chrono>
#include<future>
#include<iostream>

using namespace std::chrono;


struct Person {
public:
    int age;
};

void TerminateTime(Person& p) {
    auto startTime = high_resolution_clock::now();

    while (true) {
        if (high_resolution_clock::now() - startTime >3s) {
            std::cout << "Happy Birth Day" << p.age << "\n";
            return;
        }
    }
}



int main() {

    Person p;
    p.age = 15;

    //호출을 원할 때
    auto async = std::async(TerminateTime,std::ref(p));

    int time{};
    while (true) {
    
        if (time > 1000) {

            if (async.wait_for(0s) == std::future_status::ready) {
                async.get();

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

