#pragma once
#include <fstream>
#include <iostream>
#include <cstring>
#include "imovel.cpp"
#include <utility>

struct GerenciaCsv{

    std::fstream arquivoCsv;
    const std::string caminhoPastaArquivos{"./dataFiles/"};

    void exportarCsv(std::string nomeArquivo, std::pair<Imovel *, int> dadosImoveis){
        arquivoCsv.open(caminhoPastaArquivos + nomeArquivo, std::ios::out);

        if(!arquivoCsv.good())
            return;
        
        for(int i{0}; i<dadosImoveis.second; i++){
            arquivoCsv << dadosImoveis.first[i].id << ";" 
            << '"' << dadosImoveis.first[i].endereco << '"' << ';'
            << '"' << dadosImoveis.first[i].descricao << '"' << ';'
            << '"' << dadosImoveis.first[i].imobiliaria << '"' << ';'
            << dadosImoveis.first[i].area << ";"
            << dadosImoveis.first[i].numQuartos << ";"
            << dadosImoveis.first[i].numBanheiros << ";"
            << dadosImoveis.first[i].vagasGaragem << ";"
            << dadosImoveis.first[i].precoAluguel << ";"
            << dadosImoveis.first[i].condominio << ";"
            << dadosImoveis.first[i].iptu << ";"
            << dadosImoveis.first[i].seguroIncendio << ";"
            << "\n";
        }

        arquivoCsv.close();

    }

    std::pair<Imovel *, Imovel *> importarCsv(std::string nomeArquivo){
        arquivoCsv.open(caminhoPastaArquivos + nomeArquivo, std::ios::in);

        if(!arquivoCsv.good())
            return std::make_pair(nullptr, nullptr);

        
        int numImoveis{0}, capacidade{10}, incrementoCapacidade{10};
        Imovel *imoveis = new Imovel[capacidade];
        
        int indexCampoRegistro, indexSubstr;
        const int numCamposRegistro{12};
        std::string camposRegistro[numCamposRegistro];
        
        const char separador{';'};

        std::string linhaRegistro;

        while( std::getline(arquivoCsv, linhaRegistro) ){

            if(numImoveis >= capacidade){
                capacidade += incrementoCapacidade;
                Imovel *aux = new Imovel[capacidade];

                std::memcpy(aux, imoveis, sizeof(Imovel)*numImoveis);
                delete[] imoveis;
                imoveis = aux;
            }
            
            indexSubstr = indexCampoRegistro = 0;

            for(int i{0}; i<int(linhaRegistro.size()); i++){
                if(linhaRegistro[i] == separador){

                    if(linhaRegistro[indexSubstr] == '"')
                        camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr + 1, i - (indexSubstr + 2));
                    else
                        camposRegistro[indexCampoRegistro] = linhaRegistro.substr(indexSubstr, i - indexSubstr);

                    indexCampoRegistro++;
                    indexSubstr = i + 1;
                }
            }

            imoveis[numImoveis++].construtor(camposRegistro);
        }

        arquivoCsv.close();
        return std::make_pair(imoveis, imoveis + numImoveis);
    }


};