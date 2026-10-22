#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Proceso {
    string nombre;
    int llegada;
    int ejecucion;
    int inicio;
    int final;
};

const int UNIDAD_MS = 100;
const int NUM_CPUS = 2;

vector<Proceso> procesos = {
    {"P1", 0, 8, 0, 0},
    {"P2", 1, 4, 0, 0},
    {"P3", 2, 9, 0, 0},
    {"P4", 3, 5, 0, 0}
};

mutex m;
int siguiente = 0;
steady_clock::time_point t0;

int unidadesTranscurridas() {
    auto ms = duration_cast<milliseconds>(steady_clock::now() - t0).count();
    return (int)((ms + UNIDAD_MS / 2) / UNIDAD_MS);
}

void cpu() {
    while (true) {
        int i;
        {
            lock_guard<mutex> candado(m);
            if (siguiente >= (int)procesos.size()) return;
            i = siguiente++;
        }
        this_thread::sleep_until(t0 + milliseconds(procesos[i].llegada * UNIDAD_MS));
        procesos[i].inicio = unidadesTranscurridas();
        this_thread::sleep_for(milliseconds(procesos[i].ejecucion * UNIDAD_MS));
        procesos[i].final = unidadesTranscurridas();
    }
}

int main() {
    t0 = steady_clock::now();

    vector<thread> hilos;
    for (int i = 0; i < NUM_CPUS; i++) {
        hilos.emplace_back(cpu);
    }
    for (auto& h : hilos) {
        h.join();
    }

    double sumaRetorno = 0, sumaEspera = 0;
    cout << left << setw(8) << "Proceso" << setw(8) << "Llegada" << setw(10) << "Ejecucion"
         << setw(8) << "Inicio" << setw(8) << "Final" << setw(9) << "Retorno" << "Espera\n";

    for (const auto& p : procesos) {
        int retorno = p.final - p.llegada;
        int espera = retorno - p.ejecucion;
        sumaRetorno += retorno;
        sumaEspera += espera;
        cout << left << setw(8) << p.nombre << setw(8) << p.llegada << setw(10) << p.ejecucion
             << setw(8) << p.inicio << setw(8) << p.final << setw(9) << retorno << espera << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "Retorno promedio: " << sumaRetorno / procesos.size() << "\n";
    cout << "Espera promedio: " << sumaEspera / procesos.size() << "\n";
    return 0;
}
