#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <fstream>

struct Metadata {
	int registro_id;
	int Sector;
	int num_tipo;
	int Inicio_sector;
	int Inicio_Relleno;
	int duracion;

};

struct Info {
	int num_plato;
	int num_superficie;
	int num_pista;
	int num_sector;
};

struct Avl_info {
	int index_tabla;
	std::string dato;
	int tipo;
};

class Sector {
public:
	int id;
	int capacidadSector;
	int espacioLibre;
	std::vector<char> info;

	Sector(int id, int cap);
};

class Pista {
public:
	int id;
	std::vector<Sector> sectores;
	Pista(int id, int cantSectores, int capSector);
};

class Superficie {
public:
	int id;
	std::vector<Pista> pistas;
	Superficie(int id, int cantPistas, int cantSectores, int capSector);
};

class Plato {
public:
	int id;
	Superficie sup;
	Superficie inf;

	Plato(int id_1, int cantPistas, int cantSectores, int capSector);
};

class Disco {
public:
	std::vector <Plato> platos;
	std::vector<std::vector<std::pair<std::string, std::vector<Info>>>> tabla_final;
	Disco(int cantPlatos, int cantPistas, int cantSectores, int capSector);
	bool evaluar_registro(int espacio_requerido);
	void Insertar_data(std::string datos, int num_tipo,int real_tam, int registro_id);
	Sector& get_allinfo(int sector_id_send, std::vector<Info>& info);
	void Rebuild_data();
	void get_columnas(std::string campo);
};





