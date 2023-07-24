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
            // criar cabeçalho: numRegistros, tipo de ordenação e sentido (1 -> crescente, -1 -> decrescente)
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

        //insere, no arquivo bin, um conjunto de imoveis alocados na memoria

        arquivoDat.seekp(3*sizeof(int) + numRegistros * sizeof(Imovel));
        arquivoDat.write((char *) imoveisPtr, numInsercoes * sizeof(Imovel));

        //atualiza o numero de registros
        numRegistros += numInsercoes;
        escreverCabecalho();
    }

    std::pair<int, Imovel*> leituraDados(std::pair<int, bool> *ordenacaoTabela, std::pair<int, std::string> *filtroTabela){

        // leitura dos dados do arquivo bin

        if(!numRegistros)
            return std::make_pair(0, nullptr);
        
        if(filtroTabela->first && filtroTabela->second.size())
            return leituraDadosFiltrados(ordenacaoTabela, filtroTabela); // aplica filtros na busca dos dados
        
        return leituraTodosDados(ordenacaoTabela, filtroTabela); // busca todos os dados, apena os ordena
    }

    std::pair<int, Imovel*> leituraTodosDados(std::pair<int, bool> *ordenacaoTabela, std::pair<int, std::string> *filtroTabela){

        // busca todos os dados, ordena, aloca dinamicamente na memoria e os retorna

        tipoOrdenacao = ordenacaoTabela->first;
        sentidoOrdenacao = ordenacaoTabela->second ? 1 : -1;
        escreverCabecalho();
        
        ordenaArquivo();
        
        Imovel *imoveis = new Imovel[numRegistros];

        arquivoDat.seekp(3*sizeof(int)); //posiciona ponteiro de leitura para depois do cabecalho
        arquivoDat.read((char *) imoveis, numRegistros * sizeof(Imovel));

        return std::make_pair(numRegistros, imoveis);
    }

    std::pair<int, Imovel*> leituraDadosFiltrados(std::pair<int, bool> *ordenacaoTabela, std::pair<int, std::string> *filtroTabela){
        
        int tipoOrdenacaoAntigo{tipoOrdenacao}, sentidoOrdenacaoAntigo{sentidoOrdenacao};
        int idBusca = buscaBinaria(filtroTabela); // utiliza o codigo de busca binaria para encontrar o primeiro valor correspondente ao filtro

        if(idBusca == -1)
            return std::make_pair(0, nullptr); // caso não encontre nenhum registro correspondente
        

        // após encontrar o indice do primeiro registro que correspondeu a busca, ele percorre os valores proximos a esse registro no arquivo
        // caso encontre outros registros que também correspondam ao filtro, aloca todos na memoria e retorna

        // dessa forma a pesquisa por filtro não fica restrita a um unico valor

        int indexInicio{idBusca}, indexFinal{idBusca}, saltoCabecalho{3*sizeof(int)};

        Imovel aux;
        bool imovelValido{true};
        int comparaValores;

        while(indexInicio - 1 >= 0 && imovelValido){ // procura registros antes do primeiro correspondente

            imovelValido = false;

            arquivoDat.seekp(saltoCabecalho + (indexInicio - 1) * sizeof(Imovel));
            arquivoDat.read((char *) &aux, sizeof(Imovel));

            comparaValores = comparaImoveis(filtroTabela, &aux);

            if(!comparaValores){
                imovelValido = true;
                indexInicio--;
            }

        }

        imovelValido = true;

        while(indexFinal + 1 <= numRegistros - 1 && imovelValido){ // procura registros depois do primeiro correspondente

            imovelValido = false;

            arquivoDat.seekp(saltoCabecalho + (indexFinal + 1) * sizeof(Imovel));
            arquivoDat.read((char *) &aux, sizeof(Imovel));

            comparaValores = comparaImoveis(filtroTabela, &aux);

            if(!comparaValores){
                imovelValido = true;
                indexFinal++;
            }

        }

        int numImoveisFiltrados = indexFinal - indexInicio + 1;
            
        Imovel *imoveisFiltrados = new Imovel[numImoveisFiltrados];

        arquivoDat.seekp(3*sizeof(int) + indexInicio * sizeof(Imovel));
        arquivoDat.read((char *) imoveisFiltrados, (numImoveisFiltrados) * sizeof(Imovel));

        tipoOrdenacao = tipoOrdenacaoAntigo;
        sentidoOrdenacao = sentidoOrdenacaoAntigo;
        escreverCabecalho();

        if(numImoveisFiltrados > 1){
            // aplica ordenação aos valores filtrados
            quickSort(imoveisFiltrados, 0, numImoveisFiltrados - 1); 
        }

        tipoOrdenacao = filtroTabela->first;
        sentidoOrdenacao = 1;
        escreverCabecalho();
        
        //retorna os valores filtrados
        return std::make_pair(numImoveisFiltrados, imoveisFiltrados);

    }

    void ordenaArquivo(){

        int numImoveis = numRegistros;

        if(!numImoveis)
            return;

        Imovel *imoveis = new Imovel[numImoveis];

        //puxa todos os dados para memoria, ordena e reescreve o arquivo

        arquivoDat.seekp(3*sizeof(int)); //posiciona ponteiro de leitura para depois do cabecalho
        arquivoDat.read((char *) imoveis, numImoveis * sizeof(Imovel));
        quickSort(imoveis, 0, numImoveis - 1);

        arquivoDat.seekp(3*sizeof(int));
        arquivoDat.write((char *) imoveis, numImoveis * sizeof(Imovel));

        delete[] imoveis;

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
            
            //aplica um particionamento especifico para cada tipo de dado

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

    int buscaBinaria(std::pair<int, std::string> *filtroTabela){

        if(!numRegistros)
            return -1;

        // ordena os valores de acordo com o filtro aplicado
        // a busca só faz sentido se os valores estão ordenados de acordo com o filtro especifico utilizado

        tipoOrdenacao = filtroTabela->first;
        sentidoOrdenacao = 1;
        escreverCabecalho();
        ordenaArquivo();

        int saltoCabecalho{3*sizeof(int)};
        int indexInicio{0}, indexFinal{numRegistros - 1}, meio, comparaValores;

        Imovel aux;

        // busca binaria no arquivo binario

        while(indexInicio <= indexFinal){

            meio = (indexInicio + indexFinal)/2;

            arquivoDat.seekp(saltoCabecalho + meio * sizeof(Imovel)); //posiciona ponteiro de leitura para depois do cabecalho
            arquivoDat.read((char *) &aux, sizeof(Imovel));

            try{
                comparaValores = comparaImoveis(filtroTabela, &aux);
            }catch(...){
                return -1;
            }

            // calcula os indices da busca de acordo com o resultado da comparação

            if(!comparaValores)
                return meio;
            if(comparaValores < 0)
                indexFinal = meio - 1;
            else{
                indexInicio = meio + 1;
            }

        }

    return -1;

    }

    int comparaImoveis(std::pair<int, std::string> *filtroTabela, Imovel *imovel){

        // aplica uma função especifica para comparar os diferentes campos do imovel

        switch(filtroTabela->first){
                case 4:
                    return diferencaAluguel(filtroTabela->second, imovel);
                case 2:
                    return diferencaString(filtroTabela->second, imovel->endereco);
                case 3:
                    return diferencaString(filtroTabela->second, imovel->imobiliaria);
                default:
                case 1:
                    return diferencaId(filtroTabela->second, imovel);
            }
    }

    int diferencaId(std::string valorFiltro, Imovel *auxPtr){
        return std::stoi(valorFiltro) - auxPtr->id;
    }

    int diferencaAluguel(std::string valorFiltro, Imovel *auxPtr){

        // float margemErro{0.05};
        float diferencaValores = std::stof(valorFiltro) - auxPtr->precoAluguel;
        if(!diferencaValores)
            return diferencaValores;
        if(diferencaValores < 0)
            return -1;
        return 1;

    }

    int diferencaString(std::string valorFiltro, char *valorImovel){

        std::string valorImovelStr = std::string(valorImovel).substr(0, valorFiltro.size());

        std::cout << valorFiltro << " - " << valorImovelStr << "\n";

        if(valorFiltro == valorImovelStr)
            return 0;
        if(valorFiltro < valorImovelStr)
            return -1;
        return 1;
        
    }

    void deletarRegistro(int idImovel){

        // busca binaria pelo id (campo confiavel para identificar registro)

        std::pair<int, std::string> filtroDeletarRegistro = std::make_pair(1, std::to_string(idImovel));
        int indexImovel = buscaBinaria(&filtroDeletarRegistro); 

        if(indexImovel == -1)
            return;
        
        int saltoCabecalho = 3*sizeof(int);

        Imovel aux;

        // desloca todos os valores posteriores ao registro deletado, uma casa à esquerda (para trás)

        for(int i{indexImovel + 1}; i<numRegistros; i++){
            arquivoDat.seekp(saltoCabecalho + i * sizeof(Imovel));
            arquivoDat.read((char *) &aux, sizeof(Imovel));
            arquivoDat.seekp(saltoCabecalho + (i - 1) * sizeof(Imovel));
            arquivoDat.write((char *) &aux, sizeof(Imovel));
        }

        // atualiza o numero de registros
        numRegistros--;
        escreverCabecalho();
    }

    Imovel* buscarImovelSelecionado(int idImovel){

        // busca binaria pelo id (campo confiavel para identificar registro)
        std::pair<int, std::string> filtroDeletarRegistro = std::make_pair(1, std::to_string(idImovel));
        int indexImovel = buscaBinaria(&filtroDeletarRegistro);

        if(indexImovel == -1)
            return {nullptr};
        
        int saltoCabecalho = 3*sizeof(int);

        // busca o imovel no arquivo, aloca dinamicamente e retorna-o

        Imovel *aux = new Imovel;
        arquivoDat.seekp(saltoCabecalho + indexImovel * sizeof(Imovel));
        arquivoDat.read((char *) aux, sizeof(Imovel));

        return aux;
    }

    int calculaIdNovoImovel(){

        if(!numRegistros)
            return 1; // se não houver registros o id do proximo imovel a ser criado é 1

        
        //ordena os arquivos pelo id, de maneira crescente
        
        tipoOrdenacao = 1;
        sentidoOrdenacao = 1;
        escreverCabecalho();

        ordenaArquivo();

        // busca o ultimo registro (possui o maior id), e incrementa em 1

        Imovel aux;

        arquivoDat.seekp(3*sizeof(int) + (numRegistros - 1) * sizeof(Imovel));
        arquivoDat.read((char *) &aux, sizeof(Imovel));

        return aux.id + 1;

    }

    void editarImovel(std::string *valoresImovel){

        // busca binaria pelo id (campo confiavel para identificar registro)

        std::pair<int, std::string> filtroEditarRegistro = std::make_pair(1, valoresImovel[0]);
        int indexImovel = buscaBinaria(&filtroEditarRegistro);

        if(indexImovel == -1)
            return;
        
        int saltoCabecalho = 3*sizeof(int);

        Imovel aux;
        aux.construtor(valoresImovel);

        // cria um registro de acordo com os valores informados pelo usuário
        // sobreescreve o registro antigo

        arquivoDat.seekp(saltoCabecalho + indexImovel * sizeof(Imovel));
        arquivoDat.write((char *) &aux, sizeof(Imovel));
        return;

    }

    void criarImovel(std::string *valoresImovel){
        
        int saltoCabecalho = 3*sizeof(int);

        Imovel aux;
        aux.construtor(valoresImovel);

        // cria um registro de acordo com os valores informados pelo usuário
        // adiciona no final do arquivo binario

        arquivoDat.seekp(saltoCabecalho + numRegistros * sizeof(Imovel));
        arquivoDat.write((char *) &aux, sizeof(Imovel));

        // atualiza o numero de registros
        numRegistros++;
        escreverCabecalho();

    }

};