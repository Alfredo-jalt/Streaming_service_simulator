#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

// Clase abstracta Video
class Video {
protected:
    string id;
    string nombre;
    double duracion;
    string genero;
    vector<double> calificaciones;

public:
    Video(string id, string nombre, double duracion, string genero)
        : id(id), nombre(nombre), duracion(duracion), genero(genero) {}

    virtual ~Video() {}

    string getId() const { return id; }
    string getNombre() const { return nombre; }
    double getDuracion() const { return duracion; }
    string getGenero() const { return genero; }
    
    double getCalificacionPromedio() const {
        if (calificaciones.empty()) return 0;
        double sum = 0;
        for (double cal : calificaciones) {
            sum += cal;
        }
        double promedio = sum / calificaciones.size();
        return round(promedio * 10) / 10; // Redondear a un decimal
    }

    void agregarCalificacion(double calificacion) {
        if (calificacion < 0 || calificacion > 5 || floor(calificacion * 10) != calificacion * 10) {
            throw invalid_argument("La calificacion debe estar entre 0 y 5 con un solo decimal.");
        }
        calificaciones.push_back(calificacion);
    }

    string obtenerCalificacionesComoString() const {
        stringstream ss;
        for (size_t i = 0; i < calificaciones.size(); ++i) {
            if (i != 0) ss << ";";
            ss << calificaciones[i];
        }
        return ss.str();
    }

    void cargarCalificacionesDesdeString(const string& str) {
        stringstream ss(str);
        string cal;
        while (getline(ss, cal, ';')) {
            try {
                agregarCalificacion(stod(cal));
            } catch (const invalid_argument& e) {
                cerr << "Error al convertir cadena a numero en calificaciones: " << e.what() << endl;
            }
        }
    }

    virtual void mostrarInfo() const = 0;
    virtual void mostrarIdNombreYCalificacion() const = 0;
    virtual void guardar(ofstream& file) const = 0;
};

// Clase Pelicula que hereda de Video
class Pelicula : public Video {
public:
    Pelicula(string id, string nombre, double duracion, string genero)
        : Video(id, nombre, duracion, genero) {}

    void mostrarInfo() const override {
        cout << fixed << setprecision(1);
        cout << "Pelicula: " << nombre << " (ID: " << id
            << ", Duracion: " << duracion << " minutos, Genero: " << genero
            << ", Calificacion: " << getCalificacionPromedio() << ")\n";
    }

    void mostrarIdNombreYCalificacion() const override {
        cout << fixed << setprecision(1);
        cout << "ID: " << id << ", Nombre: " << nombre << ", Calificacion: " << getCalificacionPromedio() << "\n";
    }

    void guardar(ofstream& file) const override {
        file << id << "," << nombre << "," << duracion << "," << genero << "," << obtenerCalificacionesComoString() << "\n";
    }
};

// Clase Episodio
class Episodio {
    string id;
    string titulo;
    int temporada;
    double duracion;
    vector<double> calificaciones;

public:
    Episodio(string id, string titulo, int temporada, double duracion)
        : id(id), titulo(titulo), temporada(temporada), duracion(duracion) {}

    string getId() const { return id; }
    string getTitulo() const { return titulo; }
    int getTemporada() const { return temporada; }
    double getDuracion() const { return duracion; }
    
    double getCalificacionPromedio() const {
        if (calificaciones.empty()) return 0;
        double sum = 0;
        for (double cal : calificaciones) {
            sum += cal;
        }
        double promedio = sum / calificaciones.size();
        return round(promedio * 10) / 10; // Redondear a un decimal
    }

    void agregarCalificacion(double calificacion) {
        if (calificacion < 0 || calificacion > 5 || floor(calificacion * 10) != calificacion * 10) {
            throw invalid_argument("La calificacion debe estar entre 0 y 5 con un solo decimal.");
        }
        calificaciones.push_back(calificacion);
    }

    string obtenerCalificacionesComoString() const {
        stringstream ss;
        for (size_t i = 0; i < calificaciones.size(); ++i) {
            if (i != 0) ss << ";";
            ss << calificaciones[i];
        }
        return ss.str();
    }

