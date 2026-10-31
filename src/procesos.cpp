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

    if (cin.fail() || numeroHijos < 1 || numeroHijos > 10){
        cout << "Cantidad no valida. Ingrese un numero entre 1 y 10." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    int opcionPrograma;

    cout << "\n===== PROGRAMAS DISPONIBLES =====" << endl;
    cout << "1. Mostrar un mensaje (echo)" << endl;
    cout << "2. Listar archivos y directorios (ls)" << endl;
    cout << "3. Mostrar fecha y hora (date)" << endl;
    cout << "4. Esperar 5 segundos (sleep)" << endl;
    cout << "5. Mostrar informacion del sistema (uname)" << endl;
    cout << "6. Mostrar usuario actual (whoami)" << endl;
    cout << "7. Mostrar uso de memoria RAM (free)" << endl;
    cout << "8. Mostrar ruta del directorio actual (pwd)" << endl;

    cout << "\nSeleccione el programa: ";
    cin >> opcionPrograma;

    if (cin.fail() || opcionPrograma < 1 || opcionPrograma > 8){
        cout << "Opcion no valida." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    pid_t hijos[10];
    int cantidad = 0;

    cout << "\n[PADRE] Mi PID es: " << getpid() << endl;
    // Creacion de los hijos
    for (int i = 0; i < numeroHijos; i++){
        pid_t pid = fork();

        if (pid < 0){
            perror("Error al crear proceso hijo");
            break;
        }
        else if (pid == 0){
            cout << "\n[HIJO " << i + 1 << "] Mi PID es: " << getpid() << endl;
            cout << "[HIJO " << i + 1 << "] Mi PPID es: " << getppid() << endl;
            cout << "[HIJO " << i + 1 << "] Ejecutando el programa seleccionado..." << endl;

            switch (opcionPrograma)
            {
                case 1:
                    execl("/bin/echo", "echo",
                          "Hola desde un proceso hijo", (char *)NULL);
                    break;

                case 2:
                    execl("/bin/ls", "ls", "-l", (char *)NULL);
                    break;

                case 3:
                    execl("/bin/date", "date", (char *)NULL);
                    break;

                case 4:
                    execl("/bin/sleep", "sleep", "5", (char *)NULL);
                    break;

                case 5:
                    execl("/bin/uname", "uname", "-a", (char *)NULL);
                    break;

                case 6:
                    execl("/usr/bin/whoami", "whoami", (char *)NULL);
                    break;

                case 7:
                    execl("/usr/bin/free", "free", "-h", (char *)NULL);
                    break;

                case 8:
                    execl("/bin/pwd", "pwd", (char *)NULL);
                    break;
            }

            perror("Error al ejecutar exec");
            _exit(1);
        }
        else{
            // El padre guarda el PID del hijo creado
            hijos[cantidad] = pid;
            cantidad++;

            cout << "\n[PADRE] Se creo el hijo con PID: " << pid << endl;
        }
    }
    
    cout << "\n[PADRE] Esperando la finalizacion de los hijos..." << endl;

    for (int i = 0; i < cantidad; i++){
        int estado;
        pid_t resultado = waitpid(hijos[i], &estado, 0);

        if (resultado == -1){
            perror("Error al esperar al proceso hijo");
        }
        else if (WIFEXITED(estado)){
            cout << "[PADRE] El hijo " << resultado << " termino con codigo: " << WEXITSTATUS(estado) << endl;
        }
        else if (WIFSIGNALED(estado)){
            cout << "[PADRE] El hijo " << resultado << " termino por una senal: " << WTERMSIG(estado) << endl;
        }
    }

    cout << "\n[PADRE] Todos los hijos finalizaron." << endl;
}

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
