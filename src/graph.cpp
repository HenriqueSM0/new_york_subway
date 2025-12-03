# include <iostream>
# include <cstdio>
# include <vector>
# include <cstring>
# include <algorithm>
# include <queue>
# include <cstdlib>

# define QTD_ESTACOES 348
# define QTD_LINHAS 29
# define BRANCO 0
# define CINZA 1
# define PRETO 2

using namespace std;

class Station {
    public :
    string name;
    int cor;
    int dist_origem;
    int index_predecessor;

    Station (string name) {
        this->name = name;
    }
};

class Line {
    public :
    string name;
    vector<Station> stations;

    Line (string name) {
        this->name = name;
    }
};

class Graph {
    public :
    vector<Station> stations;
    vector<Line> lines;
    vector<vector<int>> station_connect_with;

    Graph() {
        stations.clear();
        lines.clear();
        station_connect_with.clear();
    }

    int find_station (string station) {
        for (int i = 0; i < stations.size(); i++) if (stations[i].name == station) return i;
        return -1;
    }

    bool connect_stations (int station_1_id, int station_2_id) {
        if (station_1_id < 0 || station_1_id >= stations.size() || station_2_id < 0 || station_2_id >= stations.size()) return false;
        bool found = false;
        for (int i = 0; i < station_connect_with[station_1_id].size(); i++) if (station_connect_with[station_1_id][i] == station_2_id) found = true;  
        if (!found) station_connect_with[station_1_id].push_back(station_2_id);
        return !found;
    }

    bool construct (const char * filename) {
        FILE * arq = fopen(filename, "r");
        if (arq == NULL) return false;
        char buffer[500];
        int tmp, i;
        string line_name = "", station_name = "";
        while (fgets(buffer, sizeof(buffer), arq)) {
            line_name.clear();
            for (i = 0; buffer[i] != ':'; i++) line_name += buffer[i];
            lines.push_back(*(new Line (line_name)));
            tmp = i + 1;
            for (i = tmp; buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0'; i++) {
                if (buffer[i] == ',' && buffer[i + 1] == ' ') {
                    push_station(station_name);
                    lines.at(lines.size() - 1).stations.push_back(Station(station_name));
                    station_name.clear();
                    i++;
                }
                else station_name += buffer[i];
            }
            push_station(station_name);
            lines.at(lines.size() - 1).stations.push_back(Station(station_name));
            station_name.clear();
        }
        connect();
        fclose(arq);
        return true;
    }

    void print_connections (string station_name) {
        if (station_connect_with.empty()) return;
        for (auto stt : station_connect_with[find_station(station_name)]) cout << stt << '\n';
    }

    private :

    void connect () {
        station_connect_with.resize(stations.size());
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 1; j < lines[i].stations.size(); j++) {
                connect_stations(find_station(lines[i].stations[j].name), find_station(lines[i].stations[j - 1].name));
                connect_stations(find_station(lines[i].stations[j - 1].name), find_station(lines[i].stations[j].name));
            }
        }
    }

    bool push_station (string station_name) {
        if (find_station(station_name) != -1) return false;
        stations.push_back(Station(station_name));
        return true;
    }
    
};

int index_of_station(Graph *G, string station_name) {
    for(int i=0;i<QTD_ESTACOES;i++)
    {
        if((*G).stations[i].name == station_name)
        {
            return i;
        }
    }
    return -1;
}

