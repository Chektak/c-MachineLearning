//#include <iostream>
//#include <array>
//#include <conio.h> //kbhit, getch
//
//#define TRAININGDATA_NUM 5
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
//
//using namespace std;
//
//typedef struct datas {
//    //double w;
//    double xData;
//    //double b;
//    double y;
//}TrainingDataSet;
//
//array<TrainingDataSet, TRAININGDATA_NUM> dataSet = {
//                                                     1, 1.1,
//                                                     2, 2.1,
//                                                     3, 3.1,
//                                                     4, 4.1,
//                                                     5, 5.1 };
///*
//array<TrainingDataSet, TRAININGDATA_NUM> dataSet = {
//                                                     1, 1.1,
//                                                     2, 2.1,
//                                                     3, 3.1,
//                                                     4, 4.1,
//                                                     5, 5.1 };*/
//
////array<TrainingDataSet, TRAININGDATA_NUM> dataSet = {
////                                                     4, 2,
////                                                     4, 10,
////                                                     7, 4,
////                                                     7, 22,
////                                                     8, 16,
////                                                     9, 10};
//
////w에 대한 편미분 함수
//double wUpdate(const double& w, const double& b) {
//    double newW = 0;
//    for (int i = 0; i < TRAININGDATA_NUM; i++)
//        newW += dataSet[i].xData * (dataSet[i].xData * w + b - dataSet[i].y);
//    newW = newW * 2 / TRAININGDATA_NUM;
//    return newW;
//}
//
////b에 대한 편미분 함수
//double bUpdate(const double& w, const double& b) {
//    double newB = 0;
//    for (int i = 0; i < TRAININGDATA_NUM; i++)
//        newB += dataSet[i].xData * w - dataSet[i].y + b;
//    newB = newB * 2 / TRAININGDATA_NUM;
//    return newB;
//}
//
//double Cost(const double& w, const double& b) {
//    double cost = 0;
//    double loss = 0;
//    for (int i = 0; i < TRAININGDATA_NUM; i++) {
//        //(가설데이터) - 정답데이터
//        loss = (w * dataSet[i].xData + b) - dataSet[i].y;
//        loss *= loss;
//        cost += loss;
//    }
//    cost /= TRAININGDATA_NUM;
//    return cost;
//}
//
//
//int main()
//{
//    double w = 0;
//    double b = 0;
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
//            case ENTER :
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
//            std::cout << "w 편미분 : " << wUpdate(w, b) << endl;
//            std::cout << "b 편미분 : " << bUpdate(w, b) << endl;
//            std::cout << "Cost(w, b) 결과 : " << Cost(w, b) << endl;
//            std::cout << "-------------------------------------------------" << endl;
//        }
//    } while (programRunning);
//#pragma endregion
//}
