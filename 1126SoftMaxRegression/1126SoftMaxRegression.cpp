#include <iostream>
#include <array> 

//제공된 데이터의 개수 (훈련 변수 데이터 x행렬의 행(m), 정답 데이터 Y 행렬의 행(m)으로 사용)
#define TRAININGMATRIX_DATA_M 5
//분류 클래스 개수 (훈련 가중치 데이터 w행렬의 열(n), 정답 데이터 Y 행렬의 열(n)로 사용)
#define TRAININGMATRIX_DATA_N 3
//확률에 관여하는 변수의 개수 (훈련 변수 데이터 x행렬의 열(n), w행렬 행(m)로 사용. 행렬곱이 성사되기 위한 조건)
#define TRAININGMATRIX_X_N 2

using namespace std;
enum GRADE { A = 0, B = 1, C = 2 };
typedef struct datas {
    array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix;
    array<array<int, TRAININGMATRIX_DATA_N>, TRAININGMATRIX_DATA_M> yMatrix;
}DataSet;

class Machine
{
public:
    DataSet trainingDataSet;

    array<array<double, TRAININGMATRIX_DATA_N>, TRAININGMATRIX_X_N> wMatrix;
    double cost;
    double learningRate;

    array<array<double, TRAININGMATRIX_DATA_N>, TRAININGMATRIX_X_N> bestWMatrix;
    double bestCost;
    int sameCostStack;
public:
    Machine() : cost(INT_MAX), bestCost(INT_MAX), learningRate(0), wMatrix({ 0 }), trainingDataSet({ 0 }), bestWMatrix({ 0 }), sameCostStack(0) {};

    void TrainingDataSetup() {
#pragma region 학점 분류 훈련 데이터 설정
            //x1: 학점, x2 : 출석일
            trainingDataSet.xMatrix = {
                10, 5,
                9, 5,
                3, 2,
                2, 4,
                11, 1
            };
            //y1 : 아이 = 0, 성인 = 1 구분
            trainingDataSet.yMatrix = {
                (int)GRADE::A,
                (int)GRADE::A,
                (int)GRADE::B,
                (int)GRADE::B,
                (int)GRADE::C
            };
#pragma endregion
    }

    /// <summary>
    /// </summary>
    /// <returns>cost를 반환한다.</returns>
    bool Training() {
        for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
            WUpdate(&wMatrix[i][0], i);
        }

        cost = Cost();

        //bestCost일 때 데이터를 백업해둔다
        if (cost < bestCost) {
            sameCostStack = 0;
            bestWMatrix = wMatrix;
            bestCost = cost;
        }
        //bestCost가 w 기울기 변수 갯수 * 10번 이상 발생하지 않을 경우
        else if (++sameCostStack % (TRAININGMATRIX_X_N * TRAININGMATRIX_DATA_N * 1000) == 0) {
            return false;
        }
        return true;
    }

    double Sigmoid(double hypothesis) {
        return 1 / (1 + exp(hypothesis));
    }
    //value를 업데이트한다.
    void WUpdate(double* value, const int& targetWIndex) {
        double gradient;

        gradient = WPartialDerivative(targetWIndex);
        gradient = roundl(gradient * 1000000) * 0.000001;
        //PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_DATA_N>(wMatrix);
        cout << "[" << targetWIndex << "]gradient : " << gradient << endl;
        int dir = 1;

        //편미분 방향 검출
        if (gradient > 0)
            dir = -1;
        else if (gradient < 0)
            dir = 1;
        else
            return; //편미분 결과가 0이면 나아갈 방향이 없으므로 종료

        //노이즈가 많은 현실 데이터의 경우 왠만하면 All possibe regressions을 사용함
#pragma region 변수 선택법 : All possible regressions(느리지만 정확히 학습해 다중공선성 문제 발생확률 낮음)
        * value += learningRate * dir;
#pragma endregion

        return;

    }

    /// <summary>
    /// targetW 요소의 편미분 함수(기울기를 반환한다)
    /// </summary>
    /// <param name="wMatrix">이 함수는 wMatrix의 열이 0인 경우에만 사용가능하다.</param>
    /// <param name="targetWIndex">w1이 타겟일 경우 0을 인자로 전달한다.</param>
    /// <returns></returns>
    template<int xMatM = TRAININGMATRIX_DATA_M, int xMatN = TRAININGMATRIX_X_N>
    double WPartialDerivative(int targetWIndex) {
        double gradient = 0;
        for (int i = 0; i < xMatM; i++) {
            double hypothesis = 0;
            for (int j = 0; j < xMatN; j++) {
                hypothesis += trainingDataSet.xMatrix[i][j] * wMatrix[j][0];
            }
            gradient += trainingDataSet.xMatrix[i][targetWIndex] *
                (Sigmoid(hypothesis) - trainingDataSet.yMatrix[i][0]);
        }
        gradient /= -xMatM;

        return gradient;
    }

    template<int xMatM = TRAININGMATRIX_DATA_M, int xMatN = TRAININGMATRIX_X_N>
    double Cost() {
        double cost = 0;
        for (int i = 0; i < xMatM; i++) {
            double hypothesis = 0;
            for (int j = 0; j < xMatN; j++) {
                hypothesis += trainingDataSet.xMatrix[i][j] * wMatrix[j][0];
            }
            cost -= trainingDataSet.yMatrix[i][0] * log(Sigmoid(hypothesis)) + (1 - trainingDataSet.yMatrix[i][0]) * log(1 - Sigmoid(hypothesis));
        }
        cost /= xMatM;
        return cost;
    }

    template<int MatM, int MatN>
    void PrintMatrix(array<array<double, MatN>, MatM> matrix) {
        for (int i = 0; i < MatM; i++) {
            for (int j = 0; j < MatN; j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << endl;
        }
    }
};

