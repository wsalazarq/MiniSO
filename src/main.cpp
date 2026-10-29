#include <iostream>
#include <limits>
#include "procesos.h"

using namespace std;

void mostrarMenu()
{
    cout << "\n========== MiniSO ==========" << endl;
    cout << "1. Gestion de procesos" << endl;
    cout << "2. Gestion de hilos" << endl;
    cout << "3. Gestion de memoria" << endl;
    cout << "4. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main()
{
    int opcion;

    do
    {
        mostrarMenu();
        cin >> opcion;

        if (cin.fail())
        {
            cout << "\nError: debe ingresar un numero" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcion)
        {
            case 1:
                menuProcesos();
                break;

            case 2:
                cout << "\nModulo de hilos en desarrollo." << endl;
                break;

            case 3:
                cout << "\nModulo de memoria en desarrollo." << endl;
                break;

            case 4:
                cout << "\nCerrando MiniSO..." << endl;
                break;

            default:
                cout << "\nOpcion no valida. Intente nuevamente." << endl;
        }

    } while (opcion != 4);

    return 0;
}
