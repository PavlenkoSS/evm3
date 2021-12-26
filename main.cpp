#include "headMat.h"
using namespace std;



// размер  количество выводимых строк, потоки, формула

int main(int nargs, char** args)
{
	if (nargs < 5)
	{
		cout << "Not enought args: launch as following..." << endl;


		return -1;
	}

	int n = 0;
	int m = 0;
	int k = 1;
	
	int nthreads = 1;

	char* filename = new char[128];
	if (sscanf(args[1], "%d", &n) != 1 || sscanf(args[2], "%d", &m) != 1 || sscanf(args[3], "%d", &nthreads) != 1 || sscanf(args[4], "%d", &k) != 1)
	{

delete[] filename;




		cout << "bad params" << endl;
		return -1;
	}

	if(n==0)
	{delete[] filename;



		cout << "bad param n" << endl;
		return -1;
	}
	if(args[5] == nullptr || sscanf(args[5], "%s", filename) != 1)
    {
        cout << "No filename" << endl;
delete[] filename;


    	return -1;
    }

	double* M;
	double* E;
	double* mem2;
	double* Sins;
	double* Coss;
	Sins = new double [n];
	Coss = new double[n];
	M = new double[n * n];
	mem2 = new double[4*n*nthreads];
	E = new double[n * n];	
	idMat(E, n);

	
	if (fulMat(M, n, k, string(filename)) == -1)
	{delete[] filename;
delete[]Sins;
delete[]Coss;
delete[]E;	delete[]M;
		delete[]mem2;
		return -1;
	}

	cout << "M = " << endl;
	outMat1(M, n,m);
	ARGS * argus;
	//double start_time = clock();
	    if (!(argus = (ARGS*) malloc (sizeof (ARGS))))
    {
        fprintf (stderr, "Not enough memory!\n");
        return 2;
    }
	argus[0].matrix = M;
	argus[0].mem = mem2;
	argus[0].tamrix = E;
	argus[0].erc = 0;
	argus[0].n = n;
	argus[0].to_finish = false;
	argus[0].total_threads = nthreads;
	argus[0].thread_num = 0;
	argus[0].numthr= nthreads;
	argus[0].exinum = 0;
	argus[0].Coss = Coss;
	argus[0].Sins = Sins;
	argus[0].res=0;
	//cout << "here" << endl;
	//cout << argus[0].erc << ' ' << argus[0].to_finish <<endl;
	long int tm = get_full_time();
	if(newMatInverse(argus,nthreads)!=0)
	{
				cout << "UNINVERTABLE " << endl;
delete[] filename;
delete[]Sins;
delete[]Coss;
		delete[]E;
		delete[]M;
		delete[]mem2;
		return -2;
	}
	//cout << argus[0].erc << ' ' << argus[0].to_finish <<endl;
	tm = get_full_time()-tm;
	if (argus[0].erc != 0)
	{
		cout << "UNINVERTABLE " << endl;
delete[] filename;
delete[]Sins;
delete[]Coss;
	
delete[]E;
		delete[]M;
		delete[]mem2;
		return -2;
	}
;
	fulMat(M, n, k, string(filename));
	//cout << "sins " << smartNormMat(M, E, n)<< endl;
	double elapsed = double(tm)/100;
	fulMat(argus[0].matrix, n, k, string(filename));
			argus[0].thread_num = 0;

	normThread(argus,nthreads);

	double residual = argus[0].res;

	printf("%s : residual = %e elapsed = %.2f s = %d n = %d m = %d p = %d\n",args[0], residual, elapsed, k, n, m, nthreads);
	

delete[] filename;
delete[]Sins;
delete[]Coss;	
delete[]M;
	delete[]E;
	delete[]mem2;
	return 0;
}











