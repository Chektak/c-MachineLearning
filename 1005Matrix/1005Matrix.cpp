#include <iostream>
#include <array>
using namespace std;

namespace Mathf {
    //행(m) : 세로 줄, 열(n) : 가로로 얼마나 퍼져있는지
    template<int mat1M, int mat1N, int mat2M, int mat2N>
    void MatrixProduct(const std::array<std::array<double, mat1N>, mat1M> &mat1, 
        const std::array<std::array<double, mat2N>, mat2M> &mat2,
        std::array<std::array<double, mat2N>, mat1M> &resultMat
    ) {
        if (mat1M <= 0 || mat1N <= 0 || mat2M <= 0 || mat2N <= 0)
        {
            cout << "Can't Product!!";
            return;
        }
        cout << "xInputMatrix" << endl;
        //행
        for (int i = 0; i < mat1M; i++) {
            //열
            for (int j = 0; j < mat1N; j++) {
                cout << mat1[i][j] << "  ";
            }
            cout << endl;
        }
        cout << "\nwMatrix" << endl;
        //행
        for (int i = 0; i < mat2M; i++) {
            //열
            for (int j = 0; j < mat2N; j++) {
                cout << mat2[i][j] << "  ";
            }
            cout << endl;
        }

        for (int z = 0; z < mat2N; z++) {
            //행
            for (int i = 0; i < mat1M; i++) {
                //열
                for (int j = 0; j < mat1N; j++) {
                    resultMat[i][z] += mat1[i][j] * mat2[j][z];
                }
            }
        }

        cout << "\n결과 행렬 :: ResultMatrix" << endl;
        //행
        for (int i = 0; i < mat1M; i++) {
            //열
            for (int j = 0; j < mat2N; j++) {
                cout << resultMat[i][j] << " ";
            }
            cout << endl;
        }
    }

    /// <summary>
    /// xMatrix * wMatrix = ResultMatrix 식에서 빈 wMatrix를 리턴한다
    /// </summary>
    /// <returns></returns>
    template <int mat2N, int mat1M>
    std::array<std::array<double, mat2N>, mat1M>* GetEmptyWMatrix() {
        std::array <std::array<double, mat2N>, mat1M>* emptyMat
            = new std::array <std::array<double, mat2N>, mat1M>;

        std::array<double, mat2N> tempMat;
        tempMat.fill(0);
        emptyMat->fill(tempMat);

        return emptyMat;
    }
};
int main()
{
    //5행 3열 행렬( 5m 3n
    std::array<std::array<double, 3>, 5> xMatrix = { 
        1, 1, 1, 
        2, 2, 2,
        3, 3, 3,
        1, 1, 1,
        1, 1, 1
    };
    //3행 1열 행렬
    std::array<std::array<double, 1>, 3> wMatrix = {   
        1, 
        2, 
        3
    };
    //result : 5행 1열 행렬
    std::array<std::array<double, 1>, 5> resultMatrix ={
        0,
        0,
        0,
        0,
        0
    };

    Mathf::MatrixProduct<5, 3, 3, 1>(xMatrix, wMatrix, resultMatrix);
    
    std::array<std::array<double, 1>, 3>* wEmptyMatrix;
    wEmptyMatrix = Mathf::GetEmptyWMatrix<1, 3>();
    cout << "\nxMatrix와 resultMatrix로 w행렬을 추정해 빈 행렬 생성" << endl;
    for (int i = 0; i < 1; i++) {
        //열
        for (int j = 0; j < 3; j++) {
            printf("%.0lf\n", wEmptyMatrix[i][j]);
        }
        cout << endl;
    }


}