#include "headMat.h"

//____________��������� ��� ����� ������� � ���������� ��������______________//

using namespace std;
// ��������� �������
void idMat(double(*mat), int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat[i * n + j] = 0;
		}
	}
	for (int i = 0; i < n; i++)
	{
		mat[i * (n + 1)] = 1;
	}
}
// �� ��������� �������
int fulMat(double(*mat), int n, int k, string filename)
{
	ifstream fin;
	double I = 0, J = 0;
	double N = n;
	switch (k)
	{
	case 0:
		fin.open(filename);
		if (!fin.is_open())
		{
			cout << "File was not opened"<< endl;
			return -1; // �� �������� ����
		}
		for(int i=0; i<n*n; i++)
		{
			if(!fin.eof())
			{
				fin >> mat[i];
			}
			if((fin.eof())&&(i!=(n*n-1)))
			{
				cout << "Filling error" << endl;
				return -1;
			}
			if((!fin.eof())&&(i==(n*n-1)))
			{
				cout << "Filling error" << endl;
				return -1;
			}
		}
		cout << endl << endl;
		fin.close();
		break;
	case 1: // corrected
		for (int i = 0; i < n; i++, I++)
		{
			J = 0;
			for (int j = 0; j < n; j++, J++)
			{
				mat[i * n + j] = N - max(I + 1, J + 1) + 1; // �� ������ �����
			}
		}
		break;
	case 2: // ���� ������� (������� ��� � �������)
		for (int i = 0; i < n; I++, i++)
		{
			J = 0;
			for (int j = 0; j < n; J++, j++)
			{
				mat[i * n + j] = max(I + 1, J + 1);
			}
		}
		break;
	case 3: // ��� ������� (������� ��� � �������)
		for (int i = 0; i < n; I++, i++)
		{
			J = 0;
			for (int j = 0; j < n; J++, j++)
			{
				mat[i * n + j] = abs(I - J);
			}

		}
		break;
	case 4: // ���� ������� (������� ��������)
		for (int i = 0; i < n; I++, i++)
		{
			J = 0;
			for (int j = 0; j < n; J++, j++)
			{
				mat[i * n + j] = (1 / (I + J + 1));
			}
		}
		break;
	case 5:
		for (int i = 0; i < n; I++, i++)
		{
			J = 0;
			for (int j = 0; j < n; J++, j++)
			{
				mat[i * n + j] = J + I;
			}
		}
		break;
	case 6:
		for (int i = 0; i < n; I++, i++)
		{
			for (int j = 0; j < n; J++, j++)
			{
				mat[i * n + j] = 0;
			}
			mat[i * n + i] = I + 1;
		}

		break;
	case 7:
		for (int i = 0; i < n; I++, i++)
		{
			J = 0;
			for (int j = 0; j < i + 2; J++, j++)
			{
				mat[i * n + j] = abs(I - 2 * J) + 1;
			}
			for (int j = i + 1; j < n; J++, j++)
			{
				mat[i * n + j] = 0;
			}
		}
		break;
	case 8:
		for (int i = 0; i < n; I++, i++)
		{
			J = 0;
			for (int j = 0; j < i + 2; J++, j++)
			{

				mat[i * n + j] = abs(I * I + J) + 1;
			}
		}
		for (int i = 0; i < n; I++, i++)
		{
			for (int j = 0; j < i + 2; J++, j++)
			{
				if (i < j)
				{
					mat[i * n + j] = 0;
				}
			}
		}

		I = 0;
		break;
		case 9:
		I = n-1;
		for (int i = 0; i < n; I--, i++)
		{
			J = n-1;
			for (int j = 0; j < n; J--, j++)
			{
				mat[i * n + j] = (1 / (I + J+1));
			}
		}
		break;
		case 10:
		I = n-1;
		for (int i = 0; i < n; I--, i++)
		{
			J = n-1;
			for (int j = 0; j < n; J--, j++)
			{
				if(i<=j)
				{
				mat[i * n + j] = N - max(I + 1, J + 1) + 1;
				}
			}
		}
		break;

	}
	

	return 0;
}
//��������� mat2 := mat1 * mat2
void multMat(double(*mat1), double(*mat2), double(*m), int n)
{
	double a = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a = 0;
			for (int k = 0; k < n; k++)
			{
				a = a + mat1[i * n + k] * mat2[k * n + j];
			}
			m[i * n + j] = a;
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat2[i * n + j] = m[i * n + j];
		}
	}
}
// ������� �������
int outMat1(double mat[], int n, int m)
{
	if (n < m)
	{
		return -1;
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << setprecision(3);
			cout << mat[i * n + j] << ' ';
			//mat[i * n + j]  =0;
		}
		cout << endl;
	}
	cout << endl << endl;
	return 0;
}
// mat1:= mat2
int eqMat(double(*mat1), double (*mat2), int n, int m)
{
	if (m != n)
	{
		return -3;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat1[i * n + j] = mat2[i * n + j];
		}
	}
	return 0;
}
double smartNormMat(double (*mat1), double (*mat2), int n)
{
	double a = 0;
	double A=0, max =0;
	for (int i = 0; i < n; i++)
	{
		A=0;
		for (int j = 0; j < n; j++)
		{
			a = 0;
			for (int k = 0; k < n; k++)
			{
				a = a + (mat1[i * n + k] * mat2[k * n + j]);
			}
			if(i==j)
			{
				a -= 1;
			}
			A += abs(a);
		}

		if (A>max)
		{
			max = A;
		}
	}
	return max;
}