#pragma once
#include <iostream>
#include <cstring>
#include "gerenciaCsv.cpp"
#include "gerenciaDat.cpp"
#include "imovel.cpp"

struct Indexador{

    GerenciaCsv instanciaCsv;
    GerenciaDat instanciaDat;

    int numImoveis{0}, capacidade{50};
    Imovel *imoveis = new Imovel[capacidade];

    void redimensiona(int incrementoCapacidade=50){
        capacidade += incrementoCapacidade;
        Imovel *aux = new Imovel[capacidade];

        std::memcpy(aux, imoveis, sizeof(Imovel)*numImoveis);
        delete[] imoveis;
        imoveis = aux;
    }

    void importarCsv(std::string nomeArquivo){
        std::pair registrosCsv = instanciaCsv.importarCsv(nomeArquivo);
       
        int numRegistrosImportados = registrosCsv.second - registrosCsv.first;
        int diferencaCapacidade = capacidade - numImoveis - numRegistrosImportados;

        if(diferencaCapacidade < 0)
            redimensiona(-diferencaCapacidade);

        std::memcpy(imoveis + numImoveis, registrosCsv.first, sizeof(Imovel)*numRegistrosImportados);
        numImoveis += numRegistrosImportados;
        
    }

    void exportarCsv(std::string nomeArquivo){
        instanciaCsv.exportarCsv(nomeArquivo, std::make_pair(imoveis, numImoveis));
    }
    
    void printImoveis(){
        for(int i{0}; i<numImoveis; i++){
            std::cout << imoveis[i].id << "\n";
        }
    }

    std::pair<int, Imovel **> buscaImoveis(){ // recebe um filtro e uma ordenação -> retorna um array de ponteiros (um para cada imovel)

        int numImoveisFiltrados{0}, capacidadeImoveisFiltrados{10}, incrementoCapacidade{10};
        Imovel **arrImoveisPtr = new Imovel*[capacidadeImoveisFiltrados];

        for(int i{0}; i<numImoveis; i++){

            if(imoveis[i].id != -1 && true){ //aplicar filtro aqui

                if(numImoveisFiltrados >= capacidadeImoveisFiltrados){ // redimensionar array de ponteiros

                    capacidadeImoveisFiltrados += incrementoCapacidade;
                    Imovel **aux = new Imovel*[capacidadeImoveisFiltrados];
                    std::memcpy(aux, arrImoveisPtr, sizeof(Imovel*)*numImoveisFiltrados);
                    delete[] arrImoveisPtr;
                    arrImoveisPtr = aux;
                    
                }

                arrImoveisPtr[numImoveisFiltrados++] = imoveis + i;

            }

        }

        return std::make_pair(numImoveisFiltrados, arrImoveisPtr);

    }

};