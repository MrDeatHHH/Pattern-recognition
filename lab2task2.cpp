#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

#define randMax 10
#define triesAmount 100
#define steps 5

#define UTIME (unsigned int)(time(0))	

// Macros for time in miliseconds
#define timeUsedMiliSeconds(start, end, dimention)\
	cout << "Time used for " << dimention << " - " <<\
		(double)( end - start ) << " ms"<< endl;

// Macros for time in seconds
#define timeUsedSeconds(start, end, dimention)\
	cout << "Time used for " << dimention << " - " <<\
		( end - start ) / (double)CLOCKS_PER_SEC << " s" << endl;

// A lot of time was wasted
// [0] - time for foolish calculation
// [1] - time for smart precalculation
// [2] - time for smart calculation
int timeWastedD1[steps][3];
int timeWastedD2[steps][3];
int timeWastedD3[steps][3];

// Foolish sum for 1-dimensional array
int foolishSumD1(int *mas,
				 const int length,
				 const int left,
				 const int rigth)
{
	int sum{};
	for (int i = left - 1; i < rigth; ++i)
	{
		sum += mas[i];
	}
	return sum;
}

// Foolish sum for 2-dimensional array
int foolishSumD2(int **mas,
			     const int length,
				 const int left1,
				 const int left2,
				 const int rigth1,
				 const int rigth2)
{
	int sum{};
	for (int i = left1 - 1; i < rigth1; ++i)
		for (int j = left2 - 1; j < rigth2; ++j)
			sum += mas[i][j];
	return sum;
}

// Foolish sum for 3-dimensional array
int foolishSumD3(int ***mas,
				 const int length,
				 const int left1,
				 const int left2,
				 const int left3,
				 const int rigth1,
				 const int rigth2,
				 const int rigth3)
{
	int sum{};
	for (int i = left1 - 1; i < rigth1; ++i)
		for (int j = left2 - 1; j < rigth2; ++j)
			for (int k = left3 - 1; k < rigth3; ++k)
				sum += mas[i][j][k];
	return sum;
}

// Smart sum for 1-dimensional array
int* createSmartSumD1(int *mas, const int length)
{
	int *smartSum = new int[length];
	smartSum[0] = mas[0];

	// Filling the line
	for (int i = 1; i < length; ++i)
	{
		smartSum[i] = smartSum[i - 1] + mas[i];
	}

	return smartSum;
}

// Smart sum for 2-dimensional array
int** createSmartSumD2(int **mas, const int length)
{
	int **smartSum = new int*[length];
	for (int i = 0; i < length; ++i)
		smartSum[i] = new int[length];
	smartSum[0][0] = mas[0][0];

	// Filling borders
	for (int i = 1; i < length; ++i)
	{
		smartSum[i][0] = smartSum[i - 1][0] + mas[i][0];
		smartSum[0][i] = smartSum[0][i - 1] + mas[0][i];
	}

	// Filling from the left top corner to the half of the square
	for (int i = 1; i < length; ++i)	
		for (int j = 1; j < i; ++j)
			smartSum[i - j][j] = smartSum[i - 1 - j][j] +
								 smartSum[i - j][j - 1] +
								 mas[i - j][j] -
								 smartSum[i - 1 - j][j - 1];

	// Filling second half of the square
	for (int j = 1; j < length; ++j)
		for (int i = 0; i < length - j; ++i)
			smartSum[length - 1 - i][j + i] = smartSum[length - 2 - i][j + i] +
											  smartSum[length - 1 - i][j - 1 + i] +
											  mas[length - 1 - i][j + i] -
											  smartSum[length - 2 - i][j - 1 + i];

	return smartSum;
}

