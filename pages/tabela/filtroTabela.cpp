#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "colunaInfo.cpp"
#include "../../globals.cpp";

struct OpcaoFiltro{

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Text> instanciaTexto;

    float posX, posY, largura, altura;
    std::string rotuloFiltro;

    int numColunas, *idFiltro;
    ColunaInfo *colunas;

    sf::Sprite iconeAlternarFiltroDireita, iconeAlternarFiltroEsquerda;
    sf::Texture texturaAlternarFiltroDireita, texturaAlternarFiltroEsquerda;

    void construtor(float posX, float posY, float altura, ColunaInfo *colunas, int numColunas, int *idFiltro){

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(0, altura));
        formaRetangulo->setPosition(posX, posY);
        formaRetangulo->setFillColor(sf::Color(29, 155, 240));

        instanciaTexto = std::make_shared<sf::Text>();
        instanciaTexto->setFont(utilitarios.font);
        instanciaTexto->setFillColor(sf::Color::Black);
        instanciaTexto->setCharacterSize(16);
        instanciaTexto->setPosition(posX + 20, posY + 5);

        this->posX = posX;
        this->posY = posY;
        this->altura = altura;
        this->colunas = colunas;
        this->numColunas = numColunas;
        this->idFiltro = idFiltro;

        utilitarios.carregaIcones(iconeAlternarFiltroEsquerda, texturaAlternarFiltroEsquerda, 0.025, 0.025, posX + 5, posY + 10, "changeFilterLeft.png");
        utilitarios.carregaIcones(iconeAlternarFiltroDireita, texturaAlternarFiltroDireita, 0.025, 0.025, 0, 0, "changeFilterRight.png");

        atualizaOpcaoFiltro();

    }

    void atualizaOpcaoFiltro(){
        rotuloFiltro = colunas[(*idFiltro) - 1].nome;
        float larguraTexto = rotuloFiltro.size() * instanciaTexto->getCharacterSize() * proporcaoCaractere;
        largura = larguraTexto + 40;
    }

    void desenhaOpcoes(std::shared_ptr<sf::RenderWindow> windowPtr){

        formaRetangulo->setSize(sf::Vector2f(largura, altura));
        windowPtr->draw(*formaRetangulo);

        instanciaTexto->setString(rotuloFiltro);
        windowPtr->draw(*instanciaTexto);

        iconeAlternarFiltroDireita.setPosition(posX + largura - 17, posY + 10);
        windowPtr->draw(iconeAlternarFiltroEsquerda);
        windowPtr->draw(iconeAlternarFiltroDireita);


    }

    void eventosOpcoes(std::shared_ptr<sf::Event> event){

        if(
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + largura, posY, posY + altura)
        ){

            if(event->mouseButton.x <= posX + largura/2){ //opção anterior de filtro
                if(*idFiltro == 1)
                    return;
                (*idFiltro)--;

            }else{ // opção posterior de filtro

                if(*idFiltro == 4)
                    return;
                (*idFiltro)++;
            }

            atualizaOpcaoFiltro();
        }

    }

};

struct InputFiltro{

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    std::shared_ptr<sf::Text> instanciaTexto;
    std::shared_ptr<sf::Color> corBranca, corPreta, corBordaAtiva, corFundoDesativo;

    std::string *textoInput;
    int limiteCaracteres{50};
    bool ehAtivo{false};

    float posX, posY, largura, altura;

    float larguraBorda{2};

    void construtor(float posX, float posY, float altura, float largura, std::string *textoFiltro){

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(largura - 2*larguraBorda, altura - 2*larguraBorda));
        formaRetangulo->setPosition(posX + larguraBorda, posY + larguraBorda);
        formaRetangulo->setOutlineThickness(larguraBorda);

        instanciaTexto = std::make_shared<sf::Text>();
        instanciaTexto->setFont(utilitarios.font);
        instanciaTexto->setFillColor(sf::Color::Black);
        instanciaTexto->setCharacterSize(14);
        instanciaTexto->setPosition(posX + 5, posY + 5);

        corBranca = std::make_shared<sf::Color>(255, 255, 255);
        corPreta = std::make_shared<sf::Color>(0, 0, 0);
        corBordaAtiva = std::make_shared<sf::Color>(29, 155, 240);
        corFundoDesativo = std::make_shared<sf::Color>(169,169,169);

        this->textoInput = textoFiltro;
        this->posX = posX;
        this->posY = posY;
        this->altura = altura;
        this->largura = largura;

    }

    void eventosInput(std::shared_ptr<sf::Event> event){

        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + largura, posY, posY + altura)){
            ehAtivo = !ehAtivo;
        }else if(event->type == sf::Event::MouseButtonPressed && !utilitarios.verificaAreaEvento(event->mouseButton, posX, posX + largura, posY, posY + altura)){
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

    void desenhaInput(std::shared_ptr<sf::RenderWindow> windowPtr){

        formaRetangulo->setFillColor(ehAtivo ? *corBranca : *corFundoDesativo);
        formaRetangulo->setOutlineColor(ehAtivo ? *corBordaAtiva : *corPreta);
        windowPtr->draw(*formaRetangulo);

        instanciaTexto->setString(*textoInput);
        windowPtr->draw(*instanciaTexto);

    }

};

struct FiltroTabela{

    float posX, posY, largura, altura;
    std::pair<int, std::string> *filtroTabela{nullptr};
    bool *paginaAtualizada{nullptr}, *filtroAtivado{nullptr};

    std::shared_ptr<sf::RectangleShape> formaRetangulo;
    sf::Sprite iconePesquisa;
    sf::Texture texturaIconePesquisa;

    OpcaoFiltro opcaoFiltro;
    InputFiltro inputFiltro;
    
    void construtor(
        float posX, float posY, float largura, float altura, ColunaInfo *colunas, int numColunas, 
        std::pair<int, std::string> *filtroTabela, bool *paginaAtualizada, bool *filtroAtivado
    ){

        formaRetangulo = std::make_shared<sf::RectangleShape>(sf::Vector2f(30, 30));
        formaRetangulo->setPosition(posX + largura - 30, posY);
        formaRetangulo->setFillColor(sf::Color(29, 155, 240));

        utilitarios.carregaIcones(iconePesquisa, texturaIconePesquisa, 0.04, 0.04, posX + largura - 25, posY + 5, "searchIcon.png");

        this->posX = posX;
        this->posY = posY;
        this->altura = altura;
        this->largura = largura;
        this->filtroTabela = filtroTabela;
        this->paginaAtualizada = paginaAtualizada;
        this->filtroAtivado = filtroAtivado;

        opcaoFiltro.construtor(posX, posY - 30, altura, colunas, numColunas, &filtroTabela->first);
        inputFiltro.construtor(posX, posY, altura, 970, &filtroTabela->second);

    }

    void desenhaFiltro(std::shared_ptr<sf::RenderWindow> windowPtr){

        opcaoFiltro.desenhaOpcoes(windowPtr);
        inputFiltro.desenhaInput(windowPtr);

        windowPtr->draw(*formaRetangulo);
        windowPtr->draw(iconePesquisa);

    }

    void eventosFiltro(std::shared_ptr<sf::Event> event){
        opcaoFiltro.eventosOpcoes(event);
        inputFiltro.eventosInput(event);

        if(
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, posX + largura - 30, posX + largura, posY, posY + altura) &&
            filtroTabela->first &&
            filtroTabela->second.size()
        ){
            *paginaAtualizada = true;
            *filtroAtivado = true;
        }
        
    }


};