    void cargarCalificacionesDesdeString(const string& str) {
        stringstream ss(str);
        string cal;
        while (getline(ss, cal, ';')) {
            try {
                agregarCalificacion(stod(cal));
            } catch (const invalid_argument& e) {
                cerr << "Error al convertir cadena a numero en calificaciones: " << e.what() << endl;
            }
        }
    }

    void mostrarInfo() const {
        cout << fixed << setprecision(1);
        cout << "Episodio: " << titulo << " (ID: " << id
            << ", Temporada: " << temporada
            << ", Duracion: " << duracion << " minutos, Calificacion: " << getCalificacionPromedio() << ")\n";
    }

    void mostrarIdNombreYCalificacion() const {
        cout << fixed << setprecision(1);
        cout << "ID: " << id << ", Titulo: " << titulo << ", Calificacion: " << getCalificacionPromedio() << "\n";
    }

    void guardar(ofstream& file, const string& serieId) const {
        file << serieId << "," << id << "," << titulo << "," << temporada << "," << duracion << "," << obtenerCalificacionesComoString() << "\n";
    }
};

// Clase Serie que hereda de Video
class Serie : public Video {
    vector<Episodio> episodios;
    int numTemporadas;

public:
    Serie(string id, string nombre, int numTemporadas, string genero)
        : Video(id, nombre, 0, genero), numTemporadas(numTemporadas) {}

    int getNumTemporadas() const { return numTemporadas; }

    void agregarEpisodio(Episodio episodio) {
        episodios.push_back(episodio);
    }

    void mostrarInfo() const override {
        cout << fixed << setprecision(1);
        cout << "Serie: " << nombre << " (ID: " << id
            << ", Genero: " << genero
            << ", Numero de temporadas: " << numTemporadas
            << ", Numero de episodios: " << episodios.size()
            << ", Calificacion: " << getCalificacionPromedio() << ")\n";
        for (const auto& episodio : episodios) {
            episodio.mostrarInfo();
        }
    }

    void mostrarInfoBasica() const {
        cout << "Serie: " << nombre << " (ID: " << id << ")\n";
    }

    void mostrarIdNombreYCalificacion() const override {
        cout << fixed << setprecision(1);
        cout << "ID: " << id << ", Nombre: " << nombre << ", Calificacion: " << getCalificacionPromedio() << "\n";
    }

    void guardar(ofstream& file) const override {
        file << id << "," << nombre << "," << numTemporadas << "," << genero << "," << obtenerCalificacionesComoString() << "\n";
    }

    void guardarEpisodios(ofstream& file) const {
        for (const auto& episodio : episodios) {
            episodio.guardar(file, id);
        }
    }

    vector<Episodio>& getEpisodios() {
        return episodios;
    }
};

// Clase ServicioStreaming que maneja los videos
class ServicioStreaming {
    vector<Video*> videos;

public:
    ~ServicioStreaming() {
        for (auto video : videos) {
            delete video;
        }
    }

    void agregarVideo(Video* video) {
        videos.push_back(video);
    }

    bool idExiste(const string& id) const {
        for (const auto& video : videos) {
            if (video->getId() == id) {
                return true;
            }
        }
        return false;
    }

