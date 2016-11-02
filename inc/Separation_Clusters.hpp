/*
 * Separation_Clusters.hpp
 * Copyright (C) 2016 lucas <lucas@localhost.localdomain>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SEPARATION_CLUSTERS_HPP
#define SEPARATION_CLUSTERS_HPP

#include <vector>
#include <algorithm>

#include "Cluster_Types.hpp"

namespace {

    float CorrectBox(float d, float box){
        if(d > box/2)
            return d-box;
        if(d < -box/2)
            return d+box;
        return d;
    }

    std::vector<Connection> FindConnections(const std::vector<float2>& p, float condition2, float box) {
        std::vector<Connection> pairs;
        for(unsigned i=0; i<p.size()-1; i++){
            for(unsigned j=i+1; j<p.size(); j++) {
                float dx = CorrectBox(p[i].x - p[j].x, box);
                float dy = CorrectBox(p[i].y - p[j].y, box);

                float r2 = dx*dx + dy*dy;
                if (r2 <= condition2){
                    pairs.emplace(pairs.end(), i, j);
                }
            }
        }
        return pairs;
    }

    void ClearUsed(std::vector<Connection>& p, const Cluster& s) {

        //Remove any trace of the set in the connections
        for(auto& i: s) {
            p.erase(
                    std::remove_if(p.begin(), p.end(), 
                        [&](Connection a)
                        { return a.first == i || a.second == i; }),
                    p.end()
                   );
        }
    }

}

std::vector<Cluster> IdentifyClusters(const std::vector<float2>& pos, float condition2, float box) {

    std::vector<Cluster> clusters;

    auto pairs = FindConnections(pos, condition2, box);

    while(pairs.size() > 0){
        Cluster s;
        
        //Bootstrap the new cluster
        s.insert(pairs[0].first);
        s.insert(pairs[0].second);

        //If one of the particles in the cluster is in the connection
        //both particles of the connection are in the cluster
        
        //TODO: Check if it is faster to use two separate ifs
        for(auto& p: pairs) {
            if( isIn(p.first, s) || isIn(p.second, s)) {
                s.insert(p.first);
                s.insert(p.second);
            }
        }

        //Remove the used pairs. I avoided doing it on-the-fly
        //due to iterator invalidation
        ClearUsed(pairs, s);

        clusters.push_back(s);
    }

    return clusters;

}
#endif /* !SEPARATION_CLUSTERS_HPP */
