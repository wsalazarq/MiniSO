
#include <iostream>
#include <vector>
#include <limits>
#include "memoria.h"

using namespace std;

struct BloqueMemoria
{
    int inicio;
    int tamano;
    bool ocupado;
    int idProceso;
};

vector<BloqueMemoria> memoria;

bool memoriaInicializada = false;

int siguienteIdProceso = 1;


struct EntradaPagina
{
    int numeroMarco;
    bool presente;
};

struct ProcesoPaginado
{
    int idProceso;
    int cantidadPaginas;
    int tamanoProceso;
    vector<EntradaPagina> tablaPaginas;
};

int tamanoPagina = 0;
int cantidadMarcos = 0;

vector<bool> marcosOcupados;

vector<ProcesoPaginado> procesosPaginados;

bool paginacionInicializada = false;

int siguienteIdPaginado = 1;


void inicializarMemoria()
{
    int tamanoTotal;

    cout << "\n========== INICIALIZAR MEMORIA ==========" << endl;
    cout << "Ingrese el tamano total de memoria (en KB): ";
    cin >> tamanoTotal;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (tamanoTotal <= 0)
    {
        cout << "\nError: el tamano debe ser mayor que cero." << endl;
        return;
    }

    memoria.clear();

    BloqueMemoria bloqueInicial;

    bloqueInicial.inicio = 0;
    bloqueInicial.tamano = tamanoTotal;
    bloqueInicial.ocupado = false;
    bloqueInicial.idProceso = -1;

    memoria.push_back(bloqueInicial);

    memoriaInicializada = true;
    siguienteIdProceso = 1;

    cout << "\nMemoria inicializada correctamente." << endl;
    cout << "Tamano total: " << tamanoTotal << " KB" << endl;
    cout << "Memoria disponible: " << tamanoTotal << " KB" << endl;
}


void solicitarBloque()
{
    if (!memoriaInicializada)
    {
        cout << "\nError: primero debe inicializar la memoria." << endl;
        return;
    }

    int tamanoSolicitado;

    cout << "\n========== SOLICITAR BLOQUE ==========" << endl;
    cout << "Ingrese el tamano de memoria requerido (en KB): ";
    cin >> tamanoSolicitado;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (tamanoSolicitado <= 0)
    {
        cout << "\nError: el tamano debe ser mayor que cero." << endl;
        return;
    }

    for (size_t i = 0; i < memoria.size(); i++)
    {
        if (!memoria[i].ocupado && memoria[i].tamano >= tamanoSolicitado)
        {
            int id = siguienteIdProceso++;

            BloqueMemoria nuevoBloque;

            nuevoBloque.inicio = memoria[i].inicio;
            nuevoBloque.tamano = tamanoSolicitado;
            nuevoBloque.ocupado = true;
            nuevoBloque.idProceso = id;

            memoria[i].inicio += tamanoSolicitado;
            memoria[i].tamano -= tamanoSolicitado;

            memoria.insert(memoria.begin() + i, nuevoBloque);

            cout << "\nMemoria asignada correctamente." << endl;
            cout << "ID del proceso: " << id << endl;
            cout << "Memoria asignada: " << tamanoSolicitado << " KB" << endl;
            return;
        }
    }

    cout << "\nNo se encontro un bloque libre suficientemente grande." << endl;
}


void liberarBloque()
{
    if (!memoriaInicializada)
    {
        cout << "\nError: primero debe inicializar la memoria." << endl;
        return;
    }

    int idProceso;

    cout << "\n========== LIBERAR BLOQUE ==========" << endl;
    cout << "Ingrese el ID del proceso que desea liberar: ";
    cin >> idProceso;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    for (size_t i = 0; i < memoria.size(); i++)
    {
        if (memoria[i].ocupado && memoria[i].idProceso == idProceso)
        {
            memoria[i].ocupado = false;
            memoria[i].idProceso = -1;

            cout << "\nMemoria liberada correctamente." << endl;
            cout << "Se liberaron " << memoria[i].tamano << " KB." << endl;

            unirBloquesLibres();
            return;
        }
    }

    cout << "\nError: no existe un proceso con ese ID." << endl;
}


