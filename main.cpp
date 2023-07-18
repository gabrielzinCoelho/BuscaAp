#pragma once
#include "interface.cpp"
#include "globals.cpp"

int main()
{
    InterfaceGrafica interface;

    utilitarios.construtor();

    interface.construtor();
    interface.run();    

    return 0;
}