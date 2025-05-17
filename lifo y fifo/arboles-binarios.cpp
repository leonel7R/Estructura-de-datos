// edwin leonel baltan carabali lio
//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <malloc.h>
#include <limits>

// Estructura para representar una pelicula en el Arbol Binario de busqueda
struct Pelicula {
    std::string nombre;      // Nombre de la pelicula
    int anio;               // Ano de estreno
    std::string genero;     // Genero de la pelicula
    float recaudacion;      // Recaudacion en millones de dolares
    Pelicula* izq;          // Puntero al hijo izquierdo
    Pelicula* der;          // Puntero al hijo derecho

    // Constructor para inicializar una pelicula
    Pelicula(std::string nombre, int anio, std::string genero, float recaudacion)
        : nombre(nombre), anio(anio), genero(genero), recaudacion(recaudacion), izq(NULL), der(NULL) {}
};

//Clase para gestionar un arbol Binario de Busqueda de peliculas
class ArbolPeliculas {
private:
    Pelicula* raiz; //Raiz del arbol

    //Inserta una pelicula recursivamente, ordenando por año
    Pelicula* insertar(Pelicula* nodo, std::string nombre, int anio, std::string genero, float recaudacion) {
        //Caso base: crear un nuevo nodo si el subarbol esta vacio
        if (nodo == NULL) {
            Pelicula* nuevo = (Pelicula*)malloc(sizeof(Pelicula));
            if (nuevo == NULL) {
                std::cerr << "Error: No se pudo asignar ." << std::endl;
                return NULL;  
            }
            // Inicializar manualmente los campos
            nuevo->nombre = nombre;
            nuevo->anio = anio;
            nuevo->genero = genero;
            nuevo->recaudacion = recaudacion;
            nuevo->izq = NULL;
            nuevo->der = NULL;
            return nuevo;
        }
        //Insertar en el subrbol izquierdo si el año es menor o igual
        if (anio <= nodo->anio) {
            nodo->izq = insertar(nodo->izq, nombre, anio, genero, recaudacion);
        } 
        // Insertar en el sub arbol derecho si el año es mayor
        else {
            nodo->der = insertar(nodo->der, nombre, anio, genero, recaudacion);
        }
        return nodo;
    }

    // Recorre el arbol en inorden (izquierda, raiz, derecha)
    void inOrden(Pelicula* nodo) const {
        if (nodo != NULL) {
            inOrden(nodo->izq);
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            inOrden(nodo->der);
        }
    }

    //Recorre el arbol en preorden (raiz, izquierda, derecha)
    void preOrden(Pelicula* nodo) const {
        if (nodo != NULL) {
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
            preOrden(nodo->izq);
            preOrden(nodo->der);
        }
    }

    // Recorre el arbol en postorden (izquierda, derecha, raiz)
    void postOrden(Pelicula* nodo) const {
        if (nodo != NULL) {
            postOrden(nodo->izq);
            postOrden(nodo->der);
            std::cout << "Pelicula: " << nodo->nombre << ", Ano: " << nodo->anio
                      << ", Genero: " << nodo->genero << ", Recaudacion: " << nodo->recaudacion << "M\n";
        }
    }

    //Busca una pelicula por nombre en el arbol
    Pelicula* buscarPorNombre(Pelicula* nodo, std::string nombre) const {
        if (nodo == NULL) {
            return NULL;
        }
        if (nodo->nombre == nombre) {
            return nodo;
        }
        // Buscar en el subarbol izquierdo
        Pelicula* encontrada = buscarPorNombre(nodo->izq, nombre);
        if (encontrada != NULL) {
            return encontrada;
        }
        // Buscar en el sub arbol derecho
        return buscarPorNombre(nodo->der, nombre);
    }

    //Muestra todas las peliculas de un genero especifico
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

    //Recolecta todas las peliculas en un vector para ordenarlas
    void recolectarPeliculas(Pelicula* nodo, std::vector<Pelicula*>& peliculas) const {
        if (nodo != NULL) {
            recolectarPeliculas(nodo->izq, peliculas);
            peliculas.push_back(nodo);
            recolectarPeliculas(nodo->der, peliculas);
        }
    }

    //Busca una pelicula por nombre y devuelve su nodo y el nodo padre
    Pelicula* buscarConPadrePorNombre(Pelicula* nodo, std::string nombre, Pelicula** padre) const {
        *padre = NULL;
        Pelicula* actual = nodo;
        while (actual != NULL) {
            if (actual->nombre == nombre) {
                return actual;
            }
            *padre = actual;
            // Buscar en el subarbol izquierdo
            Pelicula* encontrada = buscarPorNombre(actual->izq, nombre);
            if (encontrada != NULL) {
                return encontrada;
            }
            // Continuar en el subrbol derecho
            actual = actual->der;
        }
        return NULL;
    }

