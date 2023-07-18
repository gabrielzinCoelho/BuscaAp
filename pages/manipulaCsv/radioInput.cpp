#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"

struct RadioInput{

    std::shared_ptr<sf::RectangleShape> formaRadioInput;
    std::shared_ptr<sf::Color> corBranca, corSelecionado;
    std::shared_ptr<sf::Text> instanciaTexto;

    int *opcaoRadio{nullptr}, id;

    float posX, posY, tamanhoLado;
    std::string textoRotulo;

    void construtor(
        std::shared_ptr<sf::RectangleShape> formaRadioInput, std::shared_ptr<sf::Color> corBranca, std::shared_ptr<sf::Color> corSelecionado,
        std::shared_ptr<sf::Text> instanciaTexto, std::string textoRotulo, float posX, float posY, float tamanhoLado, int *opcaoRadio, int id
    ){
        this->formaRadioInput = formaRadioInput;
        this->corBranca = corBranca;
        this->corSelecionado = corSelecionado;

        this->instanciaTexto = instanciaTexto;
        this->textoRotulo = textoRotulo;

        this->posX = posX;
        this->posY = posY;
        this->tamanhoLado = tamanhoLado;

        this->opcaoRadio = opcaoRadio;
        this->id = id;
    }

    void eventosRadio(std::shared_ptr<sf::Event> event){

        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + tamanhoLado, posY, posY + tamanhoLado)){
            *opcaoRadio = id;
        }
    }

    void desenhaRadio(std::shared_ptr<sf::RenderWindow> windowPtr){

        formaRadioInput->setFillColor(id == *opcaoRadio ? *corSelecionado : *corBranca);
        formaRadioInput->setPosition(posX, posY);
        windowPtr->draw(*formaRadioInput);

        instanciaTexto->setString(textoRotulo);
        instanciaTexto->setPosition(posX + 30, posY - 2);
        windowPtr->draw(*instanciaTexto);
    }

};