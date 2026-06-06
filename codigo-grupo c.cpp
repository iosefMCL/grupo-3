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
