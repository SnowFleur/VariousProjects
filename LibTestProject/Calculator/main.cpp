#pragma comment(lib,"MathLib.lib")

#include"Calculator.h"

#include"Math.h"
#include"Vector.h"

int main() {
    CCalculator calculater;

    Vector2 lhs{ 15,14 };
    Vector2 rhs{ 15,16 };

    calculater.AddVector(lhs, rhs);

}