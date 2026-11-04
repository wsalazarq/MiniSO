# MiniSO

Proyecto desarrollado para el curso de Sistemas Operativos, utilizando C++ en Linux.

## Módulos

### 1. Gestión de procesos
- Creación de procesos hijos mediante `fork()`.
- Ejecución de programas externos utilizando `exec()`.
- Obtención de PID y PPID.
- Espera y finalización de procesos mediante `waitpid()`.
- Visualización de procesos utilizando `ps` y `pstree`.

### 2. Gestión de hilos y concurrencia
- Creación de múltiples hilos utilizando POSIX Threads (`pthread`).
- Implementación del problema productor-consumidor.
- Configuración de productores, consumidores, capacidad del búfer y cantidad de elementos.
- Sincronización mediante mutex y variables de condición.
- Verificación de elementos producidos y consumidos.

### 3. Gestión de memoria
- Simulación de particionamiento mediante First-Fit.
- Asignación y liberación de bloques.
- Visualización del estado de la memoria y fragmentación externa.
- Simulación de paginación y tablas de páginas.
- Traducción de direcciones lógicas a físicas.

## Requisitos

- Sistema operativo Linux (Ubuntu).
- Compilador `g++`.
- Soporte para POSIX Threads.

## Compilación

Desde la carpeta principal del proyecto:

```bash
g++ -std=c++17 -pthread src/main.cpp src/procesos.cpp src/hilos.cpp src/memoria.cpp -o MiniSO
```

## Ejecución

```bash
./MiniSO
```

Al iniciar, se muestra un menú principal desde el cual se puede acceder a los tres módulos.

Cada módulo incluye opciones para ejecutar sus funcionalidades y regresar al menú principal.

## Estructura del proyecto

```text
MiniSO/
├── README.md
├── src/
│   ├── main.cpp
│   ├── procesos.cpp
│   ├── procesos.h
│   ├── hilos.cpp
│   ├── hilos.h
│   ├── memoria.cpp
│   └── memoria.h
```

## Desarrollo

El proyecto se desarrolla de manera colaborativa utilizando Git y GitHub, mediante ramas de trabajo y commits que registran las distintas etapas de implementación, integración y validación.