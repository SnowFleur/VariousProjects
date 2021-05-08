#include<iostream>
#include<future>

void DoWork(std::promise<int> thePromise) {
    // ���ϴ� �۾��� �����Ѵ�.
    // ���� ����� Promise�� �����Ѵ�.
    thePromise.set_value(2);
}

int main() {

    // �����忡 ������ Promise�� �����Ѵ�.
    std::promise<int> myPromise;

    // �� promise�� ���� future�� �����´�.
    auto theFuture = myPromise.get_future();

    // �����带 �����ϰ� �տ��� ���� promise�� �μ��� �����Ѵ�.
    std::thread theThread{ DoWork,std::move(myPromise) };

    //���ϴ� �۾��� �����Ѵ�.

    // ���� ����� �����´�.
    int result = theFuture.get();
    std::cout << result << "\n";


    theThread.join();

    /*
    ���������� future�� ���� ����� ���Դ��� �ֱ������� �˻��ϵ��� �����ϰų�(wait_for())
    ���� ������ ���� ����ȭ ����� ����ϵ��� �����Ѵ�. �׷��� ����� ������ ����
    ������ ���� �� ��ٸ��� �ʰ� �ٸ� �۾��� ������ �� �ִ�.
    */
}

