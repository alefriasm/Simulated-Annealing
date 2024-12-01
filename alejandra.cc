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
#include <sstream>

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
using std::stringstream;

string getTimestamp() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
  char buffer[10]; // Suficiente espacio para "HH-MM-SS"
    std::strftime(buffer, sizeof(buffer), "%H-%M-%S", &tm);
    return string(buffer);   
}

int ALEJANDRA::get_node_index(const string& node, std::vector<std::string>& nodeList, int& currentNodeIndex){
    for (int i = 0; i < nodeList.size(); i++){
        if (nodeList[i] == node){
            return i;
        }
    }

    nodeList.push_back(node);
    return currentNodeIndex++;

}

void ALEJANDRA::MENU(){
    int opcion;
    cout << "Seleccione una opción:\n1. Red desde archivo\n2. Generar red aleatoria\n.";
    cin >> opcion;

    if (opcion == 1){
        cargarRED();
    } else if (opcion == 2){
        generarTORIA();
    } else {
        cout << "Opcion no válida.\n";
    }
    };

void ALEJANDRA::generarTORIA(){
     cout << "Ingrese el número de nodos: ";
    cin >> num_nodes;

    cout << "Ingrese el número de aristas: ";
    cin >> num_edges;

    nodes.clear();
    edges.clear();

    std:: vector<std::string>nodeList;
    for (int i = 0; i < num_nodes; i++){
        string node = "Nodo " +std::to_string(i);
        nodes.insert(node);
        nodeList.push_back(node);
    }

        graphb_inst.make_nw(num_nodes, num_edges, true);

            //Traducción
        int currentNodeIndex = 0;
        for (const auto& edge : edges){
            int source = get_node_index(edge.first, nodeList, currentNodeIndex);
            int target = get_node_index(edge.second, nodeList, currentNodeIndex);
            graphb_inst.change_interaction(source, target, true);
        }

    // Mostrar los nodos y aristas leídos
        cout << "Nodos:" << endl;
        for (const auto& node : nodes) {
            cout << node << " ";
        }
        cout << endl;

        cout << "Aristas:" << endl;
        for (const auto& edge : edges) {
            cout << edge.first << " - " << edge.second << endl;
        }

        cout << "Lista de nodos: " << endl;
        for (int i = 0; i < nodeList.size(); i++){
            cout << "Nodo " << i << ": " << nodeList[i] << endl;
        }
        
        graphb_inst.printnw(cout);
        graphb_inst.print_dot("GRAFSA");
        basics.get_dot_fig("GRAFSA");

        cout << "Ingrese el número de capas: ";
        cin >> nucapas;

        capas = new set<int>[nucapas];

//Asignación aleatoria de nodos a capas
        std::vector<std::string> nodeVector(nodes.begin(), nodes.end());
        for (int i = 0; i < num_nodes; ++i) {
                int capasAlea = alea.randint(0, nucapas); //declarar fuera del ciclo
                capas[capasAlea].insert(i);
            }


// Mostrar los nodos almacenados en cada capa
        for (int i = 0; i < nucapas; ++i) {
            cout << "Capa " << i + 1 << ": ";
            for (int node : capas[i]) {
                cout << node << " ";
            }
            cout << endl;
        }
    }


void ALEJANDRA::cargarRED(){ 
        string filename;
        cout << "Ingrese el nombre del archivo: ";
        cin >> filename;

        ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo " << filename << endl;
            return;
        }

        string algo, yqs;
        stringstream algodon;
        int nose;
        string node1, node2;

        std::vector<std::string> nodeList;
        int currentNodeIndex = 0;

        nodes.clear();
        edges.clear();


        while (getline(file, algo))
        { 
            algodon.str(algo);
            nose = 0;
            while (algodon >> yqs)
            {
                cout << yqs << " ";
                nose++;
            }
             algodon.clear();

            cout << nose << endl;
            if (nose == 2) {
                algodon.str(algo);
                algodon >> node1;
                algodon >> node2;
                edges.push_back({node1, node2});
                nodes.insert(node1);
                nodes.insert(node2);
                algodon.clear();
            }
           

        }

        file.close();

        num_nodes = nodes.size();
        num_edges = edges.size();
        graphb_inst.make_nw(num_nodes, true); //Red vacía con el número de nodos, pero sin aristas

        //Traducción
        for (const auto& edge : edges){
            int source = get_node_index(edge.first, nodeList, currentNodeIndex);
            int target = get_node_index(edge.second, nodeList, currentNodeIndex);
            graphb_inst.change_interaction(source, target, true);
        }

        cout << "Número de nodos: " << num_nodes << endl;
        cout << "Número de aristas: " << num_edges << endl;

    // Mostrar los nodos y aristas leídos
        cout << "Nodos:" << endl;
        for (const auto& node : nodes) {
            cout << node << " ";
        }
        cout << endl;

        cout << "Aristas:" << endl;
        for (const auto& edge : edges) {
            cout << edge.first << " - " << edge.second << endl;
        }

        cout << "Lista de nodos: " << endl;
        for (int i = 0; i < nodeList.size(); i++){
            cout << "Nodo " << i << ": " << nodeList[i] << endl;
        }
        
        graphb_inst.printnw(cout);
        graphb_inst.print_dot("GRAFSA");
        basics.get_dot_fig("GRAFSA");

        cout << "Ingrese el número de capas: ";
        cin >> nucapas;

        capas = new set<int>[nucapas];

