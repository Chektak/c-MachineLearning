#include <iostream>
#include <array> 
//�Ʒ� ������ x������, Y ����� ��(m) ����
#define TRAININGMATRIX_DATA_M 10
//�Ʒ� ������ X������ ����� ��(n) ����
#define TRAININGMATRIX_X_N 3
//�Ʒ� ������ Y��� ��(n) ����
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
#pragma region �/���� ���� �Ʒ� ������ ����(������ ��ó : ������������ 2018 �Ҿ� ǥ�� ����)

        if (sex == DATASEX::MEN) {
            //x1: ������, x2 : Ű, x3 : ��� ��
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
            //y1 : ���� = 0, ���� = 1 ����
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
            //x1 : ������, x2 : Ű, x3 : ��� ��
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
            //y1 : ���� = 0, ���� = 1 ����
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
    /// <returns>cost�� ��ȯ�Ѵ�.</returns>
    bool Training() {
        for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
            WUpdate(&wMatrix[i][0], i);
        }

        cost = Cost();

        //bestCost�� �� �����͸� ����صд�
        if (cost < bestCost) {
            sameCostStack = 0;
            bestWMatrix = wMatrix;
            bestCost = cost;
        }
        //bestCost�� w ���� ���� ���� * 10�� �̻� �߻����� ���� ���
        else if (++sameCostStack % (TRAININGMATRIX_X_N * TRAININGMATRIX_Y_N * 1000) == 0) {
            return false;
        }
        return true;
    }

    double Sigmoid(double hypothesis) {
        return 1 / (1 + exp(hypothesis));
    }
    //value�� ������Ʈ�Ѵ�.
    void WUpdate(double* value, const int& targetWIndex) {
        double gradient;

        gradient = WPartialDerivative(targetWIndex);
        gradient = roundl(gradient * 1000000) * 0.000001;
        //PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(wMatrix);
        cout << "["<< targetWIndex <<"]gradient : " << gradient << endl;
        int dir = 1;

        //��̺� ���� ����
        if (gradient > 0)
            dir = -1;
        else if (gradient < 0)
            dir = 1;
        else
            return; //��̺� ����� 0�̸� ���ư� ������ �����Ƿ� ����

        //����� ���� ���� �������� ��� �ظ��ϸ� All possibe regressions�� �����
#pragma region ���� ���ù� : All possible regressions(�������� ��Ȯ�� �н��� ���߰����� ���� �߻�Ȯ�� ����)
        *value += learningRate * dir;
#pragma endregion

#pragma region ���� ���ù� : Forward selection (�ſ� �������� ���� �н��� ���߰����� ���� �߻�Ȯ�� ����)
                //while (true) {
                //    *value += dir * learningRate;

                //    gradient = WPartialDerivative(targetWIndex);
                //    gradient = roundl(gradient * 1000000) * 0.000001;

                //    //���� �������� ������ ��, wGradient�� ����� �Ǹ� �����Ѵ�. ���� ������ ��쵵 ��������
                //    if ((dir == 1 && gradient >= 0) || (dir == -1 && gradient <= 0)) {
                //        break;
                //    }
                //}
#pragma endregion
        return;

    }

    /// <summary>
    /// targetW ����� ��̺� �Լ�(���⸦ ��ȯ�Ѵ�)
    /// </summary>
    /// <param name="wMatrix">�� �Լ��� wMatrix�� ���� 0�� ��쿡�� ��밡���ϴ�.</param>
    /// <param name="targetWIndex">w1�� Ÿ���� ��� 0�� ���ڷ� �����Ѵ�.</param>
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

    std::cout << "\n\n cost = -(1/m)��(y * log(Sigmoid(X)) + (1 - y)log(1-Sigmoid(X)))���� ������ƽ ȸ�� ���� �Ʒ�" << endl;
    std::cout << "�������� ����(���� : 0, ���� : 1)�� �������ּ���. : ";
    int sexInt;
    cin >> sexInt;
    DATASEX sex = (DATASEX)sexInt;
    if (sex == DATASEX::MEN) {
        std::cout << "�Ʒ� �����ʹ� ���� 3�� ~ �������� ����Ǿ����ϴ�." << endl;
    }
    else {
        std::cout << "�Ʒ� �����ʹ� ���� 3�� ~ �������� ����Ǿ����ϴ�." << endl;
    }
    machine->TrainingDataSetup(sex);


    std::cout << "�Ʒ� Ƚ���� �Է����ּ���(��õ�� : 100 �̻�) : ";
    cin >> trainingNum;

    while (machine->learningRate == 0) {
        cout << "0�� ������ �н����� �������ּ���.(��õ�� : 0.001 ����)" << endl;
        cout << " ���ڰ� Ŭ���� �н� �������� Ŀ����, �������� �н� ��Ȯ���� �������ϴ�. : ";
        cin >> machine->learningRate;
    }

    //�Ҽ��� ����
    std::cout << fixed;
    //double�� �ִ� ���� ������ �Ҽ��� 15�ڸ� �̻���� ���
    std::cout.precision(15);

    double printCost = -1;
    printCost = machine->cost;
    std::cout << "------------------------------------------------" << endl;
    std::cout << "0��° ����" << endl;
    std::cout << "BestCost : " << printCost << endl;
    std::cout << "BestWMatrix : " << endl;
    machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(machine->wMatrix);
    std::cout << "------------------------------------------------" << endl;
