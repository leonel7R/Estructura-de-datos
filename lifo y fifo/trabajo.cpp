// edwin leonel baltan carabali 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <malloc.h> // Para malloc 

//Estructura para representar una pelicula en el arbol Binario de Busqueda
struct Pelicula {
    std::string nombre;  // Nombre de la pelicula
    int anio;            // Año de realizacion
    std::string genero;  // Genero de la pelicula
    float recaudacion;   // Dinero recaudado en millones de dolares
    Pelicula* izq;       // Puntero al hijo izquierdo
    Pelicula* der;       // Puntero al hijo derecho

    Pelicula(std::string nombre, int anio, std::string genero, float recaudacion)
        : nombre(nombre), anio(anio), genero(genero), recaudacion(recaudacion), izq(NULL), der(NULL) {}
};

// Clase para manejar el arbol Binario de Busqueda
class ArbolPeliculas {
private:
    Pelicula* raiz;

    // Funcion para insertar una pelicula
    Pelicula* insertar(Pelicula* nodo, std::string nombre, int anio, std::string genero, float recaudacion) {
        if (nodo == NULL) {
            Pelicula* nuevo = (Pelicula*)malloc(sizeof(Pelicula));
            if (nuevo == NULL) {
                std::cerr << "Error: No se pudo asignar." << std::endl;
                exit(1); 
            }
            // Inicializacion manual 
            nuevo->nombre = nombre;
            nuevo->anio = anio;
            nuevo->genero = genero;
            nuevo->recaudacion = recaudacion;
            nuevo->izq = NULL;
            nuevo->der = NULL;
            return nuevo;
        }
        if (anio <= nodo->anio) { // Si el año es menor o igual, va a la izquierda
            nodo->izq = insertar(nodo->izq, nombre, anio, genero, recaudacion);
        } else { // Si el año es mayor, va a la derecha
            nodo->der = insertar(nodo->der, nombre, anio, genero, recaudacion);
        }
        return nodo;
    }

    // Funcio para recorrer el arbol en inorden
    void inOrden(Pelicula* nodo) const {
        if (nodo != NULL) {
            inOrden(nodo->izq);
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            inOrden(nodo->der);
        }
    }

    // Funcion para recorrer el arbol en preorden
    void preOrden(Pelicula* nodo) const {
        if (nodo != NULL) {
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            preOrden(nodo->izq);
            preOrden(nodo->der);
        }
    }

    // Funcion para recorrer el arbol en postorden
    void postOrden(Pelicula* nodo) const {
        if (nodo != NULL) {
            postOrden(nodo->izq);
            postOrden(nodo->der);
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
        }
    }

    // Funcion para buscar una pelicula por nombre
    Pelicula* buscarPorNombre(Pelicula* nodo, std::string nombre) const {
        if (nodo == NULL) {
            return NULL;
        }
        if (nodo->nombre == nombre) {
            return nodo;
        }
        Pelicula* encontrada = buscarPorNombre(nodo->izq, nombre);
        if (encontrada != NULL) {
            return encontrada;
        }
        return buscarPorNombre(nodo->der, nombre);
    }

    // Funcion para mostrar todas las peliculas de un genero
    void mostrarPorGenero(Pelicula* nodo, std::string genero) const {
        if (nodo != NULL) {
            mostrarPorGenero(nodo->izq, genero);
            if (nodo->genero == genero) {
                std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                          << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            }
            mostrarPorGenero(nodo->der, genero);
        }
    }

    // Funcion para recolectar todas las peliculas en un vector
    void recolectarPeliculas(Pelicula* nodo, std::vector<Pelicula*>& peliculas) const {
        if (nodo != NULL) {
            recolectarPeliculas(nodo->izq, peliculas);
            peliculas.push_back(nodo);
            recolectarPeliculas(nodo->der, peliculas);
        }
    }

    // Funcion para liberar la memoria del arbol
    void liberarArbol(Pelicula* nodo) {
        if (nodo != NULL) {
            liberarArbol(nodo->izq);
            liberarArbol(nodo->der);
            nodo->~Pelicula(); 
            free(nodo); // Liberar memoria
        }
    }

public:
    ArbolPeliculas() : raiz(NULL) {}

    ~ArbolPeliculas() {
        liberarArbol(raiz);
    }

    void insertar(std::string nombre, int anio, std::string genero, float recaudacion) {
        raiz = insertar(raiz, nombre, anio, genero, recaudacion);
    }