void unirBloquesLibres()
{
    if (!memoriaInicializada)
    {
        return;
    }

    int i = 0;

    while (i < (int)memoria.size() - 1)
    {
        if (!memoria[i].ocupado && !memoria[i + 1].ocupado)
        {
            memoria[i].tamano += memoria[i + 1].tamano;

            memoria.erase(memoria.begin() + i + 1);
        }
        else
        {
            i++;
        }
    }
}

void mostrarEstadoMemoria()
{
    if (!memoriaInicializada)
    {
        cout << "\nError: primero debe inicializar la memoria." << endl;
        return;
    }

    cout << "\n========== ESTADO DE LA MEMORIA ==========" << endl;

    cout << "\n";
    cout << "ID\tInicio\tTamano\tEstado" << endl;
    cout << "------------------------------------------" << endl;

    int memoriaLibre = 0;
    int memoriaOcupada = 0;

    for (int i = 0; i < (int)memoria.size(); i++)
    {
        cout << memoria[i].idProceso << "\t"
             << memoria[i].inicio << " KB\t"
             << memoria[i].tamano << " KB\t";

        if (memoria[i].ocupado)
        {
            cout << "Ocupado" << endl;
            memoriaOcupada += memoria[i].tamano;
        }
        else
        {
            cout << "Libre" << endl;
            memoriaLibre += memoria[i].tamano;
        }
    }

    cout << "\n------------------------------------------" << endl;
    cout << "Memoria ocupada: " << memoriaOcupada << " KB" << endl;
    cout << "Memoria libre: " << memoriaLibre << " KB" << endl;
    cout << "Total: " << memoriaOcupada + memoriaLibre << " KB" << endl;
}


void mostrarFragmentacionExterna()
{
    if (!memoriaInicializada)
    {
        cout << "\nError: primero debe inicializar la memoria." << endl;
        return;
    }

    int memoriaLibre = 0;
    int bloqueLibreMayor = 0;
    int cantidadBloquesLibres = 0;

    for (int i = 0; i < (int)memoria.size(); i++)
    {
        if (!memoria[i].ocupado)
        {
            memoriaLibre += memoria[i].tamano;
            cantidadBloquesLibres++;

            if (memoria[i].tamano > bloqueLibreMayor)
            {
                bloqueLibreMayor = memoria[i].tamano;
            }
        }
    }

    cout << "\n========== FRAGMENTACION EXTERNA ==========" << endl;

    cout << "Memoria libre total: " << memoriaLibre << " KB" << endl;
    cout << "Cantidad de bloques libres: " << cantidadBloquesLibres << endl;
    cout << "Bloque libre mas grande: " << bloqueLibreMayor << " KB" << endl;

    if (cantidadBloquesLibres > 1)
    {
        cout << "\nExiste fragmentacion externa." << endl;
        cout << "La memoria libre se encuentra dividida en varios bloques." << endl;
    }
    else
    {
        cout << "\nNo se observa fragmentacion externa." << endl;
    }
}

void menuMemoria()
{
    int opcion;

    do
    {
        cout << "\n========== GESTION DE MEMORIA ==========" << endl;
        cout << "1. Particionamiento de memoria" << endl;
        cout << "2. Paginacion de memoria" << endl;
        cout << "3. Regresar al menu principal" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        if (cin.fail())
        {
            cout << "\nError: debe ingresar un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = 0;
            continue;
        }

        switch (opcion)
        {
            case 1:
                menuParticionamiento();
                break;

            case 2:
                menuPaginacion();
                break;

            case 3:
                cout << "\nRegresando al menu principal..." << endl;
                break;

            default:
                cout << "\nOpcion no valida. Intente nuevamente." << endl;
        }

    } while (opcion != 3);
}

void menuParticionamiento()
{
    int opcion;

    do
    {
        cout << "\n========== PARTICIONAMIENTO DE MEMORIA ==========" << endl;
        cout << "1. Inicializar memoria" << endl;
        cout << "2. Solicitar bloque de memoria (First-Fit)" << endl;
        cout << "3. Liberar bloque de memoria" << endl;
        cout << "4. Mostrar estado de la memoria" << endl;
        cout << "5. Mostrar fragmentacion externa" << endl;
        cout << "6. Regresar al menu de memoria" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        if (cin.fail())
        {
            cout << "\nError: debe ingresar un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = 0;
            continue;
        }

        switch (opcion)
        {
            case 1:
                inicializarMemoria();
                break;

            case 2:
                solicitarBloque();
                break;

            case 3:
                liberarBloque();
                break;

            case 4:
                mostrarEstadoMemoria();
                break;

            case 5:
                mostrarFragmentacionExterna();
                break;

            case 6:
                cout << "\nRegresando al menu de memoria..." << endl;
                break;

            default:
                cout << "\nOpcion no valida. Intente nuevamente." << endl;
        }

    } while (opcion != 6);
}

