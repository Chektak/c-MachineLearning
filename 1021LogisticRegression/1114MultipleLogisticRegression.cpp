#include <iostream>
#include <array> 
//훈련 데이터 x데이터, Y 행렬의 행(m) 개수
#define TRAININGMATRIX_DATA_M 10
//훈련 데이터 X데이터 행렬의 열(n) 개수
#define TRAININGMATRIX_X_N 3
//훈련 데이터 Y행렬 열(n) 개수
#define TRAININGMATRIX_Y_N 1

using namespace std;

typedef struct datas {
    array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix;
    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> yMatrix;
}DataSet;

enum DATASEX {
    MEN = 0, GIRL = 1
};

class Machine
{
public:
    DataSet trainingDataSet;
    
    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> wMatrix;
    double cost;
    double learningRate;

    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> bestWMatrix;
    double bestCost;
    int sameCostStack;
public:
    Machine() : cost(INT_MAX), bestCost(INT_MAX), learningRate(0), wMatrix({ 0 }), trainingDataSet({ 0 }), bestWMatrix({0}), sameCostStack(0) {};

    void TrainingDataSetup(DATASEX sex = DATASEX::MEN) {
#pragma region 어른/아이 예측 훈련 데이터 설정(데이터 출처 : 질병관리본부 2018 소아 표준 신장)

        if (sex == DATASEX::MEN) {
            //x1: 몸무게, x2 : 키, x3 : 언어 수
            trainingDataSet.xMatrix = {
                73, 180, 2,
                90, 175, 2,
                80, 190, 2,
                65, 171, 3,
                50, 165, 2,
                35, 138, 1,
                19, 109, 1,
                24, 122, 1,
                40, 144, 1,
                50, 150, 1
            };
            //y1 : 아이 = 0, 성인 = 1 구분
            trainingDataSet.yMatrix = {
                1,
                1,
                1,
                1,
                1,
                0,
                0,
                0,
                0,
                0
            };
        }
        else {
            //x1 : 몸무게, x2 : 키, x3 : 언어 수
            trainingDataSet.xMatrix = {
                63, 170, 2,
                60, 165, 2,
                70, 180, 2,
                55, 161, 3,
                46, 163, 2,
                40, 138, 1,
                19, 109, 1,
                24, 122, 1,
                40, 144, 1,
                50, 160, 1
            };
            //y1 : 아이 = 0, 성인 = 1 구분
            trainingDataSet.yMatrix = {
                1,
                1,
                1,
                1,
                1,
                0,
                0,
                0,
                0,
                0
            };
        }
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
        else if (++sameCostStack % (TRAININGMATRIX_X_N * TRAININGMATRIX_Y_N * 1000) == 0) {
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
        //PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(wMatrix);
        cout << "["<< targetWIndex <<"]gradient : " << gradient << endl;
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
        *value += learningRate * dir;
#pragma endregion

#pragma region 변수 선택법 : Forward selection (매우 빠르지만 대충 학습해 다중공선성 문제 발생확률 높음)
                //while (true) {
                //    *value += dir * learningRate;

                //    gradient = WPartialDerivative(targetWIndex);
                //    gradient = roundl(gradient * 1000000) * 0.000001;

                //    //양의 방향으로 가야할 때, wGradient가 양수가 되면 종료한다. 음수 방향의 경우도 마찬가지
                //    if ((dir == 1 && gradient >= 0) || (dir == -1 && gradient <= 0)) {
                //        break;
                //    }
                //}
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
            cost -= trainingDataSet.yMatrix[i][0] * log(Sigmoid(hypothesis)) + (1 - trainingDataSet.yMatrix[i][0])*log(1 - Sigmoid(hypothesis));
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

    std::cout << "\n\n cost = -(1/m)Σ(y * log(Sigmoid(X)) + (1 - y)log(1-Sigmoid(X)))다중 로지스틱 회귀 문제 훈련" << endl;
    std::cout << "데이터의 성별(남성 : 0, 여성 : 1)을 설정해주세요. : ";
    int sexInt;
    cin >> sexInt;
    DATASEX sex = (DATASEX)sexInt;
    if (sex == DATASEX::MEN) {
        std::cout << "훈련 데이터는 남성 3세 ~ 성인으로 편향되었습니다." << endl;
    }
    else {
        std::cout << "훈련 데이터는 여성 3세 ~ 성인으로 편향되었습니다." << endl;
    }
    machine->TrainingDataSetup(sex);


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
    machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(machine->wMatrix);
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

            machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(machine->wMatrix);
            if (printCost <= 0)
                break;
        }
    }
    
#pragma endregion

    std::cout << "------------------------------------------------" << endl;
    std::cout << curRepeatNum << "번째 세대" << endl;
    std::cout << "BestCost : " << machine->bestCost << endl;
    std::cout << "BestWMatrix : " << endl;
    machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(machine->bestWMatrix);

    
    
    if(sex == DATASEX::MEN)
        std::cout << "데이터를 입력하면 (남성) 타겟의 성인/아이 유무를 예측합니다." << endl;
    else
        std::cout << "데이터를 입력하면 (여성) 타겟의 성인/아이 유무를 예측합니다." << endl;
    int yesOrNo = 1;
    while (yesOrNo == 1) {
        std::cout << "모델이 학습하지 못한 데이터율 : " << machine->bestCost * 100 << "%" << endl;

        int process = 0;
        int newData[3];
        while (process < 3)
        {
            switch (process) {
            case 0:
                std::cout << "  샘플의 몸무게를 입력해주세요 : ";
                break;
            case 1:
                std::cout << "  샘플의 키를 입력해주세요 : ";
                break;
            case 2:
                std::cout << "  샘플의 언어 수를 입력해주세요 : ";
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
            std::cout << "대상은 " << 100 / (1+logistic) << " 확률로 아이입니다." << endl;

        std::cout << "한번 더 예측하시겠습니까? (예 : 1, 아니오 : 0)" << endl;
        cin >> yesOrNo;
    }

    system("pause");
    return 0;
}