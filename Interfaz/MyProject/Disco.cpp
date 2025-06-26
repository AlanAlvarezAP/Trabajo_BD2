#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <string>
#include <sstream>
#include "Disco.h"
#include "AVL.h"
#include "RAM.h"

Sector::Sector(int id, int cap) {
	this->id = id;
	capacidadSector = cap;
	espacioLibre = cap;
}
Pista::Pista(int id, int cantSectores, int capSector):id(id){
	for (int i = 0;i < cantSectores;i++) {
		sectores.push_back(Sector(id*cantSectores+i, capSector));
	}
}
Superficie::Superficie(int id, int cantPistas, int cantSectores, int capSector):id(id) {
	for (int i = 0;i < cantPistas;i++) {
		pistas.push_back(Pista(id*cantPistas+i, cantSectores, capSector));
	}
}

Plato::Plato(int id_1, int cantPistas, int cantSectores, int capSector): id(id_1),
	sup(2 * id_1, cantPistas, cantSectores, capSector),
	inf(2 * id_1 + 1, cantPistas, cantSectores, capSector) {
}
Disco::Disco(int cantPlatos, int cantPistas, int cantSectores, int capSector){
    std::ofstream out("D:/metadata.txt", std::ios::trunc);
    out << "RegistroID" << " "
        << "Sector_insertado" << " "
        << "num_tipo" << " "
        << "Inicio_sector" << " "
        << "Inicio_Relleno" << " "
        << "duracion" << std::endl;
    out.close();
	for (int i = 0;i < cantPlatos;i++) {
		platos.push_back(Plato(i,cantPistas, cantSectores, capSector));
	}
}

bool Disco::evaluar_registro(int espacio_requerido) {
    int espacio_disco = 0;
    for (int plato_id = 0; plato_id < platos.size(); plato_id++) {
        for (int sup_id = 0; sup_id < 2; sup_id++) {
            Superficie& sup = (sup_id == 0) ? platos[plato_id].sup : platos[plato_id].inf;
            for (int pista_id = 0; pista_id < sup.pistas.size(); pista_id++) {
                Pista& pist = sup.pistas[pista_id];
                for (int sector_id = 0; sector_id < pist.sectores.size(); sector_id++) {
                    Sector& sector_pe = pist.sectores[sector_id];
                    espacio_disco += sector_pe.espacioLibre;
                }
            }
        }
    }
    if (espacio_disco >= espacio_requerido) {
        return true;
    }
    else {
        return false;
    }
}

void Disco::Insertar_data(std::string datos, int num_tipo, int real_tam, int registro_id) {
    bool inserted = false;
    int relleno = datos.size() - real_tam;
    int offset = 0;
    int sin_relleno_pos = -1;
    std::vector<Metadata> informacion;
    for (int plato_id = 0; plato_id < platos.size(); plato_id++) {
        for (int sup_id = 0; sup_id < 2; sup_id++) {
            Superficie& sup = (sup_id == 0) ? platos[plato_id].sup : platos[plato_id].inf;
            for (int pista_id = 0; pista_id < sup.pistas.size(); pista_id++) {
                Pista& pist = sup.pistas[pista_id];
                for (int sector_id = 0; sector_id < pist.sectores.size(); sector_id++) {
                    Sector& sector_pe = pist.sectores[sector_id];
                    int ini = sector_pe.info.size();
                    int dur = 0;
                    sin_relleno_pos = -1;
                    while (sector_pe.espacioLibre > 0 && offset < datos.size()) {
                        if (sin_relleno_pos == -1 && offset >= relleno) {
                            sin_relleno_pos = ini + dur;
                        }
                        sector_pe.info.push_back(datos[offset]);
                        sector_pe.espacioLibre--;
                        offset++;
                        dur++;
                        inserted = true;
                    }
                    if (inserted) {
                        informacion.push_back(Metadata{registro_id,sector_pe.id,num_tipo,ini,sin_relleno_pos,dur});

                        inserted = false;
                    }
                    if (offset >= datos.size()) {
                        std::ofstream out("D:/metadata.txt", std::ios::app);
                        for (auto& p : informacion) {
                            out << p.registro_id << " "
                                << p.Sector << " "
                                << p.num_tipo << " "
                                << p.Inicio_sector << " "
                                << p.Inicio_Relleno << " "
                                << p.duracion << std::endl;
                        }
                        out.close();
                        return;
                    }
                }
            }
        }
    }
}

Sector& Disco::get_allinfo(int sector_id_send,std::vector<Info>& info) {
    for (int plato_id = 0; plato_id < platos.size(); plato_id++) {
        for (int sup_id = 0; sup_id < 2; sup_id++) {
            Superficie& sup = (sup_id == 0) ? platos[plato_id].sup : platos[plato_id].inf;
            for (int pista_id = 0; pista_id < sup.pistas.size(); pista_id++) {
                Pista& pist = sup.pistas[pista_id];
                for (int sector_id = 0; sector_id < pist.sectores.size(); sector_id++) {
                    Sector& sector_pe = pist.sectores[sector_id];
                    if (sector_id_send == sector_pe.id) {
                        info.push_back(Info{ plato_id,sup.id,pist.id,sector_pe.id });
                        return sector_pe;
                    }
                }
            }
        }
    }
}

