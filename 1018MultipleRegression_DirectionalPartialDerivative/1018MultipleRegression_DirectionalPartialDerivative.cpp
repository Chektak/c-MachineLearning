//#include <iostream>
//#include <array> 
////훈련 데이터 x데이터, Y 행렬의 행(m) 개수
//#define TRAININGMATRIX_DATA_M 5
////훈련 데이터 X데이터 행렬의 열(n) 개수
//#define TRAININGMATRIX_X_N 3
////훈련 데이터 Y행렬 열(n) 개수
//#define TRAININGMATRIX_Y_N 1
//
//using namespace std;
//
//typedef struct datas {
//    array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix;
//    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> yMatrix;
//}DataSet;
//
//namespace Mathf {
//    //행(m) : 세로 줄, 열(n) : 가로로 얼마나 퍼져있는지
//    template<int mat1M, int mat1N, int mat2M, int mat2N>
//    void MatrixProduct(const std::array<std::array<double, mat1N>, mat1M>& mat1,
//        const std::array<std::array<double, mat2N>, mat2M>& mat2,
//        std::array<std::array<double, mat2N>, mat1M>& resultMat) {
//
//        if (mat1M <= 0 || mat1N <= 0 || mat2M <= 0 || mat2N <= 0)
//        {
//            cout << "Can't Product!!";
//            return;
//        }
//        for (int z = 0; z < mat2N; z++) {
//            //행
//            for (int i = 0; i < mat1M; i++) {
//                //열
//                for (int j = 0; j < mat1N; j++) {
//                    resultMat[i][z] += mat1[i][j] * mat2[j][z];
//                }
//            }
//        }
//    }
//};
//
//class Machine
//{
//public:
//    DataSet trainingDataSet;
//
//    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> wMatrix;
//    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> wGradientMatrix;
//    double cost;
//
//    double learningRate;
//
//
//    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> bestWMatrix;
//    double bestCost;
//    int sameCostStack;
//public:
//    Machine() : cost(INT_MAX), bestCost(DBL_MAX), learningRate(0), wMatrix({ 0 }), wGradientMatrix({ INT_MAX }) {};
//
//    void TrainingDataSetup() {
//#pragma region 훈련 데이터 설정
//        trainingDataSet.xMatrix = {
//            73, 80, 75,
//            93, 88, 93,
//            89, 91, 90,
//            96, 98, 100,
//            73, 66, 70
//        };
//        trainingDataSet.yMatrix = {
//            152,
//            185,
//            180,
//            196,
//            142
//        };
//
//        ////이 데이터에서 wMatrix는 1, 1, 1을 예측해야 함
//        //trainingDataSet.xMatrix = {
//        //    100, 100, 100,
//        //    100, 0, 0,
//        //    0, 0, 100,
//        //    0, 100, 0,
//        //    100, 100, 0
//        //};
//        //trainingDataSet.yMatrix = {
//        //    300,
//        //    100,
//        //    100,
//        //    100,
//        //    200
//        //};
//#pragma endregion
//    }
//
//    /// <summary>
//    /// </summary>
//    /// <returns>cost를 반환한다.</returns>
//    bool Training() {
//        for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
//             WUpdate(&wMatrix[i][0], i);
//        }
//
//        cost = Cost<TRAININGMATRIX_DATA_M, TRAININGMATRIX_X_N,
//            TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>();
//
//        //bestCost일 때 데이터를 백업해둔다
//        if (cost < bestCost) {
//            sameCostStack = 0;
//            bestWMatrix = wMatrix;
//            bestCost = cost;
//        }
//        //bestCost가 w 기울기 변수 갯수 * 10번 이상 발생하지 않을 경우
//        else if (++sameCostStack % (TRAININGMATRIX_X_N * TRAININGMATRIX_Y_N * 10) == 0) {
//            cost = bestCost;
//            wMatrix = bestWMatrix;
//            return false;
//        }
//        return true;
//    }
//
//    //value를 업데이트한다.
//    void WUpdate(double *value, const int& targetWIndex) {
//        double* gradient = &wGradientMatrix[targetWIndex][0];
//
//        *gradient = WPartialDerivative<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(wMatrix, targetWIndex);
//        *gradient = roundl(*gradient * 1000000) * 0.000001;
//
//        int dir = 1;
//
//        //편미분 방향 검출
//        if (*gradient > 0)
//            dir = -1;
//        else if (*gradient < 0)
//            dir = 1;
//        else
//            return; //편미분 결과가 0이면 나아갈 방향이 없으므로 종료
//
//        //노이즈가 많은 현실 데이터의 경우 왠만하면 All possibe regressions을 사용함
//#pragma region 변수 선택법 : All possible regressions(느리지만 다중공선성 문제 발생확률 낮음)
//        * value += dir * learningRate;
//#pragma endregion
//
//#pragma region 변수 선택법 : Forward selection (매우 빠르지만 다중공선성 문제 발생확률 높음)
////        while (true) {
////            *value += dir * learningRate;
////
////            *gradient = WPartialDerivative<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(wMatrix, targetWIndex);
////            *gradient = roundl(*gradient * 1000000) * 0.000001;
////            //양의 방향으로 가야할 때, wGradient가 양수가 되면 종료한다. 음수 방향의 경우도 마찬가지
////            if ((dir == 1 && *gradient >= 0) || (dir == -1 && *gradient <= 0)) {
////                break;
////            }
////        }
//#pragma endregion
//    }
//
//    /// <summary>
//    /// targetW 요소의 편미분 함수(기울기를 반환한다)
//    /// </summary>
//    /// <param name="wMatrix">이 함수는 wMatrix의 열이 0인 경우에만 사용가능하다.</param>
//    /// <param name="targetWIndex">w1이 타겟일 경우 0을 인자로 전달한다.</param>
//    /// <returns></returns>
//    template<int wMatM, int wMatN>
//    double WPartialDerivative(array<array<double, wMatN>, wMatM> wMatrix, const int& targetWIndex) {
//        double newW = 0;
//        for (int m = 0; m < TRAININGMATRIX_DATA_M; m++) {
//            newW += trainingDataSet.xMatrix[m][targetWIndex] * 
//                (trainingDataSet.xMatrix[m][0] * wMatrix[0][0] +
//                    trainingDataSet.xMatrix[m][1] * wMatrix[1][0] + 
//                    trainingDataSet.xMatrix[m][2] * wMatrix[2][0] - 
//                    trainingDataSet.yMatrix[m][0]);
//        }
//        //행렬 요소들에 대한 기울기의 평균 구하기
//        newW = newW * 2 / TRAININGMATRIX_DATA_M;
//        return newW;
//    }
//
//    template<int xMatM, int xMatN, int wMatM, int wMatN>
//    double Cost() {
//        array<array<double, wMatN>, xMatM> tempYMatrix{ 0 };
//        //tempYMatrix에 xMatrix와 wMatrix의 행렬곱을 받는다
//        Mathf::MatrixProduct<xMatM, xMatN, wMatM, wMatN>(trainingDataSet.xMatrix, wMatrix, tempYMatrix);
//
//        double cost = 0;
//        for (int i = 0; i < xMatM; i++) {
//            cost += Loss(tempYMatrix[i][0], trainingDataSet.yMatrix[i][0]);
//        }
//        cost /= xMatM;
//        return cost;
//    }
//
//    /// <summary>
//    /// Loss : (가설 H = wx+b) - 정답데이터 y
//    /// </summary>
//    /// <returns></returns>
//    double Loss(double hData, double yData) {
//        //y = H(X) = XW + b에서 y 행렬를 반환한다. (XW는 행렬곱)
//
//        double loss = hData - yData;
//        loss *= loss;
//        return loss;
//    }
//};
//
//int main()
//{
//    Machine* machine = new Machine();
//    machine->TrainingDataSetup();
//
//    int trainingNum = 0;
//
//    std::cout << "\n\nH(X) = XW + b 다변수 선형회귀 문제 훈련" << endl;
//
//    std::cout << "훈련 횟수를 입력해주세요 : ";
//    cin >> trainingNum;
//
//    while (machine->learningRate == 0) {
//        cout << "0을 제외한 학습률을 설정해주세요.";
//        cout << " 숫자가 클수록 변동폭이 커지며 1 이하를 추천합니다 : ";
//        cin >> machine->learningRate;
//    }
//
//#pragma region 훈련 출력
//    //소수점 고정
//    std::cout << fixed;
//    //double형 최대 보장 범위인 소수점 15자리 이상까지 출력
//    std::cout.precision(15);
//
//    double printCost = -1;
//
//    std::cout << "------------------------------------------------" << endl;
//    std::cout << "0번째 세대" << endl;
//    std::cout << "BestCost : " << printCost << endl;
//    std::cout << "BestWMatrix : " << endl;
//    for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
//        for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
//            std::cout << machine->wMatrix[i][j] << " ";
//        }
//        std::cout << endl;
//    }
//
//    int curRepeatNum = 1;
//    for (; curRepeatNum <= trainingNum; curRepeatNum++) {
//        if (machine->Training() == false) {
//            cout << "코스트 개선이 변수 갯수 * 10번 실패, 최적해이므로 훈련 종료" << endl;
//            break;
//        }
//        printCost = machine->cost;
//
//        //소수점 15자리 이상까지 반올림
//        printCost = roundl(printCost * 100000000000000) * 0.00000000000001;
//
//        //100번마다 출력하거나, 코스트가 0이 되었을 때 출력
//        if (curRepeatNum % 25 == 0 || printCost <= 0) {
//            std::cout << "------------------------------------------------" << endl;
//            std::cout << curRepeatNum << "번째 세대" << endl;
//
//            std::cout << "cost : " << printCost << endl;
//            std::cout << "wMatrix : " << endl;
//
//            for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
//                for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
//                    std::cout << machine->wMatrix[i][j] << " ";
//                }
//                std::cout << endl;
//            }
//            if (printCost <= 0)
//                break;
//        }
//    }
//    std::cout << "------------------------------------------------" << endl;
//    std::cout << curRepeatNum << "번째 세대" << endl;
//    std::cout << "BestCost : " << machine->bestCost << endl;
//    std::cout << "BestWMatrix : " << endl;
//    for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
//        for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
//            std::cout << machine->bestWMatrix[i][j] << " ";
//        }
//        std::cout << endl;
//    }
//#pragma endregion
//    system("pause");
//    return 0;
//}