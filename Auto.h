/*******************************************************************************************
 *            UNIVERSIDAD DE LAS FUERZAS ARMADAS ESPE                                      *
 * Proposito:                      Programa Estacionamiento ESPE                           *
 * Autor:                          Leandro Mendoza, Steven Yaydacela                       *
 * Fecha de creacion:              06/12/2024                                              *
 * Fecha de modificacion:          09/12/2024                                              *
 * Materia:                        Estructura de datos                                     *
 * NRC :                           1978                                                    *
 ******************************************************************************************/

#ifndef AUTO_H
#define AUTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Auto {
private:
    char placa[10];
    char nombrePropietario[50];
    char cedula[15];
    char correo[50];
    char fechaIngreso[11];

public:
    Auto(const char* placa = "", const char* nombrePropietario = "", const char* cedula = "", const char* correo = "", const char* fechaIngreso = "") {
        setPlaca(placa);
        setNombrePropietario(nombrePropietario);
        setCedula(cedula);
        setCorreo(correo);
        setFechaIngreso(fechaIngreso);
    }

    const char* getPlaca() const { return placa; }
    const char* getNombrePropietario() const { return nombrePropietario; }
    const char* getCedula() const { return cedula; }
    const char* getCorreo() const { return correo; }
    const char* getFechaIngreso() const { return fechaIngreso; }

    void setPlaca(const char* p) { strncpy(placa, p, sizeof(placa) - 1); placa[sizeof(placa) - 1] = '\0'; }
    void setNombrePropietario(const char* np) { strncpy(nombrePropietario, np, sizeof(nombrePropietario) - 1); nombrePropietario[sizeof(nombrePropietario) - 1] = '\0'; }
    void setCedula(const char* c) { strncpy(cedula, c, sizeof(cedula) - 1); cedula[sizeof(cedula) - 1] = '\0'; }
    void setCorreo(const char* e) { strncpy(correo, e, sizeof(correo) - 1); correo[sizeof(correo) - 1] = '\0'; }
    void setFechaIngreso(const char* f) { strncpy(fechaIngreso, f, sizeof(fechaIngreso) - 1); fechaIngreso[sizeof(fechaIngreso) - 1] = '\0'; }
};

#endif
