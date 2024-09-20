#include "Serie.hpp"
#include <iostream>
#include <iomanip>

Serie::Serie(std::string id, std::string nombre, int numTemporadas, std::string genero)
    : Video(id, nombre, 0, genero), numTemporadas(numTemporadas) {}

int Serie::getNumTemporadas() const {
    return numTemporadas;
}

void Serie::agregarEpisodio(const Episodio& episodio) {
    episodios.push_back(episodio);
}

void Serie::mostrarInfo() const {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Serie: " << nombre << " (ID: " << id
              << ", Genero: " << genero
              << ", Numero de temporadas: " << numTemporadas
              << ", Numero de episodios: " << episodios.size()
              << ", Calificacion: " << getCalificacionPromedio() << ")\n";
    for (const auto& episodio : episodios) {
        episodio.mostrarInfo();
    }
}

void Serie::mostrarInfoBasica() const {
    std::cout << "Serie: " << nombre << " (ID: " << id << ")\n";
}

void Serie::mostrarIdNombreYCalificacion() const {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "ID: " << id << ", Nombre: " << nombre << ", Calificacion: " << getCalificacionPromedio() << "\n";
}

void Serie::guardar(std::ofstream& file) const {
    file << id << "," << nombre << "," << numTemporadas << "," << genero << "," << obtenerCalificacionesComoString() << "\n";
}

void Serie::guardarEpisodios(std::ofstream& file) const {
    for (const auto& episodio : episodios) {
        episodio.guardar(file, id);
    }
}

std::vector<Episodio>& Serie::getEpisodios() {
    return episodios;
}
