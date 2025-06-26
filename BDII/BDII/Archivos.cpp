#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <string>
#include <sstream>
#include "Archivos.h"
#include "Disco.h"


std::string Archivos::Cortar_espacio(std::string val) {
	size_t start_space = val.find_first_not_of(' ');
	if (start_space == std::string::npos) {
		return "";
	}
	size_t end_space = val.find_last_not_of(' ');
	
	return val.substr(start_space, end_space - start_space + 1);
}

std::vector<std::string> Archivos::Limpiar_registro(std::string linea) {
	std::vector<std::string> result;
	bool comillas = false;
	std::string atributo;
	char caracter;
	for (int i = 0;i < linea.size();i++) {
		caracter = linea[i];

		if (caracter == '"') {
			comillas = !comillas;
		}
		else if (caracter == ',' && !comillas) {

			result.push_back(Cortar_espacio(atributo));
			atributo.clear();
		}
		else {
			atributo = atributo + caracter;
		}
	}
	result.push_back(Cortar_espacio(atributo));
	return result;
}

void Archivos::Rellenar(int id_type,std::string &ren) {
	Tipo_dato tipo = tipos[id_type];

	if (tipo.punto) {
		ren.erase(std::remove(ren.begin(), ren.end(), '.'), ren.end());
	}

	if (ren.size() > tipo.longitud) {
		ren = ren.substr(0, tipo.longitud);
		return;
	}
	if (ren.size() == tipo.longitud) {
		return;
	}
	else {
		if (tipo.not_null || tipo.pk) {
			ren = std::string(tipo.longitud - ren.size(), ' ') + ren;
		}
	}

	
}

void Archivos::Leer_info_csv(Disco &disco) {
	std::string linea;
	int registro_id = 0;
	bool cabecera = true;
	leedor.open("D:/taxables (1).csv");
	if (!leedor.is_open()) {
		std::cout << " No se pudo leer el archivo " << std::endl;
		return;
	}

	while (std::getline(leedor, linea)) {
		if (cabecera) {
			cabecera = false;
			continue;
		}
		std::string id, Item, Cost, Tax,Total;
		int realTam_id, realTam_Item, realTam_Cost, realTam_Tax, realTam_Total;

		std::vector<std::string> campo = Limpiar_registro(linea);
		if (campo.size() != 5) {
			std::cout << "Linea RARA " << linea << std::endl;
			continue;
		}
		id = campo[0];
		Item = campo[1];
		Cost = campo[2];
		Tax = campo[3];
		Total = campo[4];

		realTam_id = id.size();
		realTam_Item = Item.size();
		realTam_Cost = Cost.size()-1;
		realTam_Tax = Tax.size()-1;
		realTam_Total = Total.size()-1;

		std::cout << " ID -> " << id << " | Item -> " << Item << "| Cost -> " << Cost << "| Tax -> " << Tax << "| Total -> " << Total << std::endl;

		Rellenar(0,id);
		Rellenar(1, Item);
		Rellenar(2, Cost);
		Rellenar(3, Tax);
		Rellenar(4, Total);

		if (disco.evaluar_registro(id.size() + Item.size() + Cost.size() + Tax.size() + Total.size())) {
			disco.Insertar_data(id, 0, realTam_id, registro_id);
			disco.Insertar_data(Item, 1, realTam_Item, registro_id);
			disco.Insertar_data(Cost, 2, realTam_Cost, registro_id);
			disco.Insertar_data(Tax, 3, realTam_Tax, registro_id);
			disco.Insertar_data(Total, 4, realTam_Total, registro_id);
		}
		else {
			std::cout << " Registro rechazado num " << registro_id << std::endl;
		}
		registro_id++;
	}
	leedor.close();
}

void Archivos::Leer_info_sql() {
	std::string linea;
	std::stringstream inicio;
	std::string name, tipo,pk_null;
	
	leedor.open("D:/struct_table.txt");
	if (!leedor.is_open()) {
		std::cout << " No se pudo leer el archivo " << std::endl;
		return;
	}
	std::getline(inicio, linea, '(');
	while (std::getline(leedor, linea)) {
		int longitud=0, decimal = -1;
		bool punto = false, not_null = false, pk = false;
		if (linea.find("CREATE") != std::string::npos)
			continue;

		if (linea == "); ") { // OJO consultar hay un espacio al final
			break;
		}
		std::stringstream info(linea);
		
		info >> name;
		// Eliminar el espacio de molestoso antes del decimal
		std::getline(info, tipo,')');
		tipo += ')';
		tipo.erase(std::remove(tipo.begin(), tipo.end(), ' '), tipo.end());
		std::getline(info, pk_null);

		// Sacar bien el tipo
		size_t prim_parentesis = tipo.find('(');
		if (prim_parentesis != std::string::npos) {
			std::string tmp_tipo = tipo.substr(0, prim_parentesis);
			size_t second_parentesis = tipo.find(')', prim_parentesis);
			std::string numeros = tipo.substr(prim_parentesis + 1, second_parentesis - prim_parentesis - 1);


			size_t find_coma = numeros.find(',');
			if (find_coma != std::string::npos) {
				longitud = std::stoi(numeros.substr(0, find_coma));
				decimal = std::stoi(numeros.substr(find_coma + 1));
				punto = true;
			}
			else {
				longitud = std::stoi(numeros);
			}
			tipo = tmp_tipo;
		}
		
		// Procesar Extra

		if (pk_null.find("PRIMARY") != std::string::npos && pk_null.find("KEY") != std::string::npos)
			pk = true;
		if (pk_null.find("NOT") != std::string::npos && pk_null.find("NULL") != std::string::npos)
			not_null = true;
		tipos.push_back(Tipo_dato{name,tipo,longitud,decimal,punto,not_null,pk});
	}
	leedor.close();

	for (const auto& t : tipos) {
		std::cout << "Nombre: " << t.name
			<< ", Tipo: " << t.tipo
			<< ", Long: " << t.longitud
			<< ", Dec: " << t.decimal_capacidad
			<< ", Not Null: " << t.not_null
			<< ", PK: " << t.pk << std::endl;
	}
}