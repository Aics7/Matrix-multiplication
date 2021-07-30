#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*function declaration for the readMatrix function*/
void readMatrix(char []);

int main(int argc, char *argv[])
{
	/*Reading arguments from terminal*/
    readMatrix(argv[1]);
    readMatrix(argv[2]);

    /*Creating names for temporary text files that were created in the readMatrix method*/
    char temp1[50]= "temp_";
	strcat(temp1,argv[1]);

	char temp2[50]= "temp_";
	strcat(temp2, argv[2]);

	/*Reading matrixA and writing to a multidimensional (2D) array*/
	FILE *matfile1;
	matfile1 = fopen(temp1,"r");
	if (matfile1 == NULL)printf("An error occured while trying to access a temporary file, Please try again");
	char row[256];

	fgets(row, sizeof(row), matfile1);
    
	char *line = strtok(row, "\n");
	char line1[256] = "";
	strcat(line1, line);

	fgets(row, sizeof(row), matfile1);
	line = strtok(row, "\n");

	char line2[256] = "";
	strcat(line2, line);
	
	char *rowcol = strtok(line2, " ");
	int rowCount1 = atoi(rowcol);
	rowcol = strtok(NULL, " ");
	int colCount1 = atoi(rowcol);

	//creating matrixA
	int matrixA[rowCount1][colCount1];
	
	/*loop through string to obtain numbers in row*/
	char *digit = strtok(line1, " ");
	int curRow=0;
	int curCol=0;
	int x;
	while( digit != NULL ) {
		x = atoi(digit);

		digit = strtok(NULL, " ");
		matrixA[curRow][curCol]=x;
		curCol++;
		
		if(curCol>=colCount1)
		{
			curCol=0;
			curRow++;
		}
	}
    fclose(matfile1);
    remove(temp1);//delete temporary file

    /*Reading matrixB and writing to a multidimensional (2D) array*/
	matfile1 = fopen(temp2,"r");
	if (matfile1 == NULL)printf("An error occured while trying to access a temporary file, Please try again");
	fgets(row, sizeof(row), matfile1);
    
	line = strtok(row, "\n");
	char bline1[256] = "";
	strcat(bline1, line);

	fgets(row, sizeof(row), matfile1);
	line = strtok(row, "\n");

	char bline2[256] = "";
	strcat(bline2, line);

	rowcol = strtok(bline2, " ");
	int rowCount2 = atoi(rowcol);
	rowcol = strtok(NULL, " ");
	int colCount2 = atoi(rowcol);

	//creating matrixB
	int matrixB[rowCount2][colCount2];

	digit = strtok(bline1, " ");
	curRow=0;
	curCol=0;
	while( digit != NULL ) {
		int x = atoi(digit);
	
		digit = strtok(NULL, " ");
		matrixB[curRow][curCol]=x;

		curCol++;
		if(curCol>=colCount2)
		{
			curCol=0;
			curRow++;
		}
	}
    fclose(matfile1);
    remove(temp2);//delete temporary file

    
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
		printf("One of the matrices has an invalid dimension. The dimensions are %dx%d by %dx%d\n",
		rowCount1,colCount1,rowCount2,colCount2);
	}
	//performing matrix multiplication
	else
	{
		int result[rowCount1][colCount2];
		int i, j, k; 
	    for (i = 0; i < rowCount1; i++) 
	    { 
	        for (j = 0; j < colCount2; j++) 
	        { 
	            result[i][j] = 0; 
	            for (k = 0; k < colCount1; k++)
	            {
	                result[i][j] += matrixA[i][k]*matrixB[k][j]; 
	            }
	            fprintf(resultfile, "%d ", result[i][j]);
	            printf("%d\t", result[i][j]);
	        }
	        fprintf(resultfile, "\n");
	        printf("\n"); 
	    } 
	    printf("\n\nResults are in a file named matrixC.txt as well\n");
	}
	fclose(resultfile);
	return 0;
}

/*Reads matrix from a text file, counts the number of rows and columns,
and writes the elements of the matrix into a temporary text file.
The elements of the matrix are written in the first line of the temporary file
and the second line contains the number of rows and columns separated by space.
This method allows me know the dimensions of the matrices so I can easily deduce
if multiplication is possible and the dimensions of the resulting a matrix (2D Array)*/
void readMatrix(char fileName[])
{
	/*Opening the matrix textfile for reading */
	FILE *matfile;
	matfile = fopen(fileName,"r");
	if (matfile == NULL)printf("Could not open file %s, Please specify an available file.",fileName);
	char row[256];
	
	int rowCount = 0;
	int columnCount = 0;
	int prevColumnCount = 0;
	char tempName[]= "temp_";
	strcat(tempName,fileName);
	/*Opening a temporary textfile for writing */
	FILE *tempfile;
	tempfile = fopen(tempName,"w+");

	/*Obtains lines from input text file*/
	while (fgets(row, sizeof(row), matfile)) {

        /*take off \n character and splits rows by space*/
		char *number = strtok(row, "\n");
		number = strtok(row, " ");

		columnCount = 0;
		/*loop through string to obtain numbers in row
		and print them to the temporary file*/
		while( number != NULL ) {
			int x = atoi(number);
			fprintf(tempfile, "%d ", x);
			columnCount++;
			number = strtok(NULL, " ");
		}
		if(prevColumnCount!=0 && prevColumnCount!=columnCount)
		{
			printf("Some rows in %s have different dimensions\n", fileName);
			return;
		}
		else
		{
			prevColumnCount = columnCount;
			rowCount++;
		}
    }
    fprintf(tempfile, "\n%d %d", rowCount, columnCount);
    fclose(tempfile);
    fclose(matfile);
}