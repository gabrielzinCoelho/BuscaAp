#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"

struct ColunaCabecalho{

    std::string texto;
    float posX, posY, largura, altura;
    std::shared_ptr<sf::Text> instanciaTexto;
    sf::Sprite *iconeOrdenacaoAsc{nullptr}, *iconeOrdenacaoDesc{nullptr};

    int idOrdenacao;
    std::pair<int, bool> *ordenacaoTabela;
    bool *paginaAtualizada;

    void construtor(
        std::shared_ptr<sf::Text> instanciaTexto, float posX, float posY, float largura, float altura, std::string texto, 
        sf::Sprite *iconeOrdenacaoAsc, sf::Sprite *iconeOrdenacaoDesc, int idOrdenacao, std::pair<int, bool> *ordenacaoTabela, bool *paginaAtualizada
    ){
        this->instanciaTexto = instanciaTexto;
        this->posX = posX;
        this->posY = posY;
        this->largura = largura;
        this->altura = altura;
        this->texto = texto;

        this->iconeOrdenacaoAsc = iconeOrdenacaoAsc;
        this->iconeOrdenacaoDesc = iconeOrdenacaoDesc;

        this->idOrdenacao = idOrdenacao;
        this->ordenacaoTabela = ordenacaoTabela;

        this->paginaAtualizada = paginaAtualizada;
    }

    void desenhaColuna(std::shared_ptr<sf::RenderWindow> windowPtr){
        instanciaTexto->setPosition(posX, posY);
        instanciaTexto->setString(texto);
        windowPtr->draw(*instanciaTexto);

        if(idOrdenacao == ordenacaoTabela->first){

            sf::Sprite *iconeOrdenacao = ordenacaoTabela->second ? iconeOrdenacaoAsc : iconeOrdenacaoDesc;
            iconeOrdenacao->setPosition(posX + largura - 20, posY + 5);
            windowPtr->draw(*iconeOrdenacao);
        }

    }

    void eventosColuna(std::shared_ptr<sf::Event> event){

        if(
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + largura, posY, posY + altura) &&
            idOrdenacao
        ){
            ordenacaoTabela->second = ordenacaoTabela->first == idOrdenacao ? !ordenacaoTabela->second : true;
            ordenacaoTabela->first = idOrdenacao;
            *paginaAtualizada = true;
        }

    }
};