#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>

// Incluya aquí los headers que necesite

using std::string;
using std::set;
using std::map;
using std::vector;

typedef unsigned id_comm;
	
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
		// TODO: Definir la estructura y describir claramente el invariante de representación de la misma.
		
		int cant_palabras;
		vector<string> palabras;
		
		

	
		};
