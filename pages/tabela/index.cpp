#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "cabecalhoTabela.cpp"
#include "../../indexador.cpp"
#include "linhaTabela.cpp"

struct Tabela{

    bool paginaAtualizada{true}; // controla a atualização da página para buscar novos registros

    Indexador *indexadorPtr;

    const int numLinhas{20};
    const float posX{140}, posY{100}, largura{1000}, altura{500};

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Color> corFundo, corFundoLinha_1, corFundoLinha_2, corFundoLinhaSelecionada;

    sf::Sprite iconeEdicao, iconeAdicionar, iconeDeletar, iconeCsv;
    sf::Texture texturaIconeEdicao, texturaIconeAdicionar, texturaIconeDeletar, texturaIconeCsv;

    CabecalhoTabela cabecalho;

    int idTabela{0};
    std::pair<int, Imovel **> resultadoBusca; // numero de linhas filtradas e array de ponteiro dos imoveis filtrados

    LinhaTabela *linhasTabela = new LinhaTabela[numLinhas - 1];

    std::pair<std::string, int> colunasTabela[5] = {
        std::make_pair("Id", 5),
        std::make_pair("Endereco", 30),
        std::make_pair("Imobiliaria", 20),
        std::make_pair("Aluguel", 10),
        std::make_pair("Descricao", 35)
    };

    void construtor(Indexador *indexador){

        this->indexadorPtr = indexador;
        
        corFundo = std::make_shared<sf::Color>(169,169,169);
        corFundoLinha_1 = std::make_shared<sf::Color>(29, 155, 240);
        corFundoLinha_2 = std::make_shared<sf::Color>(29, 155, 240);
        corFundoLinhaSelecionada = std::make_shared<sf::Color>(29, 155, 240);

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura, altura));
        formaRetangulo->setFillColor(*corFundo);
        formaRetangulo->setPosition(posX, posY);

        int alturaLinha{altura/numLinhas};

        cabecalho.construtor(posX, posY, largura, alturaLinha, colunasTabela, 5);

        for(int i{0}; i<numLinhas - 1; i++)
            linhasTabela[i].construtor(posX, posY + (i + 1)*alturaLinha, largura, alturaLinha, colunasTabela, 5);
        

        utilitarios.carregaIcones(iconeEdicao, texturaIconeEdicao, 0.05, 0.05, 1025, 625, "editIcon.png");
        utilitarios.carregaIcones(iconeAdicionar, texturaIconeAdicionar, 0.05, 0.05, 1070, 625, "addHome.png");
        utilitarios.carregaIcones(iconeDeletar, texturaIconeDeletar, 0.05, 0.05, 1115, 625, "delHome.png");

        utilitarios.carregaIcones(iconeCsv, texturaIconeCsv, 0.05, 0.05, 1115, 50, "csvIcon.png");
         
    }

    void eventosTabela(std::shared_ptr<sf::Event> event, int &paginaAtual){

        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 1115, 1140, 50, 75) ){
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

        }


    }

    void desenhaTabela(std::shared_ptr<sf::RenderWindow> windowPtr){

        windowPtr->draw(*formaRetangulo);

        windowPtr->draw(iconeEdicao);
        windowPtr->draw(iconeAdicionar);
        windowPtr->draw(iconeDeletar);
        windowPtr->draw(iconeCsv);

        cabecalho.desenhaCabecalho(windowPtr);

        int idTabelaFinal = resultadoBusca.first - 1 > idTabela + (numLinhas - 2) ? idTabela + (numLinhas - 2) : resultadoBusca.first - 1;

        for(int i{idTabela}; i<=idTabelaFinal; i++){
            linhasTabela[i - idTabela].desenhaLinha(windowPtr, resultadoBusca.second[i]);
        }
    }

    void atualizaPagina(){

        if(paginaAtualizada){
            // atualiza pagina
            delete[] this->resultadoBusca.second; // desaloca array da busca anterior
            this->resultadoBusca.first = 0;
            idTabela = 0;

            this->resultadoBusca = indexadorPtr->buscaImoveis();
            std::cout << resultadoBusca.first << "\n";
            paginaAtualizada = false;
        }

    }


};