// algoritmos.cpp
// Implementación de los seis algoritmos de la clase Algoritmos.
// Ninguno usa std::sort ni otra función de ordenamiento de la biblioteca:
// todo está implementado a mano. Solo usamos std::swap para intercambiar.
// ---------------------------------------------------------------
#include "algoritmos.h"
#include <utility> 


void Algoritmos::insertionSort(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());

    for (int i = 1; i < n; i++) {
        int llave = arr[i];
        int j = i - 1;      

        while (j >= 0 && arr[j] > llave) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = llave;
    }
}

void Algoritmos::selectionSort(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());

    for (int i = 0; i < n - 1; i++) {
        int indiceMinimo = i; 

        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[indiceMinimo]) {
                indiceMinimo = j;
            }
        }

        if (indiceMinimo != i) {
            std::swap(arr[i], arr[indiceMinimo]);
        }
    }
}

void Algoritmos::bubbleSort(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());

    for (int pasada = 0; pasada < n - 1; pasada++) {
        bool huboIntercambio = false;

        // Los últimos 'pasada' elementos ya están en su lugar final.
        for (int j = 0; j < n - 1 - pasada; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                huboIntercambio = true;
            }
        }

        if (!huboIntercambio) {
            break; // ya está ordenado
        }
    }
}

void Algoritmos::mergeSort(std::vector<int>& arr) {
    if (arr.size() < 2) return;

    // Creamos el arreglo auxiliar UNA sola vez (en vez de uno por cada
    // mezcla) para no gastar tiempo reservando memoria repetidamente.
    std::vector<int> aux(arr.size());
    mergeSortRecursivo(arr, aux, 0, static_cast<int>(arr.size()) - 1);
}

void Algoritmos::mergeSortRecursivo(std::vector<int>& arr, std::vector<int>& aux,
                                           int izq, int der) {
    if (izq >= der) return; // 0 o 1 elemento: caso base

    int medio = izq + (der - izq) / 2; 
    mergeSortRecursivo(arr, aux, izq, medio);     
    mergeSortRecursivo(arr, aux, medio + 1, der); 
    mezclar(arr, aux, izq, medio, der);           
}

void Algoritmos::mezclar(std::vector<int>& arr, std::vector<int>& aux,
                                int izq, int medio, int der) {
    // Copiamos el tramo al arreglo auxiliar.
    for (int k = izq; k <= der; k++) {
        aux[k] = arr[k];
    }

    int i = izq;       
    int j = medio + 1; 
    int k = izq;       

    while (i <= medio && j <= der) {
        if (aux[i] <= aux[j]) {
            arr[k++] = aux[i++];
        } else {
            arr[k++] = aux[j++];
        }
    }

    while (i <= medio) {
        arr[k++] = aux[i++];
    }
}

void Algoritmos::heapSort(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());

    for (int i = n / 2 - 1; i >= 0; i--) {
        hundir(arr, n, i);
    }

    for (int fin = n - 1; fin > 0; fin--) {
        std::swap(arr[0], arr[fin]); 
        hundir(arr, fin, 0);         
    }
}

void Algoritmos::hundir(std::vector<int>& arr, int n, int i) {
    while (true) {
        int mayor = i;
        int izq = 2 * i + 1;
        int der = 2 * i + 2;

        if (izq < n && arr[izq] > arr[mayor]) mayor = izq;
        if (der < n && arr[der] > arr[mayor]) mayor = der;

        if (mayor == i) {
            return; 
        }

        std::swap(arr[i], arr[mayor]);
        i = mayor; 
    }
}


void Algoritmos::quickSort(std::vector<int>& arr) {
    if (arr.size() < 2) return;
    quickSortRecursivo(arr, 0, static_cast<int>(arr.size()) - 1);
}

void Algoritmos::quickSortRecursivo(std::vector<int>& arr, int izq, int der) {
    if (izq >= der) return; // 0 o 1 elemento: caso base

    int p = particion(arr, izq, der);
    quickSortRecursivo(arr, izq, p);     
    quickSortRecursivo(arr, p + 1, der); 
}

int Algoritmos::particion(std::vector<int>& arr, int izq, int der) {
    int pivote = arr[izq + (der - izq) / 2];
    int i = izq - 1;
    int j = der + 1;

    while (true) {
        do { i++; } while (arr[i] < pivote); 
        do { j--; } while (arr[j] > pivote); 

        if (i >= j) {
            return j; 
        }
        std::swap(arr[i], arr[j]);
    }
}