    // Encuentra el nodo con el año minimo en un subarbol sucesor para eliminacion
    Pelicula* encontrarMinimo(Pelicula* nodo, Pelicula** padreMin) const {
        *padreMin = NULL;
        while (nodo->izq != NULL) {
            *padreMin = nodo;
            nodo = nodo->izq;
        }
        return nodo;
    }

    // Elimina una pelicula por nombre, manejando todos los casos
    Pelicula* eliminar(Pelicula* nodo, std::string nombre, bool& eliminado) {
        if (nodo == NULL) {
            eliminado = false;
            return nodo;
        }

        // Buscar el nodo a eliminar y su padre
        Pelicula* padre = NULL;
        Pelicula* objetivo = buscarConPadrePorNombre(nodo, nombre, &padre);
        if (objetivo == NULL) {
            eliminado = false;
            return nodo;
        }

        // Determinar el hijo o NULL si es hoja
        Pelicula* hijo = (objetivo->izq != NULL) ? objetivo->izq : objetivo->der;

        // Caso: Nodo con dos hijos
        if (objetivo->izq != NULL && objetivo->der != NULL) {
            Pelicula* padreSucesor = NULL;
            Pelicula* sucesor = encontrarMinimo(objetivo->der, &padreSucesor);
            //Copia los datos del sucesor al nodo objetivo
            objetivo->nombre = sucesor->nombre;
            objetivo->anio = sucesor->anio;
            objetivo->genero = sucesor->genero;
            objetivo->recaudacion = sucesor->recaudacion;

            // Ajustar punteros para eliminar el sucesor
            if (padreSucesor == NULL) {
                objetivo->der = sucesor->der;
            } else {
                padreSucesor->izq = sucesor->der;
            }
            // Liberar memoria del sucesor
            sucesor->~Pelicula();
            free(sucesor);
            eliminado = true;
            return nodo;
        }
 
        // Ajustar el puntero del padre o raiz si es el nodo raiz
        if (padre == NULL) {
            raiz = hijo;
        } else if (padre->izq == objetivo) {
            padre->izq = hijo;
        } else {
            padre->der = hijo;
        }

        // Liberar memoria del nodo objetivo
        objetivo->~Pelicula();
        free(objetivo);
        eliminado = true;
        return nodo;
    }

    //Libera toda la memoria del arbol recursivamente
    void liberarArbol(Pelicula* nodo) {
        if (nodo != NULL) {
            liberarArbol(nodo->izq);
            liberarArbol(nodo->der);
            nodo->~Pelicula();
            free(nodo);
        }
    }

public:
    // Constructor: inicializa el arbol vacio
    ArbolPeliculas() : raiz(NULL) {}

    // Destructor: libera toda la memoria del arbol
    ~ArbolPeliculas() {
        liberarArbol(raiz);
    }

    // Inserta una nueva pelicula, verificar que el nombre no exista
    void insertar(std::string nombre, int anio, std::string genero, float recaudacion) {
        // Verificar si el nombre ya existe
        if (buscarPorNombre(raiz, nombre) != NULL) {
            std::cout << "Error: La pelicula '" << nombre << "' ya existe en el arbol.\n";
            return;
        }
        //Insertar la pelicula si el nombre es unico
        Pelicula* nuevoNodo = insertar(raiz, nombre, anio, genero, recaudacion);
        if (nuevoNodo == NULL) {
            std::cout << "Error al insertar la pelicula:\n";
            return;
        }
        raiz = nuevoNodo;
        std::cout << "Pelicula agregada exitosamente.\n";
    }