// Smart sum for 3-dimensional array
int*** createSmartSumD3(int ***mas, const int length)
{
	int ***smartSum = new int**[length];
	for (int i = 0; i < length; ++i)
	{

		smartSum[i] = new int*[length];

		for (int j = 0; j < length; ++j)
			smartSum[i][j] = new int[length];
	}

	smartSum[0][0][0] = mas[0][0][0];

	// Filling border lines
	for (int i = 1; i < length; ++i)
	{
		smartSum[i][0][0] = smartSum[i - 1][0][0] + mas[i][0][0];
		smartSum[0][i][0] = smartSum[0][i - 1][0] + mas[0][i][0];
		smartSum[0][0][i] = smartSum[0][0][i - 1] + mas[0][0][i];
	}

	// Filling border planes, first half
	for (int i = 1; i < length; ++i)
		for (int j = 1; j < i; ++j)
		{
			smartSum[i - j][j][0] = smartSum[i - 1 - j][j][0] +
				smartSum[i - j][j - 1][0] +
				mas[i - j][j][0] -
				smartSum[i - 1 - j][j - 1][0];

			smartSum[i - j][0][j] = smartSum[i - 1 - j][0][j] +
				smartSum[i - j][0][j - 1] +
				mas[i - j][0][j] -
				smartSum[i - 1 - j][0][j - 1];

			smartSum[0][i - j][j] = smartSum[0][i - 1 - j][j] +
				smartSum[0][i - j][j - 1] +
				mas[0][i - j][j] -
				smartSum[0][i - 1 - j][j - 1];
		}

	// Filling border planes, second half
	for (int j = 1; j < length; ++j)
		for (int i = 0; i < length - j; ++i)
		{
			smartSum[length - 1 - i][j + i][0] = smartSum[length - 2 - i][j + i][0] +
				smartSum[length - 1 - i][j - 1 + i][0] +
				mas[length - 1 - i][j + i][0] -
				smartSum[length - 2 - i][j - 1 + i][0];

			smartSum[length - 1 - i][0][j + i] = smartSum[length - 2 - i][0][j + i] +
				smartSum[length - 1 - i][0][j - 1 + i] +
				mas[length - 1 - i][0][j + i] -
				smartSum[length - 2 - i][0][j - 1 + i];

			smartSum[0][length - 1 - i][j + i] = smartSum[0][length - 2 - i][j + i] +
				smartSum[0][length - 1 - i][j - 1 + i] +
				mas[0][length - 1 - i][j + i] -
				smartSum[0][length - 2 - i][j - 1 + i];
		}
	
	// Filling first half of the cube
	for (int i = 1; i < length; ++i)
		for (int j = 1; j < i + 1; ++j)
			for (int k = 1; k < i - j + 2; ++k)
				smartSum[j][i - j - k + 2][k] = mas[j][i - j - k + 2][k] +
					smartSum[j - 1][i - j - k + 2][k] +
					smartSum[j][i - j - k + 1][k] +
					smartSum[j][i - j - k + 2][k - 1] -
					smartSum[j - 1][i - j - k + 1][k] -
					smartSum[j - 1][i - j - k + 2][k - 1] - 
					smartSum[j][i - j - k + 1][k - 1] + 
					smartSum[j - 1][i - j - k + 1][k - 1];

	// Filling second half of the cube
	for (int i = length; i < 2 * length; ++i)
		for (int j = 1; j < length; ++j)
			for (int k = 1; k < i - j + 2; ++k)
				if ( (k < length) && (i - j - k + 2 < length) )
					smartSum[j][i - j - k + 2][k] = mas[j][i - j - k + 2][k] +
						smartSum[j - 1][i - j - k + 2][k] +
						smartSum[j][i - j - k + 1][k] +
						smartSum[j][i - j - k + 2][k - 1] -
						smartSum[j - 1][i - j - k + 1][k] -
						smartSum[j - 1][i - j - k + 2][k - 1] -
						smartSum[j][i - j - k + 1][k - 1] +
						smartSum[j - 1][i - j - k + 1][k - 1];

	return smartSum;
}

// Testing 1-dimensional algorithms
void checkD1(const int length1, const int left1, const int right1, int step)
{
	cout << "-------------------" << endl;

	clock_t start = clock();
	int *mas1 = new int[length1];

	clock_t middle = clock();
	for (int i = 0; i < length1; ++i)
		mas1[i] = 1 + rand() % randMax;

	clock_t end = clock();
	timeUsedSeconds(start, middle, "SM");
	timeUsedSeconds(middle, end, "ME");

	clock_t start1 = clock();
	int sum1;
	for (int i = 0; i < triesAmount; ++i)
		sum1 = foolishSumD1(mas1,
			length1,
			left1,
			right1);
	clock_t end1 = clock();
	timeWastedD1[step][0] = end1 - start1;
	timeUsedMiliSeconds(start1, end1, "D1 foolish calc");
	cout << sum1 << endl;

	start1 = clock();
		int *smartSum1 = createSmartSumD1(mas1, length1);
	end1 = clock();
	timeWastedD1[step][1] = end1 - start1;
	timeUsedMiliSeconds(start1, end1, "D1 smart precalc");

	start1 = clock();
	int smartSum;
	for (int i = 0; i < triesAmount; ++i)
		smartSum = smartSum1[right1 - 1] - ((left1 - 2 < 0) ? 0 : smartSum1[left1 - 2]);
	end1 = clock();
	timeWastedD1[step][2] = end1 - start1;
	timeUsedMiliSeconds(start1, end1, "D1 smart calc");

	cout << smartSum << endl;

	delete[] smartSum1;
	delete[] mas1;
}

