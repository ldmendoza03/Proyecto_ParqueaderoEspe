/*******************************************************************************************
 *            UNIVERSIDAD DE LAS FUERZAS ARMADAS ESPE                                      *
 * Proposito:                      Programa Estacionamiento ESPE                           *
 * Autor:                          Leandro Mendoza, Steven Yaydacela                       *
 * Fecha de creacion:              06/12/2024                                              *
 * Fecha de modificacion:          09/12/2024                                              *
 * Materia:                        Estructura de datos                                     *
 * NRC :                           1978                                                    *
 ******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h> // Para Windows
#else
#include <termios.h>
#include <unistd.h>
#endif

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

// Función para inicializar el parqueadero
Parqueadero* crearParqueadero(int capacidad) {
    Parqueadero* parqueadero = (Parqueadero*)malloc(sizeof(Parqueadero));
    parqueadero->inicio = NULL;
    parqueadero->capacidad = capacidad;
    parqueadero->ocupados = 0;
    return parqueadero;
}

// Función para leer una tecla (para navegar con las flechas)
#ifdef _WIN32
int leerTecla() {
    return _getch(); // Lee una tecla en Windows
}
#else
int leerTecla() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

// Función para validar los datos
int esPlacaValida(const char* placa) {
    int len = strlen(placa);
    if (len < 5 || len > 8) return 0;
    for (int i = 0; i < 3; i++) {
        if (placa[i] < 'A' || placa[i] > 'Z') return 0;
    }
    if (placa[3] != '-') return 0;
    for (int i = 4; i < len; i++) {
        if (placa[i] < '0' || placa[i] > '9') return 0;
    }
    return 1;
}

int esNombreValido(const char* nombre) {
    int len = strlen(nombre);
    int espacio = 0;
    for (int i = 0; i < len; i++) {
        if (isalpha(nombre[i]) || nombre[i] == ' ') {
            if (nombre[i] == ' ') espacio++;
        } else {
            return 0;
        }
    }
    return espacio == 1;
}

int esCedulaValida(const char* cedula) {
    int len = strlen(cedula);
    if (len != 10) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(cedula[i])) return 0;
    }
    return 1;
}

int esCorreoValido(const char* correo) {
    const char* dominios[] = {"@gmail.com", "@hotmail.com", "@espe.edu.ec", "@yahoo.com", "@iCloud.com", "@outlook.com"};
    int numDominios = sizeof(dominios) / sizeof(dominios[0]);
    for (int i = 0; i < numDominios; i++) {
        if (strstr(correo, dominios[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

// Función para agregar un vehículo al parqueadero
void agregarVehiculo(Parqueadero* parqueadero, char* placa, char* propietario, char* cedula, char* correo) {
    Vehiculo* nuevo = (Vehiculo*)malloc(sizeof(Vehiculo));
    strcpy(nuevo->placa, placa);
    strcpy(nuevo->propietario, propietario);
    strcpy(nuevo->cedula, cedula);
    strcpy(nuevo->correo, correo);

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

// Función para mostrar el menú con navegación por flechas
void menu(Parqueadero* parqueadero) {
    const char* opciones[] = {
        "Ingresar vehiculo",
        "Buscar vehiculo",
        "Registrar salida de vehiculo",
        "Imprimir estado del parqueadero",
        "Salir"
    };
    int numOpciones = sizeof(opciones) / sizeof(opciones[0]);
    int seleccion = 0; // Índice de la opción seleccionada

    while (1) {
        system("clear || cls"); // Limpia la consola
        printf("\n--- Menu Parqueadero ---\n");
        for (int i = 0; i < numOpciones; i++) {
            if (i == seleccion) {
                printf(" > %s\n", opciones[i]); // Resalta la opción seleccionada
            } else {
                printf("   %s\n", opciones[i]);
            }
        }

        int tecla = leerTecla();
#ifdef _WIN32
        if (tecla == 224) { // Código inicial para flechas en Windows
            tecla = leerTecla();
            if (tecla == 72) { // Flecha arriba
                seleccion = (seleccion - 1 + numOpciones) % numOpciones;
            } else if (tecla == 80) { // Flecha abajo
                seleccion = (seleccion + 1) % numOpciones;
            }
        } else if (tecla == 13) { // Enter
#else
        if (tecla == '\033') { // Secuencia para flechas en sistemas POSIX
            leerTecla(); // Ignorar el siguiente carácter '['
            tecla = leerTecla();
            if (tecla == 'A') { // Flecha arriba
                seleccion = (seleccion - 1 + numOpciones) % numOpciones;
            } else if (tecla == 'B') { // Flecha abajo
                seleccion = (seleccion + 1) % numOpciones;
            }
        } else if (tecla == '\n') { // Enter
#endif
            switch (seleccion) {
                case 0: {
                    char placa[10], propietario[30], cedula[15], correo[50];
                    Vehiculo* encontrado;
                    do {
                        printf("Ingrese la placa del vehiculo: ");
                        scanf("%s", placa);
                        getchar();
                        if (!esPlacaValida(placa)) {
                            printf("Placa invalida. Debe ser en formato ecuatoriano (Ej: ABC-1234).\n");
                        }
                    } while (!esPlacaValida(placa));

                    encontrado = buscarVehiculo(parqueadero, placa);
                    if (encontrado) {
                        printf("El vehiculo con placa %s ya se encuentra registrado.\n", placa);
                    } else {
                        printf("Vehiculo no encontrado, ingresando datos del vehiculo\n");

                        do {
                            printf("Ingrese el nombre del propietario (Ej: Camila Haro): ");
                            fgets(propietario, sizeof(propietario), stdin);
                            propietario[strcspn(propietario, "\n")] = '\0';
                            if (!esNombreValido(propietario)) {
                                printf("Nombre invalido. Solo se permiten letras y debe incluir nombre y apellido.\n");
                            }
                        } while (!esNombreValido(propietario));

                        do {
                            printf("Ingrese la cedula del propietario: ");
                            scanf("%s", cedula);
                            getchar();
                            if (!esCedulaValida(cedula)) {
                                printf("Cedula invalida. Debe contener10 dígitos.\n");
                            }
                        } while (!esCedulaValida(cedula));

                        do {
                            printf("Ingrese el correo electronico del propietario: ");
                            scanf("%s", correo);
                            getchar();
                            if (!esCorreoValido(correo)) {
                                printf("Correo inválido. Debe tener un dominio válido (Ej: @gmail.com, @hotmail.com).\n");
                            }
                        } while (!esCorreoValido(correo));

                        agregarVehiculo(parqueadero, placa, propietario, cedula, correo);
                        printf("Vehiculo ingresado exitosamente.\n");
                    }
                    break;
                }
                case 1: {
                    char placa[10];
                    printf("Ingrese la placa del vehiculo a buscar: ");
                    scanf("%s", placa);
                    Vehiculo* vehiculo = buscarVehiculo(parqueadero, placa);
                    if (vehiculo) {
                        printf("Vehiculo encontrado: %s (Propietario: %s, Fecha y hora de ingreso: %s)\n",
                               vehiculo->placa, vehiculo->propietario, vehiculo->fechaHoraIngreso);
                    } else {
                        printf("Vehiculo no encontrado.\n");
                    }
                    getchar(); // Espera una tecla para continuar
                    break;
                }
                case 2: {
                    char placa[10];
                    printf("Ingrese la placa del vehiculo a registrar la salida: ");
                    scanf("%s", placa);
                    registrarSalidaVehiculo(parqueadero, placa);
                    getchar(); // Espera una tecla para continuar
                    break;

                }
                case 3: {
                    imprimirEstadoParqueadero(parqueadero);
                    break;
                }
                case 4:
                    printf("Saliendo del programa...\n");
                    return;
                default:
                    printf("Opción no válida.\n");
                    break;
            }
            printf("Presione cualquier tecla para continuar...\n");
            getchar(); // Espera una tecla para continuar
        }
    }
}

// Función principal
int main() {
    Parqueadero* parqueadero = crearParqueadero(100); // Capacidad de 100 vehículos
    menu(parqueadero);
    return 0;
}
