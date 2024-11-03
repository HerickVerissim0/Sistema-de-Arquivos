#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

class Node {
public:
    string nome;
    int tamanho;
    bool ehDiretorio;
    vector<Node*> filhos;

    Node(string n, int t, bool dir = false) {
        nome = n;
        tamanho = t;
        ehDiretorio = dir;
    }
};

class SistemaArquivos {
private:
    Node* raiz;

    Node* buscarArquivo(Node* atual, string nome) {
        if (atual->nome == nome) return atual;
        
        for (Node* filho : atual->filhos) {
            Node* encontrado = buscarArquivo(filho, nome);
            if (encontrado) return encontrado;
        }
        return nullptr;
    }

public:
    SistemaArquivos() {
        raiz = new Node("/", 0, true);
    }

    void criarArquivo(string nome, int tamanho) {
        // Cria o arquivo no sistema virtual
        Node* novoArquivo = new Node(nome, tamanho);
        raiz->filhos.push_back(novoArquivo);
        
        // Cria o arquivo físico
        ofstream arquivo(nome.c_str());
        if (arquivo.is_open()) {
            // Preenche o arquivo com o tamanho especificado
            for (int i = 0; i < tamanho; i++) {
                arquivo.put('0');
            }
            arquivo.close();
            cout << "Arquivo criado: " << nome << endl;
            cout << "Localizacao: pasta atual\\" << nome << endl;
        } else {
            cout << "Erro ao criar arquivo fisico!" << endl;
        }
    }

    void excluirArquivo(string nome) {
        auto it = find_if(raiz->filhos.begin(), raiz->filhos.end(),
            [nome](Node* n) { return n->nome == nome; });
        
        if (it != raiz->filhos.end()) {
            // Remove o arquivo físico
            if (remove(nome.c_str()) == 0) {
                delete *it;
                raiz->filhos.erase(it);
                cout << "Arquivo excluido: " << nome << endl;
            } else {
                cout << "Erro ao excluir arquivo fisico!" << endl;
            }
        } else {
            cout << "Arquivo nao encontrado!" << endl;
        }
    }

    void alterarTamanho(string nome, int novoTamanho) {
        Node* arquivo = buscarArquivo(raiz, nome);
        if (arquivo) {
            // Atualiza o arquivo físico
            ofstream arq(nome.c_str(), ios::trunc);
            if (arq.is_open()) {
                for (int i = 0; i < novoTamanho; i++) {
                    arq.put('0');
                }
                arq.close();
                arquivo->tamanho = novoTamanho;
                cout << "Tamanho alterado para: " << novoTamanho << endl;
            } else {
                cout << "Erro ao modificar arquivo fisico!" << endl;
            }
        } else {
            cout << "Arquivo nao encontrado!" << endl;
        }
    }

    void buscar(string nome) {
        Node* arquivo = buscarArquivo(raiz, nome);
        if (arquivo) {
            cout << "Arquivo encontrado!" << endl;
            cout << "Nome: " << arquivo->nome << endl;
            cout << "Tamanho: " << arquivo->tamanho << endl;
            
            // Verifica se o arquivo físico existe
            ifstream arq(nome.c_str());
            if (arq.good()) {
                cout << "Arquivo fisico existe na pasta atual" << endl;
            } else {
                cout << "Arquivo fisico nao encontrado!" << endl;
            }
            arq.close();
        } else {
            cout << "Arquivo nao encontrado!" << endl;
        }
    }
};

int main() {
    SistemaArquivos fs;
    int opcao;
    
    while (true) {
        cout << "\nSistema de Arquivos" << endl;
        cout << "1. Criar arquivo" << endl;
        cout << "2. Excluir arquivo" << endl;
        cout << "3. Alterar tamanho" << endl;
        cout << "4. Buscar arquivo" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        string nome;
        int tamanho;

        switch (opcao) {
            case 1:
                cout << "Nome do arquivo: ";
                cin >> nome;
                cout << "Tamanho inicial do arquivo em bytes: ";
                cin >> tamanho;
                fs.criarArquivo(nome, tamanho);
                break;
            
            case 2:
                cout << "Nome do arquivo: ";
                cin >> nome;
                fs.excluirArquivo(nome);
                break;
            
            case 3:
                cout << "Nome do arquivo: ";
                cin >> nome;
                cout << "Novo tamanho: ";
                cin >> tamanho;
                fs.alterarTamanho(nome, tamanho);
                break;
            
            case 4:
                cout << "Nome do arquivo: ";
                cin >> nome;
                fs.buscar(nome);
                break;
            
            case 0:
                return 0;
            
            default:
                cout << "Opcao invalida!" << endl;
        }
    }
    
    return 0;
}