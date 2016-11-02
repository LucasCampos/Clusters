/*
 * Cluster_Types.hpp
 * Copyright (C) 2016 lucas <lucas@localhost.localdomain>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CLUSTER_TYPES_HPP
#define CLUSTER_TYPES_HPP

#include <set>

typedef std::pair<unsigned, unsigned> Connection;
typedef std::set<unsigned> Cluster;

struct float2{
    float x,y;
    float2(): x(0), y(0){};
};

bool isIn(unsigned elem, const Cluster& a) {
    return a.find(elem) != a.end();
}

bool isIn(const Cluster& a, const Cluster& b) {
    for(unsigned elem: b){
        if (!isIn(elem, a))
            return false;
    }
    return true;
}

#endif /* !CLUSTER_TYPES_HPP */
