#pragma once
#include<iostream>

#include"Math.h"
#include"Vector.h"

class CCalculator {
private:

public:

    void AddVector(Vector2& lhs,Vector2& rhs) {
        auto result=MATH::Add_Vector2(lhs, rhs);
        std::cout << result.m_x << ", " << result.m_y << "\n";
    }

};