//Asignación aleatoria de nodos a capas
        std::vector<std::string> nodeVector(nodes.begin(), nodes.end());
        for (int i = 0; i < num_nodes; ++i) {
                int capasAlea = alea.randint(0, nucapas); //declarar fuera del ciclo
                capas[capasAlea].insert(i);
            }


// Mostrar los nodos almacenados en cada capa
        for (int i = 0; i < nucapas; ++i) {
            cout << "Capa " << i + 1 << ": ";
            for (int node : capas[i]) {
                cout << node << " ";
            }
            cout << endl;
        }
    }
    

void ALEJANDRA::moverNododeCapa() {
        int cual = alea.randint(0, graphb_inst.number_of_nodes()-1);
            cout << "Nodo seleccionado: " << cual << endl;
        int dondeesta = -1;
        int i;
        for (i = 0; i < nucapas; i++) {
            if (basics.contains(cual, capas[i])) {
            dondeesta = i;
            break;
            }
        }
        if (dondeesta < 0) {
            cout << "Error: nunca encontre ese nodo\n";
            return;
        }
        int padonde;
        do {
            padonde = alea.randint(0, nucapas - 1);
        } while (padonde == dondeesta);
        capas[dondeesta].erase(cual);
        capas[padonde].insert(cual);

        cout << "Se movió el nodo: " << cual << " de la capa " << dondeesta + 1 << " a la capa " << padonde + 1 << endl;
        este = cual;
        deaqui = dondeesta;
        paca = padonde;
            return;  
    }

double ALEJANDRA::calcularHS() {
        double hs =graphb_inst.hierarchical_score(capas, nucapas);
        cout << "HS: " << hs << endl;
        return hs;
    }

double ALEJANDRA::calcularCHS(){

        std::vector<int> niveles;
        for (int i = 0; i < nucapas-1; i++) {
        niveles.push_back(i);
    }

        int Nd_obs = graphb_inst.Nd_obs;
        int Nu_obs = graphb_inst.Nu_obs;
        int Nh_obs = graphb_inst.Nh_obs;

        int Nd_exp = 0, Nu_exp = 0, Nh_exp = 0;
        for (size_t i = 0; i < niveles.size(); i++){
            Nh_exp += graphb_inst.cal_nodos_esp(capas[i])*graphb_inst.cal_nodos_esp(capas[i]);
            for (size_t j = 0; j < i; j++){
                Nd_exp += graphb_inst.cal_nodos_esp(capas[i])*graphb_inst.cal_nodos_esp(capas[j]);
            }
        }

            Nu_exp = Nd_exp;

    //MOstrar los valores observados y esperados
        cout << "Nd_obs: " << Nd_obs << ", Nu_obs: " << Nu_obs << ", Nh_obs: " << Nh_obs << endl;
        cout << "Nd_exp: " << Nd_exp << ", Nu_exp: " << Nu_exp << ", Nh_exp: " << Nh_exp << endl;
        
        if (Nd_exp == 0 || Nu_exp == 0 || Nh_exp == 0) {
            cout << "Error: uno de los valores esperados es cero." << endl;
            return 1000000;  
    }

        double chs = graphb_inst.corrected_hierarchical_score(Nd_obs, Nu_obs, Nh_obs, Nd_exp, Nu_exp, Nh_exp);
        cout << "CHS: " << chs << endl;
        return chs;
    }

double ALEJANDRA::calcularPHS(){
        double phs = graphb_inst.probabilistic_hierarchical_score(capas, nucapas, num_nodes);
        cout << "PHS: " << phs << endl;
        return phs;
    }

set<int>* ALEJANDRA::copiarCapas(set<int>* original, int numCapas) {
        set<int>* copia = new set<int>[numCapas];
        for (int i = 0; i < numCapas; i++) {
            copia[i] = original[i];
        }
        return copia;
    }

