#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <ctime>
using namespace std;

// Consideracoes inicais:
// 1- A entrada eh recebida via terminal
// 2- Estruturas dinamicas sao usadas apenas em casos especificos. Ex: ler a entrada
// 3- A estrutura principal utilizada eh um array bidimencional padrao do C
//  vetor[numero_de_vertices][2] => vetor[i][0] representa o valor de x na posicao i
//                               => vetor[i][1] representa o valor de y na posicao i
// 4- A execucao eh contralada pela constante TEMPO

// Variavel global para guardar qual movimento deve ser feito no 3Opt
int MOV;

// Constante que define quantas tentativas de melhoramento serao executadas
clock_t tempo_inicial = clock();
clock_t tempo_final;
const int TENTATIVAS = 5;
const int TEMPO = 300; // tempo de execuao em segundos
// funcao para calcular a distancia euclidiana entre dois pontos
float pitagoras(float x1, float y1, float x2, float y2)
{
    float dx = (x1 - x2);
    float dy = (y1 - y2);
    return sqrt(dx * dx + dy * dy);
}

// Algoritmo construtor: gera um caminho aleatorio embaralhando os dados da entrada
void vizinhoAleatorio(float **vertex, int size, float **path)
{
    // Foi nescessario utilizar vector para utilizar a funcao shuffle
    // para embaralhar o vetor com os pontos
    vector<int> pos_possiveis;
    for (int i = 0; i < size; i++)
    {
        pos_possiveis.push_back(i);
    }
    std::srand(unsigned(std::time(0)));
    std::random_shuffle(pos_possiveis.begin(), pos_possiveis.end());
    for (int i = 0; i < size; i++)
    {
        path[i][0] = vertex[pos_possiveis[i]][0];
        path[i][1] = vertex[pos_possiveis[i]][1];
    }
}

// Verifica qual dos 4 movimentos de 3 vertices deve ser feito
float melhor_troca(float **aux, int i, int j, int k, int size)
{
    float aX, aY;
    aX = aux[i - 1][0];
    aY = aux[i - 1][1];

    float bX, bY;
    bX = aux[i][0];
    bY = aux[i][1];

    float cX, cY;
    cX = aux[j - 1][0];
    cY = aux[j - 1][1];

    float dX, dY;
    dX = aux[j][0];
    dY = aux[j][1];

    float eX, eY;
    eX = aux[k - 1][0];
    eY = aux[k - 1][1];

    float fX, fY;
    fX = aux[k % size][0];
    fY = aux[k % size][1];

    float d0 = pitagoras(aX, aY, bX, bY) + pitagoras(cX, cY, dX, dY) + pitagoras(eX, eY, fX, fY); // padrao
    float d1 = pitagoras(aX, aY, cX, cY) + pitagoras(bX, bY, dX, dY) + pitagoras(eX, eY, fX, fY); // troca1
    float d2 = pitagoras(aX, aY, bX, bY) + pitagoras(cX, cY, eX, eY) + pitagoras(dX, dY, fX, fY); // troca2
    float d3 = pitagoras(aX, aY, dX, dY) + pitagoras(eX, eY, bX, bY) + pitagoras(cX, cY, fX, fY); // troca3
    float d4 = pitagoras(fX, fY, bX, bY) + pitagoras(cX, cY, dX, dY) + pitagoras(eX, eY, aX, aY); // troca4

    if (d0 > d1)
    {
        MOV = 1;
        return -d0 + d1;
    }
    else if (d0 > d2)
    {
        MOV = 2;
        return -d0 + d2;
    }
    else if (d0 > d4)
    {
        MOV = 4;
        return -d0 + d4;
    }
    else if (d0 > d3)
    {
        MOV = 3;
        return -d0 + d3;
    }

    return 0;
}

// Coloca a fatia do vetor[ini...fim] em ordem inversa
void troca_simples(float **aux, int ini, int fim)
{
    float aux1;
    float aux2;

    while (ini < fim)
    {
        aux1 = aux[ini][0];
        aux2 = aux[ini][1];
        aux[ini][0] = aux[fim][0];
        aux[ini][1] = aux[fim][1];
        aux[fim][0] = aux1;
        aux[fim][1] = aux2;

        ini += 1;
        fim -= 1;
    }
}

