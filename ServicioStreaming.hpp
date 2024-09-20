#ifndef SERVICIOSTREAMING_HPP
#define SERVICIOSTREAMING_HPP

#include "Video.hpp"
#include "Pelicula.hpp"
#include "Serie.hpp"
#include "Episodio.hpp"
#include <vector>
#include <string>
#include <fstream>

class ServicioStreaming {
    std::vector<Video*> videos;

public:
    ~ServicioStreaming();
    void agregarVideo(Video* video);
    bool idExiste(const std::string& id) const;
    void cargarDesdeCSV(const std::string& archivoPeliculas, const std::string& archivoSeries, const std::string& archivoEpisodios);
    void guardarEnCSV(const std::string& archivoPeliculas, const std::string& archivoSeries, const std::string& archivoEpisodios) const;
    void mostrarVideos() const;
    void mostrarPeliculas() const;
    void mostrarListaSeries() const;
    void mostrarEpisodiosDeSeriePorID(const std::string& idSerie) const;
    void agregarPelicula();
    void agregarSerie();
    void cambiarCalificacion();
    void buscarPorCalificacion();
};

#endif // SERVICIOSTREAMING_HPP
