#include<iostream>
#include<future>


int Cacluate() {
    return 123;
}


int main() {

    auto myFuture = std::async(Cacluate);
    // auto myFuture = std::async(std::launch::async,Cacluate);
    // auto myFuture = std::async(std::launch::deferred,Cacluate);


    //����� �����´�.
    int result = myFuture.get();
    std::cout << result << "\n";

}