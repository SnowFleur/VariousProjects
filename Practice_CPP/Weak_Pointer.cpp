#include<iostream>
#include<memory>
#include<vector>	


class TempClass
{
public:
	int32_t	number_;
	TempClass(const int32_t number) : number_(number) {}
};

int main()
{
	std::shared_ptr<TempClass> pTempClass;
	for (int i = 0; i < 1; ++i)
	{
		pTempClass = std::make_shared<TempClass>(i);
	}

	std::cout << "1-USE COUNT: " << pTempClass.use_count() << "\n";
	std::weak_ptr<TempClass> pWeakTempClass = pTempClass;
	std::cout << "2-USE COUNT: " << pTempClass.use_count() << "\n";
	pWeakTempClass.reset();
	std::cout << "3-USE COUNT: " << pTempClass.use_count() << "\n";

	try
	{
		if (pWeakTempClass.lock() == nullptr)
		{
			throw std::exception{ "유효하지 않은 포인터입니다." };
		}

		pWeakTempClass.lock()->number_ = 5;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << "\n";
	}
	catch (...)
	{
		std::cout << "에러가 난다요\n";
	}
}