#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"
#include "colunasCabecalho.cpp"
#include "colunaInfo.cpp"

struct CabecalhoTabela{

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Color> corFundo;
    std::shared_ptr<sf::Text> instanciaTexto;

    sf::Sprite iconeOrdenacaoAsc, iconeOrdenacaoDesc;
    sf::Texture texturaIconeOrdenacaoAsc, texturaIconeOrdenacaoDesc;

    ColunaCabecalho *colunasCabecalho;
    int numColunas;

    void construtor(
        float posX, float posY, float largura, float altura, ColunaInfo *colunas, int numColunas, std::pair<int, bool> *ordenacaoTabela,
        bool *paginaAtualizada
    ){

        corFundo = std::make_shared<sf::Color>(29, 155, 240);

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura, altura));
        formaRetangulo->setFillColor(*corFundo);
        formaRetangulo->setPosition(posX, posY);

        instanciaTexto = std::make_shared<sf::Text>();
        instanciaTexto->setFont(utilitarios.font);
        instanciaTexto->setFillColor(sf::Color::Black);
        instanciaTexto->setCharacterSize(16);

        utilitarios.carregaIcones(iconeOrdenacaoAsc, texturaIconeOrdenacaoAsc, 0.025, 0.025, 0, 0, "sortArrowAsc.png");
        utilitarios.carregaIcones(iconeOrdenacaoDesc, texturaIconeOrdenacaoDesc, 0.025, 0.025, 0, 0, "sortArrowDesc.png");

        this->numColunas = numColunas;
        colunasCabecalho = new ColunaCabecalho[numColunas];

        int larguraColuna;

        for(int i{0}; i<numColunas; i++){

            larguraColuna = largura * colunas[i].proporcaoLargura / float(100);

            colunasCabecalho[i].construtor(
                instanciaTexto,
                posX,
                posY,
                larguraColuna,
                altura,
                colunas[i].nome,
                &iconeOrdenacaoAsc,
                &iconeOrdenacaoDesc,
                colunas[i].idColuna,
                ordenacaoTabela,
                paginaAtualizada
            );

            posX += larguraColuna;

        }

    };

    void eventosCabecalho(std::shared_ptr<sf::Event> event){
        for(int i{0}; i<numColunas; i++)
            colunasCabecalho[i].eventosColuna(event);
    }

    void desenhaCabecalho(std::shared_ptr<sf::RenderWindow> windowPtr){
        windowPtr->draw(*formaRetangulo);

        for(int i{0}; i<numColunas; i++)
            colunasCabecalho[i].desenhaColuna(windowPtr);

    }

};