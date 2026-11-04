//Declaracion de funciones existentes dentro de memoria.cpp
#ifndef MEMORIA_H
#define MEMORIA_H

void menuMemoria();

void menuParticionamiento();

void menuPaginacion();

//funciones pertenecientes a particionamiento
void inicializarMemoria();
void solicitarBloque();
void liberarBloque();
void mostrarEstadoMemoria();
void unirBloquesLibres();
void mostrarFragmentacionExterna();

//funciones pertenecientes a paginacion
void configurarPaginacion();
void asignarPaginas();
void mostrarTablaPaginas();
void traducirDireccionLogica();

#endif
