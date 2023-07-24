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
    }

    void importarCsv(std::string nomeArquivo){
        
        std::pair registrosCsv = instanciaCsv.importarCsv(nomeArquivo);
        
        if(!registrosCsv.first)
            return; // nenhum registro importado pelo csv

        // aloca os registros salvos no csv e passa para o controlador do arquivo bin inserí-los devidamente
        instanciaDat.insercaoEmMassa(registrosCsv.second, registrosCsv.first);
        
    }

    void exportarCsv(std::string nomeArquivo){
        
        std::pair<int, bool> ordenacaoDados = std::make_pair(1, true);
        std::pair<int, std::string> filtroDados = std::make_pair(0, "");
        
        // define os parametros para buscar os dados no arquivo binario e então os exporta para o csv
        // nesse caso, ordena os dados pelo id, de forma crescente, e não aplica nenhum filto (todos dados são exportados)
        std::pair<int, Imovel*> dadosBinario = instanciaDat.leituraDados(&ordenacaoDados, &filtroDados);

        if(!dadosBinario.first)
            return; //nenhum dado para ser exportado
        
        instanciaCsv.exportarCsv(nomeArquivo, dadosBinario);
        return;
        
    }

    std::pair<int, Imovel*> buscaImoveis(std::pair<int, bool> *ordenacaoTabela, std::pair<int, std::string> *filtroTabela){
        
        //busca os registros do bin para serem mostrados na tabela
        return instanciaDat.leituraDados(ordenacaoTabela, filtroTabela);

    }

    Imovel* buscarImovelSelecionado(){
        // busca um imovel pelo Id, para edição e deleção
        return instanciaDat.buscarImovelSelecionado(imovelSelecionadoId);
    }

    void deletarImovel(int idImovel){
        // disparado pelo evento de deletar um imovel
        instanciaDat.deletarRegistro(idImovel);
    }

    int calculaIdNovoImovel(){
        // calcula id do novo imovel que será inserido
        return instanciaDat.calculaIdNovoImovel();
    }

    void editarImovel(std::string *valoresImovel){
        // disparado pelo evento de editar um imovel
        return instanciaDat.editarImovel(valoresImovel);
    }

    void criarImovel(std::string *valoresImovel){
        // disparado pelo evento de add um imovel
        return instanciaDat.criarImovel(valoresImovel);
    }

};