int * BFS(Graph *G, string start, string end) {
    int start_index = index_of_station(G, start);
    int end_index = index_of_station(G, end);
    if(start_index == -1 || end_index == -1) return NULL;
    for(int i=0;i<QTD_ESTACOES;i++) { if(i != start_index) (*G).stations[i].cor = BRANCO; (*G).stations[i].dist_origem = 0; }
    (*G).stations[start_index].cor = CINZA;
    queue<int> Q;
    Q.push(start_index);
    while(!Q.empty())
    {
        int frente = Q.front();
        Q.pop();
        for(int i=0;i<(*G).station_connect_with[frente].size();i++)
        {
            if((*G).stations[(*G).station_connect_with[frente][i]].cor == BRANCO)
            {
                (*G).stations[(*G).station_connect_with[frente][i]].cor = CINZA;
                (*G).stations[(*G).station_connect_with[frente][i]].dist_origem = (*G).stations[frente].dist_origem + 1;
                (*G).stations[(*G).station_connect_with[frente][i]].index_predecessor = frente;
                Q.push((*G).station_connect_with[frente][i]);
                if((*G).station_connect_with[frente][i] == end_index)
                {
                    int * path = new int[(*G).stations[end_index].dist_origem + 1];
                    int index = end_index;
                    int dist = (*G).stations[end_index].dist_origem;
                    while(1)
                    {
                        path[dist] = index;
                        dist--;
                        if(index == start_index) break;
                        index = (*G).stations[index].index_predecessor;
                    }
                    return path;
                }
            }
        }
        (*G).stations[frente].cor = PRETO;
    }
    return NULL;
}

void path(Graph * NewYorkStations, string start, string end) {
    int * path = BFS(NewYorkStations, start, end);
    if(path != NULL)
    {
        int distance = (*NewYorkStations).stations[index_of_station(NewYorkStations, end)].dist_origem;
        
        cout << "\033[1;35m========================================================================================================================\n";
        cout << "                                               *** MENOR CAMINHO ENCONTRADO ***\n";
        cout << "========================================================================================================================\033[m\n\n";
        
        cout << "\033[1;33mOrigem: \033[1;36m" << start << "\033[m\n";
        cout << "\033[1;33mDestino: \033[1;36m" << end << "\033[m\n";
        cout << "\033[1;33mDistancia: \033[1;32m" << distance << " estacao(s)\033[m\n\n";
        
        cout << "\033[1;35m--- ROTA COMPLETA ---\033[m\n\n";
        
        for(int i=0; i <= distance; i++)
        {
            cout << "\033[1;32m" << (i + 1) << ". \033[1;36m" << (*NewYorkStations).stations[path[i]].name << "\033[m";
            
            if (i < distance) {
                cout << "\n\033[1;35m     |\n     v\033[m\n";
            }
        }
        
        cout << "\n\n\033[1;33mVoce chegou ao seu destino! Boa viagem!\033[m\n";
    }
    else {
        cout << "\033[1;35m========================================================================================================================\n";
        cout << "                                                  *** ERRO DE ROTA ***\n";
        cout << "========================================================================================================================\033[m\n\n";
        
        cout << "\033[1;31mxXx CAMINHO NAO ENCONTRADO xXx\033[m\n\n";
        cout << "\033[1;33mVerifique se:\033[m\n";
        cout << "\033[1;36m* Os nomes das estacoes estao corretos\033[m\n";
        cout << "\033[1;36m* As estacoes existem na rede do metro\033[m\n";
        cout << "\033[1;36m* Ha conexao entre as estacoes informadas\033[m\n\n";
        cout << "\033[1;33mDica: Use a opcao [0] para ver todas as estacoes disponiveis.\033[m\n";
    }
}

void print_whole_graph_alphabetical(Graph * G) { 
    const int COL_WIDTH = 40; 
    const int NUM_COLS = 4;   

    vector<string> sorted_names;
    for (int i = 0; i < G->stations.size(); i++) {
        sorted_names.push_back(G->stations[i].name);
    }
    sort(sorted_names.begin(), sorted_names.end());

    cout << "\033[1;35m========================================================================================================================\n";
    cout << "                                               *** LISTA DE TODAS AS ESTACOES ***\n";
    cout << "========================================================================================================================\033[m\n\n";
    
    cout << "\033[1;33mTotal de estacoes: \033[1;32m" << sorted_names.size() << "\033[m\n\n";

    for (int i = 0; i < sorted_names.size(); i += NUM_COLS) {
        for (int j = 0; j < NUM_COLS; j++) {
            int current_index = i + j;
            if (current_index < sorted_names.size()) {
                string name = sorted_names[current_index];
                cout << "\033[1;32m" << (current_index + 1) << ". \033[1;36m" << name << "\033[m";
                
                string display_text = to_string(current_index + 1) + ". " + name;
                int len = display_text.length(); 
                if (j < NUM_COLS - 1) {
                    int padding = COL_WIDTH - len;
                    if (padding < 1) padding = 1;
                    for(int k=0; k<padding; k++) cout << " ";
                    cout << "\033[1;35m|\033[m "; 
                }
            }
        }
        cout << "\n";
    }
}

