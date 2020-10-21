#include <iostream>
#include <array>
#include <conio.h> //kbhit, getch

//�Ʒ� ������ x������, Y ����� ��(m) ����
#define TRAININGMATRIX_DATA_M 5
//�Ʒ� ������ X������ ����� ��(n) ����
#define TRAININGMATRIX_X_N 3
//�Ʒ� ������ Y��� ��(n) ����
#define TRAININGMATRIX_Y_N 1

#pragma region Ű���� �ڵ�

#define VK_ESC 27
#define VK_LEFT 75
#define VK_RIGHT 77
#define VK_UP 72
#define VK_DOWN 80
#define VK_ENTER 13

#define VK_a 97
#define VK_b 98
#define VK_c 99
#define VK_d 100
#define VK_e 101
#define VK_f 102
#define VK_g 103
#define VK_h 104
#define VK_i 105
#define VK_j 106
#define VK_k 107
#define VK_l 108
#define VK_m 109
#define VK_n 110
#define VK_o 111
#define VK_p 112
#define VK_q 113
#define VK_r 114
#define VK_s 115
#define VK_t 116
#define VK_u 117
#define VK_v 118
#define VK_w 119
#define VK_x 120
#define VK_y 121
#define VK_z 122
#pragma endregion
using namespace std;

array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix = {
    100, 100, 100,
    100, 0, 0,
    0, 0, 100,
    0, 100, 0,
    100, 100, 0
};

array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> wMatrix = {
    0,
    0,
    0
};
array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> yMatrix = {
    300,
    100,
    100,
    100,
    200
};

/// <summary>
/// targetW ����� ��̺� �Լ�(���⸦ ��ȯ�Ѵ�)
/// </summary>
/// <param name="wMatrix">�� �Լ��� wMatrix�� ���� 0�� ��쿡�� ��밡���ϴ�.</param>
/// <param name="targetWIndex">w1�� Ÿ���� ��� 0�� ���ڷ� �����Ѵ�.</param>
/// <returns></returns>
template<int wMatM, int wMatN>
double wUpdate(array<array<double, wMatN>, wMatM> wMatrix, const int& targetWIndex) {
    double newW = 0;
    double y = 0;
    for (int m = 0; m < TRAININGMATRIX_DATA_M; m++) {
        y = yMatrix[m][0];

        newW += xMatrix[m][targetWIndex] * (xMatrix[m][0] * wMatrix[0][0] +
            xMatrix[m][1] * wMatrix[1][0] + xMatrix[m][2] * wMatrix[2][0] - y);
    }
    //��� ��ҵ鿡 ���� ������ ��� ���ϱ�
    newW = newW * 2 / TRAININGMATRIX_DATA_M;
    return newW;
}

template<int wMatM, int wMatN>
double Cost(const array<array<double, wMatN>, wMatM> wMatrix) {
    double cost = 0;
    double loss = 0;
    for (int m = 0; m < TRAININGMATRIX_DATA_M; m++) {
        for (int xN = 0; xN < TRAININGMATRIX_X_N; xN++)
        {
            loss += xMatrix[m][xN] * wMatrix[xN][0];
            //for (int yN = 0; yN < TRAININGMATRIX_Y_N; yN++) {
            //    //(����������) - ���䵥����
            //    loss -= yMatrix[m][yN];
            //}
        }
        loss -= yMatrix[m][0];
        cost += loss * loss;
        loss = 0;
    }
    cost /= TRAININGMATRIX_DATA_M;
    return cost;
}


int main()
{
    double learningRate = 0.1;
    std::cout << "Hello World!\n";

    int key = 0;
    bool programRunning = true;

    //�Ҽ��� ��� ����
    cout << fixed;
    //double�� �ִ� ���� ������ �Ҽ��� 15�ڸ����� ���
    //double�� ������ �Ҽ��� 6�ڸ����� ���
    cout.precision(6);

#pragma region Developer_Control
    do {
        if (_kbhit()) {
            key = _getch();
            switch (key) {

            case VK_a:
                learningRate *= 10;
                break;
            case VK_s:
                learningRate /= 10;
                break;
            case VK_u:
                wMatrix[0][0] += learningRate;
                break;
            case VK_j:
                wMatrix[0][0] -= learningRate;
                break;
            case VK_i:
                wMatrix[1][0] += learningRate;
                break;
            case VK_k:
                wMatrix[1][0] -= learningRate;
                break;
            case VK_o:
                wMatrix[2][0] += learningRate;
                break;
            case VK_l:
                wMatrix[2][0] -= learningRate;
                break;
            case VK_ENTER:
                wMatrix = {
                    0,
                    0,
                    0
                };
                break;
            case VK_ESC:
                programRunning = false;
                break;
            }
            system("cls");

            std::cout << "-------------------------------------------------" << endl;
            std::cout << "LearningRate : " << learningRate << endl;
            std::cout << "w1 = " << wMatrix[0][0] << ", w2 = " << wMatrix[1][0] << ", w3 = " << wMatrix[2][0] << endl;
            std::cout << "w1 ��̺� : " << wUpdate(wMatrix, 0) << endl;
            std::cout << "w2 ��̺� : " << wUpdate(wMatrix, 1) << endl;
            std::cout << "w3 ��̺� : " << wUpdate(wMatrix, 2) << endl;
            std::cout << "Cost(w1, w2, w3) ��� : " << Cost(wMatrix) << endl;
            std::cout << "-------------------------------------------------" << endl;
        }
    } while (programRunning);
#pragma endregion
}
