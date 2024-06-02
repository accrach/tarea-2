#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

class super_string {
private:
    struct nodo {
        nodo* left = nullptr;
        nodo* right = nullptr;
        int index;
        char c;
        nodo(int index, char c) {}
        nodo() {}
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

    nodo* InsertarIndex(nodo* node, int idx, char c) {
        if (!node) {
            nodo* newNode = new nodo();
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

    nodo* eliminar_segmento(nodo* node, int l, int r) {
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

    nodo* insertar_substring(nodo* node, int& idx, const string& s, int& insert_pos) {
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
    
    void juntar(super_string& s) {
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

    void separar(int i, super_string& a, super_string& b) {
        nodo* node = buelta_index(root, i);
        a.root = root;
        a.length = i;
        b.root = node;
        b.length = length - i;
        root = nullptr;
        length = 0;
    }

    void reverso() { // No debe cambiar la altura del árbol
        root = reversoArbol(root);
    }

    void reverso_segmento(int l, int r) {
        super_string a, b, mid;
        separar(l, a, *this);
        separar(r - l + 1, mid, b);
        mid.reverso();
        juntar(a);
        juntar(mid);
        juntar(b);
    }

    int recortar() { // Retorna this->height después de recortar
        height = CaluladorDeHaltura(root);
        return height;
    }

    string stringizar() { // Debe ser O(n)
        string result;
        contruccion_string(root, result);
        return result;
    }

    void limpiar() {
        clearArbol(root);
        root = nullptr;
        length = 0;
    }

    void eliminar(int l, int r) {
        root = eliminar_segmento(root, l, r);
    }

    void insertar(int i, const string& s) {
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

    while (getline(archivo, linea)) { // lee el archivo
        cout << "Linea leída: " << linea << endl;
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

            if (instruccion == "INSERTAR") {
                int pos = stoi(stg_posicion_int);
                Arbol.insertar(pos, palabra_agregar);
                cout << "Insertado: " << palabra_agregar << " en posición " << pos << endl;
            } else if (instruccion == "ELIMINAR") {
                int l = stoi(stg_posicion_int);
                int r = stoi(palabra_agregar);
                Arbol.eliminar(l, r);
                cout << "Eliminado segmento de " << l << " a " << r << endl;
            } else if (instruccion == "REVERSO") {
                int l = stoi(stg_posicion_int);
                int r = stoi(palabra_agregar);
                Arbol.reverso_segmento(l, r);
                cout << "Reverso del segmento de " << l << " a " << r << endl;
            }
        } else {
            instruccion = linea;
            if (instruccion == "RECORTAR") {
                cout << "Altura después de recortar: " << Arbol.recortar() << endl;
            } else if (instruccion == "MOSTRAR") {
                cout << "Super string: " << Arbol.stringizar() << endl;
            } else if (instruccion == "FIN") {
                break;
            } else {
                cerr << "Instrucción desconocida o malformada: " << linea << endl;
            }
        }
    }

    archivo.close(); // cierra el archivo después de terminar de leerlo
    return 0;
}