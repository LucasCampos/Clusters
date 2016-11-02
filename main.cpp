/*
 * LifeStrings.cpp
 * Copyright (C) 2016 lucas <lucas@localhost.localdomain>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>

#include "Cluster_Types.hpp"
#include "Separation_Clusters.hpp"
#include "Life_Clusters.hpp"

using namespace std;

inline bool FileExist (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

unsigned CountLines(string filename){
    std::ifstream inFile(filename); 
    return std::count(std::istreambuf_iterator<char>(inFile), 
                      std::istreambuf_iterator<char>(), '\n');
}

vector<vector<float2>> ReadFile(string filename, unsigned N) {
    unsigned nLines = CountLines(filename);
    unsigned nSnapshots = nLines/N;

    ifstream in(filename);

    vector<vector<float2>> out(nSnapshots,
            vector<float2>(N));

    for(unsigned i=0; i<nSnapshots; i++) {
        for(unsigned j=0; j<N; j++) {
            in >> out[i][j].x >> out[i][j].y;
        }
    }

    return out;

}

int main(int argc, char** argv) {

    if(argc != 6) {
        cout << "Usage " << argv[0] << " filename N conditionProximity boxSize conditionContinuity" << endl;
        return 1;
    }

    string filename(argv[1]);
    unsigned N = atoi(argv[2]);
    float conditionP = atof(argv[3]);
    float box = atof(argv[4]);
    float conditionC = atof(argv[5]);

    if(!FileExist(filename)) {
        cout << argv[0] << ": file " << filename << ": Cannot open this file." << endl;
        return 1;
    }

    float conditionP2 = conditionP*conditionP;
    vector<vector<Cluster>> clusters;

    {
        auto data = ReadFile(filename, N);
        for(auto& d: data)
            clusters.push_back(IdentifyClusters(d, conditionP2, box));
    }

    /* cout << "Clusters at the beginning: " << clusters[0].size() << endl; */

    vector<int> cLifes = ClusterLifes(clusters, conditionC);
    /* for(auto i: cLifes) */
    /*     cout << i << endl; */

    float avgL = std::accumulate(cLifes.begin(), cLifes.end(), 0);
    avgL = avgL/cLifes.size();
    cout << avgL << endl;
}
