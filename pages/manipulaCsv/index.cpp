#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../globals.cpp"
#include "radioInput.cpp"
#include "textoInput.cpp"
#include <iostream>

struct ManipulaCsv{

    std::shared_ptr<sf::RectangleShape> formaRadioInput;
    std::shared_ptr<sf::Color> corBranca, corRadioSelecionado;
    std::shared_ptr<sf::Text> instanciaTexto;

    sf::Sprite iconeVoltar, iconeConfirmar;
    sf::Texture texturaIconeVoltar, texturaIconeConfirmar, texturaIconeConfirmarDesativo;

    RadioInput radio_1, radio_2;
    TextoInput entradaArquivo;

    int opcaoRadio{0};
    std::string textoInput;

    void construtor(){

        utilitarios.carregaIcones(iconeVoltar, texturaIconeVoltar, 0.05, 0.05, 50, 50, "leftArrow.png");
        utilitarios.carregaIcones(iconeConfirmar, 0.05, 0.05, 1115, 350);
        utilitarios.carregaTextura(texturaIconeConfirmar, "confirmIcon.png");
        utilitarios.carregaTextura(texturaIconeConfirmarDesativo, "confirmIconBlocked.png");

        corBranca = std::make_shared<sf::Color>(255, 255, 255);
        corRadioSelecionado = std::make_shared<sf::Color>(29, 155, 240);

        instanciaTexto = std::make_shared<sf::Text>();
        instanciaTexto->setFont(utilitarios.font);
        instanciaTexto->setFillColor(sf::Color::Black);
        instanciaTexto->setCharacterSize(16);

        formaRadioInput = std::make_shared<sf::RectangleShape>(sf::Vector2f(15, 15));
        
        radio_1.construtor(formaRadioInput, corBranca, corRadioSelecionado, instanciaTexto, "Importar CSV", 140, 100, 15, &opcaoRadio, 1);
        radio_2.construtor(formaRadioInput, corBranca, corRadioSelecionado, instanciaTexto, "Exportar CSV", 140, 150, 15, &opcaoRadio, 2);

        entradaArquivo.construtor(instanciaTexto, 140, 250, &textoInput);

    }

    void eventosCsv(std::shared_ptr<sf::Event> event, int &paginaAtual){

        radio_1.eventosRadio(event);
        radio_2.eventosRadio(event);

        entradaArquivo.eventosTextoInput(event);

        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 50, 75, 50, 75)){
            paginaAtual = 1;
            opcaoRadio = 0;
            textoInput = "";
        }else if(
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, 1115, 1140, 350, 375) &&
            validaNomeCsv() &&
            opcaoRadio
        ){
            std::cout << textoInput << " importado com sucesso!!!\n";
            paginaAtual = 1;
            opcaoRadio = 0;
            textoInput = "";
        }
    }

    void desenhaCsv(std::shared_ptr<sf::RenderWindow> windowPtr){
        radio_1.desenhaRadio(windowPtr);
        radio_2.desenhaRadio(windowPtr);

        entradaArquivo.desenhaTextoInput(windowPtr);

        windowPtr->draw(iconeVoltar);

        if(opcaoRadio && validaNomeCsv())
            utilitarios.atualizaTexturaIcone(iconeConfirmar, texturaIconeConfirmar);
        else
            utilitarios.atualizaTexturaIcone(iconeConfirmar, texturaIconeConfirmarDesativo);
        windowPtr->draw(iconeConfirmar);
    }

    bool validaNomeCsv(){
        int tamTextoInput = textoInput.length();
        return (tamTextoInput >= 4 && (textoInput.substr(tamTextoInput - 4, 4) == ".csv") );
    }

};