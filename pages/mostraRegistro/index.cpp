#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "campoRegistro.cpp"
#include "../../imovel.cpp"
#include "../../indexador.cpp"

struct MostraRegistro{

    int numCampos{12};
    CampoRegistro *camposRegistro = new CampoRegistro[numCampos];

    int bordaSuperior{100}, bordasLaterais{140}, larguraPagina{1000};
    Imovel *imovel{nullptr};

    std::string *valoresImovel = new std::string[numCampos];

    sf::Sprite iconeVoltar, iconeConfirmar;
    sf::Texture texturaIconeVoltar, texturaIconeConfirmar, texturaIconeConfirmarDesativo;

    void construtor(Indexador *indexadorPtr){
        
        camposRegistro[0].construtor(bordasLaterais, bordaSuperior, larguraPagina, 25, "Id: ", valoresImovel, true, 5);
        camposRegistro[1].construtor(bordasLaterais, bordaSuperior + 50, larguraPagina, 25, "Endereco: ", valoresImovel + 1, false, 30);
        camposRegistro[2].construtor(bordasLaterais, bordaSuperior + 100, larguraPagina, 125, "Descricao: ", valoresImovel + 2, false, 30);
        camposRegistro[3].construtor(bordasLaterais, bordaSuperior + 250, larguraPagina, 25, "Imobiliaria: ", valoresImovel + 3, false, 30);

        camposRegistro[4].construtor(bordasLaterais, bordaSuperior + 300, 205, 25, "Area (m2): ", valoresImovel, false, 5);
        camposRegistro[5].construtor(bordasLaterais + 265, bordaSuperior + 300, 205, 25, "Quarto(s): ", valoresImovel, false, 5);
        camposRegistro[6].construtor(bordasLaterais + 530, bordaSuperior + 300, 205, 25, "Banheiro(s): ", valoresImovel, false, 5);
        camposRegistro[7].construtor(bordasLaterais + 795, bordaSuperior + 300, 205, 25, "Garagem: ", valoresImovel, false, 5);

        camposRegistro[8].construtor(bordasLaterais, bordaSuperior + 350, 475, 25, "Aluguel (R$): ", valoresImovel, false, 5);
        camposRegistro[9].construtor(bordasLaterais + 525, bordaSuperior + 350, 475, 25, "Condominio (R$): ", valoresImovel, false, 5);

        camposRegistro[10].construtor(bordasLaterais, bordaSuperior + 400, 475, 25, "IPTU (R$): ", valoresImovel, false, 5);
        camposRegistro[11].construtor(bordasLaterais + 525, bordaSuperior + 400, 475, 25, "Seguro (R$): ", valoresImovel, false, 5);

        utilitarios.carregaIcones(iconeVoltar, texturaIconeVoltar, 0.05, 0.05, 50, 50, "leftArrow.png");
        utilitarios.carregaIcones(iconeConfirmar, 0.05, 0.05, 1115, bordaSuperior + 450);
        utilitarios.carregaTextura(texturaIconeConfirmar, "confirmIcon.png");
        utilitarios.carregaTextura(texturaIconeConfirmarDesativo, "confirmIconBlocked.png");
    }

    void eventosRegistro(std::shared_ptr<sf::Event> event, int &paginaAtual){

        for(int i{0}; i<numCampos; i++)
            camposRegistro[i].eventosCampo(event);
        
        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 50, 75, 50, 75)){
            paginaAtual = 1;
        }else if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 1115, 1140, 550, 575)){
            paginaAtual = 1;
        }
    }

    void desenhaRegistro(std::shared_ptr<sf::RenderWindow> windowPtr){

        for(int i{0}; i<numCampos; i++)
            camposRegistro[i].desenhaCampo(windowPtr);

        if(true)
            utilitarios.atualizaTexturaIcone(iconeConfirmar, texturaIconeConfirmar);
        else
            utilitarios.atualizaTexturaIcone(iconeConfirmar, texturaIconeConfirmarDesativo);
        windowPtr->draw(iconeConfirmar);
        windowPtr->draw(iconeVoltar);

    }

};