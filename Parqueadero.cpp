#include "Parqueadero.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parqueadero::Parqueadero(int capacidad) : inicio(nullptr), capacidad(capacidad), ocupados(0) {}

Parqueadero::~Parqueadero() {
    while (inicio) {
        Nodo* temp = inicio->siguiente;
        free(inicio);
        inicio = temp;
    }
}

bool Parqueadero::ingresarAuto(const Auto& auto_) {
    if (ocupados >= capacidad) return false;

    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return false;

    nuevo->vehiculo = auto_;
    if (!inicio) {
        inicio = nuevo;
        inicio->siguiente = inicio;
    } else {
        Nodo* actual = inicio;
        while (actual->siguiente != inicio) actual = actual->siguiente;
        actual->siguiente = nuevo;
        nuevo->siguiente = inicio;
    }
    ocupados++;
    return true;
}

bool Parqueadero::buscarAutoPorPlaca(const char* placa, Auto& resultado) const {
    if (!inicio) return false;

    Nodo* actual = inicio;
    do {
        if (strcmp(actual->vehiculo.getPlaca(), placa) == 0) {
            resultado = actual->vehiculo;
            return true;
        }
        actual = actual->siguiente;
    } while (actual != inicio);

    return false;
}

bool Parqueadero::buscarAutoPorFecha(const char* fecha, Auto** resultados, int& count) const {
    if (!inicio) return false;

    int maxResultados = capacidad;
    *resultados = (Auto*)malloc(maxResultados * sizeof(Auto));
    if (!*resultados) return false;

    count = 0;
    Nodo* actual = inicio;
    do {
        if (strcmp(actual->vehiculo.getFechaIngreso(), fecha) == 0) {
            (*resultados)[count++] = actual->vehiculo;
        }
        actual = actual->siguiente;
    } while (actual != inicio);

    return count > 0;
}

bool Parqueadero::salidaAuto(const char* placa) {
    if (!inicio) return false;

    Nodo* actual = inicio;
    do {
        if (strcmp(actual->vehiculo.getPlaca(), placa) == 0) {
            actual->vehiculo.setFechaIngreso("N/A"); // Marca que ya no está estacionado
            ocupados--;
            return true;
        }
        actual = actual->siguiente;
    } while (actual != inicio);

    return false;
}

void Parqueadero::imprimirEstado() const {
    printf("\n--- Estado del Parqueadero ---\n");
    if (!inicio) {
        printf("No hay vehículos estacionados.\n");
        return;
    }

    Nodo* actual = inicio;
    int espacio = 1;

    do {
        printf("Espacio %d: %s (Propietario: %s, Fecha y hora de ingreso: %s)\n",
               espacio++, actual->vehiculo.getPlaca(),
               actual->vehiculo.getNombrePropietario(),
               actual->vehiculo.getFechaIngreso());
        actual = actual->siguiente;
    } while (actual != inicio);

    printf("Capacidad total: %d | Espacios ocupados: %d\n", capacidad, ocupados);
}
