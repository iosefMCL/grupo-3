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

int main() {
    NodoLista* gestorProcesos = NULL;
    NodoCola* planificadorCPU = NULL;
    NodoPila* pilaMemoria = NULL;
    
    int contadorBloquesRAM = 0;
    int opcionPrincipal, subOpcion;

    do {
        cout << "" << endl;
        
        cout << " ===== SISTEMA DE GESTION DE PROCESOS  =====" << endl;
        cout << " 1- GESTOR DE PROCESOS (Lista Enlazada)" << endl;
        cout << " 2- PLANIFICADOR DE CPU (Cola de Prioridad)" << endl;
        cout << " 3- GESTOR DE MEMORIA RAM (Pila)" << endl;
        cout << " 4- SALIR DEL SIMULADOR" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcionPrincipal;
        
        cout << "" << endl;

        switch (opcionPrincipal) {
            case 1:
                do {
                    cout << "==== SUBMENU: GESTOR DE PROCESOS ====" << endl;
                    cout << "1. Insertar nuevo proceso" << endl;
                    cout << "2. Eliminar proceso por ID" << endl;
                    cout << "3. Buscar proceso por ID" << endl;
                    cout << "4. Modificar prioridad de un proceso" << endl;
                    cout << "5. Mostrar todos los procesos" << endl;
                    cout << "6. Volver al menu principal" << endl;
                    cout << "Seleccione: ";
                    cin >> subOpcion;

                    if (subOpcion == 1) {
                        Proceso p;
                        cout << "Ingrese ID del Proceso (entero): "; cin >> p.id;
                        cout << "Ingrese Nombre del Proceso: "; cin.ignore(); getline(cin, p.nombre);
                        cout << "Ingrese Prioridad (Numeros altos = Mas prioridad): "; cin >> p.prioridad;
                        cout << "Ingrese Consumo de Memoria (MB): "; cin >> p.memoriaMB;
                        insertarProcesoLista(gestorProcesos, p);
                    } 
                    else if (subOpcion == 2) {
                        int id;
                        cout << "Ingrese el ID del proceso a eliminar: "; cin >> id;
                        eliminarProcesoLista(gestorProcesos, id);
                    } 
                    else if (subOpcion == 3) {
                        int id;
                        cout << "Ingrese el ID a buscar: "; cin >> id;
                        NodoLista* buscado = buscarProcesoLista(gestorProcesos, id);
                        if (buscado != NULL) {
                            cout << "--> Encontrado: [" << buscado->info.nombre 
                                 << ", Prioridad: " << buscado->info.prioridad 
                                 << ", RAM: " << buscado->info.memoriaMB << "MB]" << endl;
                        } else {
                            cout << "--> Proceso no registrado" << endl;
                        }
                    } 
                    else if (subOpcion == 4) {
                        int id, nuevaPrio;
                        cout << "Ingrese el ID del proceso: "; cin >> id;
                        cout << "Ingrese la nueva prioridad: "; cin >> nuevaPrio;
                        modificarPrioridadLista(gestorProcesos, id, nuevaPrio);
                    } 
                    else if (subOpcion == 5) {
                        imprimirListaCompleta(gestorProcesos);
                    }
                } while (subOpcion != 6);
                break;

            case 2:
                do {
                    cout << "==== SUBMENU: PLANIFICADOR DE CPU ====" << endl;
                    cout << "1. Encolar proceso desde el Registro General" << endl;
                    cout << "2. Ejecutar proceso en CPU (Desencolar)" << endl;
                    cout << "3. Visualizar estado de la cola actual" << endl;
                    cout << "4. Volver al menu principal" << endl;
                    cout << "Seleccione: ";
                    cin >> subOpcion;

                    if (subOpcion == 1) {
                        int id;
                        cout << "Ingrese el ID del proceso registrado: "; cin >> id;
                        NodoLista* nodoReg = buscarProcesoLista(gestorProcesos, id);
                        if (nodoReg != NULL) {
                            encolarPorPrioridad(planificadorCPU, nodoReg->info);
                        } else {
                            cout << " Ese proceso no existe en el registro general de la lista.\n";
                        }
                    } 
                    else if (subOpcion == 2) {
                        desencolarEjecutarCPU(planificadorCPU);
                    } 
                    else if (subOpcion == 3) {
                        visualizarColaCPU(planificadorCPU);
                    }
                } while (subOpcion != 4);
                break;

            case 3:
                do {
                    cout << "==== SUBMENU: GESTOR DE MEMORIA RAM ====" << endl;
                    cout << "1. Asignar bloque de memoria a proceso (Push)" << endl;
                    cout << "2. Liberar ultimo bloque de memoria asignado (Pop)" << endl;
                    cout << "3. Verificar estado actual de la memoria RAM" << endl;
                    cout << "4. Volver al menu principal" << endl;
                    cout << "Seleccione: ";
                    cin >> subOpcion;

                    if (subOpcion == 1) {
                        int id;
                        cout << "Ingrese el ID del proceso: "; cin >> id;
                        NodoLista* nodoReg = buscarProcesoLista(gestorProcesos, id);
                        if (nodoReg != NULL) {
                            asignarMemoriaPush(pilaMemoria, contadorBloquesRAM, nodoReg->info.id, nodoReg->info.memoriaMB);
                        } else {
                            cout << " Error el proceso debe existir en el registro general" << endl;
                        }
                    } 
                    else if (subOpcion == 2) {
                        liberarMemoriaPop(pilaMemoria);
                    } 
                    else if (subOpcion == 3) {
                        verificarEstadoMemoria(pilaMemoria);
                    }
                } while (subOpcion != 4);
                break;

            case 4:
                cout << "Saliendo..." << endl;
                break;

            default:
                cout << "Opcion invalida" << endl;
        }
    } while (opcionPrincipal != 4);

    return 0;
}
