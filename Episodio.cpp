#include "Episodio.hpp"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>

Episodio::Episodio(std::string id, std::string titulo, int temporada, double duracion)
    : id(id), titulo(titulo), temporada(temporada), duracion(duracion) {}

std::string Episodio::getId() const {
    return id;
}

std::string Episodio::getTitulo() const {
    return titulo;
}

int Episodio::getTemporada() const {
    return temporada;
}

double Episodio::getDuracion() const {
    return duracion;
}

double Episodio::getCalificacionPromedio() const {
    if (calificaciones.empty()) return 0;
    double sum = 0;
    for (double cal : calificaciones) {
        sum += cal;
    }
    double promedio = sum / calificaciones.size();
    return round(promedio * 10) / 10; // Redondear a un decimal
}

void Episodio::agregarCalificacion(double calificacion) {
    if (calificacion < 0 || calificacion > 5 || floor(calificacion * 10) != calificacion * 10) {
        throw std::invalid_argument("La calificación debe estar entre 0 y 5 con un solo decimal.");
    }
    calificaciones.push_back(calificacion);
}

std::string Episodio::obtenerCalificacionesComoString() const {
    std::stringstream ss;
    for (size_t i = 0; i < calificaciones.size(); ++i) {
        if (i != 0) ss << ";";
        ss << calificaciones[i];
    }
    return ss.str();
}

void Episodio::cargarCalificacionesDesdeString(const std::string& str) {
    std::stringstream ss(str);
    std::string cal;
    while (std::getline(ss, cal, ';')) {
        try {
            agregarCalificacion(std::stod(cal));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error al convertir cadena a número en calificaciones: " << e.what() << std::endl;
        }
    }
}

void Episodio::mostrarInfo() const {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Episodio: " << titulo << " (ID: " << id
              << ", Temporada: " << temporada
              << ", Duracion: " << duracion << " minutos, Calificacion: " << getCalificacionPromedio() << ")\n";
}

void Episodio::mostrarIdNombreYCalificacion() const {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "ID: " << id << ", Titulo: " << titulo << ", Calificacion: " << getCalificacionPromedio() << "\n";
}

void Episodio::guardar(std::ofstream& file, const std::string& serieId) const {
    file << serieId << "," << id << "," << titulo << "," << temporada << "," << duracion << "," << obtenerCalificacionesComoString() << "\n";
}
