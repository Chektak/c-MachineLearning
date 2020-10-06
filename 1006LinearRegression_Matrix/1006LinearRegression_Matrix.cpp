#include <iostream>
#include <random>
#include <array>
#define TRAININGDATA_NUM 3
#define TRAININGDATA_VARSIZE 3
#define RANDOMRATE 50
using namespace std;

namespace Mathf {
    template<int size>
    double MatrixProduct(array<double, size>& rowArr, array<double, size>& columnArr) {
        if (rowArr.empty() || columnArr.empty())
        {
            std::cout << "Can't Product!!";
            return 0;
        }
        double result = 0;
        for (int i = 0; i < size; i++) {
            result += rowArr[i] * columnArr[i];
        }
        return result;
    }
};

// 시드값을 얻기 위한 random_device 생성.
random_device rd;
// random_device 를 통해 난수 생성 엔진을 초기화 한다.
mt19937 gen(rd());

typedef struct datas {
    array<double, TRAININGDATA_VARSIZE> w;
    array<double, TRAININGDATA_VARSIZE> xData;
    double b;
    double y;
}TestDataSet;

class Machine
{
public:
    array<double, TRAININGDATA_VARSIZE> bestW;
    array<double, TRAININGDATA_VARSIZE> w;
    double bestCost;
    double cost;
    double b;

    int sameCostStack;

    double wRandomRate;
    double bRandomRate;

    TestDataSet trainingDataSet[TRAININGDATA_NUM] = 
    { {{1, 0, 1},  {73, 80, 75}, 0},
      {{1, 0, 1},  {93, 88, 93}, 0},
      {{1, 0, 1},  {89, 91, 90}, 0} };

public:
    Machine() : bestW({ 0 }), w({ 0 }), bestCost(DBL_MAX), cost(DBL_MAX), b(0), sameCostStack(0), wRandomRate(RANDOMRATE), bRandomRate(RANDOMRATE) {};

    /// <summary>
    /// </summary>
    /// <returns>cost를 반환한다.</returns>
    double Training(int learningRate, int curRepeatNum) {
        //uniform_real_distribution<double> dis(bestW - wRandomRate, bestW + wRandomRate);
        uniform_real_distribution<double> dis(-1, 1);

        //w = dis(gen);
        for (int i = 0; i < TRAININGDATA_VARSIZE; i++)
            w[i] = 0;
        cost = Cost();

        //코스트 줄이기에 성공했다면
        if (cost < bestCost) {bestW = w;
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
        for (int i = 0; i < TRAININGDATA_NUM; i++) {
            cost += Loss(trainingDataSet[i].xData, trainingDataSet[i].y);
        }
        cost /= TRAININGDATA_NUM;
        return cost;
    }

    /// <summary>
    /// Loss : (가설 H = wx+b) - 정답데이터 y
    /// </summary>
    /// <param name="xInput"></param>
    /// <param name="yTestData"></param>
    /// <returns></returns>
    template<int size>
    double Loss(array<double, size>& xInputArr, double yTestData) {
        double loss = GetH<size>(w, xInputArr, b) - yTestData;
        loss *= loss;
        return loss;
    }

    /// <summary>
    /// H(X) = XW + b에서 가설 H를 반환한다. (XW는 행렬곱)
    /// </summary>
    /// <param name="xArr">영향요인(x) 배열</param>
    /// <param name="wArr">가중치 배열</param>
    /// <param name="b">절편</param>
    /// <returns></returns>
    template<int size>
    double GetH(array<double, size>& xArr, array<double, size>& wArr, double b) {
        return Mathf::MatrixProduct<size>(xArr, wArr) + b;
    }

};

int main()
{
    Machine* machine = new Machine();
    machine->wRandomRate = RANDOMRATE;

    int trainingNum = 0;
    int learningRate = 0;

    std::cout << "\n\nH(X) = WX + b 다변수 선형회귀 문제 훈련" << endl;

    TestDataSet * dataSet = machine->trainingDataSet;
    //dataSet의 y는 자동으로 계산해 넣음
    for (int i = 0; i < TRAININGDATA_NUM; i++) {
        dataSet[i].y = machine->GetH(dataSet[i].w, dataSet[i].xData, dataSet[i].b);
        std::cout << "\tDataSet " << i + 1 <<endl;
        for (int j = 0; j < TRAININGDATA_VARSIZE; j++) {
            std::cout << "\t\t { w[" << j << "], x[" << j << "], b, Y } = ";
            std::cout << dataSet[i].w[j] << ", " << dataSet[i].xData[j];
            std::cout << ", " << dataSet[i].b << ", " << dataSet[i].y << endl;
        }
    }
    std::cout << "훈련 횟수를 입력해주세요 : ";
    cin >> trainingNum;

    std::cout << "학습률을 설정해주세요.";
    std::cout << " 숫자가 클수록 미세해지며 100 ~ 1000을 추천합니다 : ";
    cin >> learningRate;

    //소수점 고정
    std::cout << fixed;
    //double형 최대 보장 범위인 소수점 15자리 이상까지 출력
    std::cout.precision(15);

    //y절편은 기울기에 0을 곱하는 것으로 구할 수 있음
    std::array<double, TRAININGDATA_VARSIZE> zeroSet;
    zeroSet.fill(0);
    machine->b = machine->GetH<TRAININGDATA_VARSIZE>(zeroSet, dataSet[0].xData, dataSet[0].b);
    
    for (int curRepeatNum = 0; curRepeatNum <= trainingNum; curRepeatNum++) {
        double bestCost = machine->Training(learningRate, curRepeatNum);
        array<double, TRAININGDATA_VARSIZE> bestW = machine->bestW;
    
        //소수점 15자리 이상까지 반올림
        //bestCost = roundl(bestCost * 100000000000000) * 0.00000000000001;
        //w의 소수점 자릿수가 6 이하일 때 cost 소수점 15자리 이하에 영향을 주므로 6자리에서 반올림
        //bestW = roundl(bestW * 1000000) * 0.000001;
    
        //100번마다 출력하거나, 코스트가 0이 되었을 때 출력
        if (curRepeatNum % 100 == 0 || bestCost <= 0) {
            std::cout << "------------------------------------------------" << endl;
            std::cout << curRepeatNum << "번째 세대" << endl;
    
            std::cout << "BestW : " << bestW[0] << endl;
            std::cout << "b : " << machine->b << endl;
            std::cout << "BestCost : " << bestCost << endl;
    
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