int main()
{
    Machine* machine = new Machine();
    int trainingNum = 0;

    std::cout << "\n\n 다변수 데이터 다중 클래스 분류 예측 훈련" << endl;
    machine->TrainingDataSetup();


    std::cout << "훈련 횟수를 입력해주세요(추천값 : 100 이상) : ";
    cin >> trainingNum;

    while (machine->learningRate == 0) {
        cout << "0을 제외한 학습률을 설정해주세요.(추천값 : 0.001 이하)" << endl;
        cout << " 숫자가 클수록 학습 변동폭이 커지고, 작을수록 학습 정확도가 높아집니다. : ";
        cin >> machine->learningRate;
    }

    //소수점 고정
    std::cout << fixed;
    //double형 최대 보장 범위인 소수점 15자리 이상까지 출력
    std::cout.precision(15);

    double printCost = -1;
    printCost = machine->cost;
    std::cout << "------------------------------------------------" << endl;
    std::cout << "0번째 세대" << endl;
    std::cout << "BestCost : " << printCost << endl;
    std::cout << "BestWMatrix : " << endl;
    machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_DATA_N>(machine->wMatrix);
    std::cout << "------------------------------------------------" << endl;
#pragma region 훈련 시작

    int curRepeatNum = 1;
    for (; curRepeatNum <= trainingNum; curRepeatNum++) {
        if (machine->Training() == false) {
            cout << "코스트 개선이 변수 갯수 * 10번 실패, 최적해이므로 훈련 종료" << endl;
            break;
        }
        printCost = machine->cost;

        //소수점 15자리 이상까지 반올림
        printCost = roundl(printCost * 100000000000000) * 0.00000000000001;

        //100번마다 출력하거나, 코스트가 0이 되었을 때 출력
        if (curRepeatNum % 1 == 0 || printCost <= 0) {
            std::cout << "------------------------------------------------" << endl;
            std::cout << curRepeatNum << "번째 세대" << endl;

            std::cout << "cost : " << printCost << endl;
            std::cout << "wMatrix : " << endl;

            machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_DATA_N>(machine->wMatrix);
            if (printCost <= 0)
                break;
        }
    }

#pragma endregion

    std::cout << "------------------------------------------------" << endl;
    std::cout << curRepeatNum << "번째 세대" << endl;
    std::cout << "BestCost : " << machine->bestCost << endl;
    std::cout << "BestWMatrix : " << endl;
    machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_DATA_N>(machine->bestWMatrix);


    std::cout << "데이터를 입력하면 (남성) 타겟의 성인/아이 유무를 예측합니다." << endl;
    int yesOrNo = 1;
    while (yesOrNo == 1) {
        std::cout << "모델이 학습하지 못한 데이터율 : " << machine->bestCost * 100 << "%" << endl;

        int process = 0;
        int newData[2];
        while (process < 2)
        {
            switch (process) {
            case 0:
                std::cout << "  샘플의 학점 입력해주세요 : ";
                break;
            case 1:
                std::cout << "  샘플의 출석일을 입력해주세요 : ";
                break;
            }
            cin >> newData[process];
            process++;
        }

        double hypothesis = 0;
        for (int j = 0; j < TRAININGMATRIX_X_N; j++) {
            hypothesis += newData[j] * machine->bestWMatrix[j][0];
        }
        double logistic = machine->Sigmoid(hypothesis);
        std::cout << "logistic : " << (logistic) << "." << endl;
        if ((logistic > 0.5))
            //std::cout << "대상은 " << (logistic - 0.5) * 200 << " 확률로 어른입니다." << endl;
            std::cout << "대상은 " << logistic * 100 << " 확률로 어른입니다." << endl;
        else
            //std::cout << "대상은 " << 100 / (1 + logistic) << " 확률로 아이입니다." << endl;
            std::cout << "대상은 " << 100 / (1 + logistic) << " 확률로 아이입니다." << endl;

        std::cout << "한번 더 예측하시겠습니까? (예 : 1, 아니오 : 0)" << endl;
        cin >> yesOrNo;
    }

    system("pause");
    return 0;
}