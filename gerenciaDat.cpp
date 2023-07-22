#pragma once
#include <fstream>
#include <cstring>
#include "imovel.cpp"

struct GerenciaDat{

    std::fstream arquivoDat;
    const std::string caminhoPastaArquivos{"./dataFiles/"};

    int numRegistros, tipoOrdenacao, sentidoOrdenacao;

    void construtor(){
        arquivoDat.open(caminhoPastaArquivos + "imoveis.dat", std::ios::in | std::ios::out);
        if(!arquivoDat.good()){

            //arquivoBinario não existe e será criado
            arquivoDat.open(caminhoPastaArquivos + "imoveis.dat", std::ios::out);
            // criar cabeçalho: numRegistros e sortType
            numRegistros = 0;
            tipoOrdenacao = 1;
            sentidoOrdenacao = 1;
            escreverCabecalho();

            arquivoDat.close();
            arquivoDat.open(caminhoPastaArquivos + "imoveis.dat", std::ios::in | std::ios::out);
            return;
        }
        
        leituraCabecalho();

        return;   
    }

    void destrutor(){
        arquivoDat.close();
    }

    void leituraCabecalho(){
        // atualiza atributos da struct com os dados do cabeçalho do arquivo binário
        arquivoDat.seekp(0);
        arquivoDat.read((char *) &numRegistros, sizeof(int));
        arquivoDat.read((char *) &tipoOrdenacao, sizeof(int));
        arquivoDat.read((char *) &sentidoOrdenacao, sizeof(int));
    }

    void escreverCabecalho(){
        // atualiza o cabeçalho do binário com os atributos da struct
        arquivoDat.seekp(0);
        arquivoDat.write((char *) &numRegistros, sizeof(int));
        arquivoDat.write((char *) &tipoOrdenacao, sizeof(int));
        arquivoDat.write((char *) &sentidoOrdenacao, sizeof(int));
    }

    void insercaoEmMassa(Imovel *imoveisPtr, int numInsercoes){
        arquivoDat.seekp(0, std::ios::end);
        arquivoDat.write((char *) imoveisPtr, numInsercoes * sizeof(Imovel));

        //chamar odenacao
        numRegistros += numInsercoes;
        escreverCabecalho();
    }

    std::pair<int, Imovel*> leituraDados(std::pair<int, bool> *ordenacaoTabela){

        int numImoveis = numRegistros;

        if(!numImoveis)
            return std::make_pair(0, nullptr);

        Imovel *imoveis = new Imovel[numImoveis];

        arquivoDat.seekp(3*sizeof(int)); //posiciona ponteiro de leitura para depois do cabecalho
        arquivoDat.read((char *) imoveis, numImoveis * sizeof(Imovel));

        tipoOrdenacao = ordenacaoTabela->first;
        sentidoOrdenacao = ordenacaoTabela->second ? 1 : -1;
        escreverCabecalho();

        ordenaDados(numImoveis, imoveis);

        return std::make_pair(numImoveis, imoveis);
    }

    void ordenaDados(int numImoveis, Imovel *imoveis){

        quickSort(imoveis, 0, numImoveis - 1);

    }

    void quickSort(Imovel *imoveis, int indexInicio, int indexFinal){

        if(indexInicio < indexFinal){
            int posPivo = particiona(imoveis, indexInicio, indexFinal);
            quickSort(imoveis, indexInicio, posPivo - 1);
            quickSort(imoveis, posPivo + 1, indexFinal);
        }

        return;

    }

    int particiona(Imovel *imoveis, int indexInicio, int indexFinal){

        int i{indexInicio + 1}, j{indexFinal};
        bool houveTroca;

        while(i <= j){
            
            switch(tipoOrdenacao){
                case 2:
                    houveTroca = particionaEndereco(imoveis, indexInicio, i, j, sentidoOrdenacao);
                    break;
                case 3:
                    houveTroca = particionaImobiliaria(imoveis, indexInicio, i, j, sentidoOrdenacao);
                    break;
                case 4:
                    houveTroca = particionaAluguel(imoveis, indexInicio, i, j, sentidoOrdenacao);
                    break;
                default:
                case 1:
                    houveTroca = particionaId(imoveis, indexInicio, i, j, sentidoOrdenacao);

            };

            if(!houveTroca){
                std::swap(imoveis[i], imoveis[j]);
                i++;
                j--;
            }
        }

        std::swap(imoveis[indexInicio], imoveis[j]);
        return j;
    }

    bool particionaId(Imovel *imoveisArr, int indexPivo, int &i, int &j, int sentidoOrdenacao=1){
        if(imoveisArr[i].id * sentidoOrdenacao <= imoveisArr[indexPivo].id * sentidoOrdenacao) i++;
        else if(imoveisArr[j].id * sentidoOrdenacao >= imoveisArr[indexPivo].id * sentidoOrdenacao) j--;
        else return false;
        return true;
    }

    bool particionaAluguel(Imovel *imoveisArr, int indexPivo, int &i, int &j, int sentidoOrdenacao=1){
        if(imoveisArr[i].precoAluguel * sentidoOrdenacao <= imoveisArr[indexPivo].precoAluguel * sentidoOrdenacao) i++;
        else if(imoveisArr[j].precoAluguel * sentidoOrdenacao >= imoveisArr[indexPivo].precoAluguel * sentidoOrdenacao) j--;
        else return false;
        return true;
    }

    bool particionaEndereco(Imovel *imoveisArr, int indexPivo, int &i, int &j, int sentidoOrdenacao=1){
        if(std::strcmp(imoveisArr[i].endereco, imoveisArr[indexPivo].endereco) * sentidoOrdenacao <= 0) i++;
        else if(std::strcmp(imoveisArr[j].endereco, imoveisArr[indexPivo].endereco) * sentidoOrdenacao >= 0) j--;
        else return false;
        return true;
    }

    bool particionaImobiliaria(Imovel *imoveisArr, int indexPivo, int &i, int &j, int sentidoOrdenacao=1){
        if(std::strcmp(imoveisArr[i].imobiliaria, imoveisArr[indexPivo].imobiliaria) * sentidoOrdenacao <= 0) i++;
        else if(std::strcmp(imoveisArr[j].imobiliaria, imoveisArr[indexPivo].imobiliaria) * sentidoOrdenacao >= 0) j--;
        else return false;
        return true;
    }

};