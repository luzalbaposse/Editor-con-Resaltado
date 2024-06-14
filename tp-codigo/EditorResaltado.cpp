#include "EditorResaltado.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

EditorResaltado::EditorResaltado(){ comentarios = vector<Comentario>(); palabras = vector<string>(); comentarios_por_palabra = vector<set<id_comm>>(); comentarios_texto = map<id_comm, string>(); }

unsigned EditorResaltado::longitud() const { /* O(1) */
    cout << "Longitud: " << palabras.size() << endl;
	return palabras.size();
}

void EditorResaltado::cargar_texto(const string& txt, const string& comments) {
    ifstream archivo_texto_stream(txt);

    ifstream archivo_comentarios_stream(comments);

    palabras.clear();
    comentarios.clear();
    comentarios_texto.clear();
    comentarios_por_palabra.clear();

    string palabra;
    while (archivo_texto_stream >> palabra) {
        palabras.push_back(palabra);
        comentarios_por_palabra.emplace_back();
    }

    string linea_comentario;
    id_comm id = 0;
    while (getline(archivo_comentarios_stream, linea_comentario)) {
        istringstream ss(linea_comentario);
        unsigned desde, hasta;
        ss >> desde >> hasta;
        string comentario;
        getline(ss, comentario);

        size_t first_non_space = comentario.find_first_not_of(" ");
        if (first_non_space != string::npos) {
            comentario = comentario.substr(first_non_space);
        }

        Comentario c = Comentario(comentario, make_pair(desde, hasta), id++);
        comentarios.push_back(c);
        comentarios_texto[c.id] = comentario;
        for (unsigned i = desde; i < hasta; ++i) { 
            comentarios_por_palabra[i].insert(c.id);
        }
    }
}


const string& EditorResaltado::palabra_en(unsigned pos) const {
/*O(1)*/
	return palabras[pos];
}

const string& EditorResaltado::texto_comentario(id_comm id) const {

    auto it = comentarios_texto.find(id); 
    return it->second;
}


const set<id_comm> & EditorResaltado::comentarios_palabra(unsigned pos) const {
	//* TODO: implementar y justificar complejidad
	return comentarios_por_palabra[pos];
}

void EditorResaltado::insertar_palabra(const string& palabra, unsigned pos) {
	//* TODO: implementar y justificar complejidad
    palabras.insert(palabras.begin() + pos, palabra);
    comentarios_por_palabra.insert(comentarios_por_palabra.begin() + pos, set<id_comm>());

    for (unsigned i = 0; i < pos; ++i) {
        for (const auto& id : comentarios_por_palabra[i]) {
            comentarios_por_palabra[pos].insert(id);
        }
    }

    for (unsigned i = pos + 1; i < comentarios_por_palabra.size(); ++i) {
        for (const auto& id : comentarios_por_palabra[i]) {
            comentarios_por_palabra[i - 1].insert(id);
        }
    }
}
void EditorResaltado::borrar_palabra(unsigned pos) {
    palabras.erase(palabras.begin() + pos);
    set<id_comm> comentarios_eliminados = comentarios_por_palabra[pos];
    comentarios_por_palabra.erase(comentarios_por_palabra.begin() + pos);

    for (auto id : comentarios_eliminados) {
        cerr << id << " ";
    }
    cerr << endl;

    for (auto id : comentarios_eliminados) {
        bool comentario_vacio = true;
        for (unsigned i = 0; i < comentarios_por_palabra.size(); ++i) {
            if (comentarios_por_palabra[i].find(id) != comentarios_por_palabra[i].end()) {
                comentario_vacio = false;
                break;
            }
        }
        cerr << "Comentario " << id << " está vacío: " << comentario_vacio << endl;
        if (comentario_vacio) {
            comentarios_texto.erase(id);
            auto it = std::remove_if(comentarios.begin(), comentarios.end(), [id](const Comentario& c) { return c.id == id; });
            if (it != comentarios.end()) {
                comentarios.erase(it, comentarios.end());
            }
        }
    }
}

id_comm EditorResaltado::comentar(const string& texto, unsigned pos_desde, unsigned pos_hasta) {
    Comentario c = Comentario(texto, make_pair(pos_desde, pos_hasta), comentarios.size());

    comentarios.push_back(c);
    comentarios_texto[comentarios.size() - 1] = texto;


    for (unsigned i = pos_desde; i < pos_hasta; ++i) {
        comentarios_por_palabra[i].insert(comentarios.size() - 1);
    }


    return comentarios.size() - 1;
}


void EditorResaltado::resolver_comentario(id_comm id) {
	//* TODO: implementar y justificar complejidad
    for (auto& comentarios_set : comentarios_por_palabra) {
        comentarios_set.erase(id);
    }
    comentarios_texto.erase(id);
    auto it = std::find_if(comentarios.begin(), comentarios.end(), [id](const Comentario& c) { return c.id == id; });
    if (it != comentarios.end()) {
        comentarios.erase(it);
    }
}

unsigned EditorResaltado::cantidad_comentarios() const {
	//* TODO: implementar y justificar complejidad
    cout << "Cantidad de comentarios: " << comentarios.size() << endl;
	return comentarios.size();
}

EditorResaltado EditorResaltado::con_texto(const string& texto) {
    EditorResaltado editor;

	int i = 0;
	while (i < texto.size()){
		string palabra = "";
		while(i < texto.size() && texto[i] != ' '){
			palabra += texto[i];
			i++;
		}
		editor.palabras.push_back(palabra);
		editor.comentarios_por_palabra.emplace_back();
		i++;
	}
	
    return editor;
}
