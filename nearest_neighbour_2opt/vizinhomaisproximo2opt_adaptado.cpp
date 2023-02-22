
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;
// Consideracoes inicais:
// 1- A entrada eh recebida via terminal
// 2- Estruturas dinamicas sao usadas apenas em casos especificos. Ex: ler a entrada
// 3- A estrutura principal utilizada eh um array padrao do C de duas dimensoes
//  vetor[numero_de_vertices][2] => vetor[i][0] representa o valor de x na posicao i
//                               => vetor[i][1] representa o valor de y na posicao i
// 4- A execucao eh contralada pela constante TEMPO

clock_t tempo_inicial = clock();
clock_t tempo_final;

const int TEMPO = 300; // tempo de execuao em segundos
const int TENTATIVAS = 5;
// funcao para calcular a distancia euclidiana entre dois pontos
float pitagoras(float x1, float y1, float x2, float y2)
{
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

// Calcula a distancia de um ciclo(caminho)
float custo(float tour[][2], int size)
{
    float d = 0;
    for (int i = 0; i < size - 1; i++)
    {
        d += pitagoras(tour[i][0], tour[i][1], tour[i + 1][0], tour[i + 1][1]);
    }
    d += pitagoras(tour[0][0], tour[0][1], tour[size - 1][0], tour[size - 1][1]);
    return d;
}

// Verifica qual dos movimentos de 2 vertices deve ser feito
float verificador(float vertex[][2], int a, int b, int size)
{
    int a2;
    if (a == 0)
    {
        a2 = size - 1;
    }
    else
    {
        a2 = a - 1;
    }
    int b2;
    if (b == size - 1)
    {
        b2 = 0;
    }
    else
    {

        b2 = b + 1;
    }

    if (b2 == a2)
        return 2;
    if (a == 0 and b == size - 1)
    {
        return 1;
    }

    // custo padrao
    float custo1 = pitagoras(vertex[a2][0], vertex[a2][1], vertex[a][0], vertex[a][1]);
    float custo2 = pitagoras(vertex[b][0], vertex[b][1], vertex[b2][0], vertex[b2][1]);
    float total = custo1 + custo2;
    // custo troca
    float custo3 = pitagoras(vertex[a][0], vertex[a][1], vertex[b2][0], vertex[b2][1]);
    float custo4 = pitagoras(vertex[a2][0], vertex[a2][1], vertex[b][0], vertex[b][1]);
    float total2 = custo3 + custo4;
    // se total2 < total == return < 0
    return total2 - total;
}

// Algoritmo construtor: gera um caminho utilizando a estrategia do vizinho mais proximo
void vizinhoMaisProx(float vertex[][2], int size, float path[][2])
{
    int tam = 0;
    bool visited[size] = {false};
    path[0][0] = vertex[0][0];
    path[0][1] = vertex[0][1];
    int idx_vertex = 0;
    float total = 0;
    tam += 1;
    while (tam < size)
    {
        float aux_custo = 2147483647;
        int idx_aux = -1;

        float par_x, par_y;
        par_x = vertex[idx_vertex][0];
        par_y = vertex[idx_vertex][1];

        for (int idx = 0; idx < size; idx++)
        {

            if (idx != idx_vertex and visited[idx] == 0)
            {

                float aux_x, aux_y;
                aux_x = vertex[idx][0];
                aux_y = vertex[idx][1];
                float h = pitagoras(par_x, par_y, aux_x, aux_y);
                if (h < aux_custo)
                {
                    idx_aux = idx;
                    aux_custo = h;
                }
            }
        }
        visited[idx_vertex] = 1;

        if (idx_aux != -1)
        {
            total += aux_custo;

            idx_vertex = idx_aux;
            path[tam][0] = vertex[idx_aux][0];
            path[tam][1] = vertex[idx_aux][1];
            tam += 1;
        }
    }
}

// Coloca a fatia do vetor[ini...fim] em ordem inversa
void troca_simples(float tour[][2], int i, int k, int size)
{
    for (; i < k; i++, k--)
    {
        float aux1, aux2;
        aux1 = tour[i][0];
        aux2 = tour[i][1];

        tour[i][0] = tour[k][0];
        tour[i][1] = tour[k][1];

        tour[k][0] = aux1;
        tour[k][1] = aux2;
    }
}

// 2opt implementado com uma adaptacao do first improvement, onde eh utilizado
// um numero de tentatativas de melhoramento ao inves de para apenas com uma melhora
void twoOpt(int size, float v[][2])
{
    int improve = 1;
    int best_i;
    int best_k;
    float diference;
    float best_diference;
    while (improve < TENTATIVAS and ((clock() - tempo_inicial) / (double)CLOCKS_PER_SEC < TEMPO))
    {
        improve = 0;

        for (int i = 0; i < size; i++)
        {
            if ((clock() - tempo_inicial) / (double)CLOCKS_PER_SEC > TEMPO)
            {
                return;
            }
            best_diference = 0;

            for (int k = i + 1; k < size; k++)
            {
                if ((clock() - tempo_inicial) / (double)CLOCKS_PER_SEC > TEMPO)
                {
                    return;
                }
                diference = verificador(v, i, k, size);
                if (diference < best_diference)
                {
                    best_i = i;
                    best_k = k;
                    best_diference = diference;
                }
            }
            if (best_diference < 0)
            {
                troca_simples(v, best_i, best_k, size);
                improve = 1;
            }
        }
    }
}

int main()
{
    vector<string> vertices;
    vector<pair<float, float>> vertex;

    int tam;
    string entrada;
    int c = 0;
    while (getline(cin, entrada) and entrada != "EOF")
    {
        if (c > 5)
        {
            float x, y;
            int i;

            sscanf(entrada.c_str(), "%d %f %f", &i, &x, &y);
            pair<int, int> p;
            p.first = x;
            p.second = y;
            vertex.push_back(p);
        }
        c += 1;
    };
    tam = vertex.size();
    // Passando a entrada (vector<pair>) para um array de 2 dimensoes simples
    float v[tam][2];
    for (int i = 0; i < tam; i++)
    {
        v[i][0] = vertex[i].first;
        v[i][1] = vertex[i].second;
    }
    float pathVizinhoMaisProximo[tam][2];
    // Constuindo a caminho
    vizinhoMaisProx(v, tam, pathVizinhoMaisProximo);
    cout << "Vizinho mais proximo: ";
    cout << fixed << custo(pathVizinhoMaisProximo, tam) << "\n";
    // Rodando o 2opt
    twoOpt(tam, pathVizinhoMaisProximo);
    cout << "2opt: ";
    cout << fixed << custo(pathVizinhoMaisProximo, tam) << "\n";

    return 0;
}