// 3opt implementado com a estratégia de best improvement
void threeOpt(int size, float **tour)
{
    bool improve = true;
    int best_i;
    int best_k;
    int best_j;
    float res;
    float best_res;
    int mov;
    while (improve)
    {
        improve = false;
        int i, j, k;
        for (i = 1; i < size; i++)
        {

            best_i = -1;
            best_k = -1;
            best_j = -1;
            best_res = 0;
            mov = -1;
            for (j = i + 2; j < size; j++)
            {

                for (k = j + 2; k < size + 1; k++)
                {
                    best_i = -1;
                    best_k = -1;
                    best_j = -1;
                    best_res = 0;
                    mov = -1;

                    res = melhor_troca(tour, i, j, k, size);
                    if (res < best_res)
                    {
                        best_res = res;
                        mov = MOV;
                        best_i = i;
                        best_k = k;
                        best_j = j;
                        if (mov != -1)
                        {
                            improve = true;
                            if (mov == 1 and best_i != -1 and best_j != -1)
                            {
                                troca_simples(tour, best_i, best_j - 1);
                            }
                            else if (mov == 2 and best_k != -1 and best_j != -1)
                            {
                                troca_simples(tour, best_j, best_k - 1);
                            }
                            else if (mov == 4 and best_i != -1 and best_k != -1)
                            {
                                troca_simples(tour, best_i, best_k - 1);
                            }
                            else
                            {
                                if (best_i != -1 and best_j != -1 and best_k != -1)
                                {
                                    vector<pair<float, float>> tmp;
                                    for (int z = best_j; z < best_k; z++)
                                    {
                                        pair<float, float> aux;
                                        aux.first = tour[z][0];
                                        aux.second = tour[z][1];
                                        tmp.push_back(aux);
                                    }
                                    for (int z = best_i; z < best_j; z++)
                                    {
                                        pair<float, float> aux;
                                        aux.first = tour[z][0];
                                        aux.second = tour[z][1];
                                        tmp.push_back(aux);
                                    }

                                    for (int f = best_i; f < best_k; f++)
                                    {
                                        tour[f][0] = tmp[f - i].first;
                                        tour[f][1] = tmp[f - i].second;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Calcula a distancai de um ciclo(caminho)
float custo(float **tour, int size)
{
    float d = 0;
    for (int i = 0; i < size - 1; i++)
    {
        d += pitagoras(tour[i][0], tour[i][1], tour[i + 1][0], tour[i + 1][1]);
    }
    d += pitagoras(tour[0][0], tour[0][1], tour[size - 1][0], tour[size - 1][1]);
    return d;
}

int main()
{
    int tam;
    vector<string> vertices;
    vector<pair<float, float>> vertex;
    string linha;
    int c = 0;
    while (getline(cin, linha) and linha != "EOF")
    {
        if (c > 5)
        {
            float x, y;
            int i;

            sscanf(linha.c_str(), "%d %f %f", &i, &x, &y);
            pair<int, int> p;
            p.first = x;
            p.second = y;
            vertex.push_back(p);
        }
        c += 1;
    };

    tam = vertex.size();

    // Passando a entrada (vector<pair>) para um array de 2 dimensoes simples
    float **v = (float **)malloc(tam * sizeof(float *));
    for (int i = 0; i < tam; i++)
    {
        v[i] = (float *)malloc(2 * sizeof(float));
    }

    for (int i = 0; i < tam; i++)
    {
        v[i][0] = vertex[i].first;
        v[i][1] = vertex[i].second;
    }

    float **pathVizinhoAleatorio = (float **)malloc(tam * sizeof(float *));
    for (int i = 0; i < tam; i++)
    {
        pathVizinhoAleatorio[i] = (float *)malloc(2 * sizeof(float));
    }
    // Constuindo a caminho
    vizinhoAleatorio(v, tam, pathVizinhoAleatorio);
    cout << "Vizinho aleatorio: ";
    cout << fixed << custo(pathVizinhoAleatorio, tam) << "\n";
    // Rodando o 3opt
    threeOpt(tam, pathVizinhoAleatorio);
    cout << "3opt: ";
    cout << fixed << custo(pathVizinhoAleatorio, tam) << "\n";

    return 0;
}