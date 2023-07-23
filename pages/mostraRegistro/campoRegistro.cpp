#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"

struct CampoRegistro{

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Text> instanciaTextoRotulo, instanciaTextoInput;
    std::shared_ptr<sf::Color> corBranca, corPreta, corBordaAtiva, corFundoDesativo;

    float posX, posY, largura, altura;
    std::string *valorCampo;
    bool bloqueado{false}, ehAtivo{false};
    int limiteCaracteres;

    void construtor(
        float posX, float posY, float largura, float altura, std::string rotuloCampo, std::string *valorCampo, bool bloqueado, int limiteCaracteres
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
        instanciaTextoRotulo->setCharacterSize(16);
        instanciaTextoRotulo->setPosition(posX, posY);

        float larguraRotulo{rotuloCampo.size() * instanciaTextoRotulo->getCharacterSize() * proporcaoCaractere};

        instanciaTextoInput = std::make_shared<sf::Text>();
        instanciaTextoInput->setFont(utilitarios.font);
        instanciaTextoInput->setFillColor(sf::Color::Black);
        instanciaTextoInput->setCharacterSize(14);
        instanciaTextoInput->setPosition(posX + larguraRotulo + 25, posY + 5);

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura - larguraRotulo - 20, altura));
        formaRetangulo->setPosition(posX + larguraRotulo + 20, posY);
        formaRetangulo->setFillColor(*corFundoDesativo);
        formaRetangulo->setOutlineColor(*corPreta);
        formaRetangulo->setOutlineThickness(2);

    }

    void eventosCampo(std::shared_ptr<sf::Event> event){



    }

    void desenhaCampo(std::shared_ptr<sf::RenderWindow> windowPtr){
        windowPtr->draw(*formaRetangulo);
        windowPtr->draw(*instanciaTextoRotulo);
        windowPtr->draw(*instanciaTextoInput);
    }

};