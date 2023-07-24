#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include "globals.cpp"
#include "./pages/tabela/index.cpp"
#include "./pages/manipulaCsv/index.cpp"
#include "./pages/mostraRegistro/index.cpp"
#include "indexador.cpp"

struct InterfaceGrafica{

    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::Color> corFundoTela;
    
    int paginaAtual;
    Tabela tabela;
    ManipulaCsv csv;
    MostraRegistro mostraRegistro;
    Indexador indexador;

    void construtor(){

        window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(larguraTela, alturaTela),
            "BuscaApe",
            sf::Style::Titlebar | sf::Style::Close
        );
        window->setPosition(sf::Vector2i(0, 0));
        window->setFramerateLimit(60);

        corFundoTela = std::make_shared<sf::Color>(211, 211, 211, 255);

        indexador.construtor();
        tabela.construtor(&indexador);
        csv.construtor(&indexador);
        mostraRegistro.construtor(&indexador);

        paginaAtual = 1;

    };

    void run(){

        while (window->isOpen()){
            events();
            draw();
        }

    };
    

    void events(){
        std::shared_ptr<sf::Event> e = std::make_shared<sf::Event>();
        while( window->pollEvent(*e) ){

            if(e->type == sf::Event::Closed)
                window->close();
            
            switch(paginaAtual){
                default:
                case 1:
                    tabela.atualizaPagina();
                    tabela.eventosTabela(e, paginaAtual);
                    break;
                case 2:
                    csv.eventosCsv(e, paginaAtual);
                    break;
                case 3:
                    mostraRegistro.atualizaPagina();
                    mostraRegistro.eventosRegistro(e, paginaAtual);
                    break;
            }
            
        }
    };

    void draw(){
        window->clear(*corFundoTela);

        switch(paginaAtual){
                default:
                case 1:
                    tabela.desenhaTabela(window);
                    break;
                case 2:
                    csv.desenhaCsv(window);
                    break;
                case 3:
                    mostraRegistro.desenhaRegistro(window);
                    break;
            }

        window->display();
    };

};