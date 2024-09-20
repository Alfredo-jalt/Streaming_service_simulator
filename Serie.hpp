#ifndef SERIE_HPP
#define SERIE_HPP

#include "Video.hpp"
#include "Episodio.hpp"
#include <vector>
#include <fstream>

class Serie : public Video {
    std::vector<Episodio> episodios;
    int numTemporadas;

public:
    Serie(std::string id, std::string nombre, int numTemporadas, std::string genero);

    int getNumTemporadas() const;
    void agregarEpisodio(const Episodio& episodio);
    void mostrarInfo() const override;
    void mostrarInfoBasica() const;
    void mostrarIdNombreYCalificacion() const override;
    void guardar(std::ofstream& file) const override;
    void guardarEpisodios(std::ofstream& file) const;
    std::vector<Episodio>& getEpisodios();
};

#endif // SERIE_HPP
