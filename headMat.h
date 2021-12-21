#ifndef headMat
#define headMat
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <ctime>
#include<math.h>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include "synchronize.h"
#include "get_time.h"
#include <mutex>
#include "new_synchronize.h"
using namespace std;
typedef struct _ARGS
{
double *matrix; /* матрица */
double *tamrix; /* обратная матрица */
double *mem; /* память */
int n; /* размер матрицы и векторов */
int thread_num; /* номер задачи */
int total_threads; /* всего задач */
int numthr; // число для синхронизаций
int exinum;
int erc; // ошибка 
bool to_finish; // когда можно начать завершающий цикл
} ARGS;

double normByMaxMat(double(*mat), int n);
int uptriangleMat(double mat[], double(*tam), double(*m), int n);
double normMat(double(*mat), double(*tam), int n);
int rotMat(double(*mat), double(*T), int n, int I, int J);
int MatInverse(double mat[], double(*tam), double(*m), int n);
double maxMat(double(*mat), int n);
void normalizeMat(double(*mat), int n, double nor);
void idMat(double(*mat), int n);
int fulMat(double(*mat), int n, int k, string filename);
void multMat(double(*mat1), double(*mat2), double(*m), int n);
int outMat1(double mat[], int n, int m);
int eqMat(double(*mat1), double(*mat2), int n, int m);
double smartNormMat(double (*mat1), double (*mat2), int n);
bool multByRot(double (*mat), double (*tam), double (*m), int n, int i, int &j, int K,int P, int &s, int &S, int eps);
void simpleMultByRot(double (*mat), double (*tam), double (*m), int n, int i, int j, int eps);
void * subUptriangleMat(void *pa);
int endSubUptriangleMat(void *pa);
int newMatInverse(void* pa, int nthreads);
#endif
