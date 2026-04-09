//
// Created by Victor ROUX-SIBILLON on 09/04/2026.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "map/simplified_map.h"

typedef struct {
    float x;
    float y;
} point_t;

typedef struct {
    point_t point;
    uint8_t node_id;
    float theta;
} pose_t;

typedef struct {
    std::vector<pose_t> v;
} path_t;

typedef struct {
    uint8_t id;
    point_t point;
} node_t;

typedef struct {
    uint8_t start_node_id;
    uint8_t end_node_id;
    float distance;
} edge_t;

struct a_star_node {
    uint8_t id;
    float f;

    bool operator >(const a_star_node &other_node) const {
        return f > other_node.f;
    }
};

class PathFindingMap {
    std::vector<node_t> *nodes = new std::vector<node_t>{STANDARD_NODES_ARRAY};
    std::vector<edge_t> *edges = new std::vector<edge_t>{STANDARD_EDGES_ARRAY};

    std::unordered_map<uint8_t,
        std::vector<std::pair<uint8_t,
            const edge_t *> > > adjacency;
    std::unordered_map<uint8_t,
        const node_t *> node_map;

public:
    PathFindingMap();

    node_t *find_closest_node_to_point(pose_t target_point);

    path_t find_path_between_points(pose_t start_point, pose_t end_point);
};
