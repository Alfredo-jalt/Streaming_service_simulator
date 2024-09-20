#include <iostream>
#include <limits>
#include "ServicioStreaming.hpp"

void mostrarMenu() {
    std::cout << "Menu:\n";
    std::cout << "1. Ver datos de todos los videos\n";
    std::cout << "2. Ver datos de todas las peliculas\n";
    std::cout << "3. Ver episodios de una serie\n";
    std::cout << "4. Agregar una nueva pelicula\n";
    std::cout << "5. Agregar una nueva serie\n";
    std::cout << "6. Cambiar calificacion de un video\n";
    std::cout << "7. Buscar peliculas y series por calificacion\n";
    std::cout << "8. Guardar y salir\n";
    std::cout << "Seleccione una opcion: ";
}

int main() {
    ServicioStreaming servicio;
    servicio.cargarDesdeCSV("peliculas.csv", "series.csv", "episodios.csv");

    int opcion;
    do {
        mostrarMenu();
        while (!(std::cin >> opcion)) {
            std::cout << "Entrada invalida. Por favor ingrese un numero: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        switch (opcion) {
            case 1:
                servicio.mostrarVideos();
                break;
            case 2:
                servicio.mostrarPeliculas();
                break;
            case 3: {
                std::cout << "Series disponibles:\n";
                servicio.mostrarListaSeries();
                std::string idSerie;
                std::cout << "Ingrese el ID de la serie para ver sus episodios: ";
                std::cin >> idSerie;
                servicio.mostrarEpisodiosDeSeriePorID(idSerie);
                break;
            }
            case 4:
                servicio.agregarPelicula();
                break;
            case 5:
                servicio.agregarSerie();
                break;
            case 6:
                servicio.cambiarCalificacion();
                break;
            case 7:
                servicio.buscarPorCalificacion();
                break;
            case 8:
                servicio.guardarEnCSV("peliculas.csv", "series.csv", "episodios.csv");
                std::cout << "Datos guardados. Saliendo...\n";
                break;
            default:
                std::cout << "Opcion invalida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 8);

    return 0;
}
