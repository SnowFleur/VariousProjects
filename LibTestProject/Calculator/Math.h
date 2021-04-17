#pragma once
#include"Vector.h"

namespace MATH {

    template<class T>
    T Add(const T lhs, const T rhs) {
        return lhs + rhs;
    }

    Vector2 Add_Vector2(const Vector2& lhs, const Vector2& rhs);

    void BugTest(int* ptr);

}