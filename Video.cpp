#include "Video.hpp"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>

Video::Video(std::string id, std::string nombre, double duracion, std::string genero)
    : id(id), nombre(nombre), duracion(duracion), genero(genero) {}

Video::~Video() {}

std::string Video::getId() const {
    return id;
}

std::string Video::getNombre() const {
    return nombre;
}

double Video::getDuracion() const {
    return duracion;
}

std::string Video::getGenero() const {
    return genero;
}

double Video::getCalificacionPromedio() const {
    if (calificaciones.empty()) return 0;
    double sum = 0;
    for (double cal : calificaciones) {
        sum += cal;
    }
    double promedio = sum / calificaciones.size();
    return round(promedio * 10) / 10; // Redondear a un decimal
}

void Video::agregarCalificacion(double calificacion) {
    if (calificacion < 0 || calificacion > 5 || floor(calificacion * 10) != calificacion * 10) {
        throw std::invalid_argument("La calificación debe estar entre 0 y 5 con un solo decimal.");
    }
    calificaciones.push_back(calificacion);
}

std::string Video::obtenerCalificacionesComoString() const {
    std::stringstream ss;
    for (size_t i = 0; i < calificaciones.size(); ++i) {
        if (i != 0) ss << ";";
        ss << calificaciones[i];
    }
    return ss.str();
}

void Video::cargarCalificacionesDesdeString(const std::string& str) {
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
