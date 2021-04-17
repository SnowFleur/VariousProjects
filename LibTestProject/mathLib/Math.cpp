#include"Math.h"

Vector2 MATH::Add_Vector2(const Vector2& lhs, const Vector2& rhs) {
    
    Vector2 temp{};
    temp.m_x = lhs.m_x + rhs.m_x;
    temp.m_y = lhs.m_y + rhs.m_y;
    return temp;
}


void MATH::BugTest(int* ptr) {
    *ptr += 5;
}