#include "Parqueadero.h"
#include <ctime> // Para manejar la fecha y hora del sistema

// Función para mostrar el menú principal
void menu() {
    printf("\n--- Menú Parqueadero ---\n");
    printf("1. Ingresar vehículo\n");
    printf("2. Buscar vehículo\n");
    printf("3. Registrar salida de vehículo\n");
    printf("4. Imprimir estado del parqueadero\n");
    printf("5. Salir\n");
    printf("Ingrese una opción: ");
}

// Función para obtener la fecha y hora actual como cadena
void obtenerFechaHora(char* buffer, size_t size) {
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    strftime(buffer, size, "%d/%m/%Y %H:%M:%S", &tstruct);
}

// Función para mostrar el submenú de búsqueda
void submenuBusqueda(Parqueadero& parqueadero) {
    int opcionBusqueda;
    printf("\n--- Búsqueda de Vehículo ---\n");
    printf("1. Buscar por placa\n");
    printf("2. Buscar por fecha\n");
    printf("Ingrese una opción: ");
    scanf("%d", &opcionBusqueda);

    if (opcionBusqueda == 1) {
        char placa[10];
        printf("Ingrese la placa del vehículo: ");
        scanf("%s", placa);

        Auto autoEncontrado;
        if (parqueadero.buscarAutoPorPlaca(placa, autoEncontrado)) {
            printf("Vehículo encontrado: %s (Propietario: %s, Fecha ingreso: %s)\n",
                   autoEncontrado.getPlaca(),
                   autoEncontrado.getNombrePropietario(),
                   autoEncontrado.getFechaIngreso());
        } else {
            printf("Vehículo no encontrado.\n");
        }
    } else if (opcionBusqueda == 2) {
        char fecha[20];
        printf("Ingrese la fecha (dd/mm/aaaa): ");
        scanf("%s", fecha);

        Auto* resultados;
        int count;
        if (parqueadero.buscarAutoPorFecha(fecha, &resultados, count)) {
            printf("Vehículos encontrados en la fecha %s:\n", fecha);
            for (int i = 0; i < count; i++) {
                printf("%s (Propietario: %s)\n", resultados[i].getPlaca(), resultados[i].getNombrePropietario());
            }
            free(resultados);
        } else {
            printf("No se encontraron vehículos en esa fecha.\n");
        }
    } else {
        printf("Opción no válida. Volviendo al menú principal...\n");
    }
}

// Función para ingresar los datos de un nuevo vehículo
void ingresarDatosAuto(Auto& nuevoAuto) {
    char nombre[50];
    char cedula[15];
    char correo[50];

    printf("Ingrese el nombre del propietario: ");
    scanf("%s", nombre);
    printf("Ingrese la cédula del propietario: ");
    scanf("%s", cedula);
    printf("Ingrese el correo del propietario: ");
    scanf("%s", correo);

    nuevoAuto.setNombrePropietario(nombre);
    nuevoAuto.setCedula(cedula);
    nuevoAuto.setCorreo(correo);
}

int main() {
    int capacidad = 10; // Capacidad del parqueadero
    Parqueadero parqueadero(capacidad);

    int opcion;
    do {
        menu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: { // Ingresar auto
                char placa[10];
                printf("Ingrese la placa del vehículo: ");
                scanf("%s", placa);

                Auto autoExistente;
                if (parqueadero.buscarAutoPorPlaca(placa, autoExistente)) {
                    printf("Vehículo encontrado. Asignando un nuevo espacio...\n");

                    char fechaHora[20];
                    obtenerFechaHora(fechaHora, sizeof(fechaHora));
                    autoExistente.setFechaIngreso(fechaHora);

                    if (parqueadero.ingresarAuto(autoExistente)) {
                        printf("Vehículo estacionado exitosamente.\n");
                    } else {
                        printf("El parqueadero está lleno.\n");
                    }
                } else {
                    printf("Vehículo no encontrado. Registrando nuevos datos...\n");
                    Auto nuevoAuto;
                    nuevoAuto.setPlaca(placa);
                    ingresarDatosAuto(nuevoAuto);

                    char fechaHora[20];
                    obtenerFechaHora(fechaHora, sizeof(fechaHora));
                    nuevoAuto.setFechaIngreso(fechaHora);

                    if (parqueadero.ingresarAuto(nuevoAuto)) {
                        printf("Vehículo registrado y estacionado exitosamente.\n");
                    } else {
                        printf("El parqueadero está lleno.\n");
                    }
                }
                break;
            }
            case 2: { // Búsqueda
                submenuBusqueda(parqueadero);
                break;
            }
            case 3: { // Salida de auto
                char placa[10];
                printf("Ingrese la placa del vehículo: ");
                scanf("%s", placa);

                if (parqueadero.salidaAuto(placa)) {
                    printf("Vehículo marcado como salida. Sus datos se conservarán.\n");
                } else {
                    printf("Vehículo no encontrado.\n");
                }
                break;
            }
            case 4: { // Imprimir estado del parqueadero
                parqueadero.imprimirEstado();
                break;
            }
            case 5:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 5);

    return 0;
}
