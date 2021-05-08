#include<iostream>
#include<future>

int CalculateSum(int lhs, int rhs) {
    return lhs + rhs;
}


int main() {

    // package_task를 생성해서 CalculateSum을 실행한다.
    std::packaged_task<int(int, int)> task(CalculateSum);

    // 생성한 package_task로부터 CaclatedSum의 결과를 담은 future를 받는다.
    auto theFuture = task.get_future();

    // 스레드를 생성한 뒤 앞에서 만든 package_task를 이동 시키고,
    // 인수를 적절히 전달해서 작업을 수행한다.
    std::thread theThread(std::move(task), 39, 3);

    int result = theFuture.get();
    std::cout << result << "\n";


    theThread.join();

    
}
