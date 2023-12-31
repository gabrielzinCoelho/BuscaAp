#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "cabecalhoTabela.cpp"
#include "../../indexador.cpp"
#include "linhaTabela.cpp"
#include "colunaInfo.cpp"
#include "filtroTabela.cpp"

struct Tabela{

    bool paginaAtualizada{true}; // controla a atualização da página para buscar novos registros

    Indexador *indexadorPtr{nullptr};

    const int numLinhas{20};
    const float posX{140}, posY{100}, largura{1000}, altura{500};

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Color> corFundo, corFundoLinha_1, corFundoLinha_2, corFundoLinhaSelecionada;

    sf::Sprite iconeEdicao, iconeAdicionar, iconeDeletar, iconeCsv;
    sf::Texture texturaIconeEdicao, texturaIconeEdicaoDesativado, texturaIconeAdicionar, texturaIconeAdicionarDesativado, 
        texturaIconeDeletar, texturaIconeDeletarDesativado, texturaIconeCsv;

    CabecalhoTabela cabecalho;

    int idTabela{0}, idRegistroSelecionado{-1};
    std::pair<int, Imovel *> resultadoBusca = std::make_pair(0, nullptr); // numero de linhas filtradas e array de ponteiro dos imoveis filtrados

    LinhaTabela *linhasTabela = new LinhaTabela[numLinhas - 1];

    ColunaInfo *colunasTabela = new ColunaInfo[5];

    FiltroTabela componenteFiltro;

    std::pair<int, bool> ordenacaoTabela = std::make_pair(1, true);
    std::pair<int, std::string> filtroTabela = std::make_pair(1, "");
    bool filtroAtivado{false};

