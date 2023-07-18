#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"

struct ColunaCabecalho{

    std::string texto;
    float posX, posY, largura, altura;
    std::shared_ptr<sf::Text> instanciaTexto;

    void construtor(std::shared_ptr<sf::Text> instanciaTexto, float posX, float posY, float largura, float altura, std::string texto){
        this->instanciaTexto = instanciaTexto;
        this->posX = posX;
        this->posY = posY;
        this->largura = largura;
        this->altura = altura;
        this->texto = texto;
    }

    void desenhaColuna(std::shared_ptr<sf::RenderWindow> windowPtr){
        instanciaTexto->setPosition(posX, posY);
        instanciaTexto->setString(texto);

        windowPtr->draw(*instanciaTexto);

    }

    void eventosColuna(){

    }
};