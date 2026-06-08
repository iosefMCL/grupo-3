#include <iostream>
#include <string>

using namespace std;

struct Proceso {
    int id;
    string nombre;
    int prioridad;
    int memoriaMB;
};

struct NodoLista {
    Proceso info;
    NodoLista* siguiente;
};

void insertarProcesoLista(NodoLista* &cabeza, Proceso nuevoProceso) {
	
    NodoLista* nuevoNodo = new NodoLista();
    nuevoNodo->info = nuevoProceso;
    nuevoNodo->siguiente = NULL;

    if (cabeza == NULL) {
        cabeza = nuevoNodo;
    } else {
        NodoLista* temp = cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }
    cout << "Proceso registrado exitosamente" << endl;
}

NodoLista* buscarProcesoLista(NodoLista* cabeza, int idBusqueda) {
    NodoLista* temp = cabeza;
    while (temp != NULL) {
        if (temp->info.id == idBusqueda) {
            return temp;
        }
        temp = temp->siguiente;
    }
    return NULL;
}

void modificarPrioridadLista(NodoLista* cabeza, int idBusqueda, int nuevaPrioridad) {
    NodoLista* procesoEncontrado = buscarProcesoLista(cabeza, idBusqueda);
    if (procesoEncontrado != NULL) {
        procesoEncontrado->info.prioridad = nuevaPrioridad;
        cout << "Prioridad modificada a (" << nuevaPrioridad << ") para el proceso ID " << idBusqueda << endl;
    } else {
        cout << "Proceso con ID " << idBusqueda << " no encontrado" << endl;
    }
}

void eliminarProcesoLista(NodoLista* &cabeza, int idEliminar) {
    if (cabeza == NULL) {
        cout << "El gestor de procesos esta vacio" << endl;
        return;
    }

    NodoLista* temp = cabeza;
    NodoLista* anterior = NULL;

    if (temp != NULL && temp->info.id == idEliminar) {
        cabeza = temp->siguiente;
        delete temp;
        cout << "Proceso ID " << idEliminar << " eliminado" << endl;
        return;
    }

    while (temp != NULL && temp->info.id != idEliminar) {
        anterior = temp;
        temp = temp->siguiente;
    }

    if (temp == NULL) {
        cout << "El proceso con ID " << idEliminar << " no existe" << endl;
        return;
    }

    anterior->siguiente = temp->siguiente;
    delete temp;
    cout << "Proceso ID " << idEliminar << " eliminado" << endl;
}

void imprimirListaCompleta(NodoLista* cabeza) {
    if (cabeza == NULL) {
        cout << "No hay procesos registrados en el sistema" << endl;
        return;
    }
    cout << "===== REGISTRO GENERAL DE PROCESOS ====" << endl;
    NodoLista* temp = cabeza;
    while (temp != NULL) {
        cout << "[ID: " << temp->info.id 
             << " | Nombre: " << temp->info.nombre 
             << " | Prioridad: " << temp->info.prioridad 
             << " | Memoria: " << temp->info.memoriaMB << " MB]" << endl;
        temp = temp->siguiente;
    }
}

struct NodoCola {
    Proceso info;
    NodoCola* siguiente;
};

void encolarPorPrioridad(NodoCola* &frente, Proceso nuevoProceso) {
    NodoCola* nuevoNodo = new NodoCola();
    nuevoNodo->info = nuevoProceso;
    nuevoNodo->siguiente = NULL;

    if (frente == NULL || nuevoProceso.prioridad > frente->info.prioridad) {
        nuevoNodo->siguiente = frente;
        frente = nuevoNodo;
    } else {
        NodoCola* temp = frente;
        while (temp->siguiente != NULL && temp->siguiente->info.prioridad >= nuevoProceso.prioridad) {
            temp = temp->siguiente;
        }
        nuevoNodo->siguiente = temp->siguiente;
        temp->siguiente = nuevoNodo;
    }
    cout << " Proceso " << nuevoProceso.nombre << "' listo en la cola" << endl;
}

void desencolarEjecutarCPU(NodoCola* &frente) {
    if (frente == NULL) {
        cout << " Nada que ejecutar La cola esta vacia" << endl;
        return;
    }
    NodoCola* temp = frente;
    frente = frente->siguiente;

    cout << " ID: " << temp->info.id 
         << " (" << temp->info.nombre << ") | Prioridad: " << temp->info.prioridad << endl;
    
    delete temp;
}

void visualizarColaCPU(NodoCola* frente) {
    if (frente == NULL) {
        cout << "La cola de planificacion de la CPU esta vacia" << endl;
        return;
    }
    cout << "==== COLA DE ESPERA EN CPU (Por Prioridad) ====" << endl;
    NodoCola* temp = frente;
    int posicion = 1;
    while (temp != NULL) {
        cout << posicion << "o. [ID: " << temp->info.id 
             << " | Nombre: " << temp->info.nombre 
             << " | Prioridad: " << temp->info.prioridad << "]" << endl;
        temp = temp->siguiente;
        posicion++;
    }
}

struct NodoPila {
    int idBloque;
    int idProcesoAsignado;
    int tamanoMB;
    NodoPila* siguiendo;
    NodoPila* siguiente;
};

void asignarMemoriaPush(NodoPila* &tope, int &contadorBloques, int idProceso, int tamano) {
    contadorBloques++;
    NodoPila* nuevoNodo = new NodoPila();
    nuevoNodo->idBloque = contadorBloques;
    nuevoNodo->idProcesoAsignado = idProceso;
    nuevoNodo->tamanoMB = tamano;
    
    nuevoNodo->siguiente = tope;
    tope = nuevoNodo;

    cout << "Bloque " << contadorBloques << " de " << tamano 
         << "MB asignado al Proceso ID " << idProceso << endl;
}

void liberarMemoriaPop(NodoPila* &tope) {
    if (tope == NULL) {
        cout << " No hay bloques asignados que liberar" << endl;
        return;
    }
    NodoPila* temp = tope;
    tope = tope->siguiente;

    cout << " Liberando Bloque " << temp->idBloque 
         << " del Proceso ID " << temp->idProcesoAsignado << endl;
    
    delete temp;
}

void verificarEstadoMemoria(NodoPila* tope) {
    if (tope == NULL) {
        cout << "La memoria RAM no tiene bloques asignados" << endl;
        return;
    }
    cout << "==== ESTADO DE PILA DE MEMORIA RAM (LIFO) ====" << endl;
    NodoPila* temp = tope;
    int totalMemoriaOcupada = 0;
    while (temp != NULL) {
        cout << "[Bloque RAM " << temp->idBloque 
             << " | Asignado a PID: " << temp->idProcesoAsignado 
             << " | Espacio: " << temp->tamanoMB << " MB] " << endl;
        totalMemoriaOcupada += temp->tamanoMB;
        temp = temp->siguiente;
    }
    cout << "Total de memoria asignada: " << totalMemoriaOcupada << " MB" << endl;
}
