#include <iostream>
#include <random>
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
    double w;
    double b;
    double lowCost;
    double highCost;
    double cost;

    double wRandomMin;
    double wRandomMax;
    double bRandomMin;
    double bRandomMax;
public:
    Machine() : w(0), b(0), lowCost(1), highCost(30000), cost(1), wRandomMin(0), wRandomMax(100), bRandomMin(0), bRandomMax(99) {};
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

    DataSet dataSet[3] = { {1, 1, 0, 1}, {1, 2, 0, 2}, {1, 3, 0, 3} };
    Machine* machine = new Machine();

    //w에 대한 편미분 경사하강
    //cost(정답과 가설간의 차)가 0이 될 때까지 반복한다.
    int trainingNum = 0;
    while (machine->cost != 0) {
        uniform_real_distribution<double> dis(machine->wRandomMin, machine->wRandomMax);

        machine->w = dis(gen);
        //machine->b = randomInt(gen);
        //machine->lowCost = machine->cost;
        machine->cost = machine->Cost(dataSet, 1, 2, 3);

        cout << "------------------------------------------------" << endl;
        cout << ++trainingNum << endl;
        cout << "w : " << machine->w << endl;
        cout << "b : " << machine->b << endl;

        cout << "cost : " << machine->cost << endl;

        //코스트 줄이기에 실패했다면
        if (machine->cost > machine->lowCost) {
            //highCost보다는 낮을 때
            if (machine->cost < machine->highCost) {
                //max는 min보다는 커야함
                if (machine->w > machine->wRandomMin) {
                    //코스트 업데이트
                    machine->highCost = machine->cost;
                    cout.precision(15);
                    cout << "wRandomMax Changed : " << machine->wRandomMax << "->" << machine->w << endl;
                    machine->wRandomMax = machine->w;
                }
            }
        }
        //코스트 줄이기에 성공했다면
        else if (machine->cost < machine->lowCost) {
            machine->cost = machine->lowCost;

            //double형이 보장하는 소수점 15자리까지 출력
            cout.precision(15);
            cout << "wRandomMin Changed : " << machine->wRandomMin << "->" << machine->w << endl;
            
            machine->wRandomMin = machine->w;
        }
        
        system("PAUSE");
    }
    return 0;
}