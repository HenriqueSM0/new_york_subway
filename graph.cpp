# include <iostream>
# include <cstdio>
# include <vector>
# include <cstring>
# include <algorithm>

using namespace std;

class Line {

    public :

    string name;
    vector<string> stations;

    Line (string name) {
        this->name = name;
    }
};

class Graph {

    public :

    vector<string> stations;
    vector<Line> lines;
    vector<vector<string>> station_connect_with;

    int find_station (string station) {
        for (int i = 0; i < stations.size(); i++) if (stations[i] == station) return i;
        return -1;
    }

    bool pull_station (string station) {
        if (find_station(station) != -1) return false;
        stations.push_back(station);
        return true;
    }

    bool connect_stations (string station_1, string station_2) {
        int i_1 = find_station(station_1), i_2 = find_station(station_2), i;
        bool found = false;
        for (i = 0; i < station_connect_with[i_1].size(); i++) if (station_connect_with[i_1][i] == station_2) found = true;  
        if (!found) station_connect_with[i_1].push_back(station_2);
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
                    pull_station(station_name);
                    lines.at(lines.size() - 1).stations.push_back(station_name);
                    station_name.clear();
                    i++;
                }
                else station_name += buffer[i];
            }
            pull_station(station_name);
            lines.at(lines.size() - 1).stations.push_back(station_name);
            station_name.clear();
        }
        connect();
        fclose(arq);
        return true;
    }

    void print_connections (string station) {
        if (station_connect_with.empty()) return;
        for (auto stt : station_connect_with[find_station(station)]) cout << stt << '\n';
    }

    private :

    void connect () {
        station_connect_with.resize(stations.size());
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 1; j < lines[i].stations.size(); j++) {
                connect_stations(lines[i].stations[j], lines[i].stations[j - 1]);
                connect_stations(lines[i].stations[j - 1], lines[i].stations[j]);
            }
        }
    }
    
};

int main () {
    Graph * G = new Graph();
    G->construct("MTA_Lines.txt");
    return 0;
}