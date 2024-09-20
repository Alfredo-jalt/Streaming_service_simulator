#include "ServicioStreaming.hpp"
#include <iostream>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <cmath>
#include <algorithm>

ServicioStreaming::~ServicioStreaming() {
    for (auto video : videos) {
        delete video;
    }
}

void ServicioStreaming::agregarVideo(Video* video) {
    videos.push_back(video);
}

bool ServicioStreaming::idExiste(const std::string& id) const {
    for (const auto& video : videos) {
        if (video->getId() == id) {
            return true;
        }
    }
    return false;
}

void ServicioStreaming::cargarDesdeCSV(const std::string& archivoPeliculas, const std::string& archivoSeries, const std::string& archivoEpisodios) {
    std::ifstream filePeliculas(archivoPeliculas);
    std::string line;
    while (std::getline(filePeliculas, line)) {
        if (line.empty() || line.find("ID") == 0) continue;
        std::stringstream ss(line);
        std::string id, nombre, duracion, genero, calificacionesStr;
        std::getline(ss, id, ',');
        std::getline(ss, nombre, ',');
        std::getline(ss, duracion, ',');
        std::getline(ss, genero, ',');
        std::getline(ss, calificacionesStr, ',');

        try {
            Pelicula* pelicula = new Pelicula(id, nombre, std::stod(duracion), genero);
            pelicula->cargarCalificacionesDesdeString(calificacionesStr);
            agregarVideo(pelicula);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error al convertir cadena a numero en peliculas.csv: " << e.what() << std::endl;
            continue;
        }
    }

    std::ifstream fileSeries(archivoSeries);
    while (std::getline(fileSeries, line)) {
        if (line.empty() || line.find("ID") == 0) continue;
        std::stringstream ss(line);
        std::string id, nombre, numTemporadas, genero, calificacionesStr;
        std::getline(ss, id, ',');
        std::getline(ss, nombre, ',');
        std::getline(ss, numTemporadas, ',');
        std::getline(ss, genero, ',');
        std::getline(ss, calificacionesStr, ',');

        Serie* serie = nullptr;
        try {
            serie = new Serie(id, nombre, std::stoi(numTemporadas), genero);
            serie->cargarCalificacionesDesdeString(calificacionesStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error al convertir cadena a numero en series.csv: " << e.what() << std::endl;
            continue;
        }

        std::ifstream fileEpisodios(archivoEpisodios);
        while (std::getline(fileEpisodios, line)) {
            if (line.empty() || line.find("ID Serie") == 0) continue;
            std::stringstream ss(line);
            std::string idSerie, idEpisodio, titulo, temporada, duracionEp, calificacionesStrEp;
            std::getline(ss, idSerie, ',');
            std::getline(ss, idEpisodio, ',');
            std::getline(ss, titulo, ',');
            std::getline(ss, temporada, ',');
            std::getline(ss, duracionEp, ',');
            std::getline(ss, calificacionesStrEp, ',');

            if (idSerie == id) {
                try {
                    Episodio episodio(idEpisodio, titulo, std::stoi(temporada), std::stod(duracionEp));
                    episodio.cargarCalificacionesDesdeString(calificacionesStrEp);
                    serie->agregarEpisodio(episodio);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error al convertir cadena a numero en episodios.csv: " << e.what() << std::endl;
                    continue;
                }
            }
        }
        agregarVideo(serie);
    }
}

void ServicioStreaming::guardarEnCSV(const std::string& archivoPeliculas, const std::string& archivoSeries, const std::string& archivoEpisodios) const {
    std::ofstream filePeliculas(archivoPeliculas);
    filePeliculas << "ID,Nombre,Duracion,Genero,Calificaciones\n";
    std::ofstream fileSeries(archivoSeries);
    fileSeries << "ID,Nombre,NumTemporadas,Genero,Calificaciones\n";
    std::ofstream fileEpisodios(archivoEpisodios);
    fileEpisodios << "ID Serie,ID Episodio,Titulo,Temporada,Duracion,Calificaciones\n";

    for (const auto& video : videos) {
        Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
        if (pelicula) {
            pelicula->guardar(filePeliculas);
        }

        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie) {
            serie->guardar(fileSeries);
            serie->guardarEpisodios(fileEpisodios);
        }
    }
}

void ServicioStreaming::mostrarVideos() const {
    for (const auto& video : videos) {
        video->mostrarInfo();
    }
}

void ServicioStreaming::mostrarPeliculas() const {
    bool peliculasEncontradas = false;
    for (const auto& video : videos) {
        Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
        if (pelicula) {
            pelicula->mostrarInfo();
            peliculasEncontradas = true;
        }
    }
    if (!peliculasEncontradas) {
        std::cout << "No hay peliculas disponibles.\n";
    }
}

