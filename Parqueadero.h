#ifndef PARQUEADERO_H
#define PARQUEADERO_H

#include "Auto.h"

struct Nodo {
    Auto vehiculo;
    Nodo* siguiente;

    Nodo(const Auto& auto_) : vehiculo(auto_), siguiente(nullptr) {}
};

class Parqueadero {
private:
    Nodo* inicio;
    int capacidad;
    int ocupados;

public:
    Parqueadero(int capacidad);
    ~Parqueadero();

    bool ingresarAuto(const Auto& auto_);
    bool buscarAutoPorPlaca(const char* placa, Auto& resultado) const;
    bool buscarAutoPorFecha(const char* fecha, Auto** resultados, int& count) const;
    bool salidaAuto(const char* placa);
    void imprimirEstado() const;

    int getOcupados() const { return ocupados; }
    int getCapacidad() const { return capacidad; }
};

#endif