void menuPaginacion()
{
    int opcion;

    do
    {
        cout << "\n========== PAGINACION DE MEMORIA ==========" << endl;
        cout << "1. Configurar paginacion" << endl;
        cout << "2. Asignar paginas a un proceso" << endl;
        cout << "3. Mostrar tabla de paginas" << endl;
        cout << "4. Traducir direccion logica a fisica" << endl;
        cout << "5. Regresar al menu de memoria" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        if (cin.fail())
        {
            cout << "\nError: debe ingresar un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = 0;
            continue;
        }

        switch (opcion)
        {
            case 1:
                configurarPaginacion();
                break;

            case 2:
                asignarPaginas();
                break;

            case 3:
                mostrarTablaPaginas();
                break;

            case 4:
                traducirDireccionLogica();
                break;

            case 5:
                cout << "\nRegresando al menu de memoria..." << endl;
                break;

            default:
                cout << "\nOpcion no valida. Intente nuevamente." << endl;
        }

    } while (opcion != 5);
}


void configurarPaginacion()
{
    int nuevoTamanoPagina;
    int nuevaCantidadMarcos;

    cout << "\n========== CONFIGURAR PAGINACION ==========" << endl;

    cout << "Ingrese el tamano de cada pagina (en KB): ";
    cin >> nuevoTamanoPagina;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (nuevoTamanoPagina <= 0)
    {
        cout << "\nError: el tamano de pagina debe ser mayor que cero." << endl;
        return;
    }

    cout << "Ingrese la cantidad de marcos fisicos: ";
    cin >> nuevaCantidadMarcos;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (nuevaCantidadMarcos <= 0)
    {
        cout << "\nError: la cantidad de marcos debe ser mayor que cero." << endl;
        return;
    }

    tamanoPagina = nuevoTamanoPagina;
    cantidadMarcos = nuevaCantidadMarcos;

    marcosOcupados.assign(cantidadMarcos, false);

    procesosPaginados.clear();

    siguienteIdPaginado = 1;

    paginacionInicializada = true;

    cout << "\nPaginacion configurada correctamente." << endl;
    cout << "Tamano de pagina: " << tamanoPagina << " KB" << endl;
    cout << "Cantidad de marcos: " << cantidadMarcos << endl;
    cout << "Memoria fisica total: "
         << tamanoPagina * cantidadMarcos << " KB" << endl;
}


void asignarPaginas()
{
    if (!paginacionInicializada)
    {
        cout << "\nError: primero debe configurar la paginacion." << endl;
        return;
    }

    int tamanoProceso;

    cout << "\n========== ASIGNAR PAGINAS ==========" << endl;
    cout << "Ingrese el tamano del proceso (en KB): ";
    cin >> tamanoProceso;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (tamanoProceso <= 0)
    {
        cout << "\nError: el tamano debe ser mayor que cero." << endl;
        return;
    }

    int cantidadPaginas =
        (tamanoProceso + tamanoPagina - 1) / tamanoPagina;

    int marcosDisponibles = 0;

    for (int i = 0; i < cantidadMarcos; i++)
    {
        if (!marcosOcupados[i])
        {
            marcosDisponibles++;
        }
    }

    if (cantidadPaginas > marcosDisponibles)
    {
        cout << "\nError: no hay suficientes marcos disponibles." << endl;
        cout << "Paginas necesarias: " << cantidadPaginas << endl;
        cout << "Marcos disponibles: " << marcosDisponibles << endl;
        return;
    }

    ProcesoPaginado nuevoProceso;

    nuevoProceso.idProceso = siguienteIdPaginado++;
    nuevoProceso.cantidadPaginas = cantidadPaginas;
    nuevoProceso.tamanoProceso = tamanoProceso;

    for (int pagina = 0; pagina < cantidadPaginas; pagina++)
    {
        EntradaPagina entrada;

        entrada.numeroMarco = -1;
        entrada.presente = false;

        for (int marco = 0; marco < cantidadMarcos; marco++)
        {
            if (!marcosOcupados[marco])
            {
                marcosOcupados[marco] = true;

                entrada.numeroMarco = marco;
                entrada.presente = true;

                break;
            }
        }

        nuevoProceso.tablaPaginas.push_back(entrada);
    }

    procesosPaginados.push_back(nuevoProceso);

    cout << "\nProceso asignado correctamente." << endl;
    cout << "ID del proceso: " << nuevoProceso.idProceso << endl;
    cout << "Tamano del proceso: " << tamanoProceso << " KB" << endl;
    cout << "Cantidad de paginas: " << cantidadPaginas << endl;
}


