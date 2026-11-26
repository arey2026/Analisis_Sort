// algoritmos.h
// ---------------------------------------------------------------
// Declaración de la clase Algoritmos, que reúne los SEIS
// algoritmos de ordenamiento del proyecto (implementados en src/algoritmos.cpp).
// Todos los algoritmos reciben el arreglo POR REFERENCIA
// (std::vector<int>&) y lo ordenan de menor a mayor en el mismo arreglo.
// ---------------------------------------------------------------
#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <vector>

// Puntero a función de ordenamiento: permite guardar los seis algoritmos
// en una lista y recorrerlos en las pruebas y en el benchmark.
using FuncionSort = void (*)(std::vector<int>&);

class Algoritmos {
public:
    // ----- Algoritmos O(n^2) -----
    static void insertionSort(std::vector<int>& arr);
    static void selectionSort(std::vector<int>& arr);
    static void bubbleSort(std::vector<int>& arr);

    // ----- Algoritmos O(n log n) -----
    static void mergeSort(std::vector<int>& arr);
    static void heapSort(std::vector<int>& arr);
    static void quickSort(std::vector<int>& arr);

private:
    // Funciones de apoyo internas (solo las usan los algoritmos de arriba).

    // Merge Sort
    static void mergeSortRecursivo(std::vector<int>& arr, std::vector<int>& aux, int izq, int der);
    static void mezclar(std::vector<int>& arr, std::vector<int>& aux, int izq, int medio, int der);

    // Heap Sort
    static void hundir(std::vector<int>& arr, int n, int i);

    // Quick Sort
    static void quickSortRecursivo(std::vector<int>& arr, int izq, int der);
    static int particion(std::vector<int>& arr, int izq, int der);
};

// ===== Funciones auxiliares (src/utils.cpp) =====

// Devuelve true si el arreglo está ordenado de menor a mayor.
bool estaOrdenado(const std::vector<int>& arr);

// Imprime el arreglo en consola, por ejemplo: [5, 2, 9]
void imprimirArreglo(const std::vector<int>& arr);

#endif // ALGORITMOS_H
