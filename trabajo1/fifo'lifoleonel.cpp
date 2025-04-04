//edwin leonel baltan carabali 
//  jhoan camilo bonilla gamboa 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estructura para representar un cliente
struct Cliente {
    int id;                // Identificador único
    char nombre[30];       // Nombre completo
    int edad;              // Edad
    char genero;           // 'M' o 'F'
    char motivo[50];       // Motivo de la llamada
    struct Cliente* sig;   // Puntero al siguiente cliente
};

// Punteros globales para la cola FIFO
struct Cliente* frente = NULL;
struct Cliente* final = NULL;

// Puntero global para la pila LIFO (historial)
struct Cliente* top = NULL;

// Función para limpiar la pantalla (portátil)
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Función para validar entrada numérica
int validarEntero(int* valor, const char* mensaje) {
    printf("%s", mensaje);
    if (scanf("%d", valor) != 1) {
        while (getchar() != '\n'); // Limpiar buffer
        return 0;
    }
    getchar(); // Limpiar salto de línea
    return 1;
}

// Función para verificar si un ID ya existe en la cola o el historial
int idExiste(int id) {
    // Verificar en la cola
    struct Cliente* aux = frente;
    while (aux != NULL) {
        if (aux->id == id) return 1; // ID encontrado en la cola
        aux = aux->sig;
    }
    // Verificar en el historial
    aux = top;
    while (aux != NULL) {
        if (aux->id == id) return 1; // ID encontrado en el historial
        aux = aux->sig;
    }
    return 0; // ID no encontrado
}

// Función para contar géneros en la cola
int contarGenero(char genero) {
    int count = 0;
    struct Cliente* aux = frente;
    while (aux != NULL) {
        if (aux->genero == genero) count++;
        aux = aux->sig;
    }
    return count;
}

// Función para agregar un cliente a la cola FIFO con validaciones
void agregarCliente() {
    struct Cliente* nuevo = (struct Cliente*)malloc(sizeof(struct Cliente));
    if (nuevo == NULL) {
        printf("Error: No hay memoria disponible.\n");
        return;
    }

    // Validación del ID único,
    do {
        if (!validarEntero(&nuevo->id, "Ingrese el ID del cliente : ")) {
            printf("Error: Ingrese un numero valido.\n");
        }   else if (idExiste(nuevo->id)) {
            printf("Error: El ID %d ya esta en uso en la cola o el historial. Ingrese un ID unico.\n", nuevo->id);
        } else {
            break;
        }
    } while (1);

    // Validación del nombre
    printf("Ingrese el nombre del cliente maximo 29 caracteres : ");
    fgets(nuevo->nombre, sizeof(nuevo->nombre), stdin);
    nuevo->nombre[strcspn(nuevo->nombre, "\n")] = '\0';
    if (strlen(nuevo->nombre) == 0) {
        printf("Error: El nombre no puede estar vacio.\n");
        free(nuevo);
        return;
    }

    // Validación de la edad (no menores de 18)
    do {
        if (!validarEntero(&nuevo->edad, "Ingrese la edad del cliente: ")) {
            printf("Error: Ingrese un numero valido.\n");
        } else if (nuevo->edad < 18) {
            printf("Error: No se permiten menores de 18 anos para ser atendido .\n");
            free(nuevo);
            return;
        } else if (nuevo->edad > 100) {
            printf("Error: La edad debe estar entre 18 y 100.\n");
        } else {
            break;
        }
    } while (1);

    // Validación del género
    char genero_input;
    do {
        printf("Ingrese el genero del cliente M o F: ");
        scanf("%c", &genero_input);
        getchar(); // Limpiar buffer
        genero_input = toupper(genero_input);
        if (genero_input != 'M' && genero_input != 'F') {
            printf("Error: Solo se acepta 'M' o 'F'.\n");
        } else {
            nuevo->genero = genero_input;
            break;
        }
    } while (1);

    // Validación por diferencia de género
    int hombres = contarGenero('M');
    int mujeres = contarGenero('F');
    if ((nuevo->genero == 'M' && hombres - mujeres > 3) || 
        (nuevo->genero == 'F' && mujeres - hombres > 3)) {
        printf("Error: No se puede agregar, diferencia de genero mayor a 4 (H: %d, M: %d).\n", hombres, mujeres);
        free(nuevo);
        return;
    }

    // Validación del motivo
    printf("Ingrese el motivo de la llamada maximo 49 caracteres: ");
    fgets(nuevo->motivo, sizeof(nuevo->motivo), stdin);
    nuevo->motivo[strcspn(nuevo->motivo, "\n")] = '\0';
    if (strlen(nuevo->motivo) == 0) {
        printf("Error: El motivo no puede estar vacio.\n");
        free(nuevo);
        return;
    }

    nuevo->sig = NULL;
    if (frente == NULL) {
        frente = final = nuevo;
    } else {
        final->sig = nuevo;
        final = nuevo;
    }
    printf("Cliente agregado exitosamente a la cola.\n");
}

