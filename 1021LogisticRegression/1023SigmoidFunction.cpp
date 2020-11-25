//#include <iostream>
//#include <array>
//#include <conio.h> //kbhit, getch
//
//#define TRAININGDATA_NUM 15
//#define VARIABLE_NUM 2 //�ӽŷ��� ���� ���� ����(���ؾ��� ���� ����)
//using namespace std;
//
//#define ESC 27
//#define LEFT 75
//#define RIGHT 77
//#define UP 72
//#define DOWN 80
//#define ENTER 13
//#define A 65
//#define S 83
//#define D 68
//#define D 68
//#define F 70
//
//typedef struct datas {
//    double x;
//    double y;
//}TrainingDataSet;
//
//array<TrainingDataSet, TRAININGDATA_NUM> dataSet = {
//         22, 0,
//         23, 0,
//         24, 0,
//         27, 0,
//         28, 0,
//         30, 0,
//         30, 0,
//         32, 0,
//         33, 0,
//         35, 1,
//         38, 0,
//         40, 0,
//         41, 1,
//         46, 0,
//         47, 0,
//};
//
///// <summary>
///// y = wx + b ���������Ŀ��� y���� �����Ѵ�.
///// </summary>
///// <param name="w">����ġ(����)</param>
///// <param name="x">����</param>
///// <param name="b">Y����</param>
///// <returns></returns>
//double GetYLinear(const double& w, const double& x, const double& b) {
//    return w * x + b;
//}
//
//double Sigmoid(const double& w, const double& x, const double& b) {
//    //exp�� ������ ��� ���� : exp(0) = 1, exp(2) = 100���� ���ڰ� ����� �� ���ϱ޼������� ����Ѵ�.
//    //�ݸ� exp(-2)�� �� 0.13���� ���ϱ޼������� 0�� ���������.
//    //���� exp�Լ��� ���ڷ� ������ �������� ���Ǹ� 0�� ��ȯ�ϸ�, 1�� �����ϰ� �ȴ�.
//    //��ǻ�� ���� double �������� �Ҽ��� 15�ڸ����� �����ϹǷ� ���㰪 -35���Ϻ��� �Ҽ��� 16�̻� �����Ⱚ�� ��ȯ�Ѵ�.
//    double expParam = -(GetYLinear(w, x, b));
//    //return 1 / (1 + std::exp((expParam <= -35) ? -35 : expParam));
//    return 1 / (1 + std::exp(expParam));
//}
//
////w�� ���� ��̺� �Լ�
//double wPartialDerivative(const double& w, const double& b) {
//    double newW = 0;
//    for (int i = 0; i < TRAININGDATA_NUM; i++)
//        newW += dataSet[i].x * (Sigmoid(w, dataSet[i].x, b) - dataSet[i].y);
//    newW = newW / TRAININGDATA_NUM;
//    return newW;
//}
//
////b�� ���� ��̺� �Լ�
//double bPartialDerivative(const double& w, const double& b) {
//    double newB = 0;
//    for (int i = 0; i < TRAININGDATA_NUM; i++)
//        newB += Sigmoid(w, dataSet[i].x, b) - dataSet[i].y;
//    newB = newB / TRAININGDATA_NUM;
//    return newB;
//}
//
////������ƽ ȸ���� Cost�Լ�
//double CrossEntropyCost(const double &w, const double &b) {
//    double cost = 0;
//    double sigmoid;
//    double temp;
//    for (int i = 0; i < TRAININGDATA_NUM; i++) {
//        temp = w * dataSet[i].x + b;
//        //CrossEntropy ������ ������, ����ȭ�� ������ ���
//        cost += log(1 + exp(temp)) - temp*dataSet[i].y;
//    }
//    cost /= TRAININGDATA_NUM;
//    return cost;
//}
//
//int main()
//{
//    double w = 0;
//    double b = 0;
//    double learningRate = 0.1;
//    std::cout << "\n\n cost = log(1 + e^(wx + b)) - (wx + b) * y  ������ƽ ȸ�� ���� �Ʒ�";
//    std::cout << "exp(-1) = " << exp(-1);
//
//    int key = 0;
//    bool programRunning = true;
//
//    //�Ҽ��� ��� ����
//    cout << fixed;
//    //double�� �ִ� ���� ������ �Ҽ��� 15�ڸ����� ���
//    //double�� ������ �Ҽ��� 6�ڸ����� ���
//    cout.precision(6);
//
//#pragma region Developer_Control
//    do {
//        if (_kbhit()) {
//            key = _getch();
//            switch (key) {
//            case UP:
//                w += learningRate;
//                break;
//            case DOWN:
//                w -= learningRate;
//                break;
//            case LEFT:
//                b -= learningRate;
//                break;
//            case RIGHT:
//                b += learningRate;
//                break;
//            case A:
//                learningRate *= 10;
//                break;
//            case S:
//                learningRate /= 10;
//                break;
//            case ENTER:
//                w = 0;
//                b = 0;
//                break;
//            case ESC:
//                programRunning = false;
//                break;
//            }
//            system("cls");
//
//            std::cout << "-------------------------------------------------" << endl;
//            std::cout << "w = " << w << ", b = " << b << endl;
//            std::cout << "w ��̺� : " << wPartialDerivative(w, b) << endl;
//            std::cout << "b ��̺� : " << bPartialDerivative(w, b) << endl;
//            std::cout << "CrossEntropyCost(w, b) ��� : " << CrossEntropyCost(w, b) << endl;
//            //std::cout << "Cost(w, b) ��� : " << MinSquareCost(w, b) << endl;
//            //std::cout << "Sigmoid(w, b) y = 1�϶� ��� : " << Sigmoid(w, dataSet[0].x, b) << endl;
//            //std::cout << "Sigmoid(w, b) y = 0�϶� ��� : " << Sigmoid(w, dataSet[TRAININGDATA_NUM-1].x, b) << endl;
//            std::cout << "-------------------------------------------------" << endl;
//        }
//    } while (programRunning);
//#pragma endregion
//}
