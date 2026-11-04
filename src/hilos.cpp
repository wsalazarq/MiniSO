
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "hilos.h"

using namespace std;

// Buffer circular compartido
const int MAX_BUFFER = 100;
const int MAX_HILOS = 10;
const int MAX_ELEMENTOS = 1000;

int buffer[MAX_BUFFER];

int in = 0;
int out = 0;
int ocupados = 0;

int capacidad = 0;
int totalElementos = 0;
int siguienteElemento = 1;
int totalConsumidos = 0;

// Comprobar elementos perdidos o duplicados
bool consumido[MAX_ELEMENTOS + 1];
int duplicados = 0;
int invalidos = 0;
bool detener = false;

// Mecanismos de sincronizacion
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hayEspacio = PTHREAD_COND_INITIALIZER;
pthread_cond_t hayElementos = PTHREAD_COND_INITIALIZER;


// Funcion del productor
void* productor(void* arg)
{
    int id = *(int*)arg;

    while (1)
    {
        // Simular tiempo de produccion
        usleep(100000);

        pthread_mutex_lock(&mutex);

        // Esperar si el buffer esta lleno
        while (ocupados == capacidad &&
               siguienteElemento <= totalElementos &&
               !detener)
        {
            cout << "Productor " << id
                 << " esperando: buffer lleno" << endl;

            pthread_cond_wait(&hayEspacio, &mutex);
        }

        // Verificar si debe finalizar
        if (detener || siguienteElemento > totalElementos)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Producir e insertar elemento
        int elemento = siguienteElemento++;

        buffer[in] = elemento;
        in = (in + 1) % capacidad;
        ocupados++;

        cout << "Productor " << id
             << " produjo elemento " << elemento << endl;

        pthread_cond_signal(&hayElementos);

        // Despertar productores si termino la produccion
        if (siguienteElemento > totalElementos)
        {
            pthread_cond_broadcast(&hayEspacio);
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    cout << "Productor " << id << " finalizo" << endl;
    pthread_mutex_unlock(&mutex);

    return NULL;
}


// Funcion del consumidor
void* consumidor(void* arg)
{
    int id = *(int*)arg;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        // Esperar si el buffer esta vacio
        while (ocupados == 0 &&
               totalConsumidos < totalElementos &&
               !detener)
        {
            cout << "Consumidor " << id
                 << " esperando: buffer vacio" << endl;

            pthread_cond_wait(&hayElementos, &mutex);
        }

        // Verificar si debe finalizar
        if (detener ||
            (ocupados == 0 && totalConsumidos == totalElementos))
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Retirar elemento del buffer
        int elemento = buffer[out];

        out = (out + 1) % capacidad;
        ocupados--;
        totalConsumidos++;

        // Verificar el elemento consumido
        if (elemento >= 1 && elemento <= totalElementos)
        {
            if (consumido[elemento])
            {
                duplicados++;
            }
            else
            {
                consumido[elemento] = true;
            }
        }
        else
        {
            invalidos++;
        }

        cout << "Consumidor " << id
             << " consumio elemento " << elemento << endl;

        pthread_cond_signal(&hayEspacio);

        // Despertar consumidores cuando todo termine
        if (totalConsumidos == totalElementos)
        {
            pthread_cond_broadcast(&hayElementos);
        }

        pthread_mutex_unlock(&mutex);

        // Simular tiempo de consumo
        usleep(200000);
    }

    pthread_mutex_lock(&mutex);
    cout << "Consumidor " << id << " finalizo" << endl;
    pthread_mutex_unlock(&mutex);

    return NULL;
}


// Configuracion y ejecucion de los hilos
void ejecutarProductorConsumidor()
{
    int numProductores;
    int numConsumidores;

    cout << "\nIngrese la cantidad de productores (2-10): ";
    cin >> numProductores;

    if (cin.fail() || numProductores < 2 || numProductores > MAX_HILOS)
    {
        cout << "Error: ingrese entre 2 y 10 productores." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    cout << "Ingrese la cantidad de consumidores (2-10): ";
    cin >> numConsumidores;

    if (cin.fail() || numConsumidores < 2 || numConsumidores > MAX_HILOS)
    {
        cout << "Error: ingrese entre 2 y 10 consumidores." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    cout << "Ingrese la capacidad del buffer (1-100): ";
    cin >> capacidad;

    if (cin.fail() || capacidad < 1 || capacidad > MAX_BUFFER)
    {
        cout << "Error: ingrese una capacidad entre 1 y 100." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    cout << "Ingrese la cantidad total de elementos (1-1000): ";
    cin >> totalElementos;

    if (cin.fail() || totalElementos < 1 ||
        totalElementos > MAX_ELEMENTOS)
    {
        cout << "Error: ingrese una cantidad entre 1 y 1000." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    // Reiniciar variables para cada simulacion
    in = 0;
    out = 0;
    ocupados = 0;
    siguienteElemento = 1;
    totalConsumidos = 0;
    duplicados = 0;
    invalidos = 0;
    detener = false;

    for (int i = 1; i <= totalElementos; i++)
    {
        consumido[i] = false;
    }

    cout << "\n===== CONFIGURACION =====" << endl;
    cout << "Productores: " << numProductores << endl;
    cout << "Consumidores: " << numConsumidores << endl;
    cout << "Capacidad del buffer: " << capacidad << endl;
    cout << "Total de elementos: " << totalElementos << endl;

    // Declarar hilos e identificadores
    pthread_t productores[MAX_HILOS];
    pthread_t consumidores[MAX_HILOS];

    int idProductores[MAX_HILOS];
    int idConsumidores[MAX_HILOS];

    int productoresCreados = 0;
    int consumidoresCreados = 0;
    bool errorCreacion = false;

    // Crear los consumidores
    for (int i = 0; i < numConsumidores; i++)
    {
        idConsumidores[i] = i + 1;

        if (pthread_create(&consumidores[i], NULL,
                           consumidor, &idConsumidores[i]) != 0)
        {
            cout << "Error al crear consumidor " << i + 1 << endl;
            errorCreacion = true;
            break;
        }

        consumidoresCreados++;
    }

    // Crear los productores
    if (!errorCreacion)
    {
        for (int i = 0; i < numProductores; i++)
        {
            idProductores[i] = i + 1;

            if (pthread_create(&productores[i], NULL,
                               productor, &idProductores[i]) != 0)
            {
                cout << "Error al crear productor " << i + 1 << endl;
                errorCreacion = true;
                break;
            }

            productoresCreados++;
        }
    }

    // Detener todos los hilos si hubo un error
    if (errorCreacion)
    {
        pthread_mutex_lock(&mutex);

        detener = true;
        pthread_cond_broadcast(&hayEspacio);
        pthread_cond_broadcast(&hayElementos);

        pthread_mutex_unlock(&mutex);
    }

    // Esperar la finalizacion de los productores
    for (int i = 0; i < productoresCreados; i++)
    {
        pthread_join(productores[i], NULL);
    }

    // Esperar la finalizacion de los consumidores
    for (int i = 0; i < consumidoresCreados; i++)
    {
        pthread_join(consumidores[i], NULL);
    }

    if (errorCreacion)
    {
        cout << "\nSimulacion cancelada por error al crear hilos." << endl;
        return;
    }

    // Verificar elementos faltantes
    int faltantes = 0;

    for (int i = 1; i <= totalElementos; i++)
    {
        if (!consumido[i])
        {
            faltantes++;
        }
    }

    // Mostrar resultados
    cout << "\n===== RESULTADOS =====" << endl;
    cout << "Elementos producidos: " << siguienteElemento - 1 << endl;
    cout << "Elementos consumidos: " << totalConsumidos << endl;
    cout << "Elementos faltantes: " << faltantes << endl;
    cout << "Elementos duplicados: " << duplicados << endl;
    cout << "Elementos invalidos: " << invalidos << endl;

    if (siguienteElemento - 1 == totalElementos &&
        totalConsumidos == totalElementos &&
        faltantes == 0 &&
        duplicados == 0 &&
        invalidos == 0 &&
        ocupados == 0)
    {
        cout << "\nSimulacion correcta: todos los elementos "
             << "fueron consumidos una sola vez." << endl;
    }
    else
    {
        cout << "\nError: la produccion y el consumo "
             << "no coinciden." << endl;
    }
}


// Submenu de hilos
void menuHilos()
{
    int opcion = 0;

    do
    {
        cout << "\n===== Gestion de hilos y concurrencia =====" << endl;
        cout << "1. Ejecutar productor-consumidor" << endl;
        cout << "2. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        if (cin.fail())
        {
            if (cin.eof())
                return;

            cout << "\nError: debe ingresar un numero." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion)
        {
            case 1:
                ejecutarProductorConsumidor();
                break;

            case 2:
                cout << "\nVolviendo al menu principal..." << endl;
                break;

            default:
                cout << "\nOpcion no valida." << endl;
        }

    } while (opcion != 2);
}
