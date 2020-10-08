#include <iostream>
#include <random>
#include <array>
//훈련 데이터 x데이터, Y 행렬의 행(m) 개수
#define TRAININGMATRIX_DATA_M 5
//훈련 데이터 X데이터 행렬의 열(n) 개수
#define TRAININGMATRIX_X_N 3
//훈련 데이터 Y행렬 열(n) 개수
#define TRAININGMATRIX_Y_N 1

#define RANDOMRATE 50
using namespace std;

namespace Mathf {
    //행(m) : 세로 줄, 열(n) : 가로로 얼마나 퍼져있는지
    template<int mat1M, int mat1N, int mat2M, int mat2N>
    void MatrixProduct(const std::array<std::array<double, mat1N>, mat1M>& mat1,
        const std::array<std::array<double, mat2N>, mat2M>& mat2,
        std::array<std::array<double, mat2N>, mat1M>& resultMat) {

        std::array<std::array<double, mat2N>, mat1M> resultMat = new std::array<std::array<double, mat2N>, mat1M>;
        if (mat1M <= 0 || mat1N <= 0 || mat2M <= 0 || mat2N <= 0)
        {
            cout << "Can't Product!!";
            return resultMat;
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
        return resultMat;
    }
};

// 시드값을 얻기 위한 random_device 생성.
random_device rd;
// random_device 를 통해 난수 생성 엔진을 초기화 한다.
mt19937 gen(rd());

typedef struct datas {
    array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix;
    double b;
    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> yMatrix;
}DataSet;

class Machine
{
public:
    DataSet trainingDataSet;

    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> bestW;
    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> wMatrix;
    double bestCost;
    double cost;
    double b;

    int sameCostStack;

    double wRandomRate;
    double bRandomRate;


public:
    Machine() : bestW({ 0 }), wMatrix({ 0 }), bestCost(DBL_MAX), cost(DBL_MAX), b(0), sameCostStack(0), wRandomRate(RANDOMRATE), bRandomRate(RANDOMRATE) {};

    void TrainingDataSetup() {
        trainingDataSet.xMatrix = {
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
        };
    }
    /// <summary>
    /// </summary>
    /// <returns>cost를 반환한다.</returns>
    double Training(int learningRate, int curRepeatNum) {
        uniform_real_distribution<double> dis(-1, 1);

        for (int i = 0; i < TRAININGMATRIX_Y_N; i++)
            for(int j=0; j < TRAININGMATRIX_X_N; j++)
                wMatrix[i][j] = dis(gen);
        cost = Cost();

        //코스트 줄이기에 성공했다면
        if (cost < bestCost) {
            bestW = wMatrix;
            bestCost = cost;
            //같은 코스트 판별 스택 초기화
            sameCostStack = 0;

            return cost;
        }
        //같은 cost가 반복되는 양상을 보이면 
        else if (++sameCostStack % 100 == 0 && learningRate != 0)
        {
            //가속하여 하강한다.
            std::cout << "#### RamdomRate Chenged!! " << wRandomRate << "->" << wRandomRate / learningRate << endl;
            wRandomRate /= learningRate;
            if (sameCostStack > 550) {
                std::cout << "예외 발생 :: 같은 코스트가 550번 이상 중복되었으므로 종료합니다." << endl;
                return -1;
            }
        }

        return bestCost;
    }

    double Cost() {
        double cost = 0;
        //for (int i = 0; i < TRAININGMATRIX_DATA_M; i++) {
        //    cost += Loss<>(i);
        //}
        //cost /= TRAININGMATRIX_DATA_M;
        cost = Loss<TRAININGMATRIX_DATA_M, TRAININGMATRIX_X_N, 
            TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>();
        return cost;
    }

