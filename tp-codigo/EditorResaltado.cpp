#include "EditorResaltado.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

EditorResaltado::EditorResaltado() {
    comentarios = vector<Comentario>(); // O(1)
    palabras = vector<string>(); // O(1)
    comentarios_por_palabra = vector<set<id_comm>>(); // O(1)
    comentarios_texto = map<id_comm, string>(); // O(1)
}

unsigned EditorResaltado::longitud() const { 
    /* O(1) porque simplemente devuelve el tamaño del vector palabras, que es una operación constante. */
    return palabras.size(); // O(1)
}

void EditorResaltado::cargar_texto(const string& txt, const string& comments) {
    /*
    O(P + C) porque leer el texto y los comentarios y actualizar las estructuras de datos correspondientes implica recorrer cada palabra y comentario una vez.
    */
    
    ifstream archivo_texto_stream(txt); // O(1)
    ifstream archivo_comentarios_stream(comments); // O(1)

    palabras.clear(); // O(P)
    comentarios.clear(); // O(C)
    comentarios_texto.clear(); // O(C)
    comentarios_por_palabra.clear(); // O(P)

    string palabra;
    while (archivo_texto_stream >> palabra) { // O(P)
        palabras.push_back(palabra); // O(1)
        comentarios_por_palabra.emplace_back(); // O(1)
    }

    string linea_comentario;
    id_comm id = 0;
    while (getline(archivo_comentarios_stream, linea_comentario)) { // O(C)
        istringstream ss(linea_comentario); // O(1)
        unsigned desde, hasta;
        ss >> desde >> hasta; // O(1)
        string comentario;
        getline(ss, comentario); // O(1)

        size_t first_non_space = comentario.find_first_not_of(" "); // O(1)
        if (first_non_space != string::npos) {
            comentario = comentario.substr(first_non_space); // O(1)
        }

        Comentario c = Comentario(comentario, make_pair(desde, hasta), id++); // O(1)
        comentarios.push_back(c); // O(1)
        comentarios_texto[c.id] = comentario; // O(log C)
        for (unsigned i = desde; i < hasta; ++i) { 
            comentarios_por_palabra[i].insert(c.id); // O(log C)
        }
    }
}

const string& EditorResaltado::palabra_en(unsigned pos) const {
    /*
    O(1) porque simplemente devuelve la palabra en la posición pos del vector palabras, que es una operación constante.
    */
    return palabras[pos]; // O(1)
}

const string& EditorResaltado::texto_comentario(id_comm id) const {
    /*
    O(log C) porque buscar un elemento en un mapa (std::map) tiene una complejidad logarítmica.
    */
    auto it = comentarios_texto.find(id); // O(log C)
    return it->second; // O(1)
}

const set<id_comm>& EditorResaltado::comentarios_palabra(unsigned pos) const {
    /*
    O(1) porque devuelve el conjunto de comentarios en la posición pos del vector comentarios_por_palabra.
    */
    return comentarios_por_palabra[pos]; // O(1)
}

void EditorResaltado::insertar_palabra(const string& palabra, unsigned pos) {
    /* 
    O(P + C) porque insertar una palabra en palabras y un nuevo conjunto en comentarios_por_palabra requiere mover elementos, cada uno con complejidad O(P). Después, ajustar los rangos de los comentarios y moverlos implica iterar sobre los comentarios, lo cual tiene una complejidad O(C), resultando en O(P + C).
    */
    palabras.insert(palabras.begin() + pos, palabra); // O(P)
    comentarios_por_palabra.insert(comentarios_por_palabra.begin() + pos, set<id_comm>()); // O(P)

    for (auto& comentario : comentarios) { // O(C)
        if (comentario.rango.first <= pos && comentario.rango.second >= pos) { // O(1)
            comentario.rango.second++; // O(1)
            comentarios_por_palabra[pos].insert(comentario.id); // O(log C)
        } else if (comentario.rango.first > pos) { // O(1)
            comentario.rango.first++; // O(1)
            comentario.rango.second++; // O(1)
        }
    }

    for (unsigned i = pos + 1; i < comentarios_por_palabra.size(); ++i) { // O(P)
        comentarios_por_palabra[i].insert(comentarios_por_palabra[i - 1].begin(), comentarios_por_palabra[i - 1].end()); // O(log C)
    }
}

