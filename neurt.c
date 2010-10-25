/** 22.10.2010 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

const int LENGHT_STR = 100;
int NR = 6, NH = 10, NOut = 6, NC = 0;
double **W1, **W2, **W1n, **W2n;
double *S1, *S2, *S3;
double **x, **y;
int State, m;
double Step = 0.4, e, Net = 0.15, Emax = 0.1, epoch = 3e3;
char *datafile = "1_input.txt", *printw1="data/w1", *printw2="data/w2";
char *resultfilename = "data/result.txt", *configname = "data/neurt.conf";

int read_config(char *configname, int *layouts, int *first_ball, int *last_ball, int *count_ball, int *result_rows, int *result_rows_count, double *ep)
{
	char confstr[LENGHT_STR], *confstrp, *filemode = "r";
	FILE* configfile;
	int i, j;
	if ((configfile = fopen(configname, filemode)) == NULL)
	{
		fprintf(stderr, "Cannot read config file\n");
		return 1;
	}
	i = 1;
	while (!feof(configfile))
	{
		if ((confstrp = fgets(confstr,LENGHT_STR,configfile)) != NULL)
		{
			if ((confstrp[0] != '\0') && (confstrp[0] != '#'))
			{
				j = atoi(confstrp);
				switch (i)
				{
					case 1:
						{
							if (j != 0) *layouts = j;
							break;
						}
					case 2:
						{
							if (j != 0) *first_ball = j;
							break;
						}
					case 3:
						{
							if (j != 0) *last_ball = j;
							break;
						}
					case 4:
						{
							if (j != 0) *count_ball = j;
							break;
						}
					case 5:
						{
							if (j != 0) *result_rows = j;
							break;
						}
					case 6:
						{
							if (j != 0) *result_rows_count = j;
							break;
						}
					case 7:
						{
							if (j != 0) *ep = j;
							break;
						}
				}
				i = i + 1;
			}
			
		}
	}
	fclose(configfile);
	return 0;
}

int rows_infile(char* filename)
{
	char confstr[100];
	FILE* fileinode;
	int i = 0;
	if ((fileinode = fopen(filename, "r")) == NULL) 
	{
		printf("\n%s - %s n", "Cannot read file ",filename);
		return i;
	}
    // go by file
	while (!feof(fileinode)) 
	{
		if (fgets(confstr,LENGHT_STR,fileinode) != NULL)
		{
			i = i + 1;			
		}
	}
	fclose(fileinode);
	return i;
}

double tointerval(int a, int b, int c)
{
	return ((double) (a - b)) / (c - b);
}

int frominterval(double a, int b, int c)
{
	return (int)(a * (c - b) + b);
}

double** read_datax(char* filename, int rows, int balls, int first, int last)
{	
	FILE* fileinode;
	int i, j, k;
	double **myarray;
	if ((fileinode = fopen(filename, "r")) == NULL) 
	{
		printf("\n%s - %s n", "Cannot read file ",filename);
		return 0;
	}
	if ((myarray = (double**)malloc(rows*sizeof(double*))) != NULL) 
	{
        for (i = 0; i < rows; i = i + 1)
        {
			if ((myarray[i] = (double*)malloc(balls*sizeof(double))) != NULL)
			{
				for (j = 0; j < balls; j = j + 1)
				{
					fscanf(fileinode, "%d", &k);
					myarray[i][j] = tointerval(k, first, last);
				}
			}
			else
			{
				fprintf(stderr, "error malloc\n");
				exit(1);
			}
		}
    }
    else
    {
		fprintf(stderr, "error malloc\n");
        exit(1);
	}
	fclose(fileinode);
	return myarray;
}

double** read_datay(double **a, int rows, int balls)
{	
	int i, j;
	double **myarray;
	if ((myarray = (double**)malloc(rows*sizeof(double*))) != NULL) 
	{
        for (i = 0; i < (rows - 1); i = i + 1)
        {
			if ((myarray[i] = (double*)malloc(balls*sizeof(double))) != NULL)
			{
				for (j = 0; j < balls; j = j + 1)
				{
					myarray[i][j] = a[i + 1][j];
				}
			}
			else
			{
				fprintf(stderr, "error malloc\n");
				exit(1);
			}
		}
    }
    else
    {
		fprintf(stderr, "error malloc\n");
        exit(1);
	}
	return myarray;
}

int output_array(int **a, int n, int m)
{
	int i, j;
	printf("\n");
	for (i = 0; i < n; i = i + 1)
	{
		for (j = 0; j < m; j = j + 1)
		{
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
	return 0;
}

int output_vect(double *a, int n, int vtype, int first, int last)
{
	int j;
	printf("\n");
	if (vtype == 1)
	{
		for (j = 0; j < n; j = j + 1)
		{
			printf("%d\t", frominterval(a[j], first, last));
		}
	}
	else
	{
		for (j = 0; j < n; j = j + 1)
		{
			printf("%f\t", a[j]);
		}
	}
	return 0;
}

int output_vectf(double *a, int n, int first, int last)
{
	FILE* fileinode;
	int j;
	if ((fileinode = fopen(resultfilename, "w")) == NULL) 
	{
		fprintf(stderr, "Cannot read file for write result \n");
		return 1;
	}
	for (j = 0; j < n; j = j + 1)
	{
		fprintf(fileinode, "%d\t", frominterval(a[j], first, last));
	}
	fclose(fileinode);
	return 0;
}

int output_doublearray(double **a, int n, int m)
{
	int i, j;
	printf("\n");
	for (i = 0; i < n; i = i + 1)
	{
		for (j = 0; j < m; j = j + 1)
		{
			printf("%f\t", a[i][j]);
		}
		printf("\n");
	}
	return 0;
}

int irand(int first, int last)
{
	double a;
	// srand((unsigned)time(NULL));
	a = ((double) rand() /(RAND_MAX + 1.0));
	return (first + a * (last - first));
}

int generate(char *filename, int n, int m, int first, int last)
{
	FILE* fileinode;
	int i, j, a;
	if ((fileinode = fopen(filename, "w")) == NULL) 
	{
		fprintf(stderr, "Cannot read file for write generation\n");
		return 1;
	}
	for (i = 0; i < n; i = i + 1)
	{
		for (j = 0; j < m; j = j + 1)
		{
			a = irand(first, last);
			// print to file
			fprintf(fileinode, "%d ", a);
		}
		if (j != (m - 1)) fprintf(fileinode, "\n");
	}
	fclose(fileinode);
	return 0;
}

int printw(char *filename, double **w, int n, int m)
{
	FILE* fileinode;
	int i, j;
	if ((fileinode = fopen(filename, "w")) == NULL) 
	{
		fprintf(stderr, "Cannot read file for write W \n");
		return 1;
	}
	for (i = 0; i < n; i = i + 1)
	{
		for (j = 0; j < m; j = j + 1)
		{
			fprintf(fileinode, "%f ", w[i][j]);
		}
		if (j != (m - 1)) fprintf(fileinode, "\n");
	}
	fclose(fileinode);
	return 0;
}

double** initw(int a, int b)
{
	int i, j;
	double x1 = -5.0, x2 = 5.0;
	double **myarray;
	if ((myarray = (double**)malloc(a*sizeof(double*))) != NULL) 
	{
		for (i = 0; i < a; i = i + 1)
        {
			if ((myarray[i] = (double*)malloc(b*sizeof(double))) != NULL)
			{
				for (j = 0; j < b; j = j + 1)
				{		
					myarray[i][j] = ((double) rand() /(RAND_MAX + 1.0));
					myarray[i][j] = x1 + myarray[i][j] * (x2 - x1);
				}
			}
			else
			{
				fprintf(stderr, "error malloc wc\n");
				exit(1);
			}
		}
	}
	else
	{
		fprintf(stderr, "error malloc wr\n");
		exit(1);
	}	
	return myarray;
}

double** initdouble(int a, int b)
{
	int i;
	double **myarray;
	if ((myarray = (double**)malloc(a*sizeof(double*))) != NULL) 
	{
		for (i = 0; i < a; i = i + 1)
        {
			if ((myarray[i] = (double*)malloc(b*sizeof(double))) == NULL)
			{
				fprintf(stderr, "error malloc wc\n");
				exit(1);
			}
		}
	}
	else
	{
		fprintf(stderr, "error malloc wr\n");
		exit(1);
	}	
	return myarray;
}

int** initint(int a, int b)
{
	int i;
	int **myarray;
	if ((myarray = (int**)malloc(a*sizeof(int*))) != NULL) 
	{
		for (i = 0; i < a; i = i + 1)
        {
			if ((myarray[i] = (int*)malloc(b*sizeof(int))) == NULL)
			{
				fprintf(stderr, "error malloc wc\n");
				exit(1);
			}
		}
	}
	else
	{
		fprintf(stderr, "error malloc wr\n");
		exit(1);
	}	
	return myarray;
}

double* initneuronstate(int a)
{
	double *s;
	if ((s = (double*)malloc(a*sizeof(double))) == NULL)
	{
		fprintf(stderr, "error malloc S1\n");
		exit(1);
	}
	return s;
}

int neuron(int i, int j)
{
	int k;
	double Sum1 = 0.0, Sum = 0.0;
	switch (i)
	{
		case 1:
		{ 
			S1[j] = x[m][j]; 
			break;
		}
		case 2:
		{
			Sum1 = 0.0;
			for (k = 0; k < NR; k = k + 1)
			{
				Sum1 = Sum1 + S1[k] * W1[k][j];
			}
			Sum1 = Sum1 + Net;
			S2[j] = 1.0 / ( 1 + exp( -Sum1 ));
			break;
		}
		case 3:
		{
			Sum = 0.0;
			for ( k = 0; k < NH; k = k + 1)
			{
				Sum = Sum + S2[k] * W2[k][j];
			}
			Sum = Sum + Net;
			S3[j] = 1.0 / (1 + exp( -Sum ));
			break;
		}
	}
	return 0;
}

int learn_neurt(int a, int b, int c)
{
	int i, j, k, OK = 0;
	double S, G3[c];
	double **W2n, **W1n;
	e = 0;
	// memory rezerf
	W1n = initdouble(a, b);
	W2n = initdouble(b, c);
	while (!OK)
	{ 
		if( State == 1)
		{
			for (i = 0; i < a; i = i + 1)
			{
				neuron(1, i);
			}
            for (i = 0; i < b; i = i + 1)
            {
				neuron(2, i);
			}
			for (i = 0; i < c; i = i + 1)
			{
				neuron(3,i);
				G3[i] = y[i][m];
			}
		}
        OK = 1;
        for ( i = 0; i < c; i = i + 1)
        {
			for(j = 0; j < b; j = j + 1)
         	W2n[j][i] = W2[j][i] + Step * (G3[i]-S3[i]) * S2[j] * S3[i] * (1 - S3[i]);
		}
        for ( i = 0; i < a; i = i + 1)
        {
			for( j = 0; j < b; j = j + 1)
			{
				S = 0.0;
                for (k = 0; k < c; k = k + 1)
                {
					S = S + (G3[k] - S3[k]) * S3[k] * (1 - S3[k]) * W2n[j][k];
				}
                W1n[i][j] = W1[i][j] + Step * S * S1[i] * S2[j] * (1 - S2[j]);
			}
		}
	}
	for (i = 0; i < a; i = i + 1)
    {
		for (j = 0; j < b; j = j + 1)
        {
			W1[i][j] = W1n[i][j];
			for (k = 0; k < c; k = k + 1)
            {
				W2[j][k] = W2n[j][k];
			}
         }
	}
	for( i = 0; i < NR; i = i + 1)
	{
		neuron(1,i);
	}
	for (i = 0; i < NH; i = i + 1)
    {
		neuron(2,i);
	}
	for(i = 0; i < NOut; i = i + 1)
    {
		neuron(3,i);
		G3[i] = y[i][m];
		e = e + 0.5 * (G3[i] - S3[i]) * (G3[i] - S3[i]);
	}
	State=2;
	return 0;
}

int testinitw(char *ww1, char *ww2, int a, int b, int c)
{
	FILE* fileinode;
	int i, j;
	float aa;
	// W1
	W1 = initdouble(a,b);
	if ((fileinode = fopen(ww1, "r")) == NULL) 
	{
		fprintf(stderr, "error read test file \n");
		exit(1);
	}
	for (i = 0; i < a; i = i + 1)
	{
		for (j = 0; j < b; j = j + 1)
		{
			fscanf(fileinode, "%f", &aa);
			W1[i][j] = aa;
		}
	}
	fclose(fileinode);
	// W2
	W2 = initdouble(b,c);
	if ((fileinode = fopen(ww2, "r")) == NULL) 
	{
		fprintf(stderr, "error read test file \n");
		exit(1);
	}
	for (i = 0; i < b; i = i + 1)
	{
		for (j = 0; j < c; j = j + 1)
		{
			fscanf(fileinode, "%f", &aa);
			W2[i][j] = aa;
		}
	}
	fclose(fileinode);
	return 0;
}

int test(int a, int b, int c, int rows, int first, int last)
{
	int i, j;
	x = initdouble(1,a);
	m = 0;
	printf("\n Enter %d numbers: ", c);
	for (i = 0; i < c; i = i + 1)
	{
		scanf("%d", &j);
		x[m][i] = tointerval(j, first, last);
	}
	// neuron
	for(i = 0; i < a; i = i + 1)
    {
		neuron(1,i);
	}
    for (i = 0; i < b; i  = i + 1)
    {
		neuron(2,i);
	}
	for (i = 0; i < c; i  = i + 1)
	{
		neuron(3,i);
	}
	//printf("\n S1");
	//output_vect(S1, a, 0, first, last);
	//printf("\n S2");
	//output_vect(S2, b, 0, first, last);
	//printf("\n S3");
	//output_vect(S3, c, 0, first, last);
	printf("\n result: ");
	output_vect(S3, c, 1, first, last);
	output_vectf(S3, c, first, last);
	return 0;
}

int main (int argc, char** argv) 
{
	int first_ball, last_ball, count_ball, layouts, result_rows, result_rows_count, p;
	char usertest;
	double E, diftime;
	clock_t st,fin;
	// defaults
	first_ball = 1;
	last_ball = 90;
	count_ball = 6;
	layouts = 2;
	result_rows = 10;
	result_rows_count = 6;
	//Start
	printf("%s", "\nProgram start\n");
	// read cinfig file
	if (read_config(configname, &layouts, &first_ball, &last_ball, &count_ball, &result_rows, &result_rows_count, &epoch) == 1)
	{
		fprintf(stderr, "\nCannot read config file");
		return 1;
	}
	// arguments
	if (argc > 1)
	{
		if (argc > 2)
		{
			fprintf(stderr, "To many arguments\n"); //or few
			return 1;
		}
		else
		{
			// generate 100 rows
			//printf("generations rand numbuers ...\n");
			generate(datafile, 100, count_ball, first_ball, last_ball);
			// count data file
			printf("\n reading datafile ...");
			NC = rows_infile(datafile);
			if (NC < 2)
			{
				fprintf(stderr, "\n Very empty data file"); //or few
				return 1;
			}
			// read count data file
			x = read_datax(datafile, NC, count_ball, first_ball, last_ball);
			y = read_datay(x, NC, count_ball);
			// output array
			//output_array(x, datafile_rows, count_ball);
			// init learns var
			NOut = result_rows_count;
			// init weights - random number's 0..1
			W1 = initw(NR, NH);
			W2 = initw(NH, NOut);
			// init neuron state's
			S1 = initneuronstate(NR);
			S2 = initneuronstate(NH);
			S3 = initneuronstate(NOut);
			State = 1;
			E = p = 0; 
			Emax = Emax * (NC -1);
			st=clock();
			// start learn 
			printf("\n learning ...\n ");
			do
			{
				E = 0;
				for (m = 0; m < (NC - 1); m = m + 1)
				{
					learn_neurt(NR, NH, NOut);
					E = E + e;
				}
				p = p + 1;
				/*
				if ((p % 1000) == 0)
				{
					printf("\n ************* p=%d \t e=%e \t E=%f", p, e, E);		
					//output_doublearray(W1, NR, NH);
				}*/
			}
			while ((E >= Emax) && (p < epoch));
			// end learn
			fin = clock();
			diftime = (fin-st)/(CLOCKS_PER_SEC);
			printf("\n Time learn: %16.14f", diftime);
			printf("\n Error: %f \t Emax=%f", E, Emax);
			printw(printw1, W1, NR, NH);
			printw(printw2, W2, NH, NOut);
			//printf("\nlayouts=%d, first_ball=%d, last_ball=%d, count_ball=%d, result_rows=%d, result_rows_count=%d, datafile=%s ", layouts, first_ball, last_ball, count_ball, result_rows, result_rows_count, datafile);
			// clear memory
			free(y);
		}
	}
	else
	{
		// test play
		while(1)
		{
			printf("\n Play:");
			// read weights
			testinitw(printw1, printw2, NR, NH, NOut);
			// init neuron state's
			S1 = initneuronstate(NR);
			S2 = initneuronstate(NH);
			S3 = initneuronstate(NOut);
			//printf("\n W1");
			//output_doublearray(W1, NR, NH);
			//printf("\n W2");
			//output_doublearray(W2, NH, NOut);
			test(NR, NH, NOut, result_rows, first_ball, last_ball);
			//output_array(x, 1, NR);
			printf("\n Continue play (y/n): ");
			scanf("%1s", &usertest);
			if (usertest != 'y') break;
		}	
	}
	free(W1);
	free(W2);
	free(S1);
	free(S2);
	free(S3);
	free(x);
	printf("%s", "\nProgram End\n");
	getchar();
	return 0;
}
