// Matrix dot product using multi-threading

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define SIZE1 3
#define SIZE2 3
#define SIZE3 3

float matrixA[SIZE1][SIZE2];
float matrixB[SIZE2][SIZE3];
float result[SIZE1][SIZE3];

int row_i = 0;
int col_j = 0;

void *dot(void* arg)
{
	int j, k;

	for(j = 0; j < SIZE2; j++)
	{
		//for(k = 0; k < SIZE3; k++)
		//{
			result[row_i][col_j] += matrixA[row_i][j] * matrixB[j][col_j];
		//}
	}

	if(col_j == SIZE3-1)
        {
                col_j = 0;
                row_i++;
        }
	else
	{
		col_j++;
	}

	pthread_exit(NULL);
}

int main(void)
{
	int i, j;
	// Generating random values
    	for (i = 0; i < SIZE1; i++) {
        	for (j = 0; j < SIZE2; j++) {
            		matrixA[i][j] = (float)(rand() % 20)/5.0;
        	}
    	}

	for (i = 0; i < SIZE2; i++) {
                for (j = 0; j < SIZE3; j++) {
                        matrixB[i][j] = (float)(rand() % 20)/5.0;
                }
        }

	// Initializing matrix result
	for (i = 0; i < SIZE1; i++) {
                for (j = 0; j < SIZE3; j++) {
                        result[i][j] = 0;
                }
        }

    	// Displaying matrixA
	printf("\nMatrix A: \n");
    	for (i = 0; i < SIZE1; i++) {
        	for (j = 0; j < SIZE2; j++)
            		printf("%f  ", matrixA[i][j]);
		printf("\n");
    	}

	// Displaying matrixB
        printf("\nMatrix B : \n");
        for (i = 0; i < SIZE2; i++) {
                for (j = 0; j < SIZE3; j++)
                        printf("%f  ", matrixB[i][j]);
                printf("\n");
        }

	// Declaring SIZE1*SIZE3 threads
	pthread_t threads[SIZE1*SIZE3];

	// Generating SIZE1*SIZE3 threads
	for(i = 0; i < SIZE1*SIZE3; i++)
	{
		if(pthread_create(&threads[i], NULL, dot, NULL))
		{
			perror("pthread_create");
		}
	}

	// Joining and waiting for all threads to complete
    	for (int i = 0; i < SIZE1*SIZE3; i++)
        	pthread_join(threads[i], NULL);

	// Displaying the result matrix
	printf("\nA dot B: \n");
    	for (i = 0; i < SIZE1; i++) {
        	for (j = 0; j < SIZE3; j++)
            		printf("%f  ", result[i][j]);
                printf("\n");
    	}

    	return 0;
}
