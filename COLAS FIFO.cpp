#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <cassert>
#include <utility>
#include <cstddef>


template <typename T>
class ColaCircular {
private:
    std::unique_ptr<T[]> datos;

    std::size_t frente;
    std::size_t cantidad;
    std::size_t capacidad;


    void redimensionar(std::size_t nuevaCapacidad) {
        std::make_unique<T[]>(nuevaCapacidad);



        for (std::size_t i = 0; i < cantidad; ++i) {
            nuevo[i] = datos[(frente + i) % capacidad];
        }

        datos = std::move(nuevo);
        capacidad = nuevaCapacidad;
        frente = 0;
    }


    void asegurarEspacio() {
        if (cantidad == capacidad) {
            if (capacidad >
                static_cast<std::size_t>(-1) / 2) {
                    throw std:length_error(
                        "la capacidad maxima fue alcanzada."
                    );
                }

                redimensionar(capacidad * 2);
        }
    }

public:
     
     explicit colaCircular(std::size_t capacidadInicial = 4)
        : datos(nullptr),
          frente(0),
          cantidad(0),
          capacidad(capacidadInicial) {

        if (capacidadInicial == 0) {
            throw std::invalid_argument(
                "la capacidad inicial debe ser mayor que cero"
            );
        }

        datos = std::make_unique<T[]>(capacidad);
    }


    colaCircular(const colaCircular&) = delete;
    colaCircular& operator=(const colaCircular&) = delete;


    colaCircular(colaCircular&&) noexcept = default;
    colaCircular& operator=(colaCircular&&) noexcept = default;


    void enqueue(const T& elemento) {
        asegurarEspacio();

        const std::size_t final = 
            (frente + cantidad) % capacidad;

        datos[final] = elemento;
        ++cantidad;
    }


    T dequeue() {
        if (empty()) {
            throw std::underflow_error(
                "no se puede extraer: la cola esta vacia."
            );
        }

        T resultado = std::move(datos[frente]);

        frente = (frente + 1) % capacidad;
        --cantidad;


        if (cantidad == 0) {
            frente = 0;
        }

        return resultado;
    }


    const T& front() const {
        if (empty()) {
            throw std::underflow_error(
                "la cola esta vacia."
            );
        }

        return datos[frente];
    }


    const T& back() const {
        if (empty()) {
            throw std::underflow_error(
                "la cola esta vacia."
            );
        }

        const std::size_t final =
            (frente + cantidad - 1) % capacidad;

        return datos[final];
    }


    bool empty() const noexcept {
        return cantidad == 0;
    }


    std::size_t size () const noexcept {
        return cantidad;
    }


    std::size_t capacity() const noexcept {
        return capacidad;
    }


    void clear() noexcept {
        frente = 0;
        cantidad = 0;
    }


    void mostrar() const {
        std::cout << "Frente -> ";

        for (std::size_t i = 0; i < cantidad; ++i) {
            const std::size_t indice =
                (frente + i) % capacidad;

            std::cout << datos[indice];

            if (i + 1 < cantidad) {
                std::cout << " -> ";
            }
        }

        std::cout << " <- Final\n";
    }
};

//====================================================================================
//MODELO DE UNA SOLICITUD
//====================================================================================

struct Solicitud {
    int id;
    std::string cliente;
    std::string descripcion;


    friend std::ostream& operator<<(
        std::ostream& salida,
        const Solicitud& solicitud
    ) {
        salida << "[ID: " << solicitud.id
               << ", Cliente: " << solicitud.cliente
               << ", solicitud: " << solicitud.descripcion
               << "]";

        return salida;
    }
};

//====================================================================================
//PRUEBAS AUTOMATIZADAS
//====================================================================================

void ejecutarPruebas() {
    ColaCircular<int> cola(3);

    assert(cola.empty());
    assert(cola.size() == 0);

    cola.enqueue(10);
    cola.enqueue(20);
    cola.enqueue(30);

    assert(cola.size() == 3);
    assert(cola.front() == 10);
    assert(cola.back() == 30);


    cola.enqueue(40);

    assert(cola.size() == 4);
    assert(cola.front() == 10);
    assert(cola.back() == 40);


    assert(cola.dequeue() == 10);
    assert(cola.dequeue() == 20);
    assert(cola.dequeue() == 30);
    assert(cola.dequeue() == 40);

    assert(cola.empty());


    bool excepcionDetectada = false;

    try {
        cola.dequeue();    
    } catch (const std::underflow_error&) {
        excepcionDetectada = true;
    }

    assert(excepcionDetectada);

    std::cout
        << "\nTodas as pruebas finalizaron correctamente. \n";
}

//====================================================================================
//PROGRAMA PRINCIPAL
//====================================================================================

int main() {
    std::cout
        << "====================================================================================\n"
        << " SIMULADOR DE COLA FIFO EN C++17\n"
        << "====================================================================================\n\n";


    ColaCircular<Solicitud> colaSolicitudes(3);


    colaSolicitudes.enqueue({
        101, "ana", "Restablecer contraseña"
    });

    colaSolicitudes.enqueue({
        102, "carlos", "Error de conexion"
    });

    colaSolicitudes.enqueue({
        103, "Maria", "Actualizar informacion"
    });

    std::cout << "Solicitudes iniciales:\n";
    colaSolicitudes.mostrar();


    std::cout << "\nAtendiendo la primera solicitud:\n";
    std::cout << colaSolicitudes.dequeue() << '\n';


    colaSolicitudes.enqueue({
        104, "Luis", "problema de facturacion"
    });

    colaSolicitudes.enqueue({
        105, "sofia", "consulta de servicio"
    });

    std::cout << "\nEstado despues de nuevas llegadas:\n";
    colaSolicitudes.mostrar();


    std::cout << "\nProcesamiento FIFO:\n";

    while (!colaSolicitudes.empty()) {
        Solicitud actual = colaSolicitudes.dequeue();

        std::cout << "procesando: "
                  << actual << '\n';
    }

    std::cout << "\nCola vacia: "
              <<std::boolalpha
              << colaSolicitudes.empty()
              << '\n';


              ejecutarPruebas();

              return 0;
}