void mostrarTablaPaginas()
{
    if (!paginacionInicializada)
    {
        cout << "\nError: primero debe configurar la paginacion." << endl;
        return;
    }

    cout << "\n========== TABLA DE PAGINAS ==========" << endl;

    if (procesosPaginados.empty())
    {
        cout << "\nNo existen procesos asignados." << endl;
        return;
    }

    for (int i = 0; i < (int)procesosPaginados.size(); i++)
    {
        cout << "\nProceso ID: "
             << procesosPaginados[i].idProceso << endl;

        cout << "Pagina\tMarco\tEstado" << endl;
        cout << "-----------------------------" << endl;

        for (int j = 0;
             j < procesosPaginados[i].cantidadPaginas; j++)
        {
            cout << j << "\t"
                 << procesosPaginados[i].tablaPaginas[j].numeroMarco
                 << "\t";

            if (procesosPaginados[i].tablaPaginas[j].presente)
            {
                cout << "Presente" << endl;
            }
            else
            {
                cout << "No presente" << endl;
            }
        }
    }

    cout << "\n========== ESTADO DE MARCOS FISICOS ==========" << endl;

    cout << "Marco\tEstado" << endl;
    cout << "----------------" << endl;

    for (int i = 0; i < cantidadMarcos; i++)
    {
        cout << i << "\t";

        if (marcosOcupados[i])
        {
            cout << "Ocupado" << endl;
        }
        else
        {
            cout << "Libre" << endl;
        }
    }
}


void traducirDireccionLogica()
{
    if (!paginacionInicializada)
    {
        cout << "\nError: primero debe configurar la paginacion." << endl;
        return;
    }

    int idProceso;
    int direccionLogica;

    cout << "\n========== TRADUCCION DE DIRECCIONES ==========" << endl;

    cout << "Ingrese el ID del proceso: ";
    cin >> idProceso;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int indiceProceso = -1;

    for (int i = 0; i < (int)procesosPaginados.size(); i++)
    {
        if (procesosPaginados[i].idProceso == idProceso)
        {
            indiceProceso = i;
            break;
        }
    }

    if (indiceProceso == -1)
    {
        cout << "\nError: no existe ese proceso." << endl;
        return;
    }

    cout << "Ingrese la direccion logica (en bytes): ";
    cin >> direccionLogica;

    if (cin.fail())
    {
        cout << "\nError: debe ingresar un numero." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (direccionLogica < 0 ||
        direccionLogica >= procesosPaginados[indiceProceso].tamanoProceso * 1024)
    {
        cout << "\nError: direccion logica fuera de los limites del proceso." << endl;
        return;
    }

    int tamanoPaginaBytes = tamanoPagina * 1024;

    int numeroPagina = direccionLogica / tamanoPaginaBytes;
    int desplazamiento = direccionLogica % tamanoPaginaBytes;

    EntradaPagina entrada =
        procesosPaginados[indiceProceso].tablaPaginas[numeroPagina];

    if (!entrada.presente)
    {
        cout << "\nError: la pagina no esta asignada a un marco." << endl;
        return;
    }

    int direccionFisica =
        entrada.numeroMarco * tamanoPaginaBytes + desplazamiento;

    cout << "\n========== RESULTADO DE LA TRADUCCION ==========" << endl;
    cout << "Proceso: " << idProceso << endl;
    cout << "Direccion logica: " << direccionLogica << " bytes" << endl;
    cout << "Numero de pagina: " << numeroPagina << endl;
    cout << "Desplazamiento: " << desplazamiento << " bytes" << endl;
    cout << "Marco fisico: " << entrada.numeroMarco << endl;
    cout << "Direccion fisica: " << direccionFisica << " bytes" << endl;
}
