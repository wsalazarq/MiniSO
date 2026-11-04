
#ifndef MEMORIA_H
#define MEMORIA_H

void menuMemoria();

void menuParticionamiento();

void menuPaginacion();

void inicializarMemoria();

void solicitarBloque();
void liberarBloque();
void mostrarEstadoMemoria();
void unirBloquesLibres();
void mostrarFragmentacionExterna();

void configurarPaginacion();
void asignarPaginas();
void mostrarTablaPaginas();
void traducirDireccionLogica();

#endif