#pragma region �Ʒ� ����

    int curRepeatNum = 1;
    for (; curRepeatNum <= trainingNum; curRepeatNum++) {
        if (machine->Training() == false) {
            cout << "�ڽ�Ʈ ������ ���� ���� * 10�� ����, �������̹Ƿ� �Ʒ� ����" << endl;
            break;
        }
        printCost = machine->cost;

        //�Ҽ��� 15�ڸ� �̻���� �ݿø�
        printCost = roundl(printCost * 100000000000000) * 0.00000000000001;

        //100������ ����ϰų�, �ڽ�Ʈ�� 0�� �Ǿ��� �� ���
        if (curRepeatNum % 1 == 0 || printCost <= 0) {
            std::cout << "------------------------------------------------" << endl;
            std::cout << curRepeatNum << "��° ����" << endl;

            std::cout << "cost : " << printCost << endl;
            std::cout << "wMatrix : " << endl;

            machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(machine->wMatrix);
            if (printCost <= 0)
                break;
        }
    }
    
#pragma endregion

    std::cout << "------------------------------------------------" << endl;
    std::cout << curRepeatNum << "��° ����" << endl;
    std::cout << "BestCost : " << machine->bestCost << endl;
    std::cout << "BestWMatrix : " << endl;
    machine->PrintMatrix<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(machine->bestWMatrix);

    
    
    if(sex == DATASEX::MEN)
        std::cout << "�����͸� �Է��ϸ� (����) Ÿ���� ����/���� ������ �����մϴ�." << endl;
    else
        std::cout << "�����͸� �Է��ϸ� (����) Ÿ���� ����/���� ������ �����մϴ�." << endl;
    int yesOrNo = 1;
    while (yesOrNo == 1) {
        std::cout << "���� �н����� ���� �������� : " << machine->bestCost * 100 << "%" << endl;

        int process = 0;
        int newData[3];
        while (process < 3)
        {
            switch (process) {
            case 0:
                std::cout << "  ������ �����Ը� �Է����ּ��� : ";
                break;
            case 1:
                std::cout << "  ������ Ű�� �Է����ּ��� : ";
                break;
            case 2:
                std::cout << "  ������ ��� ���� �Է����ּ��� : ";
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
            //std::cout << "����� " << (logistic - 0.5) * 200 << " Ȯ���� ��Դϴ�." << endl;
            std::cout << "����� " << logistic * 100 << " Ȯ���� ��Դϴ�." << endl;
        else
            //std::cout << "����� " << 100 / (1 + logistic) << " Ȯ���� �����Դϴ�." << endl;
            std::cout << "����� " << 100 / (1+logistic) << " Ȯ���� �����Դϴ�." << endl;

        std::cout << "�ѹ� �� �����Ͻðڽ��ϱ�? (�� : 1, �ƴϿ� : 0)" << endl;
        cin >> yesOrNo;
    }

    system("pause");
    return 0;
}