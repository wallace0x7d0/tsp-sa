/*
	gcc tsp-sa.c -lm -o main && ./main && python3 tsp-sa.py
	
	51 27 6 48 23 7 43 24 14 25 13 41 40 19 42 44 15 45 33 39 10 30 34 21 29 20 35 36 3 28 31 26 8 22 1 32 11 2 16 50 9 49 38 5 37 17 4 18 47 12 46 
	429.530283
	double T0 = 10.0;
	double TN = 0.005;
	double N = 500000;
	double MAX_IT_SA = 10;
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N_DADOS 51

double T0 = 10.0;
double TN = 0.005;
double N = 1000000;
double MAX_IT_SA = 100;

typedef struct{
	int id;
	int x, y;
} Data;

double euclidean_distance(Data d1, Data d2){
	return sqrt(pow((d1.x - d2.x), 2) + pow((d1.y - d2.y), 2));
}

double cost_path(int *path, double **distance_matrix){
	double cost = 0;

	for (int i = 0; i < N_DADOS - 1; ++i){
		cost += distance_matrix[(int)fmax(path[i], path[i+1]) - 1][(int)fmin(path[i], path[i+1]) - 1];
	}

	cost += distance_matrix[(int)fmax(path[N_DADOS-1], path[0]) - 1][(int)fmin(path[N_DADOS-1], path[0]) - 1];

	return cost;
}

void shuffle(int *path){
	int n_shuffles = (rand() % 3) + 1;

	for(int i = 0; i < n_shuffles; i++){
		int x = rand() % N_DADOS;
		int y = rand() % N_DADOS;
		
		int aux = path[x];
		path[x] = path[y];
		path[y] = aux;
	}
}

Data* init_data(){
	return malloc(sizeof(Data) * N_DADOS);
}

double** init_distance_matrix(){
	double **distance_matrix = malloc(sizeof(double*) * N_DADOS);

	for (int i = 0; i < N_DADOS; ++i){
		distance_matrix[i] = malloc(sizeof(double) * N_DADOS);
	}

	return distance_matrix;
}

int* init_path(){
	int *path = malloc(sizeof(int) * N_DADOS);
	
	for(int i = 0; i < N_DADOS; i++){
		path[i] = i + 1;
	}
	
	return path;
}

void load_data(Data *data){
	FILE *base = fopen("base.txt", "r");

	for(int i = 0; i < N_DADOS; i++){
		fscanf(base, "%i %i %i", &data[i].id, &data[i].x, &data[i].y);
	}
}

void load_distance_matrix(double **distance_matrix, Data *data){
	for(int i = 0; i < N_DADOS; i++){
		for(int j = i; j < N_DADOS; j++){
			distance_matrix[j][i] = euclidean_distance(data[i], data[j]);
		}
	}
}

int main(){

	srand(time(NULL));
	
	Data *data = init_data();
	double **distance_matrix = init_distance_matrix();

	load_data(data);
	load_distance_matrix(distance_matrix, data);

	/* ================= SA ================== */
	int *path      = init_path();
	int *best_path = init_path();

	shuffle(path);
	memcpy(best_path, path, sizeof(int) * N_DADOS);

	double t = T0;
	
	FILE *saida = fopen("saida.txt", "w+");

	for(int i = 0; i < N; i++){
		for (int j = 0; j < MAX_IT_SA; ++j){
			int *temp_path = init_path();
			memcpy(temp_path, path, sizeof(int) * N_DADOS);
			shuffle(temp_path);

			double delta = cost_path(temp_path, distance_matrix) - cost_path(path, distance_matrix);

			if(delta < 0){
				memcpy(path, temp_path, sizeof(int) * N_DADOS);

				if(cost_path(temp_path, distance_matrix) < cost_path(best_path, distance_matrix)){
					memcpy(best_path, temp_path, sizeof(int) * N_DADOS);
				}
			}else {
				int x = rand() % 101;

				double p = exp(-(delta/t)) * 100;

				if(x < p){
					memcpy(path, temp_path, sizeof(int) * N_DADOS);
				}
			}
		}
		
		fprintf(saida, "%lf\n", cost_path(path, distance_matrix));
		
		double A = (1.0/(double)N) * log((double)T0/(double)TN);
		t = T0 * exp(-A * i);
	}


	/* =============== END SA ================ */

	for (int i = 0; i < N_DADOS; ++i){
		printf("%i ", best_path[i]);
	}printf("\n");

	printf("%lf\n", cost_path(best_path, distance_matrix));



	return 0;
}
