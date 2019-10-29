#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <cmath>

using namespace std;

#define randMax 1000
#define triesAmount 100
#define epsilon 0.0001
#define quiteALot 10000000.

double** genPKX(const int X, const int K)
{
    double **PKX = new double*[K];
    for (int i = 0; i < K; ++i)
        PKX[i] = new double[X];

    double sum = 0;
    for (int i = 0; i < X; ++i)
    {
        sum = 0;
        for (int j = 0; j < K; ++j)
        {
            PKX[j][i] = double(rand() % randMax);
            sum += PKX[j][i];
        }

        for (int j = 0; j < K; ++j)
            PKX[j][i] /= sum;

        double checkSum = 0;
        for (int j = 0; j < K; ++j)
            checkSum += PKX[j][i];

        assert(checkSum > 1. - epsilon);
        assert(checkSum < 1. + epsilon);
    }

    return PKX;
}

int q1(const int x, const int K, double** PKX)
{
    double max = PKX[0][x];
    int indexMax = 0;
    for (int i = 1; i < K; ++i)
        if (PKX[i][x] > max)
            max = PKX[i][x], indexMax = i;

    return indexMax;
}

double R1q1(const int X, const int K, double **PKX)
{
    double R = 0;
    double px = 1. / double(X);
    for (int i = 0; i < X; ++i)
    {
        int q1i = q1(i, K, PKX);
        for (int j = 0; j < K; ++j)
            R += PKX[j][i] * px * pow(q1i - j, 2);
    }

    return R;
}

double R1q2(const int X, const int K, double **PKX)
{
    double R = 0;
    double px = 1. / double(X);
    double Rmin = quiteALot;
    double Rfull = 0;
    for (int i = 0; i < X; ++i)
    {
        Rmin = quiteALot;
        for (int k_ = 0; k_ < K; ++k_)
        {
            R = 0;

            for (int j = 0; j < K; ++j)
                R += PKX[j][i] * px * pow(k_ - j, 2);

            if (Rmin > R)
                Rmin = R;
        }
        Rfull += Rmin;
    }

    return Rfull;
}

int q3(const int x, const int K, double** PKX, const float alpha)
{
    double min = quiteALot;
    int indexMin = 0;
    double sum = 0;
    for (int i = 0; i < K; ++i)
    {
        sum = 0;
        for (int j = 0; j < K; ++j)
            sum += PKX[j][x] * pow(i - j, 2);
        sum -= alpha * PKX[i][x];

        if (min > sum)
            min = sum, indexMin = i;
    }

    return indexMin;
}

double R1q3(const int X, const int K, double **PKX, const float alpha)
{
    double R = 0;
    double px = 1. / double(X);
    for (int i = 0; i < X; ++i)
    {
        int q3i = q3(i, K, PKX, alpha);
        for (int j = 0; j < K; ++j)
            R += PKX[j][i] * px * pow(q3i - j, 2);
    }

    return R;
}

double R2q1(const int X, const int K, double **PKX)
{
    double R = 0;
    double px = 1. / double(X);
    for (int i = 0; i < X; ++i)
    {
        R += PKX[q1(i, K, PKX)][i] * px;
    }

    return 1 - R;
}

double R2q2(const int X, const int K, double **PKX)
{
    double R = 0;
    double px = 1. / double(X);
    double Rmin = quiteALot;
    double Rfull = 0;
    for (int i = 0; i < X; ++i)
    {
        Rmin = quiteALot;
        for (int k_ = 0; k_ < K; ++k_)
        {
            R = PKX[k_][i] * px;

            if (Rmin > R)
                Rmin = R;
        }
        Rfull += Rmin;
    }

    return 1 - Rfull;
}

double R2q3(const int X, const int K, double **PKX, const float alpha)
{
    double R = 0;
    double px = 1. / double(X);
    for (int i = 0; i < X; ++i)
    {
        R += PKX[q3(i, K, PKX, alpha)][i] * px;
    }

    return 1 - R;
}

void genXK(int &X, int &K, double **&PKX)
{
    cout << "Enter X, K" << endl;
    cin >> X;
    cin >> K;
    PKX = genPKX(X, K);
}

