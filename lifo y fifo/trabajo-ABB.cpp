#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Estructura para representar una película en el ABB
struct Pelicula {
    std::string nombre;  // Nombre de la película
    int anio;            // Año de realización
    std::string genero;  // Género de la película
    float recaudacion;   // Dinero recaudado en millones de dólares
    Pelicula* izq;       // Puntero al hijo izquierdo
    Pelicula* der;       // Puntero al hijo derecho

    Pelicula(std::string nombre, int anio, std::string genero, float recaudacion)
        : nombre(nombre), anio(anio), genero(genero), recaudacion(recaudacion), izq(NULL), der(NULL) {}
};

// Clase para manejar el ABB de películas
class ArbolPeliculas {
private:
    Pelicula* raiz;

    // Función para insertar una película recursivamente
    Pelicula* insertarRec(Pelicula* nodo, std::string nombre, int anio, std::string genero, float recaudacion) {
        if (nodo == NULL) {
            return new Pelicula(nombre, anio, genero, recaudacion);
        }
        if (anio <= nodo->anio) { // Si el año es menor o igual, va a la izquierda
            nodo->izq = insertarRec(nodo->izq, nombre, anio, genero, recaudacion);
        } else {
            nodo->der = insertarRec(nodo->der, nombre, anio, genero, recaudacion);
        }
        return nodo;
    }

    // Función para recorrer el árbol en inorden
    void inOrdenRec(Pelicula* nodo) const {
        if (nodo != NULL) {
            inOrdenRec(nodo->izq);
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            inOrdenRec(nodo->der);
        }
    }

    // Función para recorrer el árbol en preorden
    void preOrdenRec(Pelicula* nodo) const {
        if (nodo != NULL) {
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            preOrdenRec(nodo->izq);
            preOrdenRec(nodo->der);
        }
    }

    // Función para recorrer el árbol en postorden
    void postOrdenRec(Pelicula* nodo) const {
        if (nodo != NULL) {
            postOrdenRec(nodo->izq);
            postOrdenRec(nodo->der);
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
        }
    }

    // Función para buscar una película por nombre
    Pelicula* buscarPorNombreRec(Pelicula* nodo, std::string nombre) const {
        if (nodo == NULL) {
            return NULL;
        }
        if (nodo->nombre == nombre) {
            return nodo;
        }
        Pelicula* encontrada = buscarPorNombreRec(nodo->izq, nombre);
        if (encontrada != NULL) {
            return encontrada;
        }
        return buscarPorNombreRec(nodo->der, nombre);
    }

    // Función para mostrar todas las películas de un género
    void mostrarPorGeneroRec(Pelicula* nodo, std::string genero) const {
        if (nodo != NULL) {
            mostrarPorGeneroRec(nodo->izq, genero);
            if (nodo->genero == genero) {
                std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                          << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            }
            mostrarPorGeneroRec(nodo->der, genero);
        }
    }

    // Función para recolectar todas las películas en un vector
    void recolectarPeliculasRec(Pelicula* nodo, std::vector<Pelicula*>& peliculas) const {
        if (nodo != NULL) {
            recolectarPeliculasRec(nodo->izq, peliculas);
            peliculas.push_back(nodo);
            recolectarPeliculasRec(nodo->der, peliculas);
        }
    }

    // Función para liberar la memoria del árbol
    void liberarArbolRec(Pelicula* nodo) {
        if (nodo != NULL) {
            liberarArbolRec(nodo->izq);
            liberarArbolRec(nodo->der);
            delete nodo;
        }
    }

public:
    ArbolPeliculas() : raiz(NULL) {}

    ~ArbolPeliculas() {
        liberarArbolRec(raiz);
    }

    void insertar(std::string nombre, int anio, std::string genero, float recaudacion) {
        raiz = insertarRec(raiz, nombre, anio, genero, recaudacion);
    }

    void inOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Inorden:\n";
            inOrdenRec(raiz);
        }
    }

    void preOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Preorden:\n";
            preOrdenRec(raiz);
        }
    }

    void postOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Postorden:\n";
            postOrdenRec(raiz);
        }
    }

    void buscarPorNombre(std::string nombre) const {
        Pelicula* encontrada = buscarPorNombreRec(raiz, nombre);
        if (encontrada != NULL) {
            std::cout << "Encontrada -> Pelicula: " << encontrada->nombre << ", Ano: " << encontrada->anio
                      << ", Genero: " << encontrada->genero << ", Recaudacion: " << encontrada->recaudacion << "M\n";
        } else {
            std::cout << "Pelicula no encontrada.\n";
        }
    }

    void mostrarPorGenero(std::string genero) const {
        std::cout << "\nPeliculas del genero " << genero << ":\n";
        mostrarPorGeneroRec(raiz, genero);
    }

    void mostrarFracasos() const {
        std::vector<Pelicula*> peliculas;
        recolectarPeliculasRec(raiz, peliculas);
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

// Función para limpiar el buffer de entrada
void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Función principal con menú interactivo
int main() {
    ArbolPeliculas arbol;
    int opcion;
    std::string nombre;
    std::string genero;
    int anio;
    float recaudacion;

    do {
        std::cout << "\n=== Gestion de Peliculas (ABB)   \n";
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
            case 1: // Agregar una película
                std::cout << "Ingrese el nombre de la pelicula: ";
                std::getline(std::cin, nombre);
                if (nombre.empty()) {
                    std::cout << "El nombre no puede estar vacio.\n";
                    break;
                }

                std::cout << "Ingrese el ano de realizacion: ";
                while (!(std::cin >> anio) || anio < 1888 || anio > 2025) {
                    std::cout << "Ano invalido (debe estar entre 1888 y 2025). Intente de nuevo: ";
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

            case 5: // Buscar película por nombre
                std::cout << "Ingrese el nombre de la pelicula a buscar: ";
                std::getline(std::cin, nombre);
                arbol.buscarPorNombre(nombre);
                break;

            case 6: // Mostrar películas por género
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