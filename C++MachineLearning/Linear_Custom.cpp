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
    double prevCost;
    double cost;

    double wRandomMin;
    double wRandomMax;
    double bRandomMin;
    double bRandomMax;

    
public:
    Machine() : w(0), b(0), prevCost(1), cost(1),wRandomMin(0), wRandomMax(1), bRandomMin(0), bRandomMax(99) {};
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
    // 0 부터 99 까지 균등하게 나타나는 난수열.
    uniform_int_distribution<int> randomInt(0, 99);
    uniform_real_distribution<double> randomDouble(0, 1);

    DataSet dataSet[3] = { {1, 1, 0, 1}, {1, 2, 0, 2}, {1, 3, 0, 3} };
    Machine* machine = new Machine();

    cout << "int형 난수 예시 : " << randomInt(gen) << std::endl;
    //double형이 보장하는 소수점 15자리까지 출력
    cout.precision(15);
    cout << "double형 난수 예시 : " << randomDouble(gen) << std::endl;

    //y절편 b와 wRandomMax는 상수값으로 두었음. 각각 0과 100
    //cost(정답과 가설간의 차)가 0이 될 때까지 반복한다.
    int trainingNum = 0;
    while (machine->cost != 0) {
        uniform_real_distribution<double> dis(machine->wRandomMin, 1);

        machine->w = dis(gen);
        //machine->b = randomInt(gen);
        machine->prevCost = machine->cost;
        machine->cost = machine->Cost(dataSet, 1, 2, 3);

        cout << "------------------------------------------------" <<  endl;
        cout << ++trainingNum << endl;
        cout << "w : " << machine->w << endl;
        cout << "b : " << machine->b << endl;
        cout << "loss1 : " << machine->Loss(dataSet[0], 1) << endl;
        cout << "loss2 : " << machine->Loss(dataSet[1], 2) << endl;
        cout << "loss3 : " << machine->Loss(dataSet[2], 3) << endl;

        cout << "cost : " << machine->cost << endl;




        //코스트 줄이기에 성공했다면
        if (machine->cost < machine->prevCost) {
            //double형이 보장하는 소수점 15자리까지 출력
            cout.precision(15);
            cout << "wRandomMin Changed : " << machine->wRandomMin << "->" << machine->w<< endl;
            machine->wRandomMin = machine->w;
        }

        system("PAUSE");
    }
    return 0;
}