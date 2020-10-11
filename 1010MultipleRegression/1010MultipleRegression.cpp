#include <iostream>
#include <random>
#include <array> 
//훈련 데이터 x데이터, Y 행렬의 행(m) 개수
#define TRAININGMATRIX_DATA_M 5
//훈련 데이터 X데이터 행렬의 열(n) 개수
#define TRAININGMATRIX_X_N 3
//훈련 데이터 Y행렬 열(n) 개수
#define TRAININGMATRIX_Y_N 1

using namespace std;

// 시드값을 얻기 위한 random_device 생성.
random_device rd;
// random_device 를 통해 난수 생성 엔진을 초기화 한다.
mt19937 gen(rd());

class Weight {
public:
    double  value;
    double  bestValue;
    double  randomRate;

    int     accelStack;
    int     accelRate;
    int     sameCostStack;

    bool minDir = 0;
    bool maxDir = 1;

    Weight() : value(0), bestValue(0), accelStack(0), randomRate(10), accelRate(10), sameCostStack(0) {};

    void RandomInit() {
        uniform_real_distribution<double> dis(bestValue - (int)(minDir)*randomRate, bestValue + (int)(maxDir)*randomRate);
        value = dis(gen);
        if (value <= 0)
            value = 0;
    }

    void BestValueChange() {
        bestValue = value;
        ////6번째 연속으로 코스트 줄이기 실패가 없다면 변화폭을 늘린다
        //if (sameCostStack == 0 && ++accelStack % 6 == 0) {
        //    cout << "#### RamdomRate Accelerated!! " << randomRate << "->" << randomRate * accelRate << endl;
        //    randomRate *= accelRate;
        //    accelStack = 0;
        //}
        //같은 코스트 판별 스택 초기화
        sameCostStack = 0;
    }

    void BestValueNonChange() {

        //코스트 줄이기에 실패했으므로 다른 방향을 시도한다
        minDir = !minDir;
        maxDir = !maxDir;

        ////같은 cost가 반복되는 양상을 보이면 
        //if (++sameCostStack % 3 == 0)
        //{
        //    //변화폭을 소수점 이하로 줄인다
        //    cout << "#### RamdomRate Chenged!! " << randomRate << "->" << randomRate / accelRate << endl;
        //    randomRate /= accelRate;
        //    /*if (sameCostStack > 20) {
        //        cout << "예외 발생 :: 같은 코스트가 20번 이상 중복되었으므로 종료합니다." << endl;
        //        return -1;
        //    }*/
        //}

    }
    /// <summary>
    /// 이것의 value가 바뀌었을때 코스트를 반환
    /// </summary>
    /// <returns></returns>
    double GetPartialCost() {

    }
};

namespace Mathf {
    //행(m) : 세로 줄, 열(n) : 가로로 얼마나 퍼져있는지
    template<int mat1M, int mat1N, int mat2M, int mat2N>
    void MatrixProduct(const std::array<std::array<double, mat1N>, mat1M>& mat1,
        const std::array<std::array<Weight, mat2N>, mat2M>& mat2,
        std::array<std::array<double, mat2N>, mat1M>& resultMat) {

        if (mat1M <= 0 || mat1N <= 0 || mat2M <= 0 || mat2N <= 0)
        {
            cout << "Can't Product!!";
            return;
        }
        for (int z = 0; z < mat2N; z++) {
            //행
            for (int i = 0; i < mat1M; i++) {
                //열
                for (int j = 0; j < mat1N; j++) {
                    resultMat[i][z] += mat1[i][j] * mat2[j][z].value;
                }
            }
        }
    }
};



typedef struct datas {
    array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix;
    double b;
    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> yMatrix;
}DataSet;


class Machine
{
public:
    DataSet trainingDataSet;

    array<array<Weight, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> wMatrix;
    double bestCost;
    double cost;
    double b;

    double bRandomRate;
public:
    Machine() : bestCost(DBL_MAX), cost(DBL_MAX), b(0), bRandomRate(10) {};

    void TrainingDataSetup() {
        /*trainingDataSet.xMatrix = {
            73, 80, 75,
            93, 88, 93,
            89, 91, 90,
            96, 98, 100,
            73, 66, 70
        };
        trainingDataSet.yMatrix = {
            152,
            185,
            180,
            196,
            142
        };*/
        
        trainingDataSet.xMatrix = {
            100, 100, 100,
            100, 0, 0,
            0, 0, 100,
            0, 100, 0,
            100, 100, 0
        };
        trainingDataSet.yMatrix = {
            300,
            100,
            100,
            100,
            200
        };
    }
    /// <summary>
    /// </summary>
    /// <returns>cost를 반환한다.</returns>
    double Training(int curRepeatNum) {
        for (int i = 0; i < TRAININGMATRIX_X_N; i++)
            for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
                wMatrix[i][j].RandomInit();
                cost = Cost<TRAININGMATRIX_DATA_M, TRAININGMATRIX_X_N,
                    TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>();
                if (cost < bestCost) {
                    bestCost = cost;
                    wMatrix[i][j].BestValueChange();
                }
                else
                    wMatrix[i][j].BestValueNonChange();
            }
        /*cost = Cost<TRAININGMATRIX_DATA_M, TRAININGMATRIX_X_N,
            TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>();*/

