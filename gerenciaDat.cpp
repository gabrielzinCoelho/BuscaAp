#pragma once
#include <fstream>
#include "imovel.cpp"

struct GerenciaDat{

    std::fstream arquivoDat;
    const std::string caminhoPastaArquivos{"./dataFiles/"};

    void construtor(){
        arquivoDat.open(caminhoPastaArquivos + "imoveis.dat", std::ios::in | std::ios::out);
        if(!arquivoDat.good()){

            // criar cabeçalho: numRegistros e sortType
            arquivoDat.open(caminhoPastaArquivos + "imoveis.dat", std::ios::out);
            arquivoDat.close();
            
            arquivoDat.open(caminhoPastaArquivos + "imoveis.dat", std::ios::in | std::ios::out);
        }
        
    }

    void destrutor(){
        arquivoDat.close();
    }

    void insercaoEmMassa(Imovel *imoveisPtr, int numInsercoes){

        arquivoDat.seekp(0, std::ios::end);
        arquivoDat.write((char *) imoveisPtr, numInsercoes * sizeof(Imovel));
    }

    int calculaNumRegistros(){
        arquivoDat.seekg(0, std::ios::end);
        long int tamanhoArquivo = arquivoDat.tellg();

        return tamanhoArquivo/sizeof(Imovel);

    }

    std::pair<int, Imovel*> leituraDados(){

        int numImoveis = calculaNumRegistros();
        Imovel *imoveis = new Imovel[numImoveis];

        arquivoDat.seekp(0);
        arquivoDat.read((char *) imoveis, numImoveis * sizeof(Imovel));
        return std::make_pair(numImoveis, imoveis);
    }

};