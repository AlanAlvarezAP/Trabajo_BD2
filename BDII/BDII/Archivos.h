#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <string>
#include <sstream>
#include "Disco.h"


struct Tipo_dato {
	std::string name;
	std::string tipo;
	int longitud;
	int decimal_capacidad;
	bool punto;
	bool not_null;
	bool pk;
};

class Archivos {
public:
	std::ifstream leedor;
	std::ofstream escritor;
	std::vector<std::string> arr;
	std::vector<Tipo_dato> tipos;
	
	std::string Cortar_espacio(std::string val);
	std::vector<std::string> Limpiar_registro(std::string linea);
	void Rellenar(int id_type,std::string &ren);
	void Leer_info_csv(Disco &disco);
	void Leer_info_sql();
};