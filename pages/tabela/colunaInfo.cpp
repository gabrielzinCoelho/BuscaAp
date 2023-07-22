#pragma once
#include <string>


struct ColunaInfo{
    std::string nome;
    float proporcaoLargura;
    int idOrdenacao;

    void construtor(std::string nome, float proporcaoLargura, int idOrdenacao){
        this->nome = nome;
        this->proporcaoLargura = proporcaoLargura;
        this -> idOrdenacao = idOrdenacao;
    }

};