//
// Created by Victor ROUX-SIBILLON on 09/04/2026.
//

#include "path_finding.h"

#include <cmath>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

float distance(const point_t *a, const point_t *b) {
    return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

PathFindingMap::PathFindingMap() {
    for (const edge_t &edge: *this->edges) {
        adjacency[edge.start_node_id].emplace_back(edge.end_node_id, &edge);
        adjacency[edge.end_node_id].emplace_back(edge.start_node_id, &edge);
    }

    for (const node_t &node: *this->nodes) {
        node_map[node.id] = &node;
    }
}

node_t *PathFindingMap::find_closest_node_to_point(const pose_t target_pose) {
    node_t *closest_node = nullptr;
    float best_distance = INFINITY;
    for (node_t &node: *this->nodes) {
        if (const float d = distance(&target_pose.point, &node.point); d < best_distance) {
            closest_node = &node;
            best_distance = d;
        }
    }
    return closest_node;
}

path_t PathFindingMap::find_path_between_points(const pose_t start_point, const pose_t end_point) {
    path_t result_path;

    const node_t *start_close_node = find_closest_node_to_point(start_point);
    const node_t *end_close_node = find_closest_node_to_point(end_point);

    if (!start_close_node || !end_close_node) {
        return result_path;
    }

    auto heuristic = [&](uint8_t node_id) {
        const node_t *node = node_map[node_id];
        return distance(&node->point, &end_close_node->point);
    };

    std::priority_queue<a_star_node, std::vector<a_star_node>, std::greater<>> open_set;
    std::unordered_set<uint8_t> closed_set;
    std::unordered_map<uint8_t, uint8_t> came_from;
    std::unordered_map<uint8_t, const edge_t *> came_from_edge;
    std::unordered_map<uint8_t, float> g_score;

    const uint8_t start_id = start_close_node->id;
    const uint8_t end_id = end_close_node->id;

    g_score[start_id] = 0.0;
    open_set.push({start_id, heuristic(start_id)});

    while (!open_set.empty()) {
        auto [current_node_id, f_score] = open_set.top();

        open_set.pop();

        if (current_node_id == end_id) {
            uint8_t current_node = end_id;
            result_path.v.push_back(end_point);

            while (came_from.find(current_node) != came_from.end()) {
                const float target_theta = atan2f(node_map[current_node]->point.y - node_map[came_from[current_node]]->point.y,
                    node_map[current_node]->point.x - node_map[came_from[current_node]]->point.x);

                result_path.v.push_back({node_map[current_node]->point, current_node, target_theta});
                current_node = came_from[current_node];
            }

            result_path.v.push_back(start_point);
            std::reverse(result_path.v.begin(), result_path.v.end());

            return result_path;
        }

        if (closed_set.count(current_node_id)) {
            continue;
        }

        closed_set.insert(current_node_id);

        if (!adjacency.count(current_node_id)) {
            continue;
        }

        for (const auto &[neighbor_id, edge_ptr]: adjacency[current_node_id]) {
            if (closed_set.count(neighbor_id)) {
                continue;
            }

            if (const float g_score_guess = g_score[current_node_id] + edge_ptr->distance;
                !g_score.count(neighbor_id) || g_score_guess < g_score[neighbor_id]) {

                came_from[neighbor_id] = current_node_id;
                came_from_edge[neighbor_id] = edge_ptr;
                g_score[neighbor_id] = g_score_guess;

                open_set.push({neighbor_id, g_score_guess + heuristic(neighbor_id)});
            }
        }
    }

    return result_path;
}
