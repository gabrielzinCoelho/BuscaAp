#pragma once
#include <iostream>
#include <cstring>
#include "gerenciaCsv.cpp"
#include "gerenciaDat.cpp"
#include "imovel.cpp"

struct Indexador{

    GerenciaCsv instanciaCsv;
    GerenciaDat instanciaDat;

    void construtor(){
        instanciaDat.construtor();
        //instanciaCsv.construtor();
    }

    void importarCsv(std::string nomeArquivo){
        
        std::pair registrosCsv = instanciaCsv.importarCsv(nomeArquivo);
        
        if(!registrosCsv.first)
            return; // nenhum registro importado pelo csv

        instanciaDat.insercaoEmMassa(registrosCsv.second, registrosCsv.first);
        
    }

    void exportarCsv(std::string nomeArquivo){
        
        std::pair<int, Imovel*> dadosBinario = instanciaDat.leituraDados();

        if(!dadosBinario.first)
            return; //nenhum dado para ser exportado
        
        instanciaCsv.exportarCsv(nomeArquivo, dadosBinario);
        return;
        
    }

    std::pair<int, Imovel*> buscaImoveis(){

        return instanciaDat.leituraDados();

    }


};