#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"


struct ColunaLinha{

    float posX, posY, largura, altura;
    std::shared_ptr<sf::Text> instanciaTexto;

    void construtor(std::shared_ptr<sf::Text> instanciaTexto, float posX, float posY, float largura, float altura){

        this->instanciaTexto = instanciaTexto;
        this->posX = posX;
        this->posY = posY;
        this->largura = largura;
        this->altura = altura;
    }

    void desenhaColuna(std::shared_ptr<sf::RenderWindow> windowPtr, std::string textoColuna){
        
        instanciaTexto->setPosition(posX, posY);
        instanciaTexto->setString(textoColuna);
        windowPtr->draw(*instanciaTexto);

    }

    void eventosColuna(){

    }
};