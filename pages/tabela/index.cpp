#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "cabecalhoTabela.cpp"

struct Tabela{

    const int numLinhas{20};
    const float posX{140}, posY{100}, largura{1000}, altura{500};

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Color> corFundo, corFundoLinha_1, corFundoLinha_2, corFundoLinhaSelecionada;

    sf::Sprite iconeEdicao, iconeAdicionar, iconeDeletar, iconeCsv;
    sf::Texture texturaIconeEdicao, texturaIconeAdicionar, texturaIconeDeletar, texturaIconeCsv;

    CabecalhoTabela cabecalho;

    std::pair<std::string, int> colunasTabela[5] = {
        std::make_pair("Id", 5),
        std::make_pair("Endereco", 30),
        std::make_pair("Imobiliaria", 20),
        std::make_pair("Aluguel", 10),
        std::make_pair("Descricao", 35)
    };

    void construtor(){
        
        corFundo = std::make_shared<sf::Color>(169,169,169);
        corFundoLinha_1 = std::make_shared<sf::Color>(29, 155, 240);
        corFundoLinha_2 = std::make_shared<sf::Color>(29, 155, 240);
        corFundoLinhaSelecionada = std::make_shared<sf::Color>(29, 155, 240);

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura, altura));
        formaRetangulo->setFillColor(*corFundo);
        formaRetangulo->setPosition(posX, posY);

        cabecalho.construtor(posX, posY, largura, altura/numLinhas, colunasTabela, 5);

        utilitarios.carregaIcones(iconeEdicao, texturaIconeEdicao, 0.05, 0.05, 1025, 625, "editIcon.png");
        utilitarios.carregaIcones(iconeAdicionar, texturaIconeAdicionar, 0.05, 0.05, 1070, 625, "addHome.png");
        utilitarios.carregaIcones(iconeDeletar, texturaIconeDeletar, 0.05, 0.05, 1115, 625, "delHome.png");

        utilitarios.carregaIcones(iconeCsv, texturaIconeCsv, 0.05, 0.05, 1115, 50, "csvIcon.png");
         
    }

    void eventosTabela(std::shared_ptr<sf::Event> event, int &paginaAtual){

        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 1115, 1140, 50, 75) ){
            paginaAtual = 2;
        }

    }

    void desenhaTabela(std::shared_ptr<sf::RenderWindow> windowPtr){
        windowPtr->draw(*formaRetangulo);
        windowPtr->draw(iconeEdicao);
        windowPtr->draw(iconeAdicionar);
        windowPtr->draw(iconeDeletar);
        windowPtr->draw(iconeCsv);
        cabecalho.desenhaCabecalho(windowPtr);
    }


};