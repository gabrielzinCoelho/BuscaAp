#pragma once
#include <SFML/Graphics.hpp>

const float larguraTela{1280}, alturaTela{720};
const float proporcaoCaractere{0.6};

struct UtilitariosInterface{

    sf::Font font;

    void construtor(){

        font.loadFromFile("./resources/fonts/RobotoMono-Medium.ttf");
    }

    void carregaIcones(sf::Sprite &icone, sf::Texture &textura, float escalaIconeX, float escalaIconeY, float posX, float posY, std::string nomeArqTextura){
        
        icone.setScale(escalaIconeX, escalaIconeY);
        icone.setPosition(posX, posY);
        carregaTextura(textura, nomeArqTextura);
        icone.setTexture(textura);
    }

    void carregaIcones(sf::Sprite &icone, float escalaIconeX, float escalaIconeY, float posX, float posY){
        
        icone.setScale(escalaIconeX, escalaIconeY);
        icone.setPosition(posX, posY);

    }

    void atualizaTexturaIcone(sf::Sprite &icone, sf::Texture &textura){
        icone.setTexture(textura);
    }

    void carregaTextura(sf::Texture &textura, std::string nomeArqTextura){
        textura.loadFromFile("./resources/images/" + nomeArqTextura);
    }

    bool verificaAreaEvento(auto &instanciaEvento, int x1, int x2, int y1, int y2){
    
    bool xIsValid{x1 <= instanciaEvento.x && instanciaEvento.x <= x2};
    bool yIsValid{y1 <= instanciaEvento.y && instanciaEvento.y <= y2};

    return xIsValid && yIsValid;
}

};

UtilitariosInterface utilitarios;

float calculaTamanhoCaractere(float tamanhoFonte){
    return tamanhoFonte * proporcaoCaractere;
}
