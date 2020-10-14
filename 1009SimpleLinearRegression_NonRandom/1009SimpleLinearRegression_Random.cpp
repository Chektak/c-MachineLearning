#include <iostream>
#include <random>

#define TRAININGDATA_NUM 4
using namespace std;

// 시드값을 얻기 위한 random_device 생성.
random_device rd;
// random_device 를 통해 난수 생성 엔진을 초기화 한다.
mt19937 gen(rd());

typedef struct datas {
    double w;
    double xData;
    double b;
    double y;
}TestDataSet;

class Machine
{
public:
    double bestW;
    double w;
    double bestCost;
    double cost;
    double b;

    int sameCostStack;
    int accelStack;

    double wRandomRate;
    int accelRate;

    bool minDir = 0;
    bool maxDir = 1;
public:
    Machine() : bestW(0), w(0), bestCost(DBL_MAX), cost(DBL_MAX), b(0), sameCostStack(0), accelStack(0), wRandomRate(0), accelRate(10) {};

    /// <summary>
    /// </summary>
    /// <returns>cost를 반환한다.</returns>
    double Training(int curRepeatNum, TestDataSet testDataSet[TRAININGDATA_NUM]) {
        uniform_real_distribution<double> dis(bestW - (int)(minDir) * wRandomRate, bestW + (int)(maxDir) * wRandomRate);

        w = dis(gen);
        cost = Cost(testDataSet);

        //코스트 줄이기에 성공했다면
        if (cost < bestCost) {
            bestW = w;
            bestCost = cost;
            
            //6번째 연속으로 코스트 줄이기 실패가 없다면 변화폭을 늘린다
            if (sameCostStack == 0 && ++accelStack % 6 == 0) {
                cout << "#### RamdomRate Accelerated!! " << wRandomRate << "->" << wRandomRate * accelRate << endl;
                wRandomRate *= accelRate;
                accelStack = 0;
            }

            //같은 코스트 판별 스택 초기화
            sameCostStack = 0;
            return cost;
        }

        //코스트 줄이기에 실패했으므로 다른 방향을 시도한다
        minDir = !minDir;
        maxDir = !maxDir;

        //같은 cost가 반복되는 양상을 보이면 
        if (++sameCostStack % 3 == 0)
        {
            //변화폭을 소수점 이하로 줄인다
            cout << "#### RamdomRate Chenged!! " << wRandomRate << "->" << wRandomRate / accelRate << endl;
            wRandomRate /= accelRate;
            if (sameCostStack > 20) {
                cout << "예외 발생 :: 같은 코스트가 20번 이상 중복되었으므로 종료합니다." << endl;
                return -1;
            }
        }
        return bestCost;
    }
    
    double Cost(TestDataSet testDataSet[TRAININGDATA_NUM]) {
        double cost = 0;
        for (int i = 0; i < TRAININGDATA_NUM; i++) {
            cost += Loss(testDataSet[i].xData, testDataSet[i].y);
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

    cout << "\n\ny = wx + b 1차방정식 선형회귀 문제 훈련" << endl;

    //------------- y = wx + b 식에서             w  x  b y
    TestDataSet dataSet[TRAININGDATA_NUM] = {   {3, 1, 0},
                                                {3, 2, 0},
                                                {3, 3, 0},
                                                {3, 4, 0} };
    //dataSet의 y는 자동으로 계산해 넣음
    for (int i = 0; i < TRAININGDATA_NUM; i++) {
        dataSet[i].y = machine->GetYLinear(dataSet[i].w, dataSet[i].xData, dataSet[i].b);
        cout << "\tDataSet " << i + 1 << " { w, x, b, Y } = " 
            << dataSet[i].w << ", " << dataSet[i].xData << ", " << dataSet[i].b << ", " << dataSet[i].y << endl;
    }
    
    //시험 성적 훈련
    //------------- y = wx + b 식에서             ?  x  ?   y 가 주어짐
    //TestDataSet dataSet[TRAININGDATA_NUM] = {   {0, 2, 0, 25},
    //                                            {0, 3, 0, 50},
    //                                            {0, 4, 0, 42},
    //                                            {0, 5, 0, 61} };

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
    
    //y절편은 기울기에 0을 곱하는 것으로 구할 수 있음
    for(int i=0; i < TRAININGDATA_NUM; i++)
        machine->b += machine->GetYLinear(0, dataSet[i].xData, dataSet[i].b);
    machine->b /= TRAININGDATA_NUM;

    for (int curRepeatNum = 0; curRepeatNum <= trainingNum; curRepeatNum++) {
        double bestCost = machine->Training(curRepeatNum, dataSet);
        double bestW = machine->bestW;
        
        //소수점 15자리 이상까지 반올림
        bestCost = roundl(bestCost * 100000000000000) * 0.00000000000001;
        //w의 소수점 자릿수가 6 이하일 때 cost 소수점 15자리 이하에 영향을 주므로 6자리에서 반올림
        bestW = roundl(bestW * 1000000) * 0.000001;

        //25번마다 출력하거나, 코스트가 0이 되었을 때 출력
        if (curRepeatNum % 25 == 0 || bestCost <= 0) {
            cout << "------------------------------------------------" << endl;
            cout << curRepeatNum << "번째 세대" << endl;
            //boolalpha : bool을 true/false로 출력해줌
            //cout << "다음 세대에 시도할 방향 :: minDir : " << boolalpha << machine->minDir << ", maxDir : " << boolalpha << machine->maxDir << endl;

            cout << "BestW : " << bestW << endl;
            cout << "b : " << machine->b << endl;
            cout << "BestCost : " << bestCost << endl;

            if (bestCost == 0)
                break;
            else if (bestCost == -1) {
                cout << "마지막 BestCost : " << machine->bestCost << endl;
                break;
            }
        }
    }
    return 0;
}