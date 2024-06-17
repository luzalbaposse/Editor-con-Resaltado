#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>
using std::string;
using std::set;
using std::map;
using std::vector;
typedef unsigned id_comm;
using namespace std;
	
class EditorResaltado {
	public:
		// Constructores
		EditorResaltado();
		
		static EditorResaltado con_texto(const string& texto);

		unsigned longitud() const;

		const string& palabra_en(unsigned pos) const;

		const string& texto_comentario(id_comm id) const;

		const set<id_comm> & comentarios_palabra(unsigned pos) const;

		// Modificadores
	  	void cargar_texto(const string& archivo_texto, const string& archivo_comentarios);

		void insertar_palabra(const string& palabra, unsigned pos);

		void borrar_palabra(unsigned pos);

		id_comm comentar(const string& comentario, unsigned desde, unsigned hasta);

		void resolver_comentario(id_comm id);

		// Otras operaciones
		unsigned cantidad_comentarios() const;

	private:
		/*		
		Rep(e: estr) ≡ (∀i ∈ [0, e.longitud()), e.comentarios_por_palabra[i] ≠ ∅) ∧ (∀id ∈ claves(e.comentarios_texto), ∃c ∈ e.comentarios → c.id == id) ∧ (e.longitud() == |e.comentarios_por_palabra|) ∧ (∀id1, id2 ∈ claves(e.comentarios_texto), id1 ≠ id2) ∧ e.longitud() >= 0 ∧ e.cant_comentarios >= 0 

		Para cada posición i en el vector de palabras, existe un conjunto de IDs de comentarios. Para cada ID de comentario en comentarios_texto, existe un comentario correspondiente en comentarios con el mismo ID. La cantidad de elementos en el vector de palabras es igual a la cantidad de elementos en el vector de conjuntos de IDs de comentarios. Los IDs de los comentarios en comentarios_texto son únicos.
		*/

		struct Comentario {
			string texto;
			pair<unsigned, unsigned> rango;  // Tupla de rango del comentario
			id_comm id;                      // ID del comentario

			Comentario(string texto, pair<unsigned, unsigned> rango, id_comm id)
				: texto(texto), rango(rango), id(id) {}
		};
		
		vector<Comentario> comentarios;
				
		vector<string> palabras;

		vector<set<id_comm>> comentarios_por_palabra;

		map<id_comm, string> comentarios_texto;
		// Este map puede resultar medio redundante, pero el objetivo es reducir la complejidad de la operación texto_comentario(id_comm id) a O(1) en lugar de O(log C) si se recorriera el vector de comentarios buscando el comentario con el ID correspondiente. Lo mismo con las demás funciones.
		

		};