void Disco::Rebuild_data() {
    std::ifstream os("D:/metadata.txt");
    bool cabecera = false;
    std::string linea;
    std::string armado_final;
    std::vector<Info> informacion;
    std::vector<std::pair<std::string, std::vector<Info>>> registro;
    int registro_actual = 0;
    int atributo_actual = 0;
    
    if (!os.is_open()) {
        std::cout << " No se pudo abrir metadata para reconstru" << std::endl;
        return;
    }
    while (std::getline(os, linea)) {
        if (!cabecera) {
            cabecera = !cabecera;
            continue;
        }
        std::string registro_id_str,sector_id_str,num_tipo_str,inicio_sector_str,inicio_relleno_str,duracion_str;
        std::stringstream info(linea);
        std::getline(info, registro_id_str, ' ');
        std::getline(info, sector_id_str, ' ');
        std::getline(info, num_tipo_str, ' ');
        std::getline(info, inicio_sector_str, ' ');
        std::getline(info, inicio_relleno_str, ' ');
        std::getline(info, duracion_str, ' ');

        int registro_id = std::stoi(registro_id_str);
        int sector_id = std::stoi(sector_id_str);
        int num_tipo = std::stoi(num_tipo_str);
        int inicio_sector = std::stoi(inicio_sector_str);
        int inicio_relleno = std::stoi(inicio_relleno_str);
        int duracion = std::stoi(duracion_str);

        

        if (inicio_relleno == -1) {
            continue;
        }

        if (atributo_actual != num_tipo) {
            if (atributo_actual == 2 || atributo_actual == 3 || atributo_actual == 4) {
                armado_final.insert(armado_final.begin() + (armado_final.size() - 2), '.');
            }
            registro.push_back({armado_final,informacion});
            armado_final.clear();
            informacion.clear();
        }

        if (registro_actual != registro_id) {
            tabla_final.push_back(registro);
            registro.clear();
            armado_final.clear();
            informacion.clear();
        }

        registro_actual = registro_id;
        atributo_actual = num_tipo;
        Sector& sector_actu = get_allinfo(sector_id, informacion);
        std::string atributo;
        atributo.assign(sector_actu.info.begin() + inicio_relleno, sector_actu.info.begin() + inicio_sector + duracion);
        armado_final += atributo;

    }
    if (!armado_final.empty()) {
        if (atributo_actual == 2 || atributo_actual == 3 || atributo_actual == 4) {
            armado_final.insert(armado_final.size() - 2, 1, '.');
        }
        registro.push_back({ armado_final, informacion });
    }
    if (!registro.empty()) {
        tabla_final.push_back(registro);
    }
    os.close();

    for (int i = 0; i < tabla_final.size(); i++) {
        std::cout << "Registro " << i << ":\n";
        for (auto& campo : tabla_final[i]) {
            std::cout << "  " << campo.first << "\n";
        }
    }
}
int Disco::obtener_indice_tipo(std::string campo) {
    std::transform(campo.begin(), campo.end(), campo.begin(), ::tolower);
    if (campo == "index") { 
        return 0; 
    }
    if (campo == "item") { 
        return 1; 
    }
    if (campo == "cost") { 
        return 2; 
    }
    if (campo == "tax") { 
        return 3; 
    }
    if (campo == "total") { 
        return 4; 
    }
    return -1;
}
void Disco::mostrar_registro(int index) {
    if (index < tabla_final.size()) {
        std::cout << "  Registro:\n";
        for (auto& campo : tabla_final[index]) {
            std::cout << "    Dato: " << campo.first << "\n";
            std::cout << "    Ubicación en disco:\n";
            for (auto& info : campo.second) {
                std::cout << "      Plato: " << info.num_plato << ", Superficie: " << info.num_superficie
                    << ", Pista: " << info.num_pista << ", Sector: " << info.num_sector << "\n";
            }
        }
    }
}

void Disco::get_columnas(std::string campo, std::string valor, RAM& ram) {
    std::vector<Avl_info> columna_arbol;
    std::transform(campo.begin(), campo.end(), campo.begin(), ::tolower);

    int indice_tipo = -1;
    if (campo == "index") {
        indice_tipo = 0;
    }
    else if (campo == "item") {
        indice_tipo = 1;
    } 
    else if (campo == "cost") {
        indice_tipo = 2;
    }
    else if (campo == "tax") {
        indice_tipo = 3;
    } 
    else if (campo == "total") {
        indice_tipo = 4;
    } 

    if (indice_tipo == -1) {
        std::cout << "Campo inválido.\n";
        return;
    }

    if (tabla_final.empty()) {
        std::cout << "No hay datos cargados en memoria.\n";
        return;
    }

    for (int i = 0; i < tabla_final.size(); i++) {
        columna_arbol.push_back(Avl_info{ i, tabla_final[i][indice_tipo].first, indice_tipo });
    }

    ram.construir(campo, columna_arbol);
    auto resultado = ram.buscar(campo, valor);

    if (!resultado.first.dato.empty()) {
        std::cout << "Resultado encontrado:\n";
        mostrar_registro(resultado.first.index_tabla);

        if (!resultado.second.empty()) {
            std::cout << "  Coincidencias duplicadas:\n";
            for (auto& dup : resultado.second) {
                mostrar_registro(dup.index_tabla);
            }
        }
    }
    else {
        std::cout << "No se encontró el valor " << valor << " en el campo " << campo << "\n";
    }
}