    void cargarDesdeCSV(const string& archivoPeliculas, const string& archivoSeries, const string& archivoEpisodios) {
        ifstream filePeliculas(archivoPeliculas);
        string line;
        while (getline(filePeliculas, line)) {
            if (line.empty() || line.find("ID") == 0) continue;
            stringstream ss(line);
            string id, nombre, duracion, genero, calificacionesStr;
            getline(ss, id, ',');
            getline(ss, nombre, ',');
            getline(ss, duracion, ',');
            getline(ss, genero, ',');
            getline(ss, calificacionesStr, ',');

            try {
                Pelicula* pelicula = new Pelicula(id, nombre, stod(duracion), genero);
                pelicula->cargarCalificacionesDesdeString(calificacionesStr);
                agregarVideo(pelicula);
            } catch (const invalid_argument& e) {
                cerr << "Error al convertir cadena a numero en peliculas.csv: " << e.what() << endl;
                continue;
            }
        }

        ifstream fileSeries(archivoSeries);
        while (getline(fileSeries, line)) {
            if (line.empty() || line.find("ID") == 0) continue;
            stringstream ss(line);
            string id, nombre, numTemporadas, genero, calificacionesStr;
            getline(ss, id, ',');
            getline(ss, nombre, ',');
            getline(ss, numTemporadas, ',');
            getline(ss, genero, ',');
            getline(ss, calificacionesStr, ',');

            Serie* serie = nullptr;
            try {
                serie = new Serie(id, nombre, stoi(numTemporadas), genero);
                serie->cargarCalificacionesDesdeString(calificacionesStr);
            } catch (const invalid_argument& e) {
                cerr << "Error al convertir cadena a numero en series.csv: " << e.what() << endl;
                continue;
            }

            ifstream fileEpisodios(archivoEpisodios);
            while (getline(fileEpisodios, line)) {
                if (line.empty() || line.find("ID Serie") == 0) continue;
                stringstream ss(line);
                string idSerie, idEpisodio, titulo, temporada, duracionEp, calificacionesStrEp;
                getline(ss, idSerie, ',');
                getline(ss, idEpisodio, ',');
                getline(ss, titulo, ',');
                getline(ss, temporada, ',');
                getline(ss, duracionEp, ',');
                getline(ss, calificacionesStrEp, ',');

                if (idSerie == id) {
                    try {
                        Episodio episodio(idEpisodio, titulo, stoi(temporada), stod(duracionEp));
                        episodio.cargarCalificacionesDesdeString(calificacionesStrEp);
                        serie->agregarEpisodio(episodio);
                    } catch (const invalid_argument& e) {
                        cerr << "Error al convertir cadena a numero en episodios.csv: " << e.what() << endl;
                        continue;
                    }
                }
            }
            agregarVideo(serie);
        }
    }

