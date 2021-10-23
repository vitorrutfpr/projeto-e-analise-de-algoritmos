
/* Trabalho desenvolvido por: VITOR MATHEUS TRICHES DA ROSA RA: 2040042 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <cmath>
#include <thread>
#include <cfloat>
using namespace std;

typedef struct{
    double x, y, d;
}Ponto;

double menor_distancia(double a, double b){
    return (a < b) ? a : b;
}

void imprime_vetor_pontos(vector<Ponto>&p){
    for(unsigned int i = 0; i < p.size(); i++){
        cout << p[i].x << " " << p[i].y << endl;
    }
}

void merge(vector<Ponto>&n, int esquerda, int meio, int direita, bool x_ou_y){

    int inicio_metade = meio - esquerda + 1;
    int metade_fim = direita - meio;

    vector<Ponto>temp, temp2;

    for (unsigned int i = 0; i < inicio_metade; i++){
        temp.push_back(n[esquerda + i]);
    }

    for (unsigned int j = 0; j < metade_fim; j++){
        temp2.push_back(n[meio + 1 + j]);
    }

    int index_esquerda = 0;
    int index_direita = 0;
    int index_vetor = esquerda;

    while(index_esquerda < inicio_metade && index_direita < metade_fim){
        if(!x_ou_y){
            if(temp[index_esquerda].x <= temp2[index_direita].x){
                n[index_vetor] = temp[index_esquerda];
                index_esquerda++;
            }
            else{
                n[index_vetor] = temp2[index_direita];
                index_direita++;
            }
            index_vetor++;
        }
        else{
            if(temp[index_esquerda].y <= temp2[index_direita].y){
                n[index_vetor] = temp[index_esquerda];
                index_esquerda++;
            }
            else{
                n[index_vetor] = temp2[index_direita];
                index_direita++;
            }
            index_vetor++;
        }
    }
    while(index_esquerda < inicio_metade){
        n[index_vetor] = temp[index_esquerda];
        index_esquerda++;
        index_vetor++;
    }
    while(index_direita < metade_fim){
        n[index_vetor] = temp2[index_direita];
        index_direita++;
        index_vetor++;
    }
}

void mergesort(vector<Ponto>&n, int esquerda, int direita, bool x_ou_y){
    if(esquerda >= direita){
        return;
    }
    int meio = esquerda + (direita - esquerda) / 2;
    mergesort(n, esquerda, meio, x_ou_y);
    mergesort(n, meio+1, direita, x_ou_y);
    merge(n, esquerda, meio, direita, x_ou_y);
}


double distancia(Ponto p1, Ponto p2){
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

void forca_bruta(vector<Ponto>&p, vector<Ponto>&r)
{
    for (unsigned int i = 0; i < p.size(); ++i){
        for (unsigned int j = i+1; j < p.size(); ++j){
            if(distancia(p[i], p[j]) < r[0].d){
                r[0].d = distancia(p[i], p[j]);
                r[0].x = p[j].x;
                r[0].y = p[j].y;
                r[1].x = p[i].x;
                r[1].y = p[i].y;
            }
        }
    }
}

void closest(vector<Ponto>&px, vector<Ponto>&py, vector<Ponto>&r){
    if(px.size() <= 3){
        forca_bruta(px, r);
        return;
    }

    Ponto coord_meio = px[px.size()/2];
    vector<Ponto>ponto_x_esquerda, ponto_x_direita, ponto_y_esquerda, ponto_y_direita, faixa;

    for(unsigned int i = 0; i < px.size(); i++){
        if(i < px.size()/2){
            ponto_x_esquerda.push_back(px[i]);
        }
        else{
            ponto_x_direita.push_back(px[i]);
        }
    }

    for(unsigned int i = 0; i < py.size(); i++){
        if(py[i].x < coord_meio.x){
            ponto_y_esquerda.push_back(py[i]);
        }
        if(py[i].x >= coord_meio.x){
            ponto_y_direita.push_back(py[i]);
        }
    }

    closest(ponto_x_esquerda, ponto_y_esquerda, r);
    closest(ponto_x_direita, ponto_y_direita, r);

    for (unsigned int i = 0; i < py.size(); i++){
        if (abs(py[i].x - coord_meio.x) <= r[0].d){
            faixa.push_back(py[i]);
        }
    }

    for(unsigned int i = 0; i < faixa.size(); i++){
        for(unsigned int j = i+1; j < faixa.size() && faixa[j].y - faixa[i].y <= r[0].d; j++){
            if(distancia(faixa[i], faixa[j]) <= r[0].d){
                r[0].d = distancia(faixa[i], faixa[j]);
                r[0].x = faixa[i].x;
                r[0].y = faixa[i].y;
                r[1].x = faixa[j].x;
                r[1].y = faixa[j].y;
            }
        }
    }
}

int main(int argc, char *argv[]){
    string linha;
    vector<Ponto>pontos, py, px, resposta;
    Ponto p, r;
    
    if(argc != 2){
        return 1;
    }
    clock_t tempo = clock();
    cout << fixed << setprecision(6);
    ifstream myfile(argv[argc-1]);
    if (myfile.is_open()){
        getline(myfile, linha);

        while (!myfile.eof()){
            getline(myfile, linha);
            istringstream iss(linha);
            iss >> p.x >> p.y;
            pontos.push_back(p);
        }
        pontos.erase(pontos.begin()+pontos.size()-1);
        myfile.close();
    }

    for(unsigned int i = 0; i < pontos.size(); i++){
        py.push_back(pontos[i]);
        px.push_back(pontos[i]);
    }

    mergesort(py, 0, pontos.size()-1, true);
    mergesort(px, 0, pontos.size()-1, false);

    r.x = 0;
    r.y = 0;
    r.d = DBL_MAX;
    resposta.push_back(r);
    resposta.push_back(r);

    
    closest(px, py, resposta);

    cout << double(clock() - tempo)/(double)CLOCKS_PER_SEC << " " << 
    resposta[0].d << " " << resposta[0].x << " " <<  
    resposta[0].y << " " <<  resposta[1].x << " " << resposta[1].y << endl;

    return 0;
}