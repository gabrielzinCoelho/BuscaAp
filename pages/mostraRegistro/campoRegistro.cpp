#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"

struct CampoRegistro{

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Text> instanciaTextoRotulo, instanciaTextoInput;
    std::shared_ptr<sf::Color> corBranca, corPreta, corBordaAtiva, corFundoDesativo;

    float posX, posY, largura, altura, larguraRotulo;
    std::string *valorCampo;
    bool bloqueado{false};
    int limiteCaracteres, numeroCaracteresMaximo, id, *indexCampoAtivo;

    void construtor(
        float posX, float posY, float largura, float altura, std::string rotuloCampo, std::string *valorCampo, bool bloqueado, int limiteCaracteres,
        int id, int *indexCampoAtivo
    ){
        
        this->posX = posX;
        this->posY = posY;
        this->largura = largura;
        this->altura = altura;
        this->valorCampo = valorCampo;
        this->bloqueado = bloqueado;
        this->limiteCaracteres = limiteCaracteres;

        corBranca = std::make_shared<sf::Color>(255, 255, 255);
        corPreta = std::make_shared<sf::Color>(0, 0, 0);
        corBordaAtiva = std::make_shared<sf::Color>(29, 155, 240);
        corFundoDesativo = std::make_shared<sf::Color>(169,169,169);

        instanciaTextoRotulo = std::make_shared<sf::Text>();
        instanciaTextoRotulo->setString(rotuloCampo);
        instanciaTextoRotulo->setFont(utilitarios.font);
        instanciaTextoRotulo->setFillColor(sf::Color::Black);
        instanciaTextoRotulo->setCharacterSize(18);
        instanciaTextoRotulo->setPosition(posX, posY);

        this->larguraRotulo = rotuloCampo.size() * instanciaTextoRotulo->getCharacterSize() * proporcaoCaractere;

        instanciaTextoInput = std::make_shared<sf::Text>();
        instanciaTextoInput->setFont(utilitarios.font);
        instanciaTextoInput->setFillColor(sf::Color::Black);
        instanciaTextoInput->setCharacterSize(16);
        instanciaTextoInput->setPosition(posX + larguraRotulo + 25, posY + 5);

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura - larguraRotulo - 20, altura));
        formaRetangulo->setPosition(posX + larguraRotulo + 20, posY);
        formaRetangulo->setFillColor(*corFundoDesativo);
        formaRetangulo->setOutlineColor(*corPreta);
        formaRetangulo->setOutlineThickness(2);

        numeroCaracteresMaximo = (largura - larguraRotulo - 20)/(instanciaTextoInput->getCharacterSize() * (proporcaoCaractere + 0.05));

        this->id = id;
        this->indexCampoAtivo = indexCampoAtivo;

    }

    void eventosCampo(std::shared_ptr<sf::Event> event){

        if(
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + largura, posY, posY + altura) &&
            !bloqueado
        ){
            *indexCampoAtivo = id;
        }else if(event->type == sf::Event::TextEntered && *indexCampoAtivo == id){
            
            switch(event->text.unicode){
                case 8: //backspace
                    if((*valorCampo).size())
                        (*valorCampo) = (*valorCampo).substr(0, (*valorCampo).size() - 1);
                    break;
                case 13: //enter
                    break;
                default:
                    if(event->text.unicode < 128 && (*valorCampo).size() < limiteCaracteres)
                        (*valorCampo) += static_cast<char>(event->text.unicode);
            }
        }

    }

    void desenhaCampo(std::shared_ptr<sf::RenderWindow> windowPtr){

        formaRetangulo->setFillColor(*indexCampoAtivo == id ? *corBranca : *corFundoDesativo);
        formaRetangulo->setOutlineColor(*indexCampoAtivo == id ? *corBordaAtiva : *corPreta);
        windowPtr->draw(*formaRetangulo);

        windowPtr->draw(*instanciaTextoRotulo);

        instanciaTextoInput->setString(
            valorCampo->size() > numeroCaracteresMaximo ? valorCampo->substr(0, numeroCaracteresMaximo - 3) + "..." : *valorCampo
        );
        windowPtr->draw(*instanciaTextoInput);

    }

};