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
		// Construye un editor vacío
		// Método estático auxiliar para construir editores de forma conveniente
		static EditorResaltado con_texto(const string& texto);
		// Devuelve un editor con las palabras de texto insertadas en su orden original

		// Observadores
		unsigned longitud() const;
		// Devuelve la cantidad de palabras del texto
		const string& palabra_en(unsigned pos) const;
		// devuelve la palabra en esa posición
		const string& texto_comentario(id_comm id) const;
		// devuelve el texto del comentario id
		const set<id_comm> & comentarios_palabra(unsigned pos) const;
		// devuelve los comentarios de la palabra en esa posición

		// Modificadores
	  	void cargar_texto(const string& archivo_texto, const string& archivo_comentarios);
		// Borra el contenido viejo, carga el texto de archivo_texto y le palica los comentarios de archivo_comentarios
		void insertar_palabra(const string& palabra, unsigned pos);
		// Inserta palabra en la posición pos. Si pos = longitud se inserta al final Si pos era previamnete afectada por uno o más comentarios, los mismos se expanderán para contener a la palabra insertada también
		void borrar_palabra(unsigned pos);
		// Post: elimina la palabra ubicada en la posición pos. Elimina cualquier comentario cuya única posición afectada fuera pos
		id_comm comentar(const string& comentario, unsigned desde, unsigned hasta);
		// Devuelve el ID de un nuevo comentario que aplica a todas las palabras entre desde y hasta (sin incluir esta última posición). Nota: Los IDs deben asignarse de manera creciente empezando por 1. No se reutilizan IDs de comentarios eliminados.

		void resolver_comentario(id_comm id);
		// Se elimina id de todas las piosiciones afectadas por este
		// Otras operaciones
		unsigned cantidad_comentarios() const;
		// Devuelve la cantidad de comentarios que hay en el texto

	private:
		//* TODO: Definir la estructura y describir claramente el invariante de representación de la misma.
		//? CONSULTAR
		/*
		Invariante de representación:
		Rep(e: estr) ≡
		(∀i ∈ [0, e.longitud()), e.comentarios_por_palabra[i] ≠ ∅) ∧
		(∀id ∈ claves(e.comentarios_texto), ∃c ∈ e.comentarios → c.id == id) ∧
		(e.longitud() == |e.comentarios_por_palabra|) ∧
		(∀id1, id2 ∈ claves(e.comentarios_texto), id1 ≠ id2) ∧ e.longitud() >= 0 ∧ e.cant_comentarios >= 0 

		1. ∀i ∈ [0, e.longitud()), e.comentarios_por_palabra[i] ≠ ∅:
		Para cada posición i en el vector de palabras, existe un conjunto de IDs de comentarios.

		2. ∀id ∈ claves(e.comentarios_texto), ∃c ∈ e.comentarios → c.id == id:
		Para cada ID de comentario en comentarios_texto, existe un comentario correspondiente en comentarios con el mismo ID.

		3. e.longitud() == |e.comentarios_por_palabra|:
		La cantidad de elementos en el vector de palabras es igual a la cantidad de elementos en el vector de conjuntos de IDs de comentarios.

		4. ∀id1, id2 ∈ claves(e.comentarios_texto), id1 ≠ id2:
		Los IDs de los comentarios en comentarios_texto son únicos.
		*/

		//* Estructura de comentarios
		struct Comentario {
			string texto;
			pair<unsigned, unsigned> rango;  // Tupla de rango del comentario
			id_comm id;                      // ID del comentario
			bool resuelto;                   // Indica si el comentario fue resuelto

			Comentario(string texto, pair<unsigned, unsigned> rango, id_comm id)
				: texto(texto), rango(rango), id(id), resuelto(false) {}
		};
		
		vector<Comentario> comentarios;
				
		vector<string> palabras;
		// Mapea la posición de cada palabra a los IDs de los comentarios
		vector<set<id_comm>> comentarios_por_palabra;
		// Mapea los IDs de comentarios a su texto
		map<id_comm, string> comentarios_texto;
		

		};
