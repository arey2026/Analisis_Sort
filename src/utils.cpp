// ---------------------------------------------------------------
//   estaOrdenado()  verifica que el resultado quedó ordenado
//   imprimirArreglo() muestra el arreglo en consola.
// ---------------------------------------------------------------
#include "algoritmos.h"
#include <iostream>

bool estaOrdenado(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

void imprimirArreglo(const std::vector<int>& arr) {
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << arr[i];
    }
    std::cout << "]";
}
