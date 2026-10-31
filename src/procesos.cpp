#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include "procesos.h"

using namespace std;


void crearProcesos()
{
    int numeroHijos;

    cout << "\n¿Cuantos procesos hijos desea crear? (1-10): ";
    cin >> numeroHijos;

    if (cin.fail() || numeroHijos < 1 || numeroHijos > 10)
    {
        cout << "Cantidad no valida. Ingrese un numero entre 1 y 10." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    for (int i = 0; i < numeroHijos; i++)
    {
        pid_t pid = fork();

        if (pid < 0){
            perror("Error al crear proceso hijo");
            break;
        }
        else if (pid == 0){
            cout << "\n[HIJO " << i + 1 << "] Mi PID es: " << getpid() << endl;
            cout << "[HIJO " << i + 1 << "] Mi PPID es: " << getppid() << endl;
            sleep(2);
            _exit(1); 
        }
        else{
            cout << "\n[PADRE] Se creo el hijo con PID: " << pid << endl;
            
            int estado;
            pid_t resultado = waitpid(pid, &estado, 0);
            
            if (resultado == -1){
                perror("Error al esperar al proceso hijo");
            }
            else if (WIFEXITED(estado)){
                cout << "[PADRE] El hijo " << resultado << " termino con codigo: " << WEXITSTATUS(estado) << endl;
            }
        }
    }
}

void menuProcesos()
{
    int opcion;

    do
    {
        cout << "\n===== Gestion de procesos =====" << endl;
        cout << "1. Probar creacion de un proceso hijo" << endl;
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

        switch (opcion){
            case 1:
                crearProcesos();
                break;

            case 2:
                cout << "\nVolviendo al menu principal..." << endl;
                break;

            default:
                cout << "\nOpcion no valida." << endl;
        }

    } while (opcion != 2);
}
