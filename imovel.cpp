#pragma once
#include <iostream>

struct Imovel{

    // struct imóvel

    int id{-1}, numQuartos, numBanheiros, vagasGaragem;
    char endereco[100], descricao[300], imobiliaria[100];
    float area, precoAluguel, condominio, iptu, seguroIncendio;

    void construtor(std::string *camposImovel){

        id = std::stoi(camposImovel[0]);
        std::strcpy(endereco, camposImovel[1].c_str());
        std::strcpy(descricao, camposImovel[2].c_str());
        std::strcpy(imobiliaria, camposImovel[3].c_str());
        area = std::stof(camposImovel[4]);
        numQuartos = std::stoi(camposImovel[5]);
        numBanheiros = std::stoi(camposImovel[6]);
        vagasGaragem = std::stoi(camposImovel[7]);
        precoAluguel = std::stof(camposImovel[8]);
        condominio = std::stof(camposImovel[9]);
        iptu = std::stof(camposImovel[10]);
        seguroIncendio = std::stof(camposImovel[11]);

        return;
    };

};