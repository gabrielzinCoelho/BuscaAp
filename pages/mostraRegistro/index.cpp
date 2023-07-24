#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "campoRegistro.cpp"
#include "../../imovel.cpp"
#include "../../indexador.cpp"

struct MostraRegistro{

    bool paginaAtualizada{true};
    Indexador *indexadorPtr{nullptr};

    int numCampos{12}, indexCampoAtivo{-1};
    CampoRegistro *camposRegistro = new CampoRegistro[numCampos];

    int bordaSuperior{100}, bordasLaterais{140}, larguraPagina{1000};
    
    int idImovelSelecionado{-1};

    std::string *valoresImovel = new std::string[numCampos];

    sf::Sprite iconeVoltar, iconeConfirmar;
    sf::Texture texturaIconeVoltar, texturaIconeConfirmar, texturaIconeConfirmarDesativo;

    void construtor(Indexador *indexadorPtr){
        
        camposRegistro[0].construtor(bordasLaterais, bordaSuperior, larguraPagina, 25, "Id: ", valoresImovel, true, 5, 0, &indexCampoAtivo);
        camposRegistro[1].construtor(bordasLaterais, bordaSuperior + 50, larguraPagina, 25, "Endereco: ", valoresImovel + 1, false, 50, 1, &indexCampoAtivo);
        camposRegistro[2].construtor(bordasLaterais, bordaSuperior + 100, larguraPagina, 125, "Descricao: ", valoresImovel + 2, false, 50, 2, &indexCampoAtivo);
        camposRegistro[3].construtor(bordasLaterais, bordaSuperior + 250, larguraPagina, 25, "Imobiliaria: ", valoresImovel + 3, false, 50, 3, &indexCampoAtivo);

        camposRegistro[4].construtor(bordasLaterais, bordaSuperior + 300, 235, 25, "Area (m2): ", valoresImovel + 4, false, 7, 4, &indexCampoAtivo);
        camposRegistro[5].construtor(bordasLaterais + 295, bordaSuperior + 300, 195, 25, "Quarto(s): ", valoresImovel + 5, false, 5, 5, &indexCampoAtivo);
        camposRegistro[6].construtor(bordasLaterais + 550, bordaSuperior + 300, 195, 25, "Banheiro(s): ", valoresImovel + 6, false, 5, 6, &indexCampoAtivo);
        camposRegistro[7].construtor(bordasLaterais + 805, bordaSuperior + 300, 195, 25, "Garagem: ", valoresImovel + 7, false, 5, 7, &indexCampoAtivo);

        camposRegistro[8].construtor(bordasLaterais, bordaSuperior + 350, 475, 25, "Aluguel (R$): ", valoresImovel + 8, false, 10, 8, &indexCampoAtivo);
        camposRegistro[9].construtor(bordasLaterais + 525, bordaSuperior + 350, 475, 25, "Condominio (R$): ", valoresImovel + 9, false, 10, 9, &indexCampoAtivo);

        camposRegistro[10].construtor(bordasLaterais, bordaSuperior + 400, 475, 25, "IPTU (R$): ", valoresImovel + 10, false, 10, 10, &indexCampoAtivo);
        camposRegistro[11].construtor(bordasLaterais + 525, bordaSuperior + 400, 475, 25, "Seguro (R$): ", valoresImovel + 11, false, 10, 11, &indexCampoAtivo);

        utilitarios.carregaIcones(iconeVoltar, texturaIconeVoltar, 0.05, 0.05, 50, 50, "leftArrow.png");
        utilitarios.carregaIcones(iconeConfirmar, 0.05, 0.05, 1115, bordaSuperior + 450);
        utilitarios.carregaTextura(texturaIconeConfirmar, "confirmIcon.png");
        utilitarios.carregaTextura(texturaIconeConfirmarDesativo, "confirmIconBlocked.png");

        this->indexadorPtr = indexadorPtr;

    }

    void eventosRegistro(std::shared_ptr<sf::Event> event, int &paginaAtual){

        for(int i{0}; i<numCampos; i++)
            camposRegistro[i].eventosCampo(event);
        
        if(event->type == sf::Event::MouseButtonPressed && utilitarios.verificaAreaEvento(event->mouseButton, 50, 75, 50, 75)){
            paginaAtual = 1;
            paginaAtualizada = true;
        }else if(
            event->type == sf::Event::MouseButtonPressed && 
            utilitarios.verificaAreaEvento(event->mouseButton, 1115, 1140, 550, 575) &&
            validaForm()
        ){
            std::cout << "B\n";
            if(idImovelSelecionado == -1){ 
                //criar Imovel
                indexadorPtr->criarImovel(valoresImovel);
            }else{
                //editar Imovel
                indexadorPtr->editarImovel(valoresImovel);
            }

            paginaAtual = 1;
            paginaAtualizada = true;
        }
    }

    void desenhaRegistro(std::shared_ptr<sf::RenderWindow> windowPtr){

        for(int i{0}; i<numCampos; i++)
            camposRegistro[i].desenhaCampo(windowPtr);

        if(validaForm())
            utilitarios.atualizaTexturaIcone(iconeConfirmar, texturaIconeConfirmar);
        else
            utilitarios.atualizaTexturaIcone(iconeConfirmar, texturaIconeConfirmarDesativo);
        windowPtr->draw(iconeConfirmar);
        windowPtr->draw(iconeVoltar);

    }

    bool validaForm(){
        for(int i{0}; i<numCampos; i++)
            if(!valoresImovel[i].size())
                return false;
        return true;
    }

    void atualizaPagina(){

        if(paginaAtualizada){

            indexCampoAtivo = -1;
            paginaAtualizada = false;

            if(indexadorPtr->imovelSelecionadoId >= 0){
                idImovelSelecionado = indexadorPtr->imovelSelecionadoId;

                Imovel * imovelSelecionado = indexadorPtr->buscarImovelSelecionado();

                if(!imovelSelecionado){
                    return;
                }
                
                valoresImovel[0] = std::to_string(imovelSelecionado->id);
                valoresImovel[1] = imovelSelecionado->endereco;
                valoresImovel[2] = imovelSelecionado->descricao;
                valoresImovel[3] = imovelSelecionado->imobiliaria;
                valoresImovel[4] = std::to_string(imovelSelecionado->area);
                valoresImovel[5] = std::to_string(imovelSelecionado->numQuartos);
                valoresImovel[6] = std::to_string(imovelSelecionado->numBanheiros);
                valoresImovel[7] = std::to_string(imovelSelecionado->vagasGaragem);
                valoresImovel[8] = std::to_string(imovelSelecionado->precoAluguel);
                valoresImovel[9] = std::to_string(imovelSelecionado->condominio);
                valoresImovel[10] = std::to_string(imovelSelecionado->iptu);
                valoresImovel[11] = std::to_string(imovelSelecionado->seguroIncendio);

                delete imovelSelecionado;

            }else{
                for(int i{1}; i<numCampos; i++)
                    valoresImovel[i] = "";
                valoresImovel[0] = std::to_string(indexadorPtr->calculaIdNovoImovel());
                idImovelSelecionado = -1;
            }

            
        }

    }

};