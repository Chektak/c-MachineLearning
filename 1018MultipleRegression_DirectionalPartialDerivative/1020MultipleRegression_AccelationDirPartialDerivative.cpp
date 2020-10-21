#include <iostream>
#include <array> 
//�Ʒ� ������ x������, Y ����� ��(m) ����
#define TRAININGMATRIX_DATA_M 5
//�Ʒ� ������ X������ ����� ��(n) ����
#define TRAININGMATRIX_X_N 2
//�Ʒ� ������ Y��� ��(n) ����
#define TRAININGMATRIX_Y_N 1

using namespace std;

typedef struct datas {
    array<array<double, TRAININGMATRIX_X_N>, TRAININGMATRIX_DATA_M> xMatrix;
    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_DATA_M> yMatrix;
}DataSet;

namespace Mathf {
    //��(m) : ���� ��, ��(n) : ���η� �󸶳� �����ִ���
    template<int mat1M, int mat1N, int mat2M, int mat2N>
    void MatrixProduct(const std::array<std::array<double, mat1N>, mat1M>& mat1,
        const std::array<std::array<double, mat2N>, mat2M>& mat2,
        std::array<std::array<double, mat2N>, mat1M>& resultMat) {

        if (mat1M <= 0 || mat1N <= 0 || mat2M <= 0 || mat2N <= 0)
        {
            cout << "Can't Product!!";
            return;
        }
        for (int z = 0; z < mat2N; z++) {
            //��
            for (int i = 0; i < mat1M; i++) {
                //��
                for (int j = 0; j < mat1N; j++) {
                    resultMat[i][z] += mat1[i][j] * mat2[j][z];
                }
            }
        }
    }
};

class Machine
{
public:
    DataSet trainingDataSet;

    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> wMatrix;
    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> wGradientMatrix;
    double cost;

    double learningRate;


    array<array<double, TRAININGMATRIX_Y_N>, TRAININGMATRIX_X_N> bestWMatrix;
    double bestCost;
    int sameCostStack;
public:
    Machine() : cost(INT_MAX), bestCost(DBL_MAX), learningRate(0), wMatrix({ 0 }), wGradientMatrix({ INT_MAX }) {};

    void TrainingDataSetup() {
#pragma region �Ʒ� ������ ����
        //���� ������ ���� ������
       /* trainingDataSet.xMatrix = {
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

        //�� �����ͼ��� ����� ������ TRAININGMATRIX_X_N�� 2�� �����ؾ� ��
        //xMatrix�� ���� 2���� ���
        trainingDataSet.xMatrix = {
            10, 5,
            9,  5,
            3,  2,
            2,  4,
            11, 1
        };
        trainingDataSet.yMatrix = {
            90,
            80,
            50,
            60,
            40
        };

        //�� �����Ϳ��� wMatrix�� 1, 1, 1�� �����ؾ� ��
        /*trainingDataSet.xMatrix = {
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
        };*/
#pragma endregion
    }

    /// <summary>
    /// </summary>
    /// <returns>cost�� ��ȯ�Ѵ�.</returns>
    bool Training() {
        for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
            WUpdate(&wMatrix[i][0], i);
        }

        cost = Cost<TRAININGMATRIX_DATA_M, TRAININGMATRIX_X_N,
            TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>();

