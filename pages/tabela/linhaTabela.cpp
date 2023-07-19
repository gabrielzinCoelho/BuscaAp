#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"
#include "colunasLinha.cpp"

struct LinhaTabela{

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Text> instanciaTexto;

    std::pair<std::string, int> *colunas;
    ColunaLinha *colunasLinha;
    int numColunas;

    void construtor(float posX, float posY, float largura, float altura, std::pair<std::string, int> *colunas, int numColunas){

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura, altura));
        formaRetangulo->setPosition(posX, posY);

        instanciaTexto = std::make_shared<sf::Text>();
        instanciaTexto->setFont(utilitarios.font);
        instanciaTexto->setFillColor(sf::Color::Black);
        instanciaTexto->setCharacterSize(14);

        this->numColunas = numColunas;
        colunasLinha = new ColunaLinha[numColunas];

        int larguraColuna;

        for(int i{0}; i<numColunas; i++){

            larguraColuna = largura * colunas[i].second / float(100);

            colunasLinha[i].construtor(
                instanciaTexto,
                posX,
                posY,
                larguraColuna,
                altura
            );

            posX += larguraColuna;

        }

    }

    void eventosLinha(){

    }

    void desenhaLinha(std::shared_ptr<sf::RenderWindow> windowPtr, Imovel *imovelPtr){
        // dados que serao mostrados e cor fundo atualizados a cada renderização
        colunasLinha[0].desenhaColuna(windowPtr, std::to_string(imovelPtr->id));
        colunasLinha[1].desenhaColuna(windowPtr, imovelPtr->endereco);
        colunasLinha[2].desenhaColuna(windowPtr, imovelPtr->imobiliaria);
        colunasLinha[3].desenhaColuna(windowPtr, std::to_string(imovelPtr->precoAluguel));
        colunasLinha[4].desenhaColuna(windowPtr, imovelPtr->descricao);
    }

};