void print_all_connections(Graph * G) {
    const int COL_WIDTH = 50;
    const int NUM_COLS = 3;
    
    cout << "\033[1;35m========================================================================================================================\n";
    cout << "                                           *** TODAS AS CONEXOES ENTRE ESTACOES ***\n";
    cout << "========================================================================================================================\033[m\n\n";
    
    vector<pair<string, string>> unique_connections;
    
    for (int i = 0; i < G->stations.size(); i++) {
        string station_name = G->stations[i].name;
        
        for (int j = 0; j < G->station_connect_with[i].size(); j++) {
            int connected_station_index = G->station_connect_with[i][j];
            string connected_station_name = G->stations[connected_station_index].name;
            
            if (station_name < connected_station_name) {
                unique_connections.push_back(make_pair(station_name, connected_station_name));
            }
        }
    }
    
    cout << "\033[1;33mTotal de conexoes unicas: " << unique_connections.size() << "\033[m\n\n";
    
    for (int i = 0; i < unique_connections.size(); i += NUM_COLS) {
        for (int j = 0; j < NUM_COLS; j++) {
            int current_index = i + j;
            if (current_index < unique_connections.size()) {
                string connection_text = to_string(current_index + 1) + ". " + unique_connections[current_index].first + " <-> " + unique_connections[current_index].second;
                cout << "\033[1;32m" << to_string(current_index + 1) << ". \033[1;36m" << unique_connections[current_index].first << " \033[1;35m<->\033[1;36m " << unique_connections[current_index].second << "\033[m";
                
                int len = connection_text.length();
                if (j < NUM_COLS - 1) {
                    int padding = COL_WIDTH - len;
                    if (padding < 1) padding = 1;
                    for(int k = 0; k < padding; k++) cout << " ";
                    cout << "\033[1;35m|\033[m ";
                }
            }
        }
        cout << "\n";
    }
    
    cout << "\n\033[1;33mObservacao: Cada conexao e bidirecional (pode ser percorrida em ambas as direcoes).\033[m\n";
}

