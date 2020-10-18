#include <iostream>
#include <array>

#define TRAININGDATA_NUM 5
#define VARIABLE_NUM 2 //머신러닝 모델의 변수 갯수(구해야할 변수 갯수)
using namespace std;

typedef struct datas {
    double x;
    double y;
}TrainingDataSet;

class Machine
{
public:
    double w;
    double b;
    double wGradient;
    double bGradient;
    
    int wDir;
    int bDir;

    array<TrainingDataSet, TRAININGDATA_NUM> dataSet;

    double cost;
    double learningRate;

    int sameCostStack;
    int accelRate;
public:
    Machine() : w(10), b(10), wDir(-1), bDir(-1), 
        bGradient(DBL_MAX), wGradient(DBL_MAX), cost(DBL_MAX),  learningRate(0),
    sameCostStack(0), accelRate(10){};

    /// <summary>
    /// </summary>
    /// <returns>cost를 반환한다.</returns>
    double Training(int curRepeatNum) {
        wUpdate();
        bUpdate();

        cost = Cost();
        return cost;
    }

    bool wUpdate() {
        wGradient = wPartialDerivative(w, b);
#pragma region 편미분 방향 검출
        if (wGradient > 0)
            wDir = -1;
        else if (wGradient < 0)
            wDir = 1;
        else
            return true;
#pragma endregion

        while (true) {
            w += wDir * learningRate;
            wGradient = wPartialDerivative(w, b);

            //양의 방향으로 가야할 때, wGradient가 양수가 되면 종료한다. 음수 방향의 경우도 마찬가지
            if ((wDir == 1 && wGradient >= 0) || (wDir == -1 && wGradient <= 0)) {
                break;
            }
        }
        return false;
    }

    bool bUpdate() {
        bGradient = bPartialDerivative(w, b);
#pragma region 편미분 방향 검출
        if (bGradient > 0)
            bDir = -1;
        else if (bGradient < 0)
            bDir = 1;
        else
            return true;
#pragma endregion

        while (true) {
            b += bDir * learningRate;
            bGradient = bPartialDerivative(w, b);

            if ((bDir == 1 && bGradient >= 0) || (bDir == -1 && bGradient <= 0)) {
                break;
            }
        }
        return false;
    }
    //w에 대한 편미분 함수
    double wPartialDerivative(const double& w, const double& b) {
        double newW = 0;
        for (int i = 0; i < TRAININGDATA_NUM; i++)
            newW += dataSet[i].x * (dataSet[i].x * w + b - dataSet[i].y);
        newW = newW * 2 / TRAININGDATA_NUM;
        return newW;
    }

    //b에 대한 편미분 함수
    double bPartialDerivative(const double& w, const double& b) {
        double newB = 0;
        for (int i = 0; i < TRAININGDATA_NUM; i++)
            newB += dataSet[i].x * w - dataSet[i].y + b;
        newB = newB * 2 / TRAININGDATA_NUM;
        return newB;
    }

    double Cost() {
        double cost = 0;
        for (int i = 0; i < TRAININGDATA_NUM; i++) {
            cost += Loss(dataSet[i].x, dataSet[i].y);
        }
        cost /= TRAININGDATA_NUM;
        return cost;
    }

    /// <summary>
    /// Loss : (가설 H = wx+b) - 정답데이터 Y 
    /// </summary>
    /// <param name="xInput"></param>
    /// <param name="yTestData"></param>
    /// <returns></returns>
    double Loss(double xInput, double yTestData) {
        double loss = GetYLinear(w, xInput, b) - yTestData;
        loss *= loss;
        return loss;
    }

    /// <summary>
    /// y = wx + b 일차방정식에서 y값을 리턴한다.
    /// </summary>
    /// <param name="w">가중치(기울기)</param>
    /// <param name="x">변수</param>
    /// <param name="b">Y절편</param>
    /// <returns></returns>
    double GetYLinear(double w, double x, double b) {
        return w * x + b;
    }
};

int main()
{
    Machine* machine = new Machine();

    int trainingNum = 0;

    cout << "\n\ny = wx + b 1차방정식 선형회귀 문제 훈련(방향성을 가진 편미분 기반)" << endl;

    //시험 성적 훈련
    //------------- y = wx + b 식에서                       x시간 y점수가 주어짐
    //밑 데이터 셋에서는 w가 1, b가 1.1일 때 f(x)= y 데이터 성립
    array<TrainingDataSet, TRAININGDATA_NUM> dataSet = { 
                                                            1, 1.1,
                                                            2, 2.1,
                                                            3, 3.1,
                                                            4, 4.1,
                                                            5, 5.1 };
    machine->dataSet = dataSet;

    cout << "훈련 횟수를 입력해주세요 : ";
    cin >> trainingNum;

    while (machine->learningRate == 0) {
        cout << "0을 제외한 학습률을 설정해주세요.";
        cout << " 숫자가 클수록 변동폭이 커지며 1 이하를 추천합니다 : ";
        cin >> machine->learningRate;
    }

    //소수점 출력 고정
    cout << fixed;
    //double형 최대 보장 범위인 소수점 15자리까지 출력
    cout.precision(10);

    double printCost = 0;
    double printW = 0;
    double printB = 0;
    for (int curRepeatNum = 0; curRepeatNum <= trainingNum; curRepeatNum++) {
        printCost = machine->Training(curRepeatNum);

        //25번마다 출력하거나, 코스트가 0이 되었을 때 출력
        if (curRepeatNum % 25 == 0 || printCost <= 0) {
            printW = machine->w;
            printB = machine->b;

            //소수점 15자리 이상까지 반올림
            printCost = roundl(printCost * 100000000000000) * 0.00000000000001;
            //w의 소수점 자릿수가 6 이하일 때 cost 소수점 15자리 이하에 영향을 주므로 6자리에서 반올림
            printW = roundl(printW * 1000000) * 0.000001;
            printB = roundl(printB * 1000000) * 0.000001;

            cout << "------------------------------------------------" << endl;
            cout << curRepeatNum << "번째 세대" << endl;
            cout << "BestW : " << printW << endl;
            cout << "b : " << printB << endl;
            cout << "BestCost : " << printCost << endl;

            if (printCost == 0) {
                cout << "------------------------------------------------" << endl;
                cout << curRepeatNum << "번째 세대" << endl;
                cout << "BestW : " << printW << endl;
                cout << "b : " << printB << endl;
                cout << "BestCost : " << printCost << endl;

                break;
            }
        }
    }

    return 0;
}