    //Muestra el arbol en recorrido inorden
    void inOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Inorden:\n";
            inOrden(raiz);
        }
    }

    // Muestra el arbol en recorrido preorden
    void preOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Preorden:\n";
            preOrden(raiz);
        }
    }

    //Muestra el arbol en recorrido postorden
    void postOrden() const {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio.\n";
        } else {
            std::cout << "\nRecorrido Postorden:\n";
            postOrden(raiz);
        }
    }

    // Busca una pelicula por nombre y muestra sus detalles
    void buscarPorNombre(std::string nombre) const {
        Pelicula* encontrada = buscarPorNombre(raiz, nombre);
        if (encontrada != NULL) {
            std::cout << "Encontrada -> Pelicula: " << encontrada->nombre << ", Ano: " << encontrada->anio
                      << ", Genero: " << encontrada->genero << ", Recaudacion: " << encontrada->recaudacion << "M\n";
        } else {
            std::cout << "Pelicula no encontrada.\n";
        }
    }

    // Muestra todas las peliculas de un genero
    void mostrarPorGenero(std::string genero) const {
        std::cout << "\nPeliculas del genero " << genero << ":\n";
        mostrarPorGenero(raiz, genero);
    }

    // Muestra las tres peliculas con menor recaudacion
    void mostrarFracasos() const {
        std::vector<Pelicula*> peliculas;
        recolectarPeliculas(raiz, peliculas);
        if (peliculas.empty()) {
            std::cout << "No hay peliculas en el arbol.\n";
            return;
        }
        // Ordenar por recaudacion ascendente
        std::sort(peliculas.begin(), peliculas.end(), 
                  [](const Pelicula* a, const Pelicula* b) { return a->recaudacion < b->recaudacion; });
        std::cout << "\nLos 3 fracasos taquilleros:\n";
        for (size_t i = 0; i < 3 && i < peliculas.size(); i++) {
            std::cout << i + 1 << ". Pelicula: " << peliculas[i]->nombre << ", Ano: " << peliculas[i]->anio
                      << ", Genero: " << peliculas[i]->genero << ", Recaudacion: " << peliculas[i]->recaudacion << "M\n";
        }
    }

    //Elimina una pelicula por nombre
    void eliminarPorNombre(std::string nombre) {
        bool eliminado = false;
        raiz = eliminar(raiz, nombre, eliminado);
        if (eliminado) {
            std::cout << "Pelicula '" << nombre << "' eliminada exitosamente.\n";
        } else {
            std::cout << "Pelicula '" << nombre << "' no encontrada.\n";
        }
    }
};

// Limpia el buffer de entrada para evitar problemas
void limpiarBuffer() {
    std::cin.clear(); // Limpia el estado de error
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
}

// Programa principal con menu
int main() {
    ArbolPeliculas arbol;
    int opcion;
    std::string nombre;
    std::string genero;
    int anio;
    float recaudacion;

    do {
        // Mostrar menu
        std::cout << "\n=== Gestion de Peliculas (Arbol Binario de Busqueda) ===\n";
        std::cout << "1. Agregar una pelicula\n";
        std::cout << "2. Mostrar recorrido inorden\n";
        std::cout << "3. Mostrar recorrido preorden\n";
        std::cout << "4. Mostrar recorrido postorden\n";
        std::cout << "5. Buscar pelicula por nombre\n";
        std::cout << "6. Mostrar peliculas por genero\n";
        std::cout << "7. Mostrar los 3 fracasos taquilleros\n";
        std::cout << "8. Eliminar pelicula por nombre\n";
        std::cout << "9. Salir\n";
        std::cout << "Seleccione una opcion: ";

        // Validar entrada numerica
        std::cin >> opcion;
        if (std::cin.fail()) {
            std::cout << "Entrada invalida. Ingrese un numero.\n";
            limpiarBuffer();
            continue; // Volver a mostrar el menu
        }
        limpiarBuffer(); // Limpiar buffer despues de leer el numero

        switch (opcion) {
            case 1: // Agregar una pelicula
                std::cout << "Ingrese el nombre de la pelicula: ";
                std::getline(std::cin, nombre);
                if (nombre.empty()) {
                    std::cout << "El nombre no puede estar vacio.\n";
                    break;
                }

                std::cout << "Ingrese el ano de realizacion: ";
                std::cin >> anio;
                if (std::cin.fail() || anio < 1700 || anio > 2025) {
                    std::cout << "Ano invalido (debe estar entre 1700 y 2025).\n";
                    limpiarBuffer();
                    break;
                }
                limpiarBuffer();

                std::cout << "Ingrese el genero: ";
                std::getline(std::cin, genero);
                if (genero.empty()) {
                    std::cout << "El genero no puede estar vacio.\n";
                    break;
                }

                std::cout << "Ingrese la recaudacion en millones de dolares: ";
                std::cin >> recaudacion;
                if (std::cin.fail() || recaudacion < 0) {
                    std::cout << "Recaudacion invalida debe ser >= 0.\n";
                    limpiarBuffer();
                    break;
                }
                limpiarBuffer();

                // Insertar la pelicula verifica duplicados internamente
                arbol.insertar(nombre, anio, genero, recaudacion);
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

            case 7: //Mostrar los 3 fracasos taquilleros
                arbol.mostrarFracasos();
                break;

            case 8: // Eliminar pelcula por nombre
                std::cout << "Ingrese el nombre de la pelicula a eliminar: ";
                std::getline(std::cin, nombre);
                arbol.eliminarPorNombre(nombre);
                break;

            case 9: // Salir
                std::cout << "Saliendo del programa...\n";
                std::cout << "Presione Enter para continuar...\n";
                std::cin.get(); // Pausar
                break;

            default:
                std::cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 9);

    return 0;
}