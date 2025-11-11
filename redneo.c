#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define E 0.5  // Tasa de aprendizaje
#define EPOCAS 1000

// --- Prototipos ---
void lee_num(int *num_patrones);
float *crea_arr(int tam_a);
float **crea_mat(int ren, int col);
void carga_patrones(int **patrones, int ren, int col);
void pesos_aleatorios(float *peso, int tam);
void carga_patron(float *entradas, float *salidas, float **patrones, int tam_a, int num_p);
void regla_delta(float *peso, float *entradas, float salida, float esperado, int tam_a);
void desordenar_indices(int *indices, int n);

// --- Función principal ---
int main()
{
    srand(time(NULL));  // Inicializa semilla para aleatoriedad

    int num_patrones, tam_a = 3; // 2 entradas + 1 bias
    lee_num(&num_patrones);

    // Reservar memoria
    float *peso = crea_arr(tam_a);
    float *entradas = crea_arr(tam_a);
    float *salidas = crea_arr(1);
    float **patrones = crea_mat(num_patrones, tam_a);
    int *orden = (int *)malloc(sizeof(int) * num_patrones);

    // Cargar patrones y pesos aleatorios
    carga_patrones((int **)patrones, num_patrones, tam_a);
    pesos_aleatorios(peso, tam_a);

    // Entrenamiento con presentación aleatoria (WalkingRandom)
    for (int e = 0; e < EPOCAS; e++)
    {
        desordenar_indices(orden, num_patrones);

        for (int p = 0; p < num_patrones; p++)
        {
            int idx = orden[p];  // Índice aleatorio del patrón
            carga_patron(entradas, salidas, patrones, tam_a, idx);

            float salida_real = 0;
            for (int i = 0; i < tam_a; i++)
                salida_real += entradas[i] * peso[i];

            float esperado = salidas[0];
            regla_delta(peso, entradas, salida_real, esperado, tam_a);
        }
    }

    // Mostrar pesos finales
    printf("\nPesos entrenados:\n");
    for (int i = 0; i < tam_a; i++)
        printf("peso[%d] = %.2f\n", i, peso[i]);

    // Liberar memoria
    free(peso);
    free(entradas);
    free(salidas);
    for (int i = 0; i < num_patrones; i++)
        free(patrones[i]);
    free(patrones);
    free(orden);

    return 0;
}

// --- Funciones auxiliares ---
void lee_num(int *num_patrones)
{
    printf("\nNúmero de patrones: ");
    scanf("%d", num_patrones);
}

float *crea_arr(int tam_a)
{
    return (float *)malloc(sizeof(float) * tam_a);
}

float **crea_mat(int ren, int col)
{
    float **t = (float **)malloc(sizeof(float *) * ren);
    for (int i = 0; i < ren; i++)
        t[i] = (float *)malloc(sizeof(float) * col);
    return t;
}

void carga_patrones(int **patrones, int ren, int col)
{
    int aux;
    printf("\nCargar patrones:\n");
    for (int i = 0; i < ren; i++)
    {
        for (int j = 0; j < col - 1; j++)
        {
            printf("Patrones[%d][%d] = ", i, j);
            scanf("%d", &aux);
            patrones[i][j] = aux;
        }
        printf("Salida esperada para patrón %d: ", i);
        scanf("%d", &aux);
        patrones[i][col - 1] = aux;
    }
}

void pesos_aleatorios(float *peso, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        if (i < tam - 1)
            peso[i] = cos((float)(rand() % 181));
        else
            peso[i] = -1.0;
    }
}

void carga_patron(float *entradas, float *salidas, float **patrones, int tam_a, int num_p)
{
    for (int i = 0; i < tam_a; i++)
    {
        if (i < tam_a - 1)
            entradas[i] = patrones[num_p][i];
        else
            entradas[i] = 1.0; // Bias fijo en 1
    }
    salidas[0] = patrones[num_p][tam_a - 1];
}

void regla_delta(float *peso, float *entradas, float salida, float esperado, int tam_a)
{
    float error = esperado - salida;
    for (int i = 0; i < tam_a; i++)
    {
        peso[i] += E * error * entradas[i];
    }
}

void desordenar_indices(int *indices, int n)
{
    for (int i = 0; i < n; i++)
        indices[i] = i;

    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}
