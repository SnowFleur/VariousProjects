#include<iostream>
#include<future>

int CalculateSum(int lhs, int rhs) {
    return lhs + rhs;
}


int main() {

    // package_task�� �����ؼ� CalculateSum�� �����Ѵ�.
    std::packaged_task<int(int, int)> task(CalculateSum);

    // ������ package_task�κ��� CaclatedSum�� ����� ���� future�� �޴´�.
    auto theFuture = task.get_future();

    // �����带 ������ �� �տ��� ���� package_task�� �̵� ��Ű��,
    // �μ��� ������ �����ؼ� �۾��� �����Ѵ�.
    std::thread theThread(std::move(task), 39, 3);

    int result = theFuture.get();
    std::cout << result << "\n";


    theThread.join();

    
}