// Testing 2-dimentional algorithms
void checkD2(const int length2, const int left2[2], const int right2[2], int step)
{
	cout << "-------------------" << endl;

	clock_t start = clock();
	int **mas2 = new int*[length2];
	for (int i = 0; i < length2; ++i)
		mas2[i] = new int[length2];

	clock_t middle = clock();
	for (int i = 0; i < length2; ++i)
		for (int j = 0; j < length2; ++j)
			mas2[i][j] = 1 + rand() % randMax;

	clock_t end = clock();
	timeUsedSeconds(start, middle, "SM");
	timeUsedSeconds(middle, end, "ME");

	clock_t start2 = clock();
	int sum2;
	for (int i = 0; i < triesAmount; ++i)
		sum2 = foolishSumD2(mas2,
			length2,
			left2[0],
			left2[1],
			right2[0],
			right2[1]);
	clock_t end2 = clock();
	timeWastedD2[step][0] = end2 - start2;
	timeUsedMiliSeconds(start2, end2, "D2 foolish calc");
	cout << sum2 << endl;

	start2 = clock();
	int **smartSum2 = createSmartSumD2(mas2, length2);
	end2 = clock();
	timeWastedD2[step][1] = end2 - start2;
	timeUsedMiliSeconds(start2, end2, "D2 smart precalc");

	start2 = clock();
	int smartSum;
	for (int i = 0; i < triesAmount; ++i)
		smartSum = smartSum2[right2[0] - 1][right2[1] - 1] +
				   ((left2[0] - 2 < 0) ? 0 : ((left2[1] - 2 < 0) ? 0 : smartSum2[left2[0] - 2][left2[1] - 2])) - 
				   ((left2[0] - 2 < 0) ? 0 : smartSum2[left2[0] - 2][right2[1] - 1]) -
				   ((left2[1] - 2 < 0) ? 0 : smartSum2[right2[0] - 1][left2[1] - 2]);
	end2 = clock();
	timeWastedD2[step][2] = end2 - start2;
	timeUsedMiliSeconds(start2, end2, "D2 smart calc");

	cout << smartSum << endl;

	delete[] smartSum2;
	delete[] mas2;
}

// Testing 3-dimentional algorithms
void checkD3(const int length3, const int left3[3], const int right3[3], int step)
{
	cout << "-------------------" << endl;

	clock_t start = clock();
	int ***mas3 = new int**[length3];
	for (int createMasD3I = 0; createMasD3I < length3; ++createMasD3I)
	{

		mas3[createMasD3I] = new int*[length3];

		for (int createMasD3J = 0; createMasD3J < length3; ++createMasD3J)
			mas3[createMasD3I][createMasD3J] = new int[length3];
	}

	clock_t middle = clock();
	for (int i = 0; i < length3; ++i)
		for (int j = 0; j < length3; ++j)
			for (int k = 0; k < length3; ++k)
				mas3[i][j][k] = 1 + rand() % randMax;

	clock_t end = clock();
	timeUsedSeconds(start, middle, "SM");
	timeUsedSeconds(middle, end, "ME");

	clock_t start3 = clock();
	int sum3;
	for (int i = 0; i < triesAmount; ++i)
		sum3 = foolishSumD3(mas3,
			length3,
			left3[0],
			left3[1],
			left3[2],
			right3[0],
			right3[1],
			right3[2]);
	clock_t end3 = clock();
	timeWastedD3[step][0] = end3 - start3;
	timeUsedMiliSeconds(start3, end3, "D3 foolish calc");
	cout << sum3 << endl;

	start3 = clock();
	int ***smartSum3 = createSmartSumD3(mas3, length3);
	end3 = clock();
	timeWastedD3[step][1] = end3 - start3;
	timeUsedMiliSeconds(start3, end3, "D3 smart precalc");

	start3 = clock();
	int smartSum;
	for (int i = 0; i < triesAmount; ++i)
		smartSum = smartSum3[right3[0] - 1][right3[1] - 1][right3[2] - 1] -
					((left3[0] - 2 < 0) ? 0 : smartSum3[left3[0] - 2][right3[1] - 1][right3[2] - 1]) -
					((left3[1] - 2 < 0) ? 0 : smartSum3[right3[0] - 1][left3[1] - 2][right3[2] - 1]) -
					((left3[2] - 2 < 0) ? 0 : smartSum3[right3[0] - 1][right3[1] - 1][left3[2] - 2]) +
					((left3[0] - 2 < 0) ? 0 : ((left3[1] - 2 < 0) ? 0 : smartSum3[left3[0] - 2][left3[1] - 2][right3[2] - 1])) +
					((left3[0] - 2 < 0) ? 0 : ((left3[2] - 2 < 0) ? 0 : smartSum3[left3[0] - 2][right3[1] - 1][left3[2] - 2])) +
					((left3[1] - 2 < 0) ? 0 : ((left3[2] - 2 < 0) ? 0 : smartSum3[right3[0] - 1][left3[1] - 2][left3[2] - 2])) -
					((left3[0] - 2 < 0) ? 0 : ((left3[1] - 2 < 0) ? 0 : ((left3[2] - 2 < 0) ? 0 : smartSum3[left3[0] - 2][left3[1] - 2][left3[2] - 2])));
	end3 = clock();
	timeWastedD3[step][2] = end3 - start3;
	timeUsedMiliSeconds(start3, end3, "D3 smart calc");

	cout << smartSum << endl;

	delete[] smartSum3;
	delete[] mas3;
}

