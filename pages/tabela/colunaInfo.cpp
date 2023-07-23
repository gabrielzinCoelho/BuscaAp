#pragma once
#include <string>


struct ColunaInfo{
    std::string nome;
    float proporcaoLargura;
    int idColuna;

    void construtor(std::string nome, float proporcaoLargura, int idColuna){
        this->nome = nome;
        this->proporcaoLargura = proporcaoLargura;
        this->idColuna = idColuna;
    }

};