//#include <iostream>
//#include <array>
//#include <conio.h> //kbhit, getch
//
//#define TRAININGDATA_NUM 15
//#define VARIABLE_NUM 2 //머신러닝 모델의 변수 갯수(구해야할 변수 갯수)
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
///// y = wx + b 일차방정식에서 y값을 리턴한다.
///// </summary>
///// <param name="w">가중치(기울기)</param>
///// <param name="x">변수</param>
///// <param name="b">Y절편</param>
///// <returns></returns>
//double GetYLinear(const double& w, const double& x, const double& b) {
//    return w * x + b;
//}
//
//double Sigmoid(const double& w, const double& x, const double& b) {
//    //exp에 음수값 사용 이유 : exp(0) = 1, exp(2) = 100으로 인자가 양수일 때 기하급수적으로 상승한다.
//    //반면 exp(-2)은 약 0.13으로 기하급수적으로 0에 가까워진다.
//    //만약 exp함수의 인자로 무한한 음수값이 사용되면 0을 반환하며, 1을 리턴하게 된다.
//    //컴퓨터 언어에서 double 변수형은 소수점 15자리까지 보장하므로 인잣값 -35이하부터 소수점 16이상 쓰레기값을 반환한다.
//    double expParam = -(GetYLinear(w, x, b));
//    //return 1 / (1 + std::exp((expParam <= -35) ? -35 : expParam));
//    return 1 / (1 + std::exp(expParam));
//}
//
////w에 대한 편미분 함수
//double wPartialDerivative(const double& w, const double& b) {
//    double newW = 0;
//    for (int i = 0; i < TRAININGDATA_NUM; i++)
//        newW += dataSet[i].x * (Sigmoid(w, dataSet[i].x, b) - dataSet[i].y);
//    newW = newW / TRAININGDATA_NUM;
//    return newW;
//}
//
////b에 대한 편미분 함수
//double bPartialDerivative(const double& w, const double& b) {
//    double newB = 0;
//    for (int i = 0; i < TRAININGDATA_NUM; i++)
//        newB += Sigmoid(w, dataSet[i].x, b) - dataSet[i].y;
//    newB = newB / TRAININGDATA_NUM;
//    return newB;
//}
//
////로지스틱 회귀의 Cost함수
//double CrossEntropyCost(const double &w, const double &b) {
//    double cost = 0;
//    double sigmoid;
//    double temp;
//    for (int i = 0; i < TRAININGDATA_NUM; i++) {
//        temp = w * dataSet[i].x + b;
//        //CrossEntropy 수식을 전개해, 간략화한 수식을 사용
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
//    std::cout << "\n\n cost = log(1 + e^(wx + b)) - (wx + b) * y  로지스틱 회귀 문제 훈련";
//    std::cout << "exp(-1) = " << exp(-1);
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
//            std::cout << "w 편미분 : " << wPartialDerivative(w, b) << endl;
//            std::cout << "b 편미분 : " << bPartialDerivative(w, b) << endl;
//            std::cout << "CrossEntropyCost(w, b) 결과 : " << CrossEntropyCost(w, b) << endl;
//            //std::cout << "Cost(w, b) 결과 : " << MinSquareCost(w, b) << endl;
//            //std::cout << "Sigmoid(w, b) y = 1일때 결과 : " << Sigmoid(w, dataSet[0].x, b) << endl;
//            //std::cout << "Sigmoid(w, b) y = 0일때 결과 : " << Sigmoid(w, dataSet[TRAININGDATA_NUM-1].x, b) << endl;
//            std::cout << "-------------------------------------------------" << endl;
//        }
//    } while (programRunning);
//#pragma endregion
//}
