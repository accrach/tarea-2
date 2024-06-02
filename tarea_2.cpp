#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

class super_string {
private:
    struct nodo {
        nodo* left = nullptr; // Puntero al nodo hijo izquierdo
        nodo* right = nullptr; // Puntero al nodo hijo derecho
        int index; // Índice del nodo
        char c; // Caracter del nodo
        nodo(int index, char c) {} // Constructor que no hace nada, se debe mantener así
        nodo() {} // Constructor por defecto que no hace nada, se debe mantener así
    };

    int height = 0; // Altura del árbol
    int length = 0; // Largo del super-string
    nodo* root = nullptr; // Raíz del super-string

    // Funciones auxiliares
    void clearArbol(nodo* node) { // Eliminar nodos del árbol de forma recursiva
        if (!node) return;
        clearArbol(node->left);
        clearArbol(node->right);
        delete node;
    }

    nodo* InsertarIndex(nodo* node, int idx, char c) { // Insertar un nodo en el índice especificado
        if (!node) {
            nodo* newNode = new nodo(); // Crear un nuevo nodo
            newNode->index = idx;
            newNode->c = c;
            return newNode;
        }
        if (idx < node->index) {
            node->left = InsertarIndex(node->left, idx, c);
        } else {
            node->right = InsertarIndex(node->right, idx, c);
        }
        return node;
    }

    nodo* buelta_index(nodo* node, int idx) { // Obtener un nodo y sus subárboles según el índice
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

    nodo* reversoArbol(nodo* node) { // Revertir el árbol de forma recursiva
        if (!node) return nullptr;
        nodo* temp = node->left;
        node->left = reversoArbol(node->right);
        node->right = reversoArbol(temp);
        return node;
    }

    int CaluladorDeHaltura(nodo* node) { // Calcular la altura de forma recursiva
        if (!node) return 0;
        int left_height = CaluladorDeHaltura(node->left);
        int right_height = CaluladorDeHaltura(node->right);
        return 1 + max(left_height, right_height);
    }

    void contruccion_string(nodo* node, string& result) { // Construir el string a partir del árbol
        if (!node) return;
        contruccion_string(node->left, result);
        result += node->c;
        contruccion_string(node->right, result);
    }

    nodo* eliminar_segmento(nodo* node, int l, int r) { // Eliminar un segmento de nodos
        if (!node) return nullptr;
        if (node->index >= l && node->index <= r) {
            clearArbol(node);
            return nullptr;
        }
        if (node->index < l) {
            node->right = eliminar_segmento(node->right, l, r);
        } else {
            node->left = eliminar_segmento(node->left, l, r);
        }
        return node;
    }

    nodo* insertar_substring(nodo* node, int& idx, const string& s, int& insert_pos) { // Insertar un substring en el árbol
        if (idx < s.length()) {
            node = InsertarIndex(node, insert_pos, s[idx]);
            idx++;
            insert_pos++;
            node = insertar_substring(node, idx, s, insert_pos);
        }
        return node;
    }

public:
    super_string() {}

    void juntar(super_string& s) { // Unir dos super_strings
        super_string tmp;
        tmp.root = root;
        tmp.length = length;

        root = nullptr;
        length = 0;
        separar(tmp.length, *this, s);
    }

    void agregar(char c) { // Insertar un caracter en la última posición
        root = InsertarIndex(root, length, c);
        length++;
    }

    void separar(int i, super_string& a, super_string& b) { // Separar el super_string en dos a partir del índice i
        nodo* node = buelta_index(root, i);
        a.root = root;
        a.length = i;
        b.root = node;
        b.length = length - i;
        root = nullptr;
        length = 0;
    }

    void reverso() { // Revertir todo el super_string
        root = reversoArbol(root);
    }

    void reverso_segmento(int l, int r) { // Revertir un segmento del super_string
        super_string a, b, mid;
        separar(l, a, *this);
        separar(r - l + 1, mid, b);
        mid.reverso();
        juntar(a);
        juntar(mid);
        juntar(b);
    }

    int recortar() { // Retorna la altura del super_string después de recortar
        height = CaluladorDeHaltura(root);
        return height;
    }

    string stringizar() { // Construir el string a partir del super_string (debe ser O(n))
        string result;
        contruccion_string(root, result);
        return result;
    }

    void limpiar() { // Limpiar todo el super_string
        clearArbol(root);
        root = nullptr;
        length = 0;
    }

    void eliminar(int l, int r) { // Eliminar un segmento del super_string
        root = eliminar_segmento(root, l, r);
    }

    void insertar(int i, const string& s) { // Insertar un substring en una posición específica
        int idx = 0, insert_pos = i;
        root = insertar_substring(root, idx, s, insert_pos);
        length += s.length();
    }
};

int main() {
    super_string Arbol;
    ifstream archivo("prueba.txt");

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo" << endl;
        return 1; // error
    }

    string linea, instruccion, palabra_agregar, stg_posicion_int;

    while (getline(archivo, linea)) { // Leer el archivo línea por línea
        int tipo = linea.find(' ');
        if (tipo != -1) { // Verificar si la línea contiene un espacio
            instruccion = linea.substr(0, tipo);
            string sub_2 = linea.substr(tipo + 1); // Subcadena después del primer espacio
            int esp_sub2 = sub_2.find(' ');
            if (esp_sub2 != -1) { // Hay un segundo espacio
                stg_posicion_int = sub_2.substr(0, esp_sub2);
                palabra_agregar = sub_2.substr(esp_sub2 + 1);
            } else { // No hay segundo espacio
                stg_posicion_int = sub_2;
                palabra_agregar = "";
            }

            if (instruccion == "INSERTAR") { // Instrucción INSERTAR
                int pos = stoi(stg_posicion_int);
                Arbol.insertar(pos, palabra_agregar);
            } else if (instruccion == "ELIMINAR") { // Instrucción ELIMINAR
                int l = stoi(stg_posicion_int);
                int r = stoi(palabra_agregar);
                Arbol.eliminar(l, r);
            } else if (instruccion == "REVERSO") { // Instrucción REVERSO
                int l = stoi(stg_posicion_int);
                int r = stoi(palabra_agregar);
                Arbol.reverso_segmento(l, r);
                cout << "Reverso del segmento de " << l << " a " << r << endl;
            }
        } else {
            instruccion = linea;
            if (instruccion == "RECORTAR") { // Instrucción RECORTAR
                cout << "Altura después de recortar: " << Arbol.recortar() << endl;
            } else if (instruccion == "MOSTRAR") { // Instrucción MOSTRAR
                cout << Arbol.stringizar() << endl;
            } else if (instruccion == "FIN") { // Instrucción FIN
                break;
            } else {
                cerr << "Instrucción desconocida o malformada: " << linea << endl;
            }
        }
    }

    archivo.close(); // Cierra el archivo después de terminar de leerlo
    return 0;
}