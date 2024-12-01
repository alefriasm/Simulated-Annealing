#ifndef ALEJANDRA_H
#define ALEJANDRA_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <set>
#include <list>
#include <string>
#include "alea.h"
#include "basics.h"
#include "graphb.h"
#include "graphc.h"
#include "graphi.h"
#include "gsl/gsl_cdf.h"
#include "gsl/gsl_randist.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ostream;
using std::string;
using std::set;
using std::list;
using std::ifstream;
using std::ios;

class ALEJANDRA {
    private:
        Alea alea;
        Basics basics;
        GraphB graphb_inst;

        std::vector<int> nodos; //vector para almacenar el número de nodos en cada capa

    public:
        ALEJANDRA(int semilla) : alea(semilla), basics(alea), graphb_inst(alea) {}

        void MENU();
        void generarTORIA();
        void cargarRED();
        int num_nodes;
        int num_edges;
        void moverNododeCapa();
        double calcularHS();
        double calcularCHS();
        double calcularPHS();
        void simulatedannealing(double temperaturaInicial, double factorEnf, int iteracionesporTemperatura, int kmax, std::set<int>* capas, int nucapas);
        set <int>* capas;
        int deaqui;
        int paca;
        int este;
        double mejorHS;
        set<int>* mejorConfig;
        int repeticiones;
        set<int>* copiarCapas(set<int>* original, int numCapas);
        int nucapas;
        void resultados();
        ofstream archivo;
        void guardarResultados(ALEJANDRA& alejandra, const string& nombreArchivo);
        std::vector<std::pair<std::string, std::string>> edges;
        set<std::string> nodes;
        int get_node_index(const string& node, std::vector<std::string>& nodeList, int& currentNodeIndex);



};

#endif
