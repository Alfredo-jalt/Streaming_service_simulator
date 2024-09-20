#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <string>
#include <vector>

class Video {
protected:
    std::string id;
    std::string nombre;
    double duracion;
    std::string genero;
    std::vector<double> calificaciones;

public:
    Video(std::string id, std::string nombre, double duracion, std::string genero);
    virtual ~Video();

    std::string getId() const;
    std::string getNombre() const;
    double getDuracion() const;
    std::string getGenero() const;
    double getCalificacionPromedio() const;
    void agregarCalificacion(double calificacion);
    std::string obtenerCalificacionesComoString() const;
    void cargarCalificacionesDesdeString(const std::string& calificacionesStr);

    virtual void mostrarInfo() const = 0;
    virtual void mostrarIdNombreYCalificacion() const = 0;
    virtual void guardar(std::ofstream& file) const = 0;
};

#endif // VIDEO_HPP