    void inOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Inorden:\n";
            inOrden(raiz);
        }
    }

    void preOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Preorden:\n";
            preOrden(raiz);
        }
    }

    void postOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Postorden:\n";
            postOrden(raiz);
        }
    }

    void buscarPorNombre(std::string nombre) const {
        Pelicula* encontrada = buscarPorNombre(raiz, nombre);
        if (encontrada != NULL) {
            std::cout << "Encontrada -> Pelicula: " << encontrada->nombre << ", Ano: " << encontrada->anio
                      << ", Genero: " << encontrada->genero << ", Recaudacion: " << encontrada->recaudacion << "M\n";
        } else {
            std::cout << "Pelicula no encontrada.\n";
        }
    }

    void mostrarPorGenero(std::string genero) const {
        std::cout << "\nPeliculas del genero " << genero << ":\n";
        mostrarPorGenero(raiz, genero);
    }

    void mostrarFracasos() const {
        std::vector<Pelicula*> peliculas;
        recolectarPeliculas(raiz, peliculas);
        if (peliculas.empty()) {
            std::cout << "No hay peliculas en el arbol.\n";
            return;
        }
        std::sort(peliculas.begin(), peliculas.end(), 
                  [](const Pelicula* a, const Pelicula* b) { return a->recaudacion < b->recaudacion; });
        std::cout << "\nLos 3 fracasos taquilleros:\n";
        for (size_t i = 0; i < 3 && i < peliculas.size(); i++) {
            std::cout << i + 1 << ". Pelicula: " << peliculas[i]->nombre << ", Ano: " << peliculas[i]->anio
                      << ", Genero: " << peliculas[i]->genero << ", Recaudacion: " << peliculas[i]->recaudacion << "M\n";
        }
    }
};

// Funcion para limpiar el buffer de entrada
void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Funcion principal con menu
int main() {
    ArbolPeliculas arbol;
    int opcion;
    std::string nombre;
    std::string genero;
    int anio;
    float recaudacion;

    do {
        std::cout << "\n=== Gestion de Peliculas Arboles Binario Busqueda ===\n";
        std::cout << "1. Agregar una pelicula\n";
        std::cout << "2. Mostrar recorrido inorden\n";
        std::cout << "3. Mostrar recorrido preorden\n";
        std::cout << "4. Mostrar recorrido postorden\n";
        std::cout << "5. Buscar pelicula por nombre\n";
        std::cout << "6. Mostrar peliculas por genero\n";
        std::cout << "7. Mostrar los 3 fracasos taquilleros\n";
        std::cout << "8. Salir\n";
        std::cout << "Seleccione una opcion: ";
        while (!(std::cin >> opcion)) {
            std::cout << "Entrada invalida. Ingrese un numero: ";
            limpiarBuffer();
        }
        limpiarBuffer();

        switch (opcion) {
            case 1: // Agregar una pelicula
                std::cout << "Ingrese el nombre de la pelicula: ";
                std::getline(std::cin, nombre);
                if (nombre.empty()) {
                    std::cout << "El nombre no puede estar vacio.\n";
                    break;
                }

                std::cout << "Ingrese el ano de realizacion: ";
                while (!(std::cin >> anio) || anio < 1700 || anio > 2025) {
                    std::cout << "Ano invalido (debe estar entre 170 y 2025). Intente de nuevo: ";
                    limpiarBuffer();
                }
                limpiarBuffer();

                std::cout << "Ingrese el genero: ";
                std::getline(std::cin, genero);
                if (genero.empty()) {
                    std::cout << "El genero no puede estar vacio.\n";
                    break;
                }

                std::cout << "Ingrese la recaudacion (en millones de dolares): ";
                while (!(std::cin >> recaudacion) || recaudacion < 0) {
                    std::cout << "Recaudacion invalida (debe ser >= 0). Intente de nuevo: ";
                    limpiarBuffer();
                }
                limpiarBuffer();

                arbol.insertar(nombre, anio, genero, recaudacion);
                std::cout << "Pelicula agregada exitosamente.\n";
                break;

            case 2: // Recorrido inorden
                arbol.inOrden();
                break;

            case 3: // Recorrido preorden
                arbol.preOrden();
                break;

            case 4: // Recorrido postorden
                arbol.postOrden();
                break;

            case 5: // Buscar pelicula por nombre
                std::cout << "Ingrese el nombre de la pelicula a buscar: ";
                std::getline(std::cin, nombre);
                arbol.buscarPorNombre(nombre);
                break;

            case 6: // Mostrar peliculas por genero
                std::cout << "Ingrese el genero: ";
                std::getline(std::cin, genero);
                arbol.mostrarPorGenero(genero);
                break;

            case 7: // Mostrar los 3 fracasos taquilleros
                arbol.mostrarFracasos();
                break;

            case 8: // Salir
                std::cout << "Saliendo del programa...\n";
                break;

            default:
                std::cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 8);

    return 0;
}