    void construtor(Indexador *indexador){

        this->indexadorPtr = indexador;
        
        corFundo = std::make_shared<sf::Color>(169,169,169);
        corFundoLinha_1 = std::make_shared<sf::Color>(173, 215, 255);
        corFundoLinha_2 = std::make_shared<sf::Color>(212, 234, 255);
        corFundoLinhaSelecionada = std::make_shared<sf::Color>(47,79,79);

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura, altura));
        formaRetangulo->setFillColor(*corFundo);
        formaRetangulo->setPosition(posX, posY);

        int alturaLinha{altura/numLinhas};
        
        colunasTabela[0].construtor("Id", 5, 1);
        colunasTabela[1].construtor("Endereco", 30, 2);
        colunasTabela[2].construtor("Imobiliaria", 15, 3);
        colunasTabela[3].construtor("Aluguel", 15, 4);
        colunasTabela[4].construtor("Descricao", 35, 0);

        cabecalho.construtor(posX, posY, largura, alturaLinha, colunasTabela, 5, &ordenacaoTabela, &paginaAtualizada);

        componenteFiltro.construtor(posX, posY - 55, 1000, 30, colunasTabela, 5, &filtroTabela, &paginaAtualizada, &filtroAtivado);

        for(int i{0}; i<numLinhas - 1; i++)
            linhasTabela[i].construtor(posX, posY + (i + 1)*alturaLinha, largura, alturaLinha, colunasTabela, 5);
        

        utilitarios.carregaIcones(iconeEdicao, 0.05, 0.05, 1025, 625);
        utilitarios.carregaIcones(iconeAdicionar, texturaIconeAdicionar, 0.05, 0.05, 1070, 625, "addHome.png");
        utilitarios.carregaIcones(iconeDeletar, 0.05, 0.05, 1115, 625);

        utilitarios.carregaTextura(texturaIconeDeletar, "delHome.png");
        utilitarios.carregaTextura(texturaIconeDeletarDesativado, "delHomeBlocked.png");
        utilitarios.carregaTextura(texturaIconeEdicao, "editIcon.png");
        utilitarios.carregaTextura(texturaIconeEdicaoDesativado, "editIconBlocked.png");

        utilitarios.carregaIcones(iconeCsv, texturaIconeCsv, 0.05, 0.05, 140, 625, "csvIcon.png");
         
    }

    void eventosTabela(std::shared_ptr<sf::Event> event, int &paginaAtual){

        cabecalho.eventosCabecalho(event);
        componenteFiltro.eventosFiltro(event);

        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 140, 165, 625, 650)){
            paginaAtual = 2;
            paginaAtualizada = true;
        }
        else if(event->type == sf::Event::MouseWheelScrolled && utilitarios.verificaAreaEvento(event->mouseWheelScroll, 140, 1140, 100, 600)){
            
            int variacaoScroll{event->mouseWheelScroll.delta};
            int novoIdTabela  = idTabela + 1 * (variacaoScroll < 0 ? 1 : -1); // 1 -> fator de multiplicação do scroll

            // verifica se o novo index não ultrapassa os limites da tabela, seja no topo ou no final
            if(0 <= novoIdTabela && novoIdTabela <= resultadoBusca.first - (numLinhas - 1)){
                idTabela = novoIdTabela;
            }

        }else if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 140, 1140, 125, 600)){
            if((idTabela + (event->mouseButton.y - 125)/25) >= resultadoBusca.first)
                return;
            idRegistroSelecionado = idTabela + (event->mouseButton.y - 125)/25;
            
        }else if( //deletar imovel
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, 1115, 1140, 625, 650) &&
            idRegistroSelecionado >= 0
        ){
            indexadorPtr->deletarImovel(resultadoBusca.second[idRegistroSelecionado].id);
            idRegistroSelecionado = -1;
            paginaAtualizada = true;

        }else if( //editar imovel
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, 1025, 1050, 625, 650) &&
            idRegistroSelecionado >= 0
        ){
            paginaAtual = 3;
            indexadorPtr->imovelSelecionadoId = resultadoBusca.second[idRegistroSelecionado].id;
            idRegistroSelecionado = -1;
            paginaAtualizada = true;
        }else if( //criar imovel
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, 1070, 1095, 625, 650)
        ){
            paginaAtual = 3;
            indexadorPtr->imovelSelecionadoId = -1;
            idRegistroSelecionado = -1;
            paginaAtualizada = true;
        }


    }

    void desenhaTabela(std::shared_ptr<sf::RenderWindow> windowPtr){

        windowPtr->draw(*formaRetangulo);

        if(idRegistroSelecionado >= 0){
            utilitarios.atualizaTexturaIcone(iconeEdicao, texturaIconeEdicao);
            utilitarios.atualizaTexturaIcone(iconeDeletar, texturaIconeDeletar);
        }else{
            utilitarios.atualizaTexturaIcone(iconeEdicao, texturaIconeEdicaoDesativado);
            utilitarios.atualizaTexturaIcone(iconeDeletar, texturaIconeDeletarDesativado);
        }

        windowPtr->draw(iconeEdicao);
        windowPtr->draw(iconeAdicionar);
        windowPtr->draw(iconeDeletar);
        windowPtr->draw(iconeCsv);

        cabecalho.desenhaCabecalho(windowPtr);
        componenteFiltro.desenhaFiltro(windowPtr);

        int idTabelaFinal = resultadoBusca.first - 1 > idTabela + (numLinhas - 2) ? idTabela + (numLinhas - 2) : resultadoBusca.first - 1;

        for(int i{idTabela}; i<=idTabelaFinal; i++){
            linhasTabela[i - idTabela].desenhaLinha(
                windowPtr, 
                resultadoBusca.second + i,
                idRegistroSelecionado == i ? corFundoLinhaSelecionada : (i % 2 == 0 ? corFundoLinha_1 : corFundoLinha_2)
            );
        }
    }

    void atualizaPagina(){

        if(paginaAtualizada){
            // atualiza pagina
            delete[] this->resultadoBusca.second; // desaloca array da busca anterior
            this->resultadoBusca.first = 0;
            idTabela = 0;
            idRegistroSelecionado = -1;

            if(filtroAtivado)
                this->resultadoBusca = indexadorPtr->buscaImoveis(&ordenacaoTabela, &filtroTabela);
            else{
                std::pair<int, std::string> filtroVazio = std::make_pair(0, "");
                this->resultadoBusca = indexadorPtr->buscaImoveis(&ordenacaoTabela, &filtroVazio);
            }
            paginaAtualizada = false;
            filtroAtivado = false;
        }

    }


};