void test()
{
    int X = 2;
    int K = 3;

    double **PKX = genPKX(X, K);

    PKX[0][0] = 0.5;
    PKX[1][0] = 0.3;
    PKX[2][0] = 0.2;
    PKX[0][1] = 0.7;
    PKX[1][1] = 0.3;
    PKX[2][1] = 0.0;
    
    assert(R1q1(X, K, PKX) > 0.7 - epsilon);
    assert(R1q1(X, K, PKX) < 0.7 + epsilon);
    assert(R1q2(X, K, PKX) > 0.5 - epsilon);
    assert(R1q2(X, K, PKX) < 0.5 + epsilon);
    assert(R1q3(X, K, PKX, 1000) > 0.7 - epsilon);
    assert(R1q3(X, K, PKX, 1000) < 0.7 + epsilon);
    assert(R1q3(X, K, PKX, -1000) > 2.7 - epsilon);
    assert(R1q3(X, K, PKX, -1000) < 2.7 + epsilon);
    assert(R1q3(X, K, PKX, 0) > 0.5 - epsilon);
    assert(R1q3(X, K, PKX, 0) < 0.5 + epsilon);
    assert(R1q3(X, K, PKX, 10) > 0.7 - epsilon);
    assert(R1q3(X, K, PKX, 10) < 0.7 + epsilon);
    assert(R1q3(X, K, PKX, -10) > 1.9 - epsilon);
    assert(R1q3(X, K, PKX, -10) < 1.9 + epsilon);
    assert(R2q1(X, K, PKX) > 0.4 - epsilon);
    assert(R2q1(X, K, PKX) < 0.4 + epsilon);
    assert(R2q2(X, K, PKX) > 0.9 - epsilon);
    assert(R2q2(X, K, PKX) < 0.9 + epsilon);
    assert(R2q3(X, K, PKX, 1000) > 0.4 - epsilon);
    assert(R2q3(X, K, PKX, 1000) < 0.4 + epsilon);
    assert(R2q3(X, K, PKX, -1000) > 0.9 - epsilon);
    assert(R2q3(X, K, PKX, -1000) < 0.9 + epsilon);
    assert(R2q3(X, K, PKX, 0) > 0.5 - epsilon);
    assert(R2q3(X, K, PKX, 0) < 0.5 + epsilon);
    assert(R2q3(X, K, PKX, 10) > 0.4 - epsilon);
    assert(R2q3(X, K, PKX, 10) < 0.4 + epsilon);
    assert(R2q3(X, K, PKX, -10) > 0.85 - epsilon);
    assert(R2q3(X, K, PKX, -10) < 0.85 + epsilon);
}

void printPKX(const int X, const int K, double **PKX)
{
    for (int i = 0; i < X; ++i)
    {
        for (int j = 0; j < K; ++j)
            cout << PKX[j][i] << " ";
        cout << endl;
    }
}

int main()
{
    srand((unsigned int)(time(0)));

    test();

    int X = 100;
    int K = 10;

    double **PKX = genPKX(X, K);

    /*

    for (int i = 0; i < X; ++i)
        delete[] PKX[i];

    delete[] PKX;

    genXK(X, K, PKX);
    */
    
    //printPKX(X, K, PKX);

    cout << "R1(q1) =                " << R1q1(X, K, PKX) << endl << endl;
    cout << "R1(q2) =                " << R1q2(X, K, PKX) << endl << endl;
    cout << "R1(q3, alpha =  1000) = " << R1q3(X, K, PKX, 1000) << endl << endl;
    cout << "R1(q3, alpha = -1000) = " << R1q3(X, K, PKX, -1000) << endl << endl;
    cout << "R1(q3, alpha =  0   ) = " << R1q3(X, K, PKX, 0) << endl << endl;
    cout << "R1(q3, alpha =  10  ) = " << R1q3(X, K, PKX, 10) << endl << endl;
    cout << "R1(q3, alpha = -10  ) = " << R1q3(X, K, PKX, -10) << endl << endl;
    cout << "R2(q1) =                " << R2q1(X, K, PKX) << endl << endl;
    cout << "R2(q2) =                " << R2q2(X, K, PKX) << endl << endl;
    cout << "R2(q3, alpha =  1000) = " << R2q3(X, K, PKX, 1000) << endl << endl;
    cout << "R2(q3, alpha = -1000) = " << R2q3(X, K, PKX, -1000) << endl << endl;
    cout << "R2(q3, alpha =  0   ) = " << R2q3(X, K, PKX, 0) << endl << endl;
    cout << "R2(q3, alpha =  10  ) = " << R2q3(X, K, PKX, 10) << endl << endl;
    cout << "R2(q3, alpha = -10  ) = " << R2q3(X, K, PKX, -10) << endl << endl;

    for (int i = 0; i < K; ++i)
        delete[] PKX[i];

    delete[] PKX;

    return 0;
}

