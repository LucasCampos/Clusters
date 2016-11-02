/*
 * Life_Clusters.hpp
 * Copyright (C) 2016 lucas <lucas@localhost.localdomain>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef LIFE_CLUSTERS_HPP
#define LIFE_CLUSTERS_HPP

#include <vector>
#include <list>
#include <algorithm>

#include <iostream>

#include "Cluster_Types.hpp"

namespace {
    struct ClusterStamp {
        bool active;
        int initialTime;
        /* int finalTime; */
        Cluster c;

        ClusterStamp(): active(true), initialTime(0){}
        ClusterStamp(const Cluster& _c, int initTime): 
            active(true), initialTime(initTime)
        {
            c = _c;
        }
    };

    bool isSame(const Cluster& newCluster, const Cluster& oldCluster, float continuityCondition) {
        unsigned notIn = oldCluster.size()*(1.0f-continuityCondition);
        /* std::cout << notIn << std::endl; */
        for(unsigned elem: oldCluster){
            if (!isIn(elem, newCluster)) {
                notIn--;
                if(notIn <= 0)
                    return false;
            }
        }
        return true;
    }

    void UpdateClusterStamp(ClusterStamp& cs, const Cluster& c) {
        cs.active = true;
        //Check if the cluster grew. If so, this is the new
        //standard of cluster.
        if(cs.c.size() <= c.size())
            cs.c = c;
    }

    void ClearDead(std::vector<ClusterStamp>& inUse, std::vector<int>& lifeTime, unsigned step){

        //Now clears the dead clusters
        for(auto& u: inUse)
            if(!u.active){
                lifeTime.push_back(step - u.initialTime);
            }

        inUse.erase(
                std::remove_if(inUse.begin(), inUse.end(), 
                    [&](ClusterStamp& a)
                    { return !a.active;}),
                inUse.end()
                );
    }

}

std::vector<int> ClusterLifes(const std::vector<std::vector<Cluster>>& clusters, float continuityCondition) {

    std::vector<ClusterStamp> inUse;
    std::vector<int> lifeTime;

    /* for(auto& c: clusters[0]){ */
    /*     inUse.emplace(inUse.end(), c, 0); */
    /* } */

    for(unsigned step = 0; step < clusters.size(); step++) {
        /* if (step%100==0) */
            /* std::cout << step << std::endl; */
        for(auto& u: inUse)
            u.active = false;

        for(auto& c: clusters[step]) {
            //Search if the cluster is in the actives
            bool exists = false;
            for (auto& u:inUse) {
                //If it belongs, the cluster is still active and 
                //possibly grew
                if(isSame(c, u.c, continuityCondition)) {
                    exists = true;
                    UpdateClusterStamp(u, c);
                    //As the cluster has been found, no more search is needed
                    break;
                }
            }

            //If it isn't in the actives, it is a new cluster
            if(!exists)
                inUse.emplace(inUse.end(), c, step);
        }

        ClearDead(inUse, lifeTime, step);
    }
    return lifeTime;
}

#endif /* !LIFE_CLUSTERS_HPP */
