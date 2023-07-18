#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"

struct TextoInput{

    std::shared_ptr<sf::RectangleShape> formaInput;
    std::shared_ptr<sf::Text> instanciaTexto;
    std::shared_ptr<sf::Color> corBranca, corPreta, corBordaAtiva, corFundoDesativo;

    std::string *textoInput;
    int limiteCaracteres{50};
    bool ehAtivo{false};
    float posX, posY, larguraInput{1000}, alturaInput{30};
    
    void construtor(std::shared_ptr<sf::Text> instanciaTexto, float posX, float posY, std::string *textoInput){

        corBranca = std::make_shared<sf::Color>(255, 255, 255);
        corPreta = std::make_shared<sf::Color>(0, 0, 0);
        corBordaAtiva = std::make_shared<sf::Color>(29, 155, 240);
        corFundoDesativo = std::make_shared<sf::Color>(169,169,169);

        this->instanciaTexto = instanciaTexto;

        formaInput = std::make_shared<sf::RectangleShape>(sf::Vector2f(larguraInput, alturaInput));
        formaInput->setOutlineThickness(2);
        formaInput->setPosition(posX, posY + 30);

        this->posX = posX;
        this->posY = posY;

        this->textoInput = textoInput;

    }

    void eventosTextoInput(std::shared_ptr<sf::Event> event){
        
        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + larguraInput, posY + 30, posY + 30 + alturaInput)){
            ehAtivo = !ehAtivo;
        }else if(event->type == sf::Event::MouseButtonPressed && !utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + larguraInput, posY + 30, posY + 30 + alturaInput)){
            ehAtivo = false;
        }else if(event->type == sf::Event::TextEntered && ehAtivo){
            
            switch(event->text.unicode){
                case 8:
                    if((*textoInput).size())
                        (*textoInput) = (*textoInput).substr(0, (*textoInput).size() - 1);
                    break;
                default:
                    if(event->text.unicode < 128 && (*textoInput).size() < limiteCaracteres)
                        (*textoInput) += static_cast<char>(event->text.unicode);
            }
        }

    }

    void desenhaTextoInput(std::shared_ptr<sf::RenderWindow> windowPtr){

        formaInput->setFillColor(ehAtivo ? *corBranca : *corFundoDesativo);
        formaInput->setOutlineColor(ehAtivo ? *corBordaAtiva : *corPreta);
        windowPtr->draw(*formaInput);

        instanciaTexto->setPosition(posX, posY);
        instanciaTexto->setString("Nome do arquivo csv: ");
        windowPtr->draw(*instanciaTexto);

        instanciaTexto->setPosition(posX + 5, posY + 35);
        instanciaTexto->setString(*textoInput);
        windowPtr->draw(*instanciaTexto);

    }

};