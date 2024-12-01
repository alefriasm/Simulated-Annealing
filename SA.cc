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
#include "alejandra.h"
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


int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Número equivocado de parámetros.\n";
        exit(1);
    }
    int semilla = atoi(argv[1]);
    cout << "Semilla: " << semilla << endl;

        ALEJANDRA alejandra(semilla);
        alejandra.MENU();
        alejandra.moverNododeCapa();
        alejandra.calcularHS();
        alejandra.calcularCHS();
        alejandra.calcularPHS();
  
  //Parámetros de Simulated Annealing (Cambiar los parametros)
        double temperaturaInicial = 100.00;
        double factorEnf = 0.80;
        int iteracionesporTemperatura = 150;
        int kmax = 10000;
  

        alejandra.simulatedannealing(temperaturaInicial, factorEnf, iteracionesporTemperatura, kmax, alejandra.capas, alejandra.nucapas);
 
        alejandra.resultados();

        alejandra.guardarResultados(alejandra, "SA_BEST.txt");
  
  return 0;
  
}
