#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Vehiculo {
    char placa[10];
    char propietario[30];
    char cedula[15];
    char correo[50];
    char fechaHoraIngreso[20];
    struct Vehiculo* siguiente;
} Vehiculo;


typedef struct Parqueadero {
    Vehiculo* inicio;
    int capacidad;
    int ocupados;
} Parqueadero;

/*// Función para obtener la fecha y hora actual como cadena
void obtenerFechaHoraActual(char* buffer) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, 20, "%d/%m/%Y %H:%M", tm_info);
}*/

// Función para inicializar el parqueadero
Parqueadero* crearParqueadero(int capacidad) {
    Parqueadero* parqueadero = (Parqueadero*)malloc(sizeof(Parqueadero));
    parqueadero->inicio = NULL;
    parqueadero->capacidad = capacidad;
    parqueadero->ocupados = 0;
    return parqueadero;
}

// Función para agregar un vehículo al parqueadero
void agregarVehiculo(Parqueadero* parqueadero, char* placa, char* propietario, char* cedula, char* correo) {
    Vehiculo* nuevo = (Vehiculo*)malloc(sizeof(Vehiculo));
    strcpy(nuevo->placa, placa);
    strcpy(nuevo->propietario, propietario);
    strcpy(nuevo->cedula, cedula);
    strcpy(nuevo->correo, correo);

    // Obtener fecha y hora actual para el registro
    time_t t = time(NULL);
    struct tm* tiempoLocal = localtime(&t);
    strftime(nuevo->fechaHoraIngreso, sizeof(nuevo->fechaHoraIngreso), "%d/%m/%Y %H:%M", tiempoLocal);

    nuevo->siguiente = NULL;

    if (parqueadero->inicio == NULL) {
        parqueadero->inicio = nuevo;
    } else {
        Vehiculo* temp = parqueadero->inicio;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
    parqueadero->ocupados++;
}


// Función para buscar un vehículo por placa
Vehiculo* buscarVehiculo(Parqueadero* parqueadero, char* placa) {
    Vehiculo* actual = parqueadero->inicio;
    while (actual) {
        if (strcmp(actual->placa, placa) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

// Función para registrar la salida de un vehículo
void registrarSalidaVehiculo(Parqueadero* parqueadero, char* placa) {
    Vehiculo* actual = parqueadero->inicio;
    Vehiculo* anterior = NULL;
    while (actual) {
        if (strcmp(actual->placa, placa) == 0) {
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            } else {
                parqueadero->inicio = actual->siguiente;
            }
            free(actual);
            parqueadero->ocupados--;
            printf("Vehículo con placa %s ha salido del parqueadero.\n", placa);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("No se encontró un vehículo con la placa %s.\n", placa);
}

// Función para imprimir el estado del parqueadero
void imprimirEstadoParqueadero(Parqueadero* parqueadero) {
    printf("\n--- Estado del Parqueadero ---\n");
    Vehiculo* actual = parqueadero->inicio;
    int espacio = 1;
    while (actual) {
        printf("Espacio %d: %s (Propietario: %s, Fecha y hora de ingreso: %s)\n",
               espacio, actual->placa, actual->propietario, actual->fechaHoraIngreso);
        actual = actual->siguiente;
        espacio++;
    }
    printf("Capacidad total: %d | Espacios ocupados: %d\n",
           parqueadero->capacidad, parqueadero->ocupados);
}

// Menú principal
void menu(Parqueadero* parqueadero) {
    int opcion;
    char placa[10], propietario[30], cedula[15], correo[50];
    Vehiculo* encontrado;

    do {
        printf("\n--- Menú Parqueadero ---\n");
        printf("1. Ingresar vehículo\n");
        printf("2. Buscar vehículo\n");
        printf("3. Registrar salida de vehículo\n");
        printf("4. Imprimir estado del parqueadero\n");
        printf("5. Salir\n");
        printf("Ingrese una opción: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                printf("Ingrese la placa del vehículo: ");
                scanf("%s", placa);
                getchar();

                encontrado = buscarVehiculo(parqueadero, placa);
                if (encontrado) {
                    printf("El vehículo con placa %s ya se encuentra registrado.\n", placa);
                } else {
                    printf("Vehiculo no encontrado, ingresando datos del vehiculo\n");
                    printf("Ingrese el nombre del propietario: ");
                    fgets(propietario, sizeof(propietario), stdin);
                    propietario[strcspn(propietario, "\n")] = '\0';

                    printf("Ingrese la cédula del propietario: ");
                    scanf("%s", cedula);
                    getchar();

                    printf("Ingrese el correo electrónico del propietario: ");
                    scanf("%s", correo);
                    getchar();

                    agregarVehiculo(parqueadero, placa, propietario, cedula, correo);
                    printf("Vehículo registrado exitosamente.\n");
                }
                break;
            case 2:
                printf("Ingrese la placa del vehículo: ");
                scanf("%s", placa);
                getchar();

                encontrado = buscarVehiculo(parqueadero, placa);
                if (encontrado) {
                    printf("Vehículo encontrado: %s (Propietario: %s, Fecha y hora de ingreso: %s)\n",
                           encontrado->placa, encontrado->propietario, encontrado->fechaHoraIngreso);
                } else {
                    printf("No se encontró un vehículo con la placa %s.\n", placa);
                }
                break;
            case 3:
                printf("Ingrese la placa del vehículo: ");
                scanf("%s", placa);
                getchar();
                registrarSalidaVehiculo(parqueadero, placa);
                break;
            case 4:
                imprimirEstadoParqueadero(parqueadero);
                break;
            case 5:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
                break;
        }
    } while (opcion != 5);
}

// Función principal
int main() {
    Parqueadero* parqueadero = crearParqueadero(10); // Capacidad de 10
    menu(parqueadero);
    return 0;
}
