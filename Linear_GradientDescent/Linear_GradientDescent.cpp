#include <iostream>
#include <random>
#include <math.h>//소수점 반올림 계산을 위해 사용

#define RANDOMRATE 50
using namespace std;

typedef struct datas {
    double w;
    double xData;
    double b;
    double y;
}DataSet;

class Machine
{
public:
    double prevW;
    double w;
    double prevCost;
    double cost;
    double b;
    int sameCostStack;

    double wRandomRate;
    double bRandomRate;
public:
    Machine() : prevW(0), w(0), prevCost(DBL_MAX), cost(1), b(0), sameCostStack(0), wRandomRate(RANDOMRATE), bRandomRate(RANDOMRATE) {};
    double Cost(DataSet data[], double x1, double x2, double x3) {
        double cost = Loss(data[0], x1) + Loss(data[1], x2) + Loss(data[2], x3);
        cost /= 3;
        return cost;
    }
    double Loss(DataSet data, double xInput) {
        double loss = w * xInput + b - data.y;
        loss *= loss;
        return loss;
    }
};

int main()
{
    // 시드값을 얻기 위한 random_device 생성.
    random_device rd;
    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
    mt19937 gen(rd());

    //    y = wx + b 식에서  w  x  b  y
    DataSet dataSet[3] = { {1, 1, 0, 1},
                           {1, 2, 0, 2},
                           {1, 3, 0, 3} };

    Machine* machine = new Machine();

    //w에 대한 편미분 경사하강
    //cost(정답과 가설간의 차)가 0이 될 때까지 반복한다.
    int trainingNum = 0;
    int learningRate = 0;

    //cin >> machine->wRandomRate;
    machine->wRandomRate = RANDOMRATE;
    cout << "훈련 횟수를 입력해주세요 : ";
    cin >> trainingNum;

    cout << "학습률을 설정해주세요. " << endl << "  숫자가 클수록 미세해지며 100 ~ 1000을 추천합니다 : ";
    cin >> learningRate;

    //소수점 고정
    cout << fixed;
    //double형이 보장하는 15자리까지 출력
    cout.precision(15);
    for (int repeat = 0; repeat <= trainingNum; repeat++) {
        uniform_real_distribution<double> dis(machine->prevW - machine->wRandomRate, machine->prevW + machine->wRandomRate);

        machine->w = dis(gen);
        machine->cost = machine->Cost(dataSet, 1, 2, 3);

        //100번마다 출력하거나, 코스트가 0이 되었을 때 출력
        if (repeat % 100 == 0 || machine->cost == 0) {
            cout << "------------------------------------------------" << endl;
            cout << repeat << "번째 세대" << endl;
            cout << "W : " << machine->w << endl;
            cout << "b : " << machine->b << endl;
            cout << "Cost : " << machine->cost << endl;
            if (machine->cost == 0)
            //if (ceil(machine->cost * 100000) / 100000 == 0)
                break;
        }

        //코스트 줄이기에 성공했다면
        if (machine->cost < machine->prevCost) {
            int dir = (machine->prevW - machine->w > 0) ? -1 : +1;

            cout << "wRandomMin Changed : " << machine->prevW - dir * machine->wRandomRate << "->" <<
                machine->w - dir * machine->wRandomRate << endl;
            cout << "wRandomMax Changed : " << machine->prevW + dir * machine->wRandomRate << "->" <<
                machine->w + dir * machine->wRandomRate << endl;

            machine->prevCost = machine->cost;
            machine->prevW = machine->w;
            //코스트 스택 초기화
            machine->sameCostStack = 0;
        }
        //같은 cost가 반복되는 양상을 보이면 
        else if (++machine->sameCostStack % 100 == 0 && learningRate != 0)
        {
            //가속하여 하강한다.
            cout << "#### RamdomRate Chenged!! " << machine->wRandomRate << "->" << machine->wRandomRate / learningRate << endl;
            machine->wRandomRate /= learningRate;
        }
        //std::system("PAUSE");
    }
    return 0;
}