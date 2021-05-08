#include<iostream>
#include<future>

void DoWork(std::promise<int> thePromise) {
    // 원하는 작업을 수행한다.
    // 최종 결과를 Promise에 저장한다.
    thePromise.set_value(2);
}

int main() {

    // 스레드에 전달할 Promise를 생성한다.
    std::promise<int> myPromise;

    // 이 promise에 대한 future를 가져온다.
    auto theFuture = myPromise.get_future();

    // 스레드를 생성하고 앞에서 만든 promise를 인수로 전달한다.
    std::thread theThread{ DoWork,std::move(myPromise) };

    //원하는 작업을 수행한다.

    // 최종 결과를 가져온다.
    int result = theFuture.get();
    std::cout << result << "\n";


    theThread.join();

    /*
    실전에서는 future에 최종 결과가 나왔는지 주기적으로 검사하도록 구현하거나(wait_for())
    조건 변수와 같은 동기화 기법을 사용하도록 구현한다. 그러면 결과가 나오기 전에
    무조건 멈춘 뒤 기다리지 않고 다른 작업을 수행할 수 있다.
    */
}