void ALEJANDRA::simulatedannealing(double temperaturaInicial, double factorEnf, int iteracionesporTemperatura, int kmax, std::set<int>* capas, int nucapas){
    srand(static_cast<unsigned int>(time(0)));

        double chsActual;
        double phsActual;
        double hsVecino;
        double deltaHS;
        double hsActual = calcularHS();
        mejorHS = hsActual;
        mejorConfig = copiarCapas(capas, nucapas);
        repeticiones = 0;
        
        int aceptacionesPeores = 0;

  //Calculo del HS de la red actual
        cout << "HS actual: " << hsActual << endl;
  
        chsActual = calcularCHS();
        if (chsActual < 0) {
            cout << "CHS actual: Error en el cálculo (valor esperado cero)." << endl;
            } else {
            cout << "CHS actual: " << chsActual << endl;
            }

        phsActual = calcularPHS();
        cout << "PHS actual: " << phsActual << endl;
        
// Solicitar el nombre base del archivo al usuario
        string baseFilename;
        cout << "Ingrese el nombre base para el archivo de resultados: ";
        cin >> baseFilename;
            
// Generar nombre de archivo con marca de tiempo
        string filename = baseFilename + "_" + getTimestamp() + ".txt";
        ofstream archivo (filename, ios::out);
        archivo << "#Repetición, HS, CHS, PHS, PROBABILIDAD, ACEPTADO" << endl;
        archivo << repeticiones << ", " << hsActual << ", " << chsActual << ", " << phsActual << endl;

//Bucle principal de SA
        int totalIteraciones = 0;
        while (temperaturaInicial > 1e-5){
            for (int i = 0; i < iteracionesporTemperatura && totalIteraciones < kmax; i++){
        hsActual = calcularHS();
        moverNododeCapa();
        hsVecino = calcularHS();
        deltaHS = hsVecino - hsActual;

        double probabilidadAceptacion = exp(deltaHS / temperaturaInicial);
        double numeroaleatorio = static_cast<double>(rand()) / (RAND_MAX *1.0);
        bool aceptado = false;

        if (deltaHS > 0 || probabilidadAceptacion > numeroaleatorio){
            hsActual = hsVecino;
            chsActual = calcularCHS();
            phsActual = calcularPHS();

            if (deltaHS < 0){
                aceptacionesPeores++;
            }

            if (hsActual > mejorHS) {
                mejorHS = hsActual;
                delete[] mejorConfig;
                mejorConfig = copiarCapas(capas, nucapas);
            }
                aceptado = true;

        } else {
        capas[paca].erase(este);
        capas[deaqui].insert(este);
      }
        repeticiones ++;
        totalIteraciones ++;

        cout << "Temperatura actual: " << temperaturaInicial << endl;

        if (aceptado) {
                archivo << repeticiones << ", " << hsActual << ", " << chsActual << ", " << phsActual << ", " << probabilidadAceptacion << ", A" << endl;
            } else {
                archivo << repeticiones << ", " << hsActual << ", " << chsActual << ", " << phsActual << ", " << probabilidadAceptacion << ", NA" << endl;
            }
    }
//Enfriar la temperatura
        temperaturaInicial *= factorEnf;
    }

        archivo.close();
        cout << "Total de soluciones peores aceptadas: " << aceptacionesPeores << endl;
}

void ALEJANDRA::resultados() {
        cout << "Mejor HS: " << mejorHS << endl;
            cout << "Configuración correspondiente:" << endl;
            for (int i = 0; i < nucapas; i++) {
                cout << "Capa " << i + 1 << ": ";
                for (int node : mejorConfig[i]) {
                    cout << node << " ";
                }
                cout << endl;
            }

        cout << "Número de repeticiones del ciclo: " << repeticiones << endl;
}

void ALEJANDRA::guardarResultados(ALEJANDRA& alejandra, const string& nombreArchivo){
    ofstream archivo(nombreArchivo);

        if (!archivo.is_open()){
        cout << "NO se pudo abrir el archivo. " << endl;
        return;
        }

        archivo << "Mejor HS: " << alejandra.mejorHS << endl;
        archivo << "Configuracion correspondiente: " << endl;
        for (int i = 0; i < alejandra.nucapas; i ++){
        archivo << " Capa " << i + 1 << ": ";
        for (int node : alejandra.mejorConfig[i]){
            archivo << node << " ";
        }
        archivo << endl;
        }
        archivo << "Número de repeticiones del ciclo: " << alejandra.repeticiones << endl;

        archivo.close();
        cout << "Resultados guardados en " << nombreArchivo << endl;
    }
