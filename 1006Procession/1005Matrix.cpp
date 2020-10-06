#include <iostream>
#include <array>
using namespace std;

namespace Mathf {
    template<int size>
    double MatrixProduct(array<double, size>&rowArr, array<double, size> &columnArr) {
        if (rowArr.empty() || columnArr.empty())
        {
            cout << "Can't Product!!";
            return 0;
        }
        double result = 0;
        for (int i = 0; i < size; i++) {
            result += rowArr[i] * columnArr[i];
        }
        return result;
    }
};
int main()
{
    std::array<double, 5> rowArr = {1, 2, 3, 4, 5};
    std::array<double, 5> columnArr = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++)
        cout << i + 1 << "번째 rowArr : " << rowArr[i] << endl;
    for (int i = 0; i < 5; i++)
        cout << i + 1 << "번째 columnArr : " << columnArr[i] << endl;
    double loss = Mathf::MatrixProduct<5>(rowArr, columnArr);
    cout << "행렬곱 결과 : "<< loss << endl;
}