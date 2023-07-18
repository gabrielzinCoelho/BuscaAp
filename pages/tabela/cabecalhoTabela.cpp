#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"
#include "colunasCabecalho.cpp"

struct CabecalhoTabela{

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Color> corFundo;
    std::shared_ptr<sf::Text> instanciaTexto;

    ColunaCabecalho *colunasCabecalho;
    int numColunas;

    void construtor(float posX, float posY, float largura, float altura, std::pair<std::string, int> *colunas, int numColunas){

        corFundo = std::make_shared<sf::Color>(29, 155, 240);

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura, altura));
        formaRetangulo->setFillColor(*corFundo);
        formaRetangulo->setPosition(posX, posY);

        instanciaTexto = std::make_shared<sf::Text>();
        instanciaTexto->setFont(utilitarios.font);
        instanciaTexto->setFillColor(sf::Color::Black);
        instanciaTexto->setCharacterSize(16);

        this->numColunas = numColunas;
        colunasCabecalho = new ColunaCabecalho[numColunas];

        int larguraColuna;

        for(int i{0}; i<numColunas; i++){

            larguraColuna = largura * colunas[i].second / float(100);

            colunasCabecalho[i].construtor(
                instanciaTexto,
                posX,
                posY,
                larguraColuna,
                altura,
                colunas[i].first
            );

            posX += larguraColuna;

        }

    };

    void eventosCabecalho(){

    }

    void desenhaCabecalho(std::shared_ptr<sf::RenderWindow> windowPtr){
        windowPtr->draw(*formaRetangulo);

        for(int i{0}; i<numColunas; i++)
            colunasCabecalho[i].desenhaColuna(windowPtr);

    }

};