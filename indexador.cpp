#pragma once
#include <iostream>
#include <cstring>
#include <utility>
#include "gerenciaCsv.cpp"
#include "gerenciaDat.cpp"
#include "imovel.cpp"

struct Indexador{

    GerenciaCsv instanciaCsv;
    GerenciaDat instanciaDat;

    int imovelSelecionadoId{-1};

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
        
        std::pair<int, bool> ordenacaoDados = std::make_pair(1, true);
        std::pair<int, std::string> filtroDados = std::make_pair(0, "");
        
        std::pair<int, Imovel*> dadosBinario = instanciaDat.leituraDados(&ordenacaoDados, &filtroDados);

        if(!dadosBinario.first)
            return; //nenhum dado para ser exportado
        
        instanciaCsv.exportarCsv(nomeArquivo, dadosBinario);
        return;
        
    }

    std::pair<int, Imovel*> buscaImoveis(std::pair<int, bool> *ordenacaoTabela, std::pair<int, std::string> *filtroTabela){

        return instanciaDat.leituraDados(ordenacaoTabela, filtroTabela);

    }

    Imovel* buscarImovelSelecionado(){
        return instanciaDat.buscarImovelSelecionado(imovelSelecionadoId);
    }

    void deletarImovel(int idImovel){
        instanciaDat.deletarRegistro(idImovel);
    }

    int calculaIdNovoImovel(){
        return instanciaDat.calculaIdNovoImovel();
    }

    void editarImovel(std::string *valoresImovel){
        return instanciaDat.editarImovel(valoresImovel);
    }

    void criarImovel(std::string *valoresImovel){
        return instanciaDat.criarImovel(valoresImovel);
    }

};