        //코스트 줄이기에 성공했다면
        //if (cost < bestCost) {
        //
        //    bestCost = cost;
        //    for (int i = 0; i < TRAININGMATRIX_X_N; i++)
        //        for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
        //            wMatrix[i][j].BestValueChange();
        //        }
        //    return cost;
        //}
        /*for (int i = 0; i < TRAININGMATRIX_X_N; i++)
            for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
                wMatrix[i][j].BestValueNonChange();
            }*/
        return bestCost;
    }

    template<int xMatM, int xMatN, int wMatM, int wMatN>
    double Cost() {
        array<array<double, wMatN>, xMatM> tempYMatrix{ 0 };
        //tempYMatrix에 xMatrix와 wMatrix의 행렬곱을 받는다
        Mathf::MatrixProduct<xMatM, xMatN, wMatM, wMatN>(trainingDataSet.xMatrix, wMatrix, tempYMatrix);

        double cost = 0;
        for (int i = 0; i < xMatM; i++) {
            cost += Loss(tempYMatrix[i][0], trainingDataSet.yMatrix[i][0]);
        }
        cost /= xMatM;


        //cout << "\nx 데이터행렬 :: xMatrix" << endl;
        ////행
        //for (int i = 0; i < xMatM; i++) {
        //    //열
        //    for (int j = 0; j < xMatN; j++) {
        //        cout << trainingDataSet.xMatrix[i][j] << " ";
        //    }
        //    cout << endl;
        //}
        //
        //cout << "\nw 가중치 행렬 :: wMatrix" << endl;
        ////행
        //for (int i = 0; i < wMatM; i++) {
        //    //열
        //    for (int j = 0; j < wMatN; j++) {
        //        cout << wMatrix[i][j].bestValue << " ";
        //    }
        //    cout << endl;
        //}
        //
        //cout << "\n결과 행렬 :: tempYMatrix" << endl;
        ////행
        //for (int i = 0; i < xMatM; i++) {
        //    //열
        //    for (int j = 0; j < wMatN; j++) {
        //        cout << tempYMatrix[i][j] << " ";
        //    }
        //    cout << endl;
        //}
        //
        //cout << "\n정답 행렬 :: ResultMatrix" << endl;
        //for (int i = 0; i < xMatM; i++) {
        //    //열
        //    for (int j = 0; j < wMatN; j++) {
        //        cout << trainingDataSet.yMatrix[i][j] << " ";
        //    }
        //    cout << endl;
        //}


        return cost;
    }

    /// <summary>
    /// Loss : (가설 H = wx+b) - 정답데이터 y
    /// </summary>
    /// <returns></returns>
    double Loss(double hData, double yData) {
        //y = H(X) = XW + b에서 y 행렬를 반환한다. (XW는 행렬곱)

        double loss = hData - yData;
        loss *= loss;
        return loss;
    }
};

int main()
{
    Machine* machine = new Machine();
    machine->TrainingDataSetup();

    int trainingNum = 0;

    std::cout << "\n\nH(X) = WX + b 다변수 선형회귀 문제 훈련" << endl;

    std::cout << "훈련 횟수를 입력해주세요 : ";
    cin >> trainingNum;

   /* while (machine->wRandomRate == 0) {
        cout << "0을 제외한 학습률을 설정해주세요.";
        cout << " 숫자가 클수록 변동폭이 커지며 10 ~ 1000을 추천합니다 : ";
        cin >> machine->wRandomRate;
    }*/

    //소수점 고정
    std::cout << fixed;
    //double형 최대 보장 범위인 소수점 15자리 이상까지 출력
    std::cout.precision(15);

    //y절편은 기울기에 0을 곱하는 것으로 구할 수 있음
    //std::array<double, TRAIN> zeroSet;
    //zeroSet.fill(0);
    //machine->b = machine->GetH<TRAININGDATA_VARSIZE>(zeroSet, dataSet[0].xData, dataSet[0].b);

    for (int curRepeatNum = 0; curRepeatNum <= trainingNum; curRepeatNum++) {
        double bestCost = machine->Training(curRepeatNum);

        //100번마다 출력하거나, 코스트가 0이 되었을 때 출력
        if (curRepeatNum % 1 == 0 || bestCost <= 0) {
            std::cout << "------------------------------------------------" << endl;
            std::cout << curRepeatNum << "번째 세대" << endl;

            //std::cout << "BestW : " << bestW[0] << endl;
            //std::cout << "b : " << machine->b << endl;
            std::cout << "BestCost : " << bestCost << endl;
            std::cout << "BestWMatrix : " << endl;

            for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
                for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
                    std::cout << machine->wMatrix[i][j].bestValue << " ";
                }
                std::cout << endl;
            }
            if (bestCost == 0)
                break;
            else if (bestCost == -1) {
                std::cout << "마지막 BestCost : " << machine->bestCost << endl;
                break;
            }
        }
    }
    return 0;
}