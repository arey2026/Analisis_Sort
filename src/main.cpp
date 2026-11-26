#include "algoritmos.h"
#include <iostream>
#include <string>

struct Algoritmo {
    std::string nombre;
    FuncionSort funcion;
};

int main() {
    const std::vector<int> ejemplo = {29, 10, 14, 37, 13, 5, 88, 1, 14, -7};

    const std::vector<Algoritmo> algoritmos = {
        {"Bubble Sort   ", Algoritmos::bubbleSort},
        {"Insertion Sort", Algoritmos::insertionSort},
        {"Selection Sort", Algoritmos::selectionSort},
        {"Merge Sort    ", Algoritmos::mergeSort},
        {"Heap Sort     ", Algoritmos::heapSort},
        {"Quick Sort    ", Algoritmos::quickSort},
    };

    std::cout << "=== Tarea 3: Analisis de Algoritmos de Sort ===" << std::endl;
    std::cout << "Original      : ";
    imprimirArreglo(ejemplo);
    std::cout << std::endl;

    for (const Algoritmo& a : algoritmos) {
        std::vector<int> copia = ejemplo;
        a.funcion(copia);
        std::cout << a.nombre << ": ";
        imprimirArreglo(copia);
        std::cout << "  -> ordenado? " << (estaOrdenado(copia) ? "si" : "NO") << std::endl;
    }

    return 0;
}