        //bestCost�� �� �����͸� ����صд�
        if (cost < bestCost) {
            sameCostStack = 0;
            bestWMatrix = wMatrix;
            bestCost = cost;
        }
        //bestCost�� w ���� ���� ���� * 10�� �̻� �߻����� ���� ���
        else if (++sameCostStack % (TRAININGMATRIX_X_N * TRAININGMATRIX_Y_N * 10) == 0) {
            cost = bestCost;
            wMatrix = bestWMatrix;
            return false;
        }
        return true;
    }

    //value�� ������Ʈ�Ѵ�.
    void WUpdate(double* value, const int& targetWIndex) {
        double* gradient = &wGradientMatrix[targetWIndex][0];

        *gradient = WPartialDerivative<TRAININGMATRIX_X_N, TRAININGMATRIX_Y_N>(wMatrix, targetWIndex);
        *gradient = roundl(*gradient * 1000000) * 0.000001;

        int dir = 1;

        //��̺� ���� ����
        if (*gradient > 0)
            dir = -1;
        else if (*gradient < 0)
            dir = 1;
        else
            return; //��̺� ����� 0�̸� ���ư� ������ �����Ƿ� ����

        //����� ���� ���� �������� ��� �ظ��ϸ� All possibe regressions�� �����
#pragma region ���� ���ù� : All possible regressions(�������� ���߰����� ���� �߻�Ȯ�� ����)
        * value += dir * learningRate;
#pragma endregion
    }

    /// <summary>
    /// targetW ����� ��̺� �Լ�(���⸦ ��ȯ�Ѵ�)
    /// </summary>
    /// <param name="wMatrix">�� �Լ��� wMatrix�� ���� 0�� ��쿡�� ��밡���ϴ�.</param>
    /// <param name="targetWIndex">w1�� Ÿ���� ��� 0�� ���ڷ� �����Ѵ�.</param>
    /// <returns></returns>
    template<int wMatM, int wMatN>
    double WPartialDerivative(array<array<double, wMatN>, wMatM> wMatrix, const int& targetWIndex) {
        double newW = 0;
        for (int m = 0; m < TRAININGMATRIX_DATA_M; m++) {
            double temp = 0;
            for (int n = 0; n < TRAININGMATRIX_X_N; n++) {
                temp += trainingDataSet.xMatrix[m][n] * wMatrix[n][0];
            }
            temp -= trainingDataSet.yMatrix[m][0];
            newW += trainingDataSet.xMatrix[m][targetWIndex] * temp;
        }
        //��� ��ҵ鿡 ���� ������ ��� ���ϱ�
        newW = newW * 2 / TRAININGMATRIX_DATA_M;
        return newW;
    }

    template<int xMatM, int xMatN, int wMatM, int wMatN>
    double Cost() {
        array<array<double, wMatN>, xMatM> tempYMatrix{ 0 };
        //tempYMatrix�� xMatrix�� wMatrix�� ��İ��� �޴´�
        Mathf::MatrixProduct<xMatM, xMatN, wMatM, wMatN>(trainingDataSet.xMatrix, wMatrix, tempYMatrix);

        double cost = 0;
        for (int i = 0; i < xMatM; i++) {
            cost += Loss(tempYMatrix[i][0], trainingDataSet.yMatrix[i][0]);
        }
        cost /= xMatM;
        return cost;
    }

    /// <summary>
    /// Loss : (���� H = wx+b) - ���䵥���� y
    /// </summary>
    /// <returns></returns>
    double Loss(double hData, double yData) {
        //y = H(X) = XW + b���� y ��ĸ� ��ȯ�Ѵ�. (XW�� ��İ�)

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

    std::cout << "\n\nH(X) = XW + b �ٺ��� ����ȸ�� ���� �Ʒ�" << endl;

    std::cout << "�Ʒ� Ƚ���� �Է����ּ��� : ";
    cin >> trainingNum;

    while (machine->learningRate == 0) {
        cout << "0�� ������ �н����� �������ּ���.";
        cout << " ���ڰ� Ŭ���� �������� Ŀ���� 1 ���ϸ� ��õ�մϴ� : ";
        cin >> machine->learningRate;
    }

#pragma region �Ʒ� ���
    //�Ҽ��� ����
    std::cout << fixed;

    //�Ҽ��� 6�ڸ� ���ϱ��� ���
    std::cout.precision(6);

    double printCost = -1;

    std::cout << "------------------------------------------------" << endl;
    std::cout << "0��° ����" << endl;
    std::cout << "BestCost : " << printCost << endl;
    std::cout << "BestWMatrix : " << endl;
    for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
        for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
            std::cout << machine->wMatrix[i][j] << " ";
        }
        std::cout << endl;
    }

    int curRepeatNum = 1;
    for (; curRepeatNum <= trainingNum; curRepeatNum++) {
        if (machine->Training() == false) {
            cout << "�ڽ�Ʈ ������ ���� ���� * 20�� ����, �������̹Ƿ� �Ʒ� ����" << endl;
            break;
        }
        printCost = machine->cost;

        //�Ҽ��� 15�ڸ� �̻���� �ݿø�
        printCost = roundl(printCost * 100000000000000) * 0.00000000000001;

        //100������ ����ϰų�, �ڽ�Ʈ�� 0�� �Ǿ��� �� ���
        if (curRepeatNum % 25 == 0 || printCost <= 0) {
            std::cout << "------------------------------------------------" << endl;
            std::cout << curRepeatNum << "��° ����" << endl;

            std::cout << "cost : " << printCost << endl;
            std::cout << "wMatrix : " << endl;

            for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
                for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
                    std::cout << machine->wMatrix[i][j] << " ";
                }
                std::cout << endl;
            }
            if (printCost <= 0)
                break;
        }
    }
    std::cout << "------------------------------------------------" << endl;
    std::cout << curRepeatNum << "��° ����" << endl;
    std::cout << "BestCost : " << machine->bestCost << endl;
    std::cout << "BestWMatrix : " << endl;
    for (int i = 0; i < TRAININGMATRIX_X_N; i++) {
        for (int j = 0; j < TRAININGMATRIX_Y_N; j++) {
            std::cout << machine->bestWMatrix[i][j] << " ";
        }
        std::cout << endl;
    }
#pragma endregion
    system("pause");
    return 0;
}