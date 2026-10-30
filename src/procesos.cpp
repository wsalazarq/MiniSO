#include <iostream>
#include "procesos.h"

using namespace std;

void menuProcesos()
{
    int opcion;

    do
    {
        cout << "\n===== Gestion de procesos =====" << endl;
        cout << "1. Crear y ejecutar procesos hijos" << endl;
        cout << "2. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        if (cin.fail())
        {
            cout << "\nError: debe ingresar un numero." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0;
            continue;
        }

        switch (opcion)
        {
            case 1:
                cout << "\nEn proceso de implementacion..." << endl;
                break;

            case 2:
                cout << "\nVolviendo al menu principal..." << endl;
                break;

            default:
                cout << "\nOpcion no valida" << endl;
        }

    } while (opcion != 2);
}
