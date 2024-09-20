#ifndef EPISODIO_HPP
#define EPISODIO_HPP

#include <string>
#include <vector>
#include <fstream>

class Episodio {
    std::string id;
    std::string titulo;
    int temporada;
    double duracion;
    std::vector<double> calificaciones;

public:
    Episodio(std::string id, std::string titulo, int temporada, double duracion);

    std::string getId() const;
    std::string getTitulo() const;
    int getTemporada() const;
    double getDuracion() const;
    double getCalificacionPromedio() const;
    void agregarCalificacion(double calificacion);
    std::string obtenerCalificacionesComoString() const;
    void cargarCalificacionesDesdeString(const std::string& str);

    void mostrarInfo() const;
    void mostrarIdNombreYCalificacion() const;
    void guardar(std::ofstream& file, const std::string& serieId) const;
};

#endif // EPISODIO_HPP