    /// <summary>
    /// Loss : (가설 H = wx+b) - 정답데이터 y
    /// </summary>
    /// <returns></returns>
    template<int xMatM, int xMatN, int wMatM, int wMatN>
    double Loss() {
        //y = H(X) = XW + b에서 y 행렬를 반환한다. (XW는 행렬곱)
        array<array<double, wMatN>, xMatM> tempYMatrix;
        Mathf::MatrixProduct<xMatM, xMatN, wMatM, wMatN>(trainingDataSet.xMatrix, wMatrix, tempYMatrix);
        
        double hData = 0;
        double yData = 0;
        for (int i = 0; i < xMatM; i++) {
            hData += tempYMatrix[i][0];
            yData += trainingDataSet.yMatrix[i][0];
        }

        cout << "\n결과 행렬 :: tempYMatrix" << endl;
        //행
        for (int i = 0; i < xMatM; i++) {
            //열
            for (int j = 0; j < xMatN; j++) {
                cout << tempYMatrix[i][j] << " ";
            }
            cout << endl;
        }
        
        cout << "\n정답 행렬 :: ResultMatrix" << endl;
        for (int i = 0; i < wMatM; i++) {
            //열
            for (int j = 0; j < wMatN; j++) {
                cout << trainingDataSet.yMatrix[i][j] << " ";
            }
            cout << endl;
        }
        double loss = hData - yData;
        cout << loss << endl;
        loss *= loss;
        return loss;
    }

    /// <summary>
    /// y = H(X) = XW + b에서 y 행렬를 반환한다. (XW는 행렬곱)
    /// </summary>
    /// <param name="wMatrix">가중치 행렬</param>
    /// <returns></returns>
   /* template<int mSize, int nSize>
    double GetHMatrix(array<array<double, nSize>, mSize>& yMatrix, int index) {
        return Mathf::MatrixProduct<size>(xArr, yMatrix) + b;
    }*/
};

int main()
{
    Machine* machine = new Machine();
    machine->trainingDataSet = {
        //xMatrix 초기화
        {
            73, 80, 75,
            93, 88, 93,
            89, 91, 90,
            96, 98, 100,
            73, 66, 70
        },
        //b
        0,
        {
            152,
            185,
            180,
            196,
            142
        }
        //yMatrix 초기화
    };

    int trainingNum = 0;
    int learningRate = 0;

    std::cout << "\n\nH(X) = WX + b 다변수 선형회귀 문제 훈련" << endl;

    DataSet * dataSet = &machine->trainingDataSet;
    std::cout << "훈련 횟수를 입력해주세요 : ";
    cin >> trainingNum;

    std::cout << "학습률을 설정해주세요.";
    std::cout << " 숫자가 클수록 미세해지며 100 ~ 1000을 추천합니다 : ";
    cin >> learningRate;

    //소수점 고정
    std::cout << fixed;
    //double형 최대 보장 범위인 소수점 15자리 이상까지 출력
    std::cout.precision(15);
    machine->Cost();
    //double yData = 0;
    //for (int i = 0; i < TRAININGMATRIX_DATA_M; i++) {
    //    yData += machine->trainingDataSet.yMatrix[i][0];
    //}
    //cout << yData << endl;
    //y절편은 기울기에 0을 곱하는 것으로 구할 수 있음
    //std::array<double, TRAIN> zeroSet;
    //zeroSet.fill(0);
    //machine->b = machine->GetH<TRAININGDATA_VARSIZE>(zeroSet, dataSet[0].xData, dataSet[0].b);
    
    //for (int curRepeatNum = 0; curRepeatNum <= trainingNum; curRepeatNum++) {
    //    double bestCost = machine->Training(learningRate, curRepeatNum);
    //    array<double, TRAININGDATA_VARSIZE> bestW = machine->bestW;
    //
    //    //소수점 15자리 이상까지 반올림
    //    //bestCost = roundl(bestCost * 100000000000000) * 0.00000000000001;
    //    //w의 소수점 자릿수가 6 이하일 때 cost 소수점 15자리 이하에 영향을 주므로 6자리에서 반올림
    //    //bestW = roundl(bestW * 1000000) * 0.000001;
    //
    //    //100번마다 출력하거나, 코스트가 0이 되었을 때 출력
    //    if (curRepeatNum % 100 == 0 || bestCost <= 0) {
    //        std::cout << "------------------------------------------------" << endl;
    //        std::cout << curRepeatNum << "번째 세대" << endl;
    //
    //        std::cout << "BestW : " << bestW[0] << endl;
    //        std::cout << "b : " << machine->b << endl;
    //        std::cout << "BestCost : " << bestCost << endl;
    //
    //        if (bestCost == 0)
    //            break;
    //        else if (bestCost == -1) {
    //            std::cout << "마지막 BestCost : " << machine->bestCost << endl;
    //            break;
    //        }
    //    }
    //}
    return 0;
}