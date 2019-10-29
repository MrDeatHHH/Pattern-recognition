#include "stdafx.h"
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
    for (int k_ = 0; k_ < K; ++k_)
    {
        R = 0;
        for (int i = 0; i < X; ++i)
        {
            for (int j = 0; j < K; ++j)
                R += PKX[j][i] * px * pow(k_ - j, 2);
        }
        if (Rmin > R)
            Rmin = R;
    }

    return Rmin;
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


int main()
{
    srand((unsigned int)(time(0)));

    const int X = 2;
    const int K = 5;

    double **PKX = genPKX(X, K);

    for (int i = 0; i < X; ++i)
    {
        for (int j = 0; j < K; ++j)
            cout << PKX[j][i] << " ";
        cout << endl;
    }

    cout << q1(0, K, PKX) << endl;

    cout << "R1(q1) = " << R1q1(X, K, PKX) << endl;
    cout << "R1(q2) = " << R1q2(X, K, PKX) << endl;
    cout << "R1(q3) = " << R1q3(X, K, PKX, 0.1) << endl;

    for (int i = 0; i < X; ++i)
        delete[] PKX[i];

    delete[] PKX;

    return 0;
}