void EditorResaltado::borrar_palabra(unsigned pos) {
    /*
    O(P + C) porque eliminar una palabra de palabras y un conjunto de comentarios_por_palabra es O(P), dada la necesidad de mover elementos.

    Crear un conjunto comentarios_activos para verificar la presencia de comentarios restantes tiene una complejidad combinada de O(P + C), ya que implica iterar sobre todas las posiciones y comentarios.
    
    Chequear y eliminar comentarios vacíos tiene una complejidad O(C), porque implica iterar sobre los comentarios eliminados y realizar búsquedas en un conjunto, lo que es una operación constante.
    */
    palabras.erase(palabras.begin() + pos); // O(P)
    set<id_comm> comentarios_eliminados = comentarios_por_palabra[pos]; // O(1)
    comentarios_por_palabra.erase(comentarios_por_palabra.begin() + pos); // O(P)

    set<id_comm> comentarios_activos;
    for (const auto& comentarios_set : comentarios_por_palabra) { // O(P)
        comentarios_activos.insert(comentarios_set.begin(), comentarios_set.end()); // O(log C)
    }

    for (auto id : comentarios_eliminados) { // O(C)
        if (comentarios_activos.find(id) == comentarios_activos.end()) { // O(log C)
            comentarios_texto.erase(id); // O(log C)
            auto it = std::remove_if(comentarios.begin(), comentarios.end(), [id](const Comentario& c) { return c.id == id; }); // O(C)
            if (it != comentarios.end()) {
                comentarios.erase(it, comentarios.end()); // O(C)
            }
        }
    }
}

id_comm EditorResaltado::comentar(const string& texto, unsigned pos_desde, unsigned pos_hasta) {
    /*
    O(C) porque añade un comentario y actualiza las estructuras de datos correspondientes, lo cual implica iterar sobre cada palabra afectada por el comentario.
    */
    Comentario c = Comentario(texto, make_pair(pos_desde, pos_hasta), comentarios.size()); // O(1)
    comentarios.push_back(c); // O(1)
    comentarios_texto[comentarios.size() - 1] = texto; // O(log C)

    for (unsigned i = pos_desde; i < pos_hasta; ++i) { // O(C)
        comentarios_por_palabra[i].insert(comentarios.size() - 1); // O(log C)
    }

    return comentarios.size() - 1; // O(1)
}

void EditorResaltado::resolver_comentario(id_comm id) {
    /*
    O(log C + R_i log M) porque eliminamos el comentario de cada conjunto en comentarios_por_palabra solo si contiene el comentario (O(R_i log C)). Eliminar el comentario de comentarios_texto es O(log C), y buscar y eliminar el comentario de comentarios se realiza en O(log M).
    */
    for (auto& comentarios_set : comentarios_por_palabra) { // O(P)
        if (comentarios_set.find(id) != comentarios_set.end()) {
            comentarios_set.erase(id); // O(log C)
        }
    }

    comentarios_texto.erase(id); // O(log C)

    auto it = std::find_if(comentarios.begin(), comentarios.end(), [id](const Comentario& c) { return c.id == id; }); // O(C)
    if (it != comentarios.end()) {
        comentarios.erase(it); // O(C)
    }
}

unsigned EditorResaltado::cantidad_comentarios() const {
    /*
    O(1) porque devuelve una constante.
    */
    return comentarios.size(); // O(1)
}

EditorResaltado EditorResaltado::con_texto(const string& texto) {
    /*
    O(P) porque la función itera sobre cada carácter de la cadena una vez para dividir el texto en palabras y almacenarlas en las estructuras de datos correspondientes.
    */
    EditorResaltado editor;

    int i = 0;
    while (i < texto.size()) { // O(P)
        string palabra = "";
        while (i < texto.size() && texto[i] != ' ') { // O(L) con L = longitud de cada palabra
            palabra += texto[i]; // O(1)
            i++; // O(1)
        }
        editor.palabras.push_back(palabra); // O(1)
        editor.comentarios_por_palabra.emplace_back(); // O(1)
        i++; // O(1)
    }
    
    return editor; // O(1)
}
