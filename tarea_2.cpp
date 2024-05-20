#include <iostream>
#include <stdlib.h>
#include<fstream>
#include<string>
using namespace std;


class super_string {
private:

struct nodo {
nodo *left = nullptr, *right = nullptr;
int index;
char c;
nodo(int index, char c) {}
};

int height = 0; // Altura del árbol
int length = 0; // Largo del super-string
nodo* root = nullptr; // Raíz del super-string

public:

super_string() {}
void juntar(super_string &s);
void agregar(char c); // Insertar un caracter en la última posición
// En la izquierda esta el super_string a y en la derecha el super_string b
void separar(int i, super_string &a, super_string &b);
void reverso(); // No debe cambiar la altura del árbol
int recortar(); // Retorna this->height después de recortar
string stringizar(); // Debe ser O(n)
void limpiar(); // Se deben borrar todos los nodos del super-string

};

int main() {
    ifstream archivo("prueba.txt");
    string par1, par2, par3;

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo" << endl;
        return 1; // error
    }

    string linea;
    while (getline(archivo, linea)) { // lee el archivo 
        size_t pos1 = linea.find(' '); // Encuentra la posición del primer espacio en la línea
        if (pos1 != string::npos) { // Verifica si se encontró un espacio
            par1 = linea.substr(0, pos1); // Obtiene la primera parte
            size_t pos2 = linea.find(' ', pos1 + 1); // Encuentra la posición del segundo espacio
            if (pos2 != string::npos) { // Verifica si se encontró un segundo espacio
                par2 = linea.substr(pos1 + 1, pos2 - pos1 - 1); // Obtiene la segunda parte
                par3 = linea.substr(pos2 + 1); // Obtiene la tercera parte
            } else {
                // Si no se encontró un segundo espacio, solo hay dos partes
                par2 = linea.substr(pos1 + 1); // Obtiene la segunda parte
            }

            // Verificar si la segunda parte contiene el nombre "El_Carlos"
            size_t posElCarlos = par2.find("El_Carlos");
            if (posElCarlos != string::npos) {
                // Si la segunda parte contiene "El_Carlos", actualizarla
                par2 = par2.substr(0, posElCarlos);
            }

            cout << "Par 1: " << par1 << endl;
            cout << "Par 2: " << par2 << endl;
            if (!par3.empty()) {
                cout << "Par 3: " << par3 << endl;
            }
        } else {
            // Si no se encuentra ningún espacio, hay solo una parte
            cout << "Par único: " << linea << endl;
        }
    }

    archivo.close(); // cierra el archivo después de terminar de leerlo
    return 0;
}