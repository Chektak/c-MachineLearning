#include <iostream>
#include <random>
#include <array>

#define TRAININGDATA_NUM 5
using namespace std;

// 시드값을 얻기 위한 random_device 생성.
random_device rd;
// random_device 를 통해 난수 생성 엔진을 초기화 한다.
mt19937 gen(rd());

typedef struct datas {
    //double w;
    double xData;
    //double b;
    double y;
}TrainingDataSet;

class Machine
{
public:
    double bestW;
    double w;
    double bestWCost;
    double costW;

    bool minWDir = 0;
    bool maxWDir = 1;

    double bestB;
    double b;
    double bestBCost;
    double costB;
    
    bool minBDir = 0;
    bool maxBDir = 1;

    array<TrainingDataSet, TRAININGDATA_NUM> dataSet;

    int sameCostWStack;
    int sameCostBStack;
    int accelWStack;
    int accelBStack;

    double wRandomRate;
    double bRandomRate;
    int accelRate;

    
public:
    Machine() : bestW(0), w(0), bestWCost(DBL_MAX), costW(DBL_MAX), 
         bestB(0), b(0), bestBCost(DBL_MAX), costB(DBL_MAX), sameCostWStack(0), sameCostBStack(0), accelWStack(0), accelBStack(0), wRandomRate(0.1),
         bRandomRate(0.1), accelRate(10) {};

    /// <summary>
    /// </summary>
    /// <returns>cost를 반환한다.</returns>
    double Training(int curRepeatNum) {
        uniform_real_distribution<double> wDis(bestW-(int)(minWDir)*wRandomRate, bestW+(int)(maxWDir)*wRandomRate);
        uniform_real_distribution<double> bDis(bestB-(int)(minBDir)*bRandomRate, bestB+(int)(maxBDir)*bRandomRate);

        //double learningRate = dis(gen);
        //double learningRate = 0.001;
        w = wDis(gen);
        b = bDis(gen);

        costW = wUpdate(w, bestB);
        costW *= costW;
        if (costW < bestWCost) {
            bestWCost = costW;
            bestW = w;
        
                //6번째 연속으로 코스트 줄이기 실패가 없다면 변화폭을 늘린다
            /*if (sameCostWStack == 0 && ++accelWStack % 6 == 0) {
                cout << "#### RamdomRate Accelerated!! " << wRandomRate << "->" << wRandomRate * accelRate << endl;
                wRandomRate *= accelRate;
                accelWStack = 0;
            }*/
        
            sameCostWStack = 0;
        }
        else {
            minWDir = !minWDir;
            maxWDir = !maxWDir;
        
           //if (++sameCostWStack % 20 == 0)
           //{
           //    //변화폭을 소수점 이하로 줄인다
           //    cout << "#### RamdomRate Chenged!! " << wRandomRate << "->" << wRandomRate / accelRate << endl;
           //    wRandomRate /= accelRate;
           //    /*if (sameCostWStack > 100) {
           //        cout << "예외 발생 :: 같은 코스트가 20번 이상 중복되었으므로 종료합니다." << endl;
           //        return -1;
           //    }*/
           //}
        }

        costB = bUpdate(bestW, b);
        costB *= costB;
        if (costB < bestBCost) {
            bestBCost = costB;
            bestB = b;
        
            //6번째 연속으로 코스트 줄이기 실패가 없다면 변화폭을 늘린다
            /*if (sameCostBStack == 0 && ++accelBStack % 6 == 0) {
                cout << "#### RamdomRate Accelerated!! " << bRandomRate << "->" << bRandomRate * accelRate << endl;
                bRandomRate *= accelRate;
                accelBStack = 0;
            }*/
        
            sameCostBStack = 0;
        }
        else {
            minBDir = !minBDir;
            maxBDir = !maxBDir;
        
            //if (++sameCostBStack % 20 == 0)
            //{
            //    //변화폭을 소수점 이하로 줄인다
            //    cout << "#### RamdomRate Chenged!! " << bRandomRate << "->" << bRandomRate / accelRate << endl;
            //    bRandomRate /= accelRate;
            //   /* if (sameCostBStack > 100) {
            //        cout << "예외 발생 :: 같은 코스트가 20번 이상 중복되었으므로 종료합니다." << endl;
            //        return -1;
            //    }*/
            //}
        }


        //costW = Cost();

        ////코스트 줄이기에 성공했다면
        //if (costW < bestWCost) {
        //    bestW = w;
        //    bestB = b;
        //    bestWCost = costW;
        //
        //    
        //    //6번째 연속으로 코스트 줄이기 실패가 없다면 변화폭을 늘린다
        //    /*if (sameCostStack == 0 && ++accelStack % 6 == 0) {
        //        cout << "#### RamdomRate Accelerated!! " << wRandomRate << "->" << wRandomRate * accelRate << endl;
        //        wRandomRate *= accelRate;
        //        accelStack = 0;
        //    }*/
        //
        //    ////같은 코스트 판별 스택 초기화
        //    //sameCostStack = 0;
        //    return costW;
        //}

        //코스트 줄이기에 실패했으므로 다른 방향을 시도한다
        

        //같은 cost가 반복되는 양상을 보이면 
        //if (++sameCostStack % 3 == 0)
        //{
        //    //변화폭을 소수점 이하로 줄인다
        //    cout << "#### RamdomRate Chenged!! " << wRandomRate << "->" << wRandomRate / accelRate << endl;
        //    wRandomRate /= accelRate;
        //    //if (sameCostStack > 100) {
        //    //    cout << "예외 발생 :: 같은 코스트가 20번 이상 중복되었으므로 종료합니다." << endl;
        //    //    return -1;
        //    //}
        //}
        return bestWCost + bestBCost;
    }