    void guardarEnCSV(const string& archivoPeliculas, const string& archivoSeries, const string& archivoEpisodios) const {
        ofstream filePeliculas(archivoPeliculas);
        filePeliculas << "ID,Nombre,Duracion,Genero,Calificaciones\n";
        ofstream fileSeries(archivoSeries);
        fileSeries << "ID,Nombre,NumTemporadas,Genero,Calificaciones\n";
        ofstream fileEpisodios(archivoEpisodios);
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

    void mostrarVideos() const {
        for (const auto& video : videos) {
            video->mostrarInfo();
        }
    }

    void mostrarPeliculas() const {
        bool peliculasEncontradas = false;
        for (const auto& video : videos) {
            Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
            if (pelicula) {
                pelicula->mostrarInfo();
                peliculasEncontradas = true;
            }
        }
        if (!peliculasEncontradas) {
            cout << "No hay peliculas disponibles.\n";
        }
    }

    void mostrarListaSeries() const {
        for (const auto& video : videos) {
            Serie* serie = dynamic_cast<Serie*>(video);
            if (serie) {
                serie->mostrarInfoBasica();
            }
        }
    }

    void mostrarEpisodiosDeSeriePorID(const string& idSerie) const {
        for (const auto& video : videos) {
            Serie* serie = dynamic_cast<Serie*>(video);
            if (serie && serie->getId() == idSerie) {
                serie->mostrarInfo();
                return;
            }
        }
        cout << "Serie no encontrada.\n";
    }

    void agregarPelicula() {
        string id, nombre, genero;
        double duracion, calificacion;
        cout << "Ingrese ID de la pelicula: ";
        cin >> id;
        if (idExiste(id)) {
            cout << "El ID ya existe. No se puede agregar la pelicula.\n";
            return;
        }
        cout << "Ingrese nombre de la pelicula: ";
        cin.ignore();
        getline(cin, nombre);
        cout << "Ingrese duracion de la pelicula (minutos): ";
        while (!(cin >> duracion)) {
            cout << "Entrada invalida. Por favor ingrese un numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Ingrese genero de la pelicula: ";
        cin.ignore();
        getline(cin, genero);
        cout << "Ingrese calificacion de la pelicula: ";
        while (!(cin >> calificacion) || calificacion < 0 || calificacion > 5 || floor(calificacion * 10) != calificacion * 10) {
            cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Pelicula* pelicula = new Pelicula(id, nombre, duracion, genero);
        pelicula->agregarCalificacion(calificacion);
        agregarVideo(pelicula);
    }

    void agregarSerie() {
        string id, nombre, genero;
        int numTemporadas, numEpisodios;
        double calificacion;
        cout << "Ingrese ID de la serie: ";
        cin >> id;
        if (idExiste(id)) {
            cout << "El ID ya existe. No se puede agregar la serie.\n";
            return;
        }
        cout << "Ingrese nombre de la serie: ";
        cin.ignore();
        getline(cin, nombre);
        cout << "Ingrese genero de la serie: ";
        getline(cin, genero);
        cout << "Ingrese calificacion de la serie: ";
        while (!(cin >> calificacion) || calificacion < 0 || calificacion > 5 || floor(calificacion * 10) != calificacion * 10) {
            cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Ingrese numero de temporadas: ";
        while (!(cin >> numTemporadas)) {
            cout << "Entrada invalida. Por favor ingrese un numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Ingrese numero de episodios: ";
        while (!(cin >> numEpisodios)) {
            cout << "Entrada invalida. Por favor ingrese un numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Serie* serie = new Serie(id, nombre, numTemporadas, genero);
        serie->agregarCalificacion(calificacion);

        for (int i = 1; i <= numEpisodios; ++i) {
            string idEpisodio, titulo;
            int temporada;
            double duracion, calificacionEp;
            cout << "Ingrese ID del episodio " << i << ": ";
            cin >> idEpisodio;
            cout << "Ingrese titulo del episodio " << i << ": ";
            cin.ignore();
            getline(cin, titulo);
            cout << "Ingrese temporada del episodio " << i << ": ";
            while (!(cin >> temporada)) {
                cout << "Entrada invalida. Por favor ingrese un numero: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Ingrese duracion del episodio " << i << " (minutos): ";
            while (!(cin >> duracion)) {
                cout << "Entrada invalida. Por favor ingrese un numero: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Ingrese calificacion del episodio " << i << ": ";
            while (!(cin >> calificacionEp) || calificacionEp < 0 || calificacionEp > 5 || floor(calificacionEp * 10) != calificacionEp * 10) {
                cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            Episodio episodio(idEpisodio, titulo, temporada, duracion);
            episodio.agregarCalificacion(calificacionEp);
            serie->agregarEpisodio(episodio);
        }

        agregarVideo(serie);
    }

    void cambiarCalificacion() {
        cout << "Seleccione el tipo de video:\n";
        cout << "1. Pelicula\n";
        cout << "2. Serie\n";
        cout << "3. Episodio\n";
        int tipoOpcion;
        while (!(cin >> tipoOpcion) || tipoOpcion < 1 || tipoOpcion > 3) {
            cout << "Entrada invalida. Por favor ingrese 1, 2, o 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        string id;
        double nuevaCalificacion;
        bool encontrado = false;

        if (tipoOpcion == 1) {
            cout << "Peliculas disponibles (ID, Nombre y Calificacion):\n";
            for (const auto& video : videos) {
                Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
                if (pelicula) {
                    pelicula->mostrarIdNombreYCalificacion();
                }
            }
            cout << "Ingrese el ID de la pelicula: ";
            cin >> id;
            for (auto& video : videos) {
                Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
                if (pelicula && pelicula->getId() == id) {
                    cout << "Ingrese la nueva calificacion (0-5 con un solo decimal): ";
                    while (!(cin >> nuevaCalificacion) || nuevaCalificacion < 0 || nuevaCalificacion > 5 || floor(nuevaCalificacion * 10) != nuevaCalificacion * 10) {
                        cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    pelicula->agregarCalificacion(nuevaCalificacion);
                    cout << "Calificacion actualizada correctamente.\n";
                    encontrado = true;
                    break;
                }
            }
        } else if (tipoOpcion == 2) {
            cout << "Series disponibles (ID, Nombre y Calificacion):\n";
            for (const auto& video : videos) {
                Serie* serie = dynamic_cast<Serie*>(video);
                if (serie) {
                    serie->mostrarIdNombreYCalificacion();
                }
            }
            cout << "Ingrese el ID de la serie: ";
            cin >> id;
            for (auto& video : videos) {
                Serie* serie = dynamic_cast<Serie*>(video);
                if (serie && serie->getId() == id) {
                    cout << "Ingrese la nueva calificacion (0-5 con un solo decimal): ";
                    while (!(cin >> nuevaCalificacion) || nuevaCalificacion < 0 || nuevaCalificacion > 5 || floor(nuevaCalificacion * 10) != nuevaCalificacion * 10) {
                        cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    serie->agregarCalificacion(nuevaCalificacion);
                    cout << "Calificacion actualizada correctamente.\n";
                    encontrado = true;
                    break;
                }
            }
        } else if (tipoOpcion == 3) {
            cout << "Series disponibles (ID, Nombre y Calificacion):\n";
            for (const auto& video : videos) {
                Serie* serie = dynamic_cast<Serie*>(video);
                if (serie) {
                    serie->mostrarIdNombreYCalificacion();
                }
            }
            cout << "Ingrese el ID de la serie: ";
            cin >> id;
            Serie* serie = nullptr;
            for (auto& video : videos) {
                serie = dynamic_cast<Serie*>(video);
                if (serie && serie->getId() == id) {
                    break;
                }
            }

            if (serie) {
                cout << "Episodios disponibles (ID, Titulo y Calificacion):\n";
                for (const auto& episodio : serie->getEpisodios()) {
                    episodio.mostrarIdNombreYCalificacion();
                }
                cout << "Ingrese el ID del episodio: ";
                cin >> id;
                for (auto& episodio : serie->getEpisodios()) {
                    if (episodio.getId() == id) {
                        cout << "Ingrese la nueva calificacion (0-5 con un solo decimal): ";
                        while (!(cin >> nuevaCalificacion) || nuevaCalificacion < 0 || nuevaCalificacion > 5 || floor(nuevaCalificacion * 10) != nuevaCalificacion * 10) {
                            cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        episodio.agregarCalificacion(nuevaCalificacion);
                        cout << "Calificacion del episodio actualizada correctamente.\n";
                        encontrado = true;
                        break;
                    }
                }
            }
        }

        if (!encontrado) {
            cout << "No se encontro el video con el ID proporcionado.\n";
        }
    }

    void buscarPorCalificacion() {
        double calificacionBuscada;
        cout << "Ingrese la calificacion a buscar (0-5 con un solo decimal): ";
        while (!(cin >> calificacionBuscada) || calificacionBuscada < 0 || calificacionBuscada > 5 || floor(calificacionBuscada * 10) != calificacionBuscada * 10) {
            cout << "Entrada invalida. Por favor ingrese un numero entre 0 y 5 con un solo decimal: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        bool videosEncontrados = false;
        cout << "Videos con calificacion " << calificacionBuscada << ":\n";
        for (const auto& video : videos) {
            if (video->getCalificacionPromedio() == calificacionBuscada) {
                video->mostrarInfo();
                videosEncontrados = true;
            }
        }

        if (!videosEncontrados) {
            cout << "No se encontraron videos con la calificacion " << calificacionBuscada << ".\n";
        }
    }
};

void mostrarMenu() {
    cout << "Menu:\n";
    cout << "1. Ver datos de todos los videos\n";
    cout << "2. Ver datos de todas las peliculas\n";
    cout << "3. Ver episodios de una serie\n";
    cout << "4. Agregar una nueva pelicula\n";
    cout << "5. Agregar una nueva serie\n";
    cout << "6. Cambiar calificacion de un video\n";
    cout << "7. Buscar peliculas y series por calificacion\n";
    cout << "8. Guardar y salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    ServicioStreaming servicio;
    servicio.cargarDesdeCSV("peliculas.csv", "series.csv", "episodios.csv");

    int opcion;
    do {
        mostrarMenu();
        while (!(cin >> opcion)) {
            cout << "Entrada invalida. Por favor ingrese un numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        switch (opcion) {
            case 1:
                servicio.mostrarVideos();
                break;
            case 2:
                servicio.mostrarPeliculas();
                break;
            case 3: {
                cout << "Series disponibles:\n";
                servicio.mostrarListaSeries();
                string idSerie;
                cout << "Ingrese el ID de la serie para ver sus episodios: ";
                cin >> idSerie;
                servicio.mostrarEpisodiosDeSeriePorID(idSerie);
                break;
            }
            case 4:
                servicio.agregarPelicula();
                break;
            case 5:
                servicio.agregarSerie();
                break;
            case 6:
                servicio.cambiarCalificacion();
                break;
            case 7:
                servicio.buscarPorCalificacion();
                break;
            case 8:
                servicio.guardarEnCSV("peliculas.csv", "series.csv", "episodios.csv");
                cout << "Datos guardados. Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 8);

    return 0;
}