int main()
{
	srand(UTIME);

	const int length1 = 1000000;
	const int left1 = 10;
	const int right1 = 900000;
	for (int i = 0; i < steps; ++i)
	{
		cout << "Step number " << i + 1 << endl;
		checkD1(length1 * (i + 1), left1 * (i + 1), right1 * (i + 1), i);
	}

	ofstream D1;
	D1.open("D1.csv");
	D1 << "Foolish calc;Smart pre;Smart calc;Size;Tries" << endl;
	for (int i = 0; i < steps; ++i)
	{
		D1 << timeWastedD1[i][0] << ";"
			<< timeWastedD1[i][1] << ";"
			<< timeWastedD1[i][2] << ";"
			<< length1 * (i + 1) << ";"
			<< triesAmount << endl;
	}
	D1.close();

	const int length2 = 1000;
	const int left2[2] = { 10, 10 };
	const int right2[2] = { 900, 900 };
	for (int i = 0; i < steps; ++i)
	{
		cout << "Step number " << i + 1 << endl;
		const int left2_[2] = { left2[0] * (i + 1), left2[1] * (i + 1) };
		const int right2_[2] = { right2[0] * (i + 1), right2[1] * (i + 1) };
		checkD2(length2 * (i + 1), left2_, right2_, i);
	}

	ofstream D2;
	D2.open("D2.csv");
	D2 << "Foolish calc;Smart pre;Smart calc;Size;Tries" << endl;
	for (int i = 0; i < steps; ++i)
	{
		D2 << timeWastedD2[i][0] << ";"
			<< timeWastedD2[i][1] << ";"
			<< timeWastedD2[i][2] << ";"
			<< length2 * (i + 1) << ";"
			<< triesAmount << endl;
	}
	D2.close();

	const int length3 = 50;
	const int left3[] = { 10, 10, 10 };
	const int right3[] = { 30, 30, 30 };
	for (int i = 0; i < steps; ++i)
	{
		cout << "Step number " << i + 1 << endl;
		const int left3_[3] = { left3[0] * (i + 1), left3[1] * (i + 1), left3[2] * (i + 1) };
		const int right3_[3] = { right3[0] * (i + 1), right3[1] * (i + 1), right3[2] * (i + 1) };
		checkD3(length3 * (i + 1), left3_, right3_, i);
	}

	ofstream D3;
	D3.open("D3.csv");
	D3 << "Foolish calc;Smart pre;Smart calc;Size;Tries" << endl;
	for (int i = 0; i < steps; ++i)
	{
		D3 << timeWastedD3[i][0] << ";"
			<< timeWastedD3[i][1] << ";"
			<< timeWastedD3[i][2] << ";"
			<< length3 * (i + 1) << ";"
			<< triesAmount << endl;
	}
	D3.close();
	return 0;
}