    double wUpdate(const double &w, const double &b) {
        double newW = 0;
        for (int i = 0; i < TRAININGDATA_NUM; i++)
            newW += dataSet[i].xData * (dataSet[i].xData * w + b - dataSet[i].y);
        newW = newW * 2 / TRAININGDATA_NUM;
        return newW;
    }
    
    double bUpdate(const double &w, const double &b) {
        double newB = 0;
        for (int i = 0; i < TRAININGDATA_NUM; i++)
            newB += dataSet[i].xData * w - dataSet[i].y + b;
        newB = newB * 2 / TRAININGDATA_NUM;
        return newB;
    }

    double Cost() {
        double cost = 0;
        for (int i = 0; i < TRAININGDATA_NUM; i++) {
            cost += Loss(dataSet[i].xData, dataSet[i].y);
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
        double loss = GetYLinear(bestW, xInput, bestB) - yTestData;
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

    cout << "\n\ny = wx + b 1차방정식 선형회귀 문제 훈련" << endl;

    //시험 성적 훈련
    //------------- y = wx + b 식에서                       x시간 y점수가 주어짐
    /*array<TrainingDataSet, TRAININGDATA_NUM> dataSet = {    2, 25,
                                                            3, 50,
                                                            4, 42,
                                                            5, 61 };*/
    ///w가 1, b가 1.1일 때 f(x)= y 데이터 성립
    array<TrainingDataSet, TRAININGDATA_NUM> dataSet = {    1, 1.1,
                                                            2, 2.1,
                                                            3, 3.1,
                                                            4, 4.1,
                                                            5, 5.1 };
    /*array<TrainingDataSet, TRAININGDATA_NUM> dataSet = {    2, 20,
                                                            3, 30,
                                                            4, 40,
                                                            5, 50 };*/
    machine->dataSet = dataSet;

    cout << "훈련 횟수를 입력해주세요 : ";
    cin >> trainingNum;

    while (machine->wRandomRate == 0) {
        cout << "0을 제외한 학습률을 설정해주세요.";
        cout << " 숫자가 클수록 변동폭이 커지며 10 ~ 1000을 추천합니다 : ";
        cin >> machine->wRandomRate;
    }

    //소수점 출력 고정
    cout << fixed;
    //double형 최대 보장 범위인 소수점 15자리까지 출력
    cout.precision(15);

    double printCost = 0;
    double printW = 0;
    double printB = 0;
    for (int curRepeatNum = 0; curRepeatNum <= trainingNum; curRepeatNum++) {
        printCost = machine->Training(curRepeatNum);

        //25번마다 출력하거나, 코스트가 0이 되었을 때 출력
        if (curRepeatNum % 25 == 0 || printCost <= 0) {
            printW = machine->bestW;
            printB = machine->bestB;

            //소수점 15자리 이상까지 반올림
            printCost = roundl(printCost * 100000000000000) * 0.00000000000001;
            //w의 소수점 자릿수가 6 이하일 때 cost 소수점 15자리 이하에 영향을 주므로 6자리에서 반올림
            printW = roundl(printW * 1000000) * 0.000001;
            printB = roundl(printB * 1000000) * 0.000001;

            cout << "------------------------------------------------" << endl;
            cout << curRepeatNum << "번째 세대" << endl;
            //boolalpha : bool을 true/false로 출력해줌
            //cout << "다음 세대에 시도할 방향 :: minDir : " << boolalpha << machine->minDir << ", maxDir : " << boolalpha << machine->maxDir << endl;

            cout << "BestW : " << printW << endl;
            cout << "b : " << printB << endl;
            cout << "BestWCost + BestBCost : " << printCost << endl;
            cout << "BestCost : " << machine->Cost() << endl;

            if (printCost == 0)
                break;
            else if (printCost == -1) {
                printCost = machine->bestBCost + machine->bestWCost;
                printCost = roundl(printCost * 100000000000000) * 0.00000000000001;
                cout << "마지막 BestCost : " << printCost << endl;
                break;
            }
        }
    }
    cout << "예측 결과 : w  = " << printW << ", b = " << printB;

    return 0;
}