void print_station_connections(Graph * G, string station_name) {
    int station_index = G->find_station(station_name);
    
    if (station_index == -1) {
        cout << "\033[1;35m========================================================================================================================\n";
        cout << "                                                  *** ESTACAO NAO ENCONTRADA ***\n";
        cout << "========================================================================================================================\033[m\n\n";
        
        cout << "\033[1;31mxXx ESTACAO '" << station_name << "' NAO EXISTE xXx\033[m\n\n";
        cout << "\033[1;33mVerifique se:\033[m\n";
        cout << "\033[1;36m* O nome da estacao esta correto\033[m\n";
        cout << "\033[1;36m* A estacao existe na rede do metro\033[m\n\n";
        cout << "\033[1;33mDica: Use a opcao [0] para ver todas as estacoes disponiveis.\033[m\n";
        return;
    }
    
    cout << "\033[1;35m========================================================================================================================\n";
    cout << "                                           *** CONEXOES DA ESTACAO ***\n";
    cout << "========================================================================================================================\033[m\n\n";
    
    cout << "\033[1;33mEstacao selecionada: \033[1;36m" << station_name << "\033[m\n";
    cout << "\033[1;33mTotal de conexoes: \033[1;32m" << G->station_connect_with[station_index].size() << "\033[m\n\n";
    
    if (G->station_connect_with[station_index].empty()) {
        cout << "\033[1;31mEsta estacao nao possui conexoes diretas.\033[m\n";
        return;
    }
    
    cout << "\033[1;35m--- ESTACOES CONECTADAS ---\033[m\n\n";
    
    vector<string> connected_stations;
    for (int i = 0; i < G->station_connect_with[station_index].size(); i++) {
        int connected_index = G->station_connect_with[station_index][i];
        connected_stations.push_back(G->stations[connected_index].name);
    }
    
    sort(connected_stations.begin(), connected_stations.end());
    
    const int COL_WIDTH = 35;
    const int NUM_COLS = 3;
    
    for (int i = 0; i < connected_stations.size(); i += NUM_COLS) {
        for (int j = 0; j < NUM_COLS; j++) {
            int current_index = i + j;
            if (current_index < connected_stations.size()) {
                cout << "\033[1;32m" << (current_index + 1) << ". \033[1;36m" << connected_stations[current_index] << "\033[m";
                
                string display_text = to_string(current_index + 1) + ". " + connected_stations[current_index];
                int len = display_text.length();
                if (j < NUM_COLS - 1) {
                    int padding = COL_WIDTH - len;
                    if (padding < 1) padding = 1;
                    for(int k = 0; k < padding; k++) cout << " ";
                    cout << "\033[1;35m|\033[m ";
                }
            }
        }
        cout << "\n";
    }
    
    cout << "\n\033[1;33mObservacao: Voce pode viajar diretamente desta estacao para qualquer uma das listadas acima.\033[m\n";
}

int main () {
    cout << "\033[1;35m========================================================================================================================\n";
    cout << "                                              *** BEM-VINDO AO METRO DE NY! ***\n";
    cout << "========================================================================================================================\033[m\n\n";

    Graph * G = new Graph();
    bool loaded = G->construct("../dados/MTA_Lines.txt"); 

    if (!loaded) {
        cout << "\033[1;31mErro: Arquivo MTA_Lines.txt não encontrado!\033[m\n";
        return 1;
    }

    int opt = -1;
    string station1, station2;
    string buffer_flush; // Variável para limpar o buffer

    while (opt != 9) {
        station1.clear();
        station2.clear();

        cout << "\n\033[1;32mO que voce deseja realizar agora?\033[m\n";
        cout << "Escolha uma das opcoes abaixo:\n";
        cout << "   [ 0 ] Ver lista de estacoes (A-Z)\n";
        cout << "   [ 1 ] Ver menor caminho entre estacoes\n";
        cout << "   [ 2 ] Ver todas as conexoes entre estacoes\n";
        cout << "   [ 3 ] Ver conexoes de uma estacao especifica\n";
        cout << "   [ 8 ] Limpar terminal\n";
        cout << "   [ 9 ] Encerrar programa\n";
        cout << "Opcao: ";

        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        
        getline(cin, buffer_flush); 

        switch (opt) {
            case 0:
                print_whole_graph_alphabetical(G);
                break;
            
            case 1:
                while (station1.empty()) {
                    cout << "Escolha da estacao de partida (digite o nome exato dela): ";
                    getline(cin, station1);
                }

                while (station2.empty()) {
                    cout << "Perfeito! Agora escolha a estacao de chegada: ";
                    getline(cin, station2);
                }

                path(G, station1, station2);
                break;

            case 2:
                print_all_connections(G);
                break;

            case 3:
                while (station1.empty()) {
                    cout << "Digite o nome exato da estacao para ver suas conexoes: ";
                    getline(cin, station1);
                }
                print_station_connections(G, station1);
                break;

            case 8:
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                break;

            case 9:
                cout << "Encerrando...\n";
                break;

            default:
                cout << "\033[1;31mxXx ESTA OPÇÃO NÃO É VÁLIDA xXx\033[m\n";
                break;
        }

        if (opt != 8 && opt != 9) {
            cout << "\033[1;35m========================================================================================================================\033[m\n";

        }
    }
    
    return 0;
}