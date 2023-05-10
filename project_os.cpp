#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <bits/stdc++.h>
#define num_threads 27
using namespace std;

typedef struct
{
    int row;
    int col;
    int (* board)[9];     // The pointer to the sudoku puzzle.

} parameters;

int result[num_threads] = {0};
void *check_grid(void *params);
void *check_rows(void *params);
void *check_cols(void *params);
int sudoku_checker(int sudoku[9][9]);

int main(void)
{
    // Starting time for single thread and multithread execution
    clock_t start, end;
    start = clock();
   
    int sudoku[9][9] =
    {
            {6, 2, 4, 5, 3, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}
     };
    /*
    int sudoku[9][9],i,j;
    for(i=1;i<=9;i++){
        for(j=1;j<=9;j++){
            printf("Enter at (%d,%d)position:",i,j);
            scanf("%d",&sudoku[i][j]);
        }
    }
    */
    cout<<"Validating the sudoku using single thread:-"<<endl;
    if(sudoku_checker(sudoku))
        printf("Sudoku solution is invalid\n");
    else
        printf("Sudoku solution is valid\n");

    // Compute and return the elapsed time in milliseconds.
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Execution time for validating sudoku using a single thread : " << fixed<< time_taken << setprecision(5);
    cout << " sec \n\n " << endl;
   
    pthread_t threads[num_threads];
    int threadIndex = 0;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (i%3 == 0 && j%3 == 0){
                parameters *gridData = (parameters *) malloc(sizeof(parameters));
                gridData->row = i;
                gridData->col = j;
                gridData->board = sudoku;
                pthread_create(&threads[threadIndex++], NULL, check_grid, gridData);
            }

            if (j == 0){
                parameters *rowData = (parameters *) malloc(sizeof(parameters));
                rowData->row = i;
                rowData->col = j;
                rowData->board = sudoku;
                pthread_create(&threads[threadIndex++], NULL, check_rows, rowData);
            }

            if (i == 0){
                parameters *columnData = (parameters *) malloc(sizeof(parameters));
                columnData->row = i;
                columnData->col = j;
                columnData->board = sudoku;
                pthread_create(&threads[threadIndex++], NULL, check_cols, columnData);
            }
        }
    }

    //Parameters are Thread identifier and the return value of the function executed by the thread
    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    // If any of the entries in the valid array are 0, then the Sudoku solution is invalid
    cout<<"Validating the sudoku using 27 threads-"<<endl;
    for (int i = 0; i < num_threads; i++){
        if (result[i] == 0){
            cout << "Sudoku solution is invalid" << endl;
            // Compute and return the elapsed time in milliseconds.
            end = clock();
            double time_take = double(end - start) / double(CLOCKS_PER_SEC);
                cout << "Execution time for validating sudoku using a single thread : " << fixed<< time_take << setprecision(5);
                    cout << " sec " << endl;
        }
    }
    cout << "Sudoku solution is valid" << endl;

    // Compute and return the elapsed time in milliseconds.
    end = clock();
    double time_takens = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Execution time for validating sudoku using a single thread : " << fixed<< time_takens << setprecision(5);
    cout << " sec " << endl;
}

void *check_grid(void * params)
{
    parameters *data = (parameters *) params;
    int startRow = data->row;
    int startCol = data->col;
    int validarray[10] = {0};
    for (int i = startRow; i < startRow + 3; ++i){
        for (int j = startCol; j < startCol + 3; ++j){
            int val = data->board[i][j];
            if (validarray[val] != 0)
                pthread_exit(NULL);
            else
                validarray[val] = 1;
        }
    }

    // If the execution has reached this point, then the 3x3 sub-grid is valid.
    result[startRow + startCol/3] = 1; // Maps the 3X3 sub-grid to an index in the first 9 indices of the result array
    pthread_exit(NULL);
}

void *check_rows(void *params)
{
    parameters *data = (parameters *) params;
    int row = data->row;
    int validarray[10] = {0};
    for (int j = 0; j < 9; j++){
        int val = data->board[row][j];
        if (validarray[val] != 0)
            pthread_exit(NULL);
        else
            validarray[val] = 1;
    }

    // If the execution has reached this point, then the row is valid.
    result[9 + row] = 1; // Maps the row to an index in the second set of 9 indices of the result array
    pthread_exit(NULL);
}

void *check_cols(void *params)
{
    parameters *data = (parameters *) params;
    //int startRow = data->row;
    int col = data->col;
    int validarray[10] = {0};
    for (int i = 0; i < 9; i++){
        int val = data->board[i][col];
        if (validarray[val] != 0)
            pthread_exit(NULL);
        else
            validarray[val] = 1;
    }

    // If the execution has reached this point, then the column is valid.
    result[18 + col] = 1; // Maps the column to an index in the third set of 9 indices of the result array
    pthread_exit(NULL);
 }

int check_line(int input[9])
{
    int validarray[10] = {0};
    for (int i = 0; i < 9; i++){
        int val = input[i];
        if (validarray[val] != 0)
            return 1;
        else
            validarray[val] = 1;
    }
    return 0;
}

int check_grid(int sudoku[9][9])
{
    int temp_row, temp_col;
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
            temp_row = 3 * i;
            temp_col = 3 * j;
            int validarray[10] = {0};
            for(int p=temp_row; p < temp_row+3; p++){
                for(int q=temp_col; q < temp_col+3; q++){
                    int val = sudoku[p][q];
                    if (validarray[val] != 0)
                        return 1;
                    else
                        validarray[val] = 1;
                }
            }
        }
    }
    return 0;
}

int sudoku_checker(int sudoku[9][9])
{
    for (int i=0; i<9; i++){
        if(check_line(sudoku[i]))//check row
            return 1;
        int check_col[9];
        for (int j=0; j<9; j++)
            check_col[j] = sudoku[i][j];
        if(check_line(check_col)) //check column
            return 1;
        if(check_grid(sudoku)) //check grid
            return 1;
    }
    return 0;
}
