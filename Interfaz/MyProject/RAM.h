#pragma once
#include "AVL.h"
#include <string>
#include <vector>

class RAM {
public:
    CBinTree<int> arbol_index;
    CBinTree<std::string> arbol_item;
    CBinTree<float> arbol_cost;
    CBinTree<float> arbol_tax;
    CBinTree<float> arbol_total;

    bool construido_index = false;
    bool construido_item = false;
    bool construido_cost = false;
    bool construido_tax = false;
    bool construido_total = false;

    void construir(std::string campo, std::vector<Avl_info> dupl) {
        if (campo == "index" && !construido_index) {
            for (auto& x : dupl)
                arbol_index.Ins(std::stoi(x.dato), x);
            construido_index = true;
        }
        else if (campo == "item" && !construido_item) {
            for (auto& x : dupl)
                arbol_item.Ins(x.dato, x);
            construido_item = true;
        }
        else if (campo == "cost" && !construido_cost) {
            for (auto& x : dupl)
                arbol_cost.Ins(std::stof(x.dato), x);
            construido_cost = true;
        }
        else if (campo == "tax" && !construido_tax) {
            for (auto& x : dupl)
                arbol_tax.Ins(std::stof(x.dato), x);
            construido_tax = true;
        }
        else if (campo == "total" && !construido_total) {
            for (auto& x : dupl)
                arbol_total.Ins(std::stof(x.dato), x);
            construido_total = true;
        }
    }

    std::pair<Avl_info, std::vector<Avl_info>> buscar(std::string campo, std::string valor) {
        if (campo == "index") {
            return buscarnode(arbol_index, std::stoi(valor));
        }
        else if (campo == "item") {
            return buscarnode(arbol_item, valor);
        }
        else if (campo == "cost") {
            return buscarnode(arbol_cost, std::stof(valor));
        }
        else if (campo == "tax") {
            return buscarnode(arbol_tax, std::stof(valor));
        }
        else if (campo == "total") {
            return buscarnode(arbol_total, std::stof(valor));
        }
        return {};
    }
};