void ServicioStreaming::mostrarListaSeries() const {
    for (const auto& video : videos) {
        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie) {
            serie->mostrarInfoBasica();
        }
    }
}

void ServicioStreaming::mostrarEpisodiosDeSeriePorID(const std::string& idSerie) const {
    for (const auto& video : videos) {
        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie && serie->getId() == idSerie) {
            serie->mostrarInfo();
            return;
        }
    }
    std::cout << "Serie no encontrada.\n";
}

void ServicioStreaming::agregarPelicula() {
    std::string id, nombre, genero;
    double duracion, calificacion;
    std::cout << "Ingrese ID de la pelicula: ";
    std::cin >> id;
    if (idExiste(id)) {
        std::cout << "El ID ya existe. No se puede agregar la pelicula.\n";
        return;
    }
    std::cout << "Ingrese nombre de la pelicula: ";
    std::cin.ignore();
    std::getline(std::cin, nombre);
    std::cout << "Ingrese duracion de la pelicula (minutos): ";
    while (!(std::cin >> duracion)) {
        std::cout << "Entrada invalida. Por favor ingrese un numero: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Ingrese genero de la pelicula: ";
    std::cin.ignore();
    std::getline(std::cin, genero);
    std::cout << "Ingrese calificacion de la pelicula: ";
    while (!(std::cin >> calificacion) || calificacion < 0 || calificacion > 5 || std::floor(calificacion * 10) != calificacion * 10) {
        std::cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    Pelicula* pelicula = new Pelicula(id, nombre, duracion, genero);
    pelicula->agregarCalificacion(calificacion);
    agregarVideo(pelicula);
}

void ServicioStreaming::agregarSerie() {
    std::string id, nombre, genero;
    int numTemporadas, numEpisodios;
    double calificacion;
    std::cout << "Ingrese ID de la serie: ";
    std::cin >> id;
    if (idExiste(id)) {
        std::cout << "El ID ya existe. No se puede agregar la serie.\n";
        return;
    }
    std::cout << "Ingrese nombre de la serie: ";
    std::cin.ignore();
    std::getline(std::cin, nombre);
    std::cout << "Ingrese genero de la serie: ";
    std::getline(std::cin, genero);
    std::cout << "Ingrese calificacion de la serie: ";
    while (!(std::cin >> calificacion) || calificacion < 0 || calificacion > 5 || std::floor(calificacion * 10) != calificacion * 10) {
        std::cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Ingrese numero de temporadas: ";
    while (!(std::cin >> numTemporadas)) {
        std::cout << "Entrada invalida. Por favor ingrese un numero: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Ingrese numero de episodios: ";
    while (!(std::cin >> numEpisodios)) {
        std::cout << "Entrada invalida. Por favor ingrese un numero: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    Serie* serie = new Serie(id, nombre, numTemporadas, genero);
    serie->agregarCalificacion(calificacion);

    for (int i = 1; i <= numEpisodios; ++i) {
        std::string idEpisodio, titulo;
        int temporada;
        double duracion, calificacionEp;
        std::cout << "Ingrese ID del episodio " << i << ": ";
        std::cin >> idEpisodio;
        std::cout << "Ingrese titulo del episodio " << i << ": ";
        std::cin.ignore();
        std::getline(std::cin, titulo);
        std::cout << "Ingrese temporada del episodio " << i << ": ";
        while (!(std::cin >> temporada)) {
            std::cout << "Entrada invalida. Por favor ingrese un numero: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Ingrese duracion del episodio " << i << " (minutos): ";
        while (!(std::cin >> duracion)) {
            std::cout << "Entrada invalida. Por favor ingrese un numero: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Ingrese calificacion del episodio " << i << ": ";
        while (!(std::cin >> calificacionEp) || calificacionEp < 0 || calificacionEp > 5 || std::floor(calificacionEp * 10) != calificacionEp * 10) {
            std::cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        Episodio episodio(idEpisodio, titulo, temporada, duracion);
        episodio.agregarCalificacion(calificacionEp);
        serie->agregarEpisodio(episodio);
    }

    agregarVideo(serie);
}

void ServicioStreaming::cambiarCalificacion() {
    std::cout << "Seleccione el tipo de video:\n";
    std::cout << "1. Pelicula\n";
    std::cout << "2. Serie\n";
    std::cout << "3. Episodio\n";
    int tipoOpcion;
    while (!(std::cin >> tipoOpcion) || tipoOpcion < 1 || tipoOpcion > 3) {
        std::cout << "Entrada invalida. Por favor ingrese 1, 2, o 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string id;
    double nuevaCalificacion;
    bool encontrado = false;

    if (tipoOpcion == 1) {
        std::cout << "Peliculas disponibles (ID, Nombre y Calificacion):\n";
        for (const auto& video : videos) {
            Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
            if (pelicula) {
                pelicula->mostrarIdNombreYCalificacion();
            }
        }
        std::cout << "Ingrese el ID de la pelicula: ";
        std::cin >> id;
        for (auto& video : videos) {
            Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
            if (pelicula && pelicula->getId() == id) {
                std::cout << "Ingrese la nueva calificacion (0-5 con un solo decimal): ";
                while (!(std::cin >> nuevaCalificacion) || nuevaCalificacion < 0 || nuevaCalificacion > 5 || std::floor(nuevaCalificacion * 10) != nuevaCalificacion * 10) {
                    std::cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                pelicula->agregarCalificacion(nuevaCalificacion);
                std::cout << "Calificacion actualizada correctamente.\n";
                encontrado = true;
                break;
            }
        }
    } else if (tipoOpcion == 2) {
        std::cout << "Series disponibles (ID, Nombre y Calificacion):\n";
        for (const auto& video : videos) {
            Serie* serie = dynamic_cast<Serie*>(video);
            if (serie) {
                serie->mostrarIdNombreYCalificacion();
            }
        }
        std::cout << "Ingrese el ID de la serie: ";
        std::cin >> id;
        for (auto& video : videos) {
            Serie* serie = dynamic_cast<Serie*>(video);
            if (serie && serie->getId() == id) {
                std::cout << "Ingrese la nueva calificacion (0-5 con un solo decimal): ";
                while (!(std::cin >> nuevaCalificacion) || nuevaCalificacion < 0 || nuevaCalificacion > 5 || std::floor(nuevaCalificacion * 10) != nuevaCalificacion * 10) {
                    std::cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                serie->agregarCalificacion(nuevaCalificacion);
                std::cout << "Calificacion actualizada correctamente.\n";
                encontrado = true;
                break;
            }
        }
    } else if (tipoOpcion == 3) {
        std::cout << "Series disponibles (ID, Nombre y Calificacion):\n";
        for (const auto& video : videos) {
            Serie* serie = dynamic_cast<Serie*>(video);
            if (serie) {
                serie->mostrarIdNombreYCalificacion();
            }
        }
        std::cout << "Ingrese el ID de la serie: ";
        std::cin >> id;
        Serie* serie = nullptr;
        for (auto& video : videos) {
            serie = dynamic_cast<Serie*>(video);
            if (serie && serie->getId() == id) {
                break;
            }
        }

        if (serie) {
            std::cout << "Episodios disponibles (ID, Titulo y Calificacion):\n";
            for (const auto& episodio : serie->getEpisodios()) {
                episodio.mostrarIdNombreYCalificacion();
            }
            std::cout << "Ingrese el ID del episodio: ";
            std::cin >> id;
            for (auto& episodio : serie->getEpisodios()) {
                if (episodio.getId() == id) {
                    std::cout << "Ingrese la nueva calificacion (0-5 con un solo decimal): ";
                    while (!(std::cin >> nuevaCalificacion) || nuevaCalificacion < 0 || nuevaCalificacion > 5 || std::floor(nuevaCalificacion * 10) != nuevaCalificacion * 10) {
                        std::cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    episodio.agregarCalificacion(nuevaCalificacion);
                    std::cout << "Calificacion del episodio actualizada correctamente.\n";
                    encontrado = true;
                    break;
                }
            }
        }
    }

    if (!encontrado) {
        std::cout << "No se encontro el video con el ID proporcionado.\n";
    }
}

void ServicioStreaming::buscarPorCalificacion() {
    double calificacionBuscada;
    std::cout << "Ingrese la calificacion a buscar (0-5 con un solo decimal): ";
    while (!(std::cin >> calificacionBuscada) || calificacionBuscada < 0 || calificacionBuscada > 5 || std::floor(calificacionBuscada * 10) != calificacionBuscada * 10) {
        std::cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    bool videosEncontrados = false;
    std::cout << "Videos con calificacion " << calificacionBuscada << ":\n";
    for (const auto& video : videos) {
        if (video->getCalificacionPromedio() == calificacionBuscada) {
            video->mostrarInfo();
            videosEncontrados = true;
        }
    }

    if (!videosEncontrados) {
        std::cout << "No se encontraron videos con la calificacion " << calificacionBuscada << ".\n";
    }
}
