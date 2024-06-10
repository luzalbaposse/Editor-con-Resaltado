#include "EditorResaltado.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

//* TODO: implementar y justificar complejidad
EditorResaltado::EditorResaltado(){ comentarios = vector<Comentario>(); palabras = vector<string>(); comentarios_por_palabra = vector<set<id_comm>>(); comentarios_texto = map<id_comm, string>(); }

unsigned EditorResaltado::longitud() const {
	//* TODO: implementar y justificar complejidad
	return palabras.size();
}

void EditorResaltado::cargar_texto(const string& txt, const string& comments) {
//* TODO: implementar y justificar complejidad
    palabras.clear();
    comentarios.clear();
    comentarios_texto.clear();
    comentarios_por_palabra.clear();

    ifstream archivo_texto(txt);
    ifstream archivo_comentarios(comments);

    string palabra;
    while (archivo_texto >> palabra) { 
        palabras.push_back(palabra);
        comentarios_por_palabra.emplace_back(); 
    }

    string linea_comentario;
    id_comm id = 1; 
    while (getline(archivo_comentarios, linea_comentario)) {
     
        istringstream ss(linea_comentario);
        unsigned desde, hasta;
        ss >> desde >> hasta;
        string comentario((istreambuf_iterator<char>(ss)), istreambuf_iterator<char>());

      
        Comentario c = Comentario(comentario, make_pair(desde, hasta), id);
        comentarios.push_back(c);
        comentarios_texto[id] = comentario;

        for (unsigned i = desde; i < hasta; ++i) {
            comentarios_por_palabra[i].insert(id);
        }
        id++;
    }
}


const string& EditorResaltado::palabra_en(unsigned pos) const {
	// TODO: implementar y justificar complejidad
	return "";
}

const string& EditorResaltado::texto_comentario(id_comm id) const {
	// TODO: implementar y justificar complejidad
	return "";
}

const set<id_comm> & EditorResaltado::comentarios_palabra(unsigned pos) const {
	// TODO: implementar y justificar complejidad
	return set<id_comm>();
}

void EditorResaltado::insertar_palabra(const string& palabra, unsigned pos) {
	// TODO: implementar y justificar complejidad
}

void EditorResaltado::borrar_palabra(unsigned pos) {
	// TODO: implementar y justificar complejidad
}

id_comm EditorResaltado::comentar(const string& texto, unsigned pos_desde, unsigned pos_hasta) {
	// TODO: implementar y justificar complejidad
	return 0;
}

void EditorResaltado::resolver_comentario(id_comm id) {
	// TODO: implementar y justificar complejidad
}

unsigned EditorResaltado::cantidad_comentarios() const {
	// TODO: implementar y justificar complejidad
	return 0;
}

EditorResaltado EditorResaltado::con_texto(const string& texto) {
	// TODO: implementar y justificar complejidad
	return EditorResaltado();
}
