# include <iostream>
# include <cstdio>
# include <vector>
# include <cstring>
# include <algorithm>
# include <queue>

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
            for (i = tmp; buffer[i] != '\n'; i++) {
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

int BFS(Graph *G, string start, string end) {
    int start_index = index_of_station(G, start);
    int end_index = index_of_station(G, end);
    if(start_index == -1 || end_index == -1) return -1;
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
                Q.push((*G).station_connect_with[frente][i]);
                if((*G).station_connect_with[frente][i] == end_index)
                {
                    return (*G).stations[(*G).station_connect_with[frente][i]].dist_origem;
                }
            }
        }
        (*G).stations[frente].cor = PRETO;
    }
    return -1;
}

int main () {
    Graph * G = new Graph();
    G->construct("../dados/MTA_Lines.txt");
    // int dist = BFS(G, "Times Sq-42 St", "34 St-Penn Station");
    // int dist = BFS(G, "Times Sq-42 St", "14 St");
    int dist = BFS(G, "Times Sq-42 St", "Fulton St");
    cout << "Distancia entre Times Sq - 42 St e 14 St: " << dist << '\n';
    return 0;
}