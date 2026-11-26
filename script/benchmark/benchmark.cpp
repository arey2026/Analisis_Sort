// ---------------------------------------------------------------
// Mide el tiempo de ejecución de los seis algoritmos de la clase
// Algoritmos con 5 tamaños de arreglo y guarda los resultados en
// results/resultados_sort.csv. Al terminar, llama al script
// script/generar_excel.ps1 para crear results/resultados_sort.xlsx
// y las imágenes de los gráficos en results/graficos/.
//
// Uso (desde la carpeta raíz del proyecto):
//     benchmark.exe              -> mide y genera el Excel
//     benchmark.exe --sin-excel  -> solo mide (no genera el Excel)
//
// Reglas de medición:
//   * Mismos datos para todos: para cada tamaño se generan los datos
//     UNA vez con una semilla fija, y cada algoritmo recibe su propia copia.
//   * Solo se mide el ordenamiento: la generación de datos, las copias,
//     la verificación y las impresiones quedan FUERA del cronómetro.
//   * Se usa std::chrono::steady_clock (reloj que nunca retrocede).
//   * Cada resultado se verifica con estaOrdenado(); si falla, se aborta.
//   * Todas las pruebas se ejecutan (ninguna se omite): los tamaños se
//     eligieron para que incluso los algoritmos O(n^2) terminen en
//     un tiempo razonable sin trabar la computadora.
// ---------------------------------------------------------------
#include "algoritmos.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

// ===================== CONFIGURACIÓN =====================
const unsigned SEMILLA = 2026;               // semilla fija -> experimentos reproducibles
const int VALOR_MINIMO = 0;                  // rango de los números aleatorios
const int VALOR_MAXIMO = 10000000;
const int REPETICIONES = 5;                  // repeticiones por prueba (se promedian)
const long long ELEMENTOS_POR_LOTE = 100000; // ver explicación de "lote" más abajo

// 5 tamaños, cada uno mayor que el anterior, hasta un millón.
const std::vector<int> TAMANOS = {1000, 10000, 100000, 500000, 1000000};

// Los algoritmos O(n^2) tardan muchísimo en los tamaños grandes
// (Bubble Sort con n = 1,000,000 tarda ~25-30 minutos POR EJECUCIÓN).
// Para que el benchmark termine en un tiempo razonable, en esos tamaños
// hacen menos repeticiones. Los O(n log n) siempre hacen REPETICIONES.
int repeticionesPara(bool esCuadratico, int n) {
    if (!esCuadratico || n <= 100000) return REPETICIONES;
    if (n <= 500000) return 2;
    return 1;
}

const std::string RUTA_CSV = "results/resultados_sort.csv";
const std::string RUTA_CONFIG = "results/config_benchmark.csv";
// =========================================================

struct Algoritmo {
    std::string nombre;
    FuncionSort funcion;
    bool esCuadratico;   // true = O(n^2), false = O(n log n)
};

struct Resultado {
    int n;
    std::string algoritmo;
    double promedioMs;
    double minimoMs;
    double maximoMs;
    int repeticiones;
    int lote;
};

// Genera n números aleatorios con la semilla fija.
std::vector<int> generarDatos(int n) {
    std::mt19937 generador(SEMILLA);
    std::uniform_int_distribution<int> distribucion(VALOR_MINIMO, VALOR_MAXIMO);
    std::vector<int> datos(n);
    for (int i = 0; i < n; i++) {
        datos[i] = distribucion(generador);
    }
    return datos;
}

std::string formatoMiles(long long x) {
    std::string s = std::to_string(x);
    for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3) s.insert(i, ",");
    return s;
}

