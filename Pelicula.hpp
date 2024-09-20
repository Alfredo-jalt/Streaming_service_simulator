#ifndef PELICULA_HPP
#define PELICULA_HPP

#include "Video.hpp"
#include <fstream>

class Pelicula : public Video {
public:
    Pelicula(std::string id, std::string nombre, double duracion, std::string genero);
    void mostrarInfo() const override;
    void mostrarIdNombreYCalificacion() const override;
    void guardar(std::ofstream& file) const override;
};

#endif // PELICULA_HPP
