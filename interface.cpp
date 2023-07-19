#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include "globals.cpp"
#include "./pages/tabela/index.cpp"
#include "./pages/manipulaCsv/index.cpp"
#include "indexador.cpp"

struct InterfaceGrafica{

    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<sf::Color> corFundoTela;
    
    int paginaAtual;
    Tabela tabela;
    ManipulaCsv csv;
    Indexador indexador;

    void construtor(){

        window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(larguraTela, alturaTela),
            "BuscaApe",
            sf::Style::Titlebar | sf::Style::Close
        );
        window->setPosition(sf::Vector2i(0, 0));
        window->setFramerateLimit(10);

        corFundoTela = std::make_shared<sf::Color>(211, 211, 211, 255);

        tabela.construtor(&indexador);
        csv.construtor(&indexador);

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
            
            if(paginaAtual == 1){
                tabela.atualizaPagina();
                tabela.eventosTabela(e, paginaAtual);
            }
            else if(paginaAtual == 2)
                csv.eventosCsv(e, paginaAtual);

        }
    };

    void draw(){
        window->clear(*corFundoTela);
        
        if(paginaAtual == 1){
            tabela.desenhaTabela(window);
        }else if(paginaAtual == 2){
            csv.desenhaCsv(window);
        }

        window->display();
    };

};