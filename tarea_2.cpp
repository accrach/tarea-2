#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

class super_string {
private:
    struct nodo {
        nodo* left = nullptr, * right = nullptr;
        int index;
        char c;
        nodo(int index, char c) : index(index), c(c) {}
    };

    int height = 0; // Altura del árbol
    int length = 0; // Largo del super-string
    nodo* root = nullptr; // Raíz del super-string
    // Funciones auxiliares
    void clearArbol(nodo* node) {
        if (!node) return;
        clearArbol(node->left);
        clearArbol(node->right);
        delete node;
    }

    nodo* insert_at_index(nodo* node, int idx, char c) {
        if (!node) {
            return new nodo(idx, c);
        }
        if (idx < node->index) {
            node->left = insert_at_index(node->left, idx, c);
        } 
        else {
            node->right = insert_at_index(node->right, idx, c);
        }
        return node;
    }

    nodo* buelta_index(nodo* node, int idx) {
        if (!node) return nullptr;
        if (node->index == idx) return node;
        if (idx < node->index) {
            auto left = node->left;
            node->left = nullptr;
            return left;
        }
        auto right = node->right;
        node->right = nullptr;
        return right;
    }

    nodo* reversoArbol(nodo* node) {
        if (!node) return nullptr;
        nodo* temp = node->left;
        node->left = reversoArbol(node->right);
        node->right = reversoArbol(temp);
        return node;
    }

    int CaluladorDeHaltura(nodo* node) {
        if (!node) return 0;
        int left_height = CaluladorDeHaltura(node->left);
        int right_height = CaluladorDeHaltura(node->right);
        return 1 + max(left_height, right_height);
    }

    void contruccion_string(nodo* node, string& result) {
        if (!node) return;
        contruccion_string(node->left, result);
        result += node->c;
        contruccion_string(node->right, result);
    }

public:
    super_string() {}
    void juntar(super_string& s);
    void agregar(char c); // Insertar un caracter en la última posición
    // En la izquierda esta el super_string a y en la derecha el super_string b
    void separar(int i, super_string& a, super_string& b);
    void reverso(); // No debe cambiar la altura del árbol
    int recortar(); // Retorna this->height después de recortar
    string stringizar(); // Debe ser O(n)
    void limpiar(); // Se deben borrar todos los nodos del super-string
};


int main() {
    super_string Arbol;
    ifstream archivo("prueba.txt");
    string par1, par2, par3;

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo" << endl;
        return 1; // error
    }

    string linea;
    string instruccion;
    string palabra_agregar;
    string stg_posicion_int;
    int posicion_instruccion;

    while (getline(archivo, linea)) { // lee el archivo 
        int tipo = linea.find(' ');
        if (tipo != -1) { // verificar si cuenta con un espacio
            instruccion = linea.substr(0, tipo);
            string sub_2 = linea.substr(tipo + 1); // subcadena después del primer espacio
            int esp_sub2 = sub_2.find(' ');
            if (esp_sub2 != -1) { // hay un segundo espacio
                stg_posicion_int = sub_2.substr(0, esp_sub2);
                palabra_agregar = sub_2.substr(esp_sub2 + 1);
            } else { // no hay segundo espacio
                stg_posicion_int = sub_2;
                palabra_agregar = "";
            }

            cout << "Instrucción: " << instruccion << endl;
            cout << "Posición: " << stg_posicion_int << endl;
            cout << "Palabra a agregar: " << palabra_agregar << endl;
            
            // Procesar la instrucción
            if (instruccion == "INSERTAR") {
                // Lógica para insertar
            } else if (instruccion == "ELIMINAR") {
                // Lógica para eliminar
            } else if (instruccion == "REVERSO") {
                // Lógica para reverso
            } else if (instruccion == "RECORTAR") {
                // Lógica para recortar
            } else if (instruccion == "MOSTRAR") {
                // Lógica para mostrar
            }
        } else {
            instruccion = linea;
            if (instruccion == "FIN") {
                // Lógica para finalizar
            }
        }
    }

    archivo.close(); // cierra el archivo después de terminar de leerlo
    return 0;
}

