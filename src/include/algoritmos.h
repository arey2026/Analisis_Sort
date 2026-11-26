// sorting_algorithms.h
// ---------------------------------------------------------------
// Declaración de la clase Algoritmos, que reúne los SEIS
// algoritmos de ordenamiento del proyecto ya implementados en algoritmos.cpp
// Todos los algoritmos reciben el arreglo POR REFERENCIA
// (std::vector<int>&) y lo ordenan de menor a mayor en el mismo arreglo.
// ---------------------------------------------------------------
#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <vector>

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


    // Merge Sort
    static void mergeSortRecursivo(std::vector<int>& arr, std::vector<int>& aux, int izq, int der);
    static void mezclar(std::vector<int>& arr, std::vector<int>& aux, int izq, int medio, int der);

    // Heap Sort
    static void hundir(std::vector<int>& arr, int n, int i);

    // Quick Sort
    static void quickSortRecursivo(std::vector<int>& arr, int izq, int der);
    static int particion(std::vector<int>& arr, int izq, int der);
};


bool estaOrdenado(const std::vector<int>& arr);

void imprimirArreglo(const std::vector<int>& arr);

#endif // SORTING_ALGORITHMS_H
