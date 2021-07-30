//Matrix Multiplication Using multithreading 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*function declarations*/
void *readMatrixA(void *id);
void *readMatrixB(void *id);
void *multiply(void *id);

//declaring global variables so they are accessible anywhere
int rowCount1 = 0;
int colCount1 = 0;
int rowCount2 = 0;
int colCount2 = 0;
char matAname[256] = "";
char matBname[256] = "";
int matrixA[256];
int matrixB[256];
int neg1 = -1;

/*In my approach, I read the the matrices into separate arrays(1D) 
and use some mathematics to know the location of elements as matrices*/
int main(int argc, char *argv[])
{
	//reading input file names into string variables
// 	strcat(matAname,argv[1]);
// 	strcat(matBname,argv[2]);
    sscanf(argv[1], "%s",&matAname);
    sscanf(argv[2], "%s",&matBname);

	//creates three threads
	pthread_t threads[3];
	
	//thread to read matrixA
	if(pthread_create(&threads[0], NULL, readMatrixA, (void *)0))
	{
         printf("\nError: unable to create thread to read %s\n", matAname);
         exit(-1);
    }
    
    //thread to read matrixB
    if(pthread_create(&threads[1], NULL, readMatrixB, (void *)1))
	{
         printf("\nError: unable to create thread to read %s\n", matBname);
         exit(-1);
    }
    //wait for the two threads to finish reading before you multiply
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    
    //thread to multiply matrices
    if(pthread_create(&threads[2], NULL, multiply, (void *)2))
	{
         printf("\nError: unable to create thread to multiply matrices\n");
         exit(-1);
    }
    pthread_join(threads[2],NULL);
    
	return 0;
}

/*Reads matrixA*/
void *readMatrixA(void *id)
{
	int tid;
 	tid = (long)id;
	printf("\nThis is thread %d and I am to read %s\n", tid,matAname);

	/*Opening the matrix textfile for reading */
	FILE *matfile;
	matfile = fopen(matAname,"r");
	if (matfile == NULL)printf("\nCould not open file %s, Please specify an available file.",matAname);
	char row[256];
	
	rowCount1 = 0;
	colCount1 = 0;
	int prevColumnCount = 0;

	/*Obtains lines from input text file*/
	int index=0;
	while (fgets(row, sizeof(row), matfile)) {

        /*take off \n character and splits rows by space*/
		char *number = strtok(row, "\n");
		number = strtok(row, " ");

		colCount1 = 0;
		/*loop through string to obtain numbers in row
		and print them to the temporary file*/
		while( number != NULL ) {
			int x = atoi(number);
			matrixA[index] = x;
			index++;
			colCount1 = colCount1 + 1;
			number = strtok(NULL, " ");
		}
		if(prevColumnCount!=0 && prevColumnCount!=colCount1)
		{
			printf("\nSome rows in %s have different dimensions\n", matAname);
			return &neg1;
		}
		else
		{
			prevColumnCount = colCount1;
			rowCount1 = rowCount1 + 1;
		}
    }
    fclose(matfile);
    printf("\nThis is thread %d again and I am done reading %s\n", tid, matAname);
}

/*Reads matrixB*/
void *readMatrixB(void *id)
{
	int tid;
 	tid = (long)id;
	printf("\nThis is thread %d and I am to read %s\n", tid, matBname);

	/*Opening the matrix textfile for reading */
	FILE *matfile;
	matfile = fopen(matBname,"r");
	if (matfile == NULL)printf("\nCould not open file %s, Please specify an available file.",matBname);
	char row[256];
	
	rowCount2 = 0;
	colCount2 = 0;
	int prevColumnCount = 0;

	/*Obtains lines from input text file*/
	int index = 0;
	while (fgets(row, sizeof(row), matfile)) {

        /*take off \n character and splits rows by space*/
		char *number = strtok(row, "\n");
		number = strtok(row, " ");

		colCount2 = 0;
		/*loop through string to obtain numbers in row
		and print them to the temporary file*/
		while( number != NULL ) {
			int x = atoi(number);
			matrixB [index] = x;
			index ++;
			colCount2 = colCount2 + 1;
			number = strtok(NULL, " ");
		}
		if(prevColumnCount!=0 && prevColumnCount!=colCount2)
		{
			printf("\nSome rows in %s have different dimensions\n", matBname);
			return &neg1;
		}
		else
		{
			prevColumnCount = colCount2;
			rowCount2 = rowCount2 + 1;
		}
    }
    fclose(matfile);
    printf("\nThis is thread %d again and I am done reading %s\n", tid, matBname);
}

/*Multiplies the matrices*/
void *multiply(void *id)
{
    int tid;
 	tid = (long)id;
	printf("\nThis is thread %d and I am multiply the two matrices\n", tid);
	
	//creating result text file as matrixC.txt
	FILE *resultfile;
	resultfile = fopen("matrixC.txt","w+");

    //checking if matrices can be multiplied
	if(colCount1 != rowCount2)
	{
		printf("\n\nMatrix of dimension %dx%d cannot be multiplied by matrix of dimention %dx%d\n", rowCount1, colCount1, rowCount2, colCount2);
	}
	else if (colCount1==0 || colCount2==0 || rowCount1==0 || rowCount2==0)
	{
		printf("\nOne of the matrices has an invalid dimension. The dimensions are %dx%d by %dx%d\n",
		rowCount1,colCount1,rowCount2,colCount2);
	}
	//performing matrix multiplication
	else
	{
	    printf("\nResults are as follows:\n");
		int result[rowCount1][colCount2];
		int i, j, k; 
	    for (i = 0; i < rowCount1; i++) 
	    { 
	        for (j = 0; j < colCount2; j++) 
	        { 
	            result[i][j] = 0; 
	            for (k = 0; k < colCount1; k++)
	            {
	                result[i][j] += matrixA[(i*colCount1)+k]*matrixB[j+(k*colCount2)];
	            }
	            fprintf(resultfile, "%d ", result[i][j]);
	            printf("%d\t", result[i][j]);
	        }
	        fprintf(resultfile, "\n");
	        printf("\n"); 
	    } 
	    printf("\n\nResults are in a file named matrixC.txt as well");
	    
	    printf("\nThis is thread %d again and I am done multiplying the two matrices\n\n-------THANK YOU-------", tid);
	}
	fclose(resultfile);
}