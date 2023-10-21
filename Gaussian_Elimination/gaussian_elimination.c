#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Macro for calculating the min value
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
        float **matrix; // 2D array to store the matrix
        int num_rows; // number of num_rows
        int num_cols; // number of columns
        int thread_id; // Current thread id to calculate which rows to process
        int num_threads; // total number of threads we are using. Used with thread_id to divide the work
} thread_data_t;

void update_row(float **matrix, int row, int start_col, int num_cols,
                float factor, float *pivot_row)
{
        int j;
        for (j = start_col; j < num_cols; j++) {
                matrix[row][j] -= factor * pivot_row[j];
        }
}

void *forward_elimination(void *arg)
{
        thread_data_t *data = (thread_data_t *)arg;
        int i, k;
        float factor;

        for (k = 0; k < data->num_cols - 1; k++) {
                if (k % data->num_threads != data->thread_id) {
                        continue;
                }

                for (i = k + 1; i < data->num_rows; i++) {
                        factor = data->matrix[i][k] / data->matrix[k][k];
                        update_row(data->matrix, i, k, data->num_cols, factor,
                                   data->matrix[k]);
                }
        }

        pthread_exit(NULL);
}

void backsolve(float **matrix, int num_rows, int num_cols)
{
        int i, j;
        float *solution = malloc(num_rows * sizeof(float));
        float sum;

        for (i = num_rows - 1; i >= 0; i--) {
                sum = 0;
                for (j = i + 1; j < num_cols - 1; j++) {
                        sum += matrix[i][j] * solution[j];
                }
                solution[i] = (matrix[i][num_cols - 1] - sum) / matrix[i][i];
        }

        free(solution);
}

float **create_matrix(int num_rows, int num_cols)
{
        int i, j;
        float **matrix = malloc(num_rows * sizeof(float *));
        for (i = 0; i < num_rows; i++) {
                matrix[i] = malloc(num_cols * sizeof(float));
                for (j = 0; j < num_cols; j++) {
                        matrix[i][j] = (float)rand() / RAND_MAX * 2000 - 1000;
                }
        }
        return matrix;
}

void free_matrix(float **matrix, int num_rows)
{
        int i;
        for (i = 0; i < num_rows; i++) {
                free(matrix[i]);
        }
        free(matrix);
}

int main()
{
        int sizes[] = { 10, 100, 1000 };
        int i, j;
        for (i = 0; i < 3; i++) {
                int num_rows = sizes[i];
                int num_cols = num_rows + 1;
                float **matrix = create_matrix(num_rows, num_cols);

                pthread_t *threads = malloc(num_rows * sizeof(pthread_t));
                thread_data_t *thread_data =
                        malloc(num_rows * sizeof(thread_data_t));

                clock_t start, end;
                double cpu_time_used;

                start = clock();
                for (j = 0; j < num_rows; j++) {
                        thread_data[j].matrix = matrix;
                        thread_data[j].num_rows = num_rows;
                        thread_data[j].num_cols = num_cols;
                        thread_data[j].thread_id = j;
                        thread_data[j].num_threads = num_rows;
                        pthread_create(&threads[j], NULL, forward_elimination,
                                       &thread_data[j]);
                }

                for (j = 0; j < num_rows; j++) {
                        pthread_join(threads[j], NULL);
                }
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("Time taken for forward elimination with N=%d: %f seconds\n",
                       num_rows, cpu_time_used);

                start = clock();
                backsolve(matrix, num_rows, num_cols);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("Time taken for backsolve with N=%d: %f seconds\n",
                       num_rows, cpu_time_used);

                free_matrix(matrix, num_rows);
                free(threads);
                free(thread_data);
        }

        return 0;
}
