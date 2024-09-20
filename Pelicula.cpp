#include "Pelicula.hpp"
#include <iostream>
#include <iomanip>

Pelicula::Pelicula(std::string id, std::string nombre, double duracion, std::string genero)
    : Video(id, nombre, duracion, genero) {}

void Pelicula::mostrarInfo() const {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Pelicula: " << nombre << " (ID: " << id
              << ", Duracion: " << duracion << " minutos, Genero: " << genero
              << ", Calificacion: " << getCalificacionPromedio() << ")\n";
}

void Pelicula::mostrarIdNombreYCalificacion() const {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "ID: " << id << ", Nombre: " << nombre << ", Calificacion: " << getCalificacionPromedio() << "\n";
}

void Pelicula::guardar(std::ofstream& file) const {
    file << id << "," << nombre << "," << duracion << "," << genero << "," << obtenerCalificacionesComoString() << "\n";
}