int main(int argc, char* argv[]) {
    bool generarExcel = true;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--sin-excel") {
            generarExcel = false;
        } else {
            std::cerr << "Argumento desconocido: " << arg << ". Uso: benchmark.exe [--sin-excel]" << std::endl;
            return 1;
        }
    }

    const std::vector<Algoritmo> algoritmos = {
        {"Bubble Sort",    Algoritmos::bubbleSort,    true},  // O(n^2)
        {"Insertion Sort", Algoritmos::insertionSort, true},  // O(n^2)
        {"Selection Sort", Algoritmos::selectionSort, true},  // O(n^2)
        {"Merge Sort",     Algoritmos::mergeSort,     false}, // O(n log n)
        {"Heap Sort",      Algoritmos::heapSort,      false}, // O(n log n)
        {"Quick Sort",     Algoritmos::quickSort,     false}, // O(n log n)
    };

    std::cout << "=== Benchmark de algoritmos de ordenamiento ===" << std::endl;
    std::cout << "Semilla: " << SEMILLA << " | Repeticiones: hasta " << REPETICIONES
              << " | Tamanos: " << TAMANOS.size() << std::endl;
    std::cout << "AVISO: con n = 1,000,000 los algoritmos O(n^2) tardan muchos minutos. Duracion total estimada: ~80 min."
              << std::endl << std::endl;

    std::vector<Resultado> resultados;
    std::cout << std::fixed << std::setprecision(4);

    for (int n : TAMANOS) {
        // 1) Datos originales del tamaño n: se generan ANTES de medir.
        const std::vector<int> original = generarDatos(n);

        // 2) "Lote": ordenar pocos elementos tarda tan poco que el reloj no lo
        //    mide bien. Por eso, en tamaños pequeños, cada repetición ordena un
        //    LOTE de copias y el tiempo se divide entre el tamaño del lote.
        const int lote = static_cast<int>(std::max(1LL, ELEMENTOS_POR_LOTE / n));

        std::cout << "--- n = " << formatoMiles(n) << " (lote de " << lote << ") ---" << std::endl;

        for (const Algoritmo& a : algoritmos) {
            double sumaMs = 0.0, minMs = 1e300, maxMs = 0.0;
            const int repeticiones = repeticionesPara(a.esCuadratico, n);

            std::cout << "  " << std::left << std::setw(15) << a.nombre << std::flush; // progreso visible

            for (int rep = 0; rep < repeticiones; rep++) {
                // Copias para este lote (FUERA del cronómetro).
                std::vector<std::vector<int>> copias(lote, original);

                auto inicio = std::chrono::steady_clock::now();
                for (int k = 0; k < lote; k++) {
                    a.funcion(copias[k]);
                }
                auto fin = std::chrono::steady_clock::now();

                // Verificación (FUERA del cronómetro).
                for (int k = 0; k < lote; k++) {
                    if (!estaOrdenado(copias[k])) {
                        std::cerr << "ERROR: " << a.nombre << " no ordeno correctamente n=" << n << std::endl;
                        return 1;
                    }
                }

                double msPorSort = std::chrono::duration<double, std::milli>(fin - inicio).count() / lote;
                sumaMs += msPorSort;
                minMs = std::min(minMs, msPorSort);
                maxMs = std::max(maxMs, msPorSort);
            }

            Resultado r{n, a.nombre, sumaMs / repeticiones, minMs, maxMs, repeticiones, lote};
            resultados.push_back(r);

            std::cout << std::right << std::setw(14) << r.promedioMs << " ms  ("
                      << repeticiones << " rep.)" << std::endl;
        }
    }

    // ===== Guardar resultados en CSV =====
    std::filesystem::create_directories("results"); // por si la carpeta no existe
    std::ofstream csv(RUTA_CSV);
    if (!csv) {
        std::cerr << "No se pudo crear " << RUTA_CSV << ". Ejecuta el programa desde la raiz del proyecto." << std::endl;
        return 1;
    }
    csv << "Tamano,Algoritmo,Estado,PromedioMs,MinimoMs,MaximoMs,Repeticiones,Lote\n";
    csv << std::setprecision(6);
    for (const Resultado& r : resultados) {
        csv << r.n << "," << r.algoritmo << ",COMPLETADO," << r.promedioMs << "," << r.minimoMs << ","
            << r.maximoMs << "," << r.repeticiones << "," << r.lote << "\n";
    }
    csv.close();

    std::ofstream config(RUTA_CONFIG);
    config << "Clave,Valor\n";
    config << "Semilla," << SEMILLA << "\n";
    config << "Rango de valores,[" << VALOR_MINIMO << " - " << VALOR_MAXIMO << "]\n";
    config << "Repeticiones O(n log n)," << REPETICIONES << " en todos los tamanos\n";
    config << "Repeticiones O(n^2)," << REPETICIONES << " hasta n = 100000; 2 en n = 500000; 1 en n = 1000000\n";
    config << "Elementos por lote," << ELEMENTOS_POR_LOTE << "\n";
    config << "Compilador,GCC " << __VERSION__ << "\n";
#ifdef __OPTIMIZE__
    config << "Optimizacion,Activada (-O2)\n";
#else
    config << "Optimizacion,Desactivada\n";
#endif
    config << "Reloj,std::chrono::steady_clock\n";
    config.close();

    std::cout << "\nResultados guardados en " << RUTA_CSV << std::endl;

    // ===== Generar el Excel =====
    if (generarExcel) {
        std::cout << "Generando results/resultados_sort.xlsx con Excel..." << std::endl;
        int codigo = std::system("powershell -NoProfile -ExecutionPolicy Bypass -File script\\generar_excel.ps1");
        if (codigo != 0) {
            std::cerr << "No se pudo generar el Excel (codigo " << codigo << ")." << std::endl;
            return 1;
        }
    }
    return 0;
}