// Función para mostrar los clientes en espera
void mostrarClientes() {
    if (frente == NULL) {
        printf("No hay clientes en espera.\n");
        return;
    }
    printf("\n    Clientes en Espera:  \n");
    struct Cliente* aux = frente;
    while (aux != NULL) {
        printf("ID: %d | Nombre: %s | Edad: %d | Genero: %c | Motivo: %s\n",
               aux->id, aux->nombre, aux->edad, aux->genero, aux->motivo);
        aux = aux->sig;
    }
    printf(" liooooo\n");
}

// Función para contar los clientes en espera
void contarClientes() {
    int contador = 0;
    struct Cliente* aux = frente;
    while (aux != NULL) {
        contador++;
        aux = aux->sig;
    }
    printf("Clientes en espera: %d\n", contador);
}

// Función para atender al siguiente cliente (FIFO → LIFO)
void atenderCliente() {
    if (frente == NULL) {
        printf("No hay clientes en espera.\n");
        return;
    }

    struct Cliente* atendido = frente;

    frente = frente->sig;
    if (frente == NULL) final = NULL;

    atendido->sig = top;
    top = atendido;
    printf("Cliente %s (ID: %d) atendido y registrado en el historial.\n", atendido->nombre, atendido->id);
}

// Función para mostrar el historial de atenciones
void mostrarHistorial() {
    if (top == NULL) {
        printf("No hay historial de atenciones.\n");
        return;
    }
    printf("\n=== Historial de Atenciones ===\n");
    struct Cliente* aux = top;
    while (aux != NULL) {
        printf("ID: %d | Nombre: %s | Edad: %d | Género: %c | Motivo: %s\n",
               aux->id, aux->nombre, aux->edad, aux->genero, aux->motivo);
        aux = aux->sig;
    }
    printf(" lioonel \n");
}

// Función para deshacer la última atención (LIFO → FIFO)
void deshacerAtencion() {
    if (top == NULL) {
        printf("No hay atencion previa para deshacer.\n");
        return;
    }

    struct Cliente* deshacer = top;
    top = top->sig;

    deshacer->sig = frente;
    frente = deshacer;
    if (final == NULL) final = frente;
    printf("Atencion de %s (ID: %d) deshecha. Cliente reintegrado a la cola.\n", deshacer->nombre, deshacer->id);
}

// Liberar toda la memoria al finalizar
void liberarMemoria() {
    struct Cliente* aux;
    while (frente != NULL) {
        aux = frente;
        frente = frente->sig;
        free(aux);
    }
    while (top != NULL) {
        aux = top;
        top = top->sig;
        free(aux);
    }
}

// Menú principal
int main() {
    int opc;
    do {
        limpiarPantalla();
        printf("\n                                 \n");
        printf("   Sistema de Atencion al Cliente\n");
        printf("                                      \n");
        printf("1. Agregar cliente a la cola\n");
        printf("2. Mostrar clientes en espera\n");
        printf("3. Contar clientes en espera\n");
        printf("4. Atender siguiente cliente\n");
        printf("5. Mostrar historial de atenciones\n");
        printf("6. Deshacer ultima atencion\n");
        printf("7. Salir\n");
        printf("leonel                             \n");
        if (!validarEntero(&opc, "Seleccione una opcion (1-7): ")) {
            printf("Error: Ingrese un numero valido.\n");
            printf("Presione Enter para continuar...");
            getchar();
            continue;
        }

        switch (opc) {
            case 1: 
                agregarCliente(); 
                break;
            case 2: 
                mostrarClientes(); 
                break;
            case 3: 
                contarClientes(); 
                break;
            case 4: 
                atenderCliente(); 
                break;
            case 5: 
                mostrarHistorial(); 
                break;
            case 6: 
                deshacerAtencion(); 
                break;
            case 7: 
                printf("Saliendo del sistema...\n");
                liberarMemoria();
                break;
            default: 
                printf("Opcion invalida.\n");
        }
        if (opc != 7) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
    } while (opc != 7);

    return 0;
}