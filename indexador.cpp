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

        std::cout << numRegistrosImportados << " " << capacidade << " " << diferencaCapacidade;

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
        std::cout << "\n" << numImoveis << " " << capacidade << "\n";
    }

};