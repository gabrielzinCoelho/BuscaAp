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
    
    int paginaAtual; // variavel de controle para alternar entre as páginas da aplicação
    Tabela tabela;
    ManipulaCsv csv;
    MostraRegistro mostraRegistro;
    Indexador indexador;

    void construtor(){

        // configurações de renderização

        window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(larguraTela, alturaTela),
            "BuscaApe",
            sf::Style::Titlebar | sf::Style::Close
        );
        window->setPosition(sf::Vector2i(0, 0));
        window->setFramerateLimit(60);

        corFundoTela = std::make_shared<sf::Color>(211, 211, 211, 255);

        // inicializa as telas da aplicação e os controladores

        indexador.construtor();
        tabela.construtor(&indexador);
        csv.construtor(&indexador);
        mostraRegistro.construtor(&indexador);

        paginaAtual = 1; // pagina da tabela renderizada inicialmente

    };

    void run(){
        // loop para capturar eventos e renderizar a tela
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
            
            // distribui os eventos respectivos a cada página
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

        // cada página é responsável por definir como seus elementos serão renderizados
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