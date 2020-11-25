//#include <iostream>
//#include <array>
//#include <conio.h> //kbhit, getch
//
////훈련 데이터 x데이터, Y 행렬의 행(m) 개수
//#define TRAININGMATRIX_DATA_M 5
////훈련 데이터 X데이터 행렬의 열(n) 개수
//#define TRAININGMATRIX_X_N 3
////훈련 데이터 Y행렬 열(n) 개수
//#define TRAININGMATRIX_Y_N 1
//
//#pragma region 키보드 코드
//
//#define VK_ESC 27
//#define VK_LEFT 75
//#define VK_RIGHT 77
//#define VK_UP 72
//#define VK_DOWN 80
//#define VK_ENTER 13
//
//#define VK_a 97
//#define VK_b 98
//#define VK_c 99
//#define VK_d 100
//#define VK_e 101
//#define VK_f 102
//#define VK_g 103
//#define VK_h 104
//#define VK_i 105
//#define VK_j 106
//#define VK_k 107
//#define VK_l 108
//#define VK_m 109
//#define VK_n 110
//#define VK_o 111
//#define VK_p 112
//#define VK_q 113
//#define VK_r 114
//#define VK_s 115
//#define VK_t 116
//#define VK_u 117
//#define VK_v 118
//#define VK_w 119
//#define VK_x 120
//#define VK_y 121
//#define VK_z 122
//#pragma endregion
//using namespace std;
////x1 : 몸무게, x2 : 키, x3 : 언어 수
//
//array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix = {
//    73, 180, 1,
//    60, 170, 2,
//    40, 145, 1,
//    65, 171, 3,
//    70, 160, 1
//};
//
//array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> wMatrix = {
//    0,
//    0,
//    0
//};
////y1 : 아이 = 0, 성인 = 1 구분
//
//array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> yMatrix = {
//    1,
//    1,
//    0,
//    1,
//    0
//};
//
//double Sigmoid(double hypothesis) {
//    return 1 / (1 + exp(hypothesis));
//}
//
///// <summary>
///// targetWIndex의 편미분 함수
///// </summary>
///// <param name="targetWIndex"></param>
///// <returns></returns>
//double wUpdate(const int& targetWIndex) {
//    double newW = 0;
//    for (int m = 0; m < TRAININGMATRIX_DATA_M; m++) {
//        double hypothesis = 0;
//        for (int n = 0; n < TRAININGMATRIX_X_N; n++) {
//            hypothesis += xMatrix[m][n] * wMatrix[n][0];
//        }
//        newW += xMatrix[m][targetWIndex] * (Sigmoid(hypothesis) - yMatrix[m][0]);
//    }
//    //행렬 요소들에 대한 기울기의 평균 구하기
//    newW /= TRAININGMATRIX_DATA_M;
//    return newW;
//}
//
///// <summary>
///// x변수 행렬과 w가중치 행렬곱으로로 나온 y정답률 행렬의 평균을 반환한다
///// </summary>
///// <returns></returns>
//double Cost() {
//    double cost = 0;
//    for (int xM = 0; xM < TRAININGMATRIX_DATA_M; xM++) {
//        double hypothesis = 0;
//        for (int xN = 0; xN < TRAININGMATRIX_X_N; xN++)
//        {
//            hypothesis += xMatrix[xM][xN] * wMatrix[xN][0];
//        }
//        cost += log(1 + exp(hypothesis) - hypothesis * yMatrix[xM][0]);
//    }
//    cost /= TRAININGMATRIX_DATA_M;
//    return cost;
//}
//
//
//int main()
//{
//    double learningRate = 0.1;
//    std::cout << "Hello World!\n";
//
//    int key = 0;
//    bool programRunning = true;
//
//    //소수점 출력 고정
//    cout << fixed;
//    //double형 최대 보장 범위인 소수점 15자리까지 출력
//    //double형 변수를 소수점 6자리까지 출력
//    cout.precision(6);
//
//#pragma region Developer_Control
//    do {
//        if (_kbhit()) {
//            key = _getch();
//            switch (key) {
//
//            case VK_a:
//                learningRate *= 10;
//                break;
//            case VK_s:
//                learningRate /= 10;
//                break;
//            case VK_u:
//                wMatrix[0][0] += learningRate;
//                break;
//            case VK_j:
//                wMatrix[0][0] -= learningRate;
//                break;
//            case VK_i:
//                wMatrix[1][0] += learningRate;
//                break;
//            case VK_k:
//                wMatrix[1][0] -= learningRate;
//                break;
//            case VK_o:
//                wMatrix[2][0] += learningRate;
//                break;
//            case VK_l:
//                wMatrix[2][0] -= learningRate;
//                break;
//            case VK_ENTER:
//                wMatrix = {
//                    0,
//                    0,
//                    0
//                };
//                break;
//            case VK_ESC:
//                programRunning = false;
//                break;
//            }
//            system("cls");
//
//            std::cout << "-------------------------------------------------" << endl;
//            std::cout << "LearningRate : " << learningRate << endl;
//            std::cout << "w1 = " << wMatrix[0][0] << ", w2 = " << wMatrix[1][0] << ", w3 = " << wMatrix[2][0] << endl;
//            std::cout << "w1 편미분 : " << wUpdate(0) << endl;
//            std::cout << "w2 편미분 : " << wUpdate(1) << endl;
//            std::cout << "w3 편미분 : " << wUpdate(2) << endl;
//            std::cout << "Cost(w1, w2, w3) 결과 : " << Cost() << endl;
//            std::cout << "-------------------------------------------------" << endl;
//        }
//    } while (programRunning);
//#pragma endregion
//}
//
