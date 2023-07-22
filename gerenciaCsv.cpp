#pragma once
#include <fstream>
#include <iostream>
#include <cstring>
#include "imovel.cpp"
#include <utility>

struct GerenciaCsv{

    std::fstream arquivoCsv;
    const std::string caminhoPastaArquivos{"./dataFiles/"};

    void exportarCsv(std::string nomeArquivo, std::pair<int, Imovel *> dadosImoveis){
        arquivoCsv.open(caminhoPastaArquivos + nomeArquivo, std::ios::out);

        if(!arquivoCsv.good())
            return;
        
        for(int i{0}; i<dadosImoveis.first; i++){
            arquivoCsv << dadosImoveis.second[i].id << ";" 
            << '"' << dadosImoveis.second[i].endereco << '"' << ';'
            << '"' << dadosImoveis.second[i].descricao << '"' << ';'
            << '"' << dadosImoveis.second[i].imobiliaria << '"' << ';'
            << dadosImoveis.second[i].area << ";"
            << dadosImoveis.second[i].numQuartos << ";"
            << dadosImoveis.second[i].numBanheiros << ";"
            << dadosImoveis.second[i].vagasGaragem << ";"
            << dadosImoveis.second[i].precoAluguel << ";"
            << dadosImoveis.second[i].condominio << ";"
            << dadosImoveis.second[i].iptu << ";"
            << dadosImoveis.second[i].seguroIncendio << ";"
            << "\n";
        }

        arquivoCsv.close();
        delete[] dadosImoveis.second; // desalocar dados buscados no arquivo binário
        return;

    }

    std::pair<int, Imovel *> importarCsv(std::string nomeArquivo){
        arquivoCsv.open(caminhoPastaArquivos + nomeArquivo, std::ios::in);

        if(!arquivoCsv.good())
            return std::make_pair(0, nullptr);

        
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
        return std::make_pair(numImoveis, imoveis);
    }


};