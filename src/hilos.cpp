
#include <iostream>
#include "hilos.h"

using namespace std;

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

        // Validacion de entrada
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
                cout << "\nProductor-consumidor en desarrollo..." << endl;
                break;

            case 2:
                cout << "\nVolviendo al menu principal..." << endl;
                break;

            default:
                cout << "\nOpcion no valida." << endl;
        }

    } while (opcion != 2);
}

