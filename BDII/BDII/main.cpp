#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include "Disco.h"
#include "Archivos.h"
#include "RAM.h"

int main() {
	int cant_platos;
	int cant_pistas;
	int cant_sectores;
	int cant_bytes_sector;
	
	std::cout << "¡Bienvenido que configuracion desea utilizar !" << std::endl;
	std::cout << " Ingresa cantidad de platos " << std::endl;
	std::cin >> cant_platos;
	std::cout << " Ingresa cantidad de pistas " << std::endl;
	std::cin >> cant_pistas;
	std::cout << " Ingresa cantidad de sectores " << std::endl;
	std::cin >> cant_sectores;
	std::cout << " Ingresa cantidad de bytes por sector " << std::endl;
	std::cin >> cant_bytes_sector;

	Disco disco(cant_platos,cant_pistas,cant_sectores,cant_bytes_sector);
	RAM ram;
	Archivos prueba;
	prueba.Leer_info_sql();
	prueba.Leer_info_csv(disco);
	disco.Rebuild_data();
	disco.get_columnas("TaX","216.00",ram);
}
