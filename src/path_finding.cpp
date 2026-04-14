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

/**
 *
 * @param l1sx Line 1 start x
 * @param l1sy Line 1 start y
 * @param l1ex Line 1 end x
 * @param l1ey Line 1 end y
 * @param l2sx Line 2 start x
 * @param l2sy Line 2 start y
 * @param l2ex Line 2 end x
 * @param l2ey Line 2 end y
 * @return True if the two lines intersect
 */
bool two_line_collision(float l1sx, float l1sy, float l1ex, float l1ey, float l2sx, float l2sy, float l2ex,
                        float l2ey) {
    // doc : https://www.jeffreythompson.org/collision-detection/line-rect.php
    // line dir
    float l1_dir = ((l2ex - l2sx) * (l1sy - l2sy) - (l2ey - l2sy) * (l1sx - l2sx)) / (
                       (l2ey - l2sy) * (l1ex - l1sx) - (l2ex - l2sx) * (l1ey - l1sy));
    float l2_dir = ((l1ex - l1sx) * (l1sy - l2sy) - (l1ey - l1sy) * (l1sx - l2sx)) / (
                       (l2ey - l2sy) * (l1ex - l1sx) - (l2ex - l2sx) * (l1ey - l1sy));

    // if uA and uB in [0; 1] -> collision
    return (l1_dir >= 0 && l1_dir <= 1) && (l2_dir >= 0 && l2_dir <= 1);
}

/**
 * @param rx Rectangle base x
 * @param ry Rectangle base y
 * @param rw Rectangle width
 * @param rh Rectangle height
 * @return List of vector intersecting with one of the rectangle edges
 */
std::vector<edge_t> PathFindingMap::found_edges_intersecting_rectangle(float rx, float ry, float rw, float rh) {
    std::vector<edge_t> result;
    for (const edge_t &edge: *this->edges) {
        const node_t *n1 = this->node_map[edge.start_node_id];
        const node_t *n2 = this->node_map[edge.end_node_id];

        if (two_line_collision(rx, ry, rx + rw, ry, n1->point.x, n1->point.y, n2->point.x, n2->point.y)) {
            result.push_back(edge);
            continue;
        }
        if (two_line_collision(rx, ry, rx, ry + rh, n1->point.x, n1->point.y, n2->point.x, n2->point.y)) {
            result.push_back(edge);
            continue;
        }
        if (two_line_collision(rx, ry + rh, rx + rw, ry + rh, n1->point.x, n1->point.y, n2->point.x, n2->point.y)) {
            result.push_back(edge);
            continue;
        }
        if (two_line_collision(rx + rw, ry, rx + rw, ry + rh, n1->point.x, n1->point.y, n2->point.x, n2->point.y)) {
            result.push_back(edge);
            continue;
        }

        bool n1_inside = rx <= n1->point.x && n1->point.x <= rx + rw && n1->point.y >= ry && n1->point.y <= ry + rh;
        bool n2_inside = rx <= n2->point.x && n2->point.x <= rx + rw && n2->point.y >= ry && n2->point.y <= ry + rh;

        if (n1_inside && n2_inside) {
            result.push_back(edge);
        }
    }
    return result;
}

float distance(const point_t *a, const point_t *b) {
    return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

PathFindingMap::PathFindingMap() {
    this->enable_all_edges();
    this->update_base_map();
}

void PathFindingMap::update_base_map() {
    adjacency.clear();
    for (const edge_t &edge: *this->edges) {
        if (edge.enabled) {
            adjacency[edge.start_node_id].emplace_back(edge.end_node_id, &edge);
            adjacency[edge.end_node_id].emplace_back(edge.start_node_id, &edge);
        }
    }

    node_map.clear();
    for (const node_t &node: *this->nodes) {
        node_map[node.id] = &node;
    }
}

void PathFindingMap::enable_all_edges() {
    for (edge_t &edge: *this->edges) {
        edge.enabled = true;
    }
}

bool PathFindingMap::toggle_edge_between_two_nodes(const uint8_t node1_id, const uint8_t node2_id, const bool enabled) {
    for (edge_t &edge: *this->edges) {
        if ((edge.start_node_id == node1_id && edge.end_node_id == node2_id) ||
            (edge.start_node_id == node2_id && edge.end_node_id == node1_id)) {
            edge.enabled = enabled;
            return true;
        }
    }
    return false;
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

path_t PathFindingMap::find_path_between_points(const pose_t start_point, const pose_t end_point,
                                                const bool remove_coax_nodes) {
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
                const float target_theta = atan2f(
                    node_map[current_node]->point.y - node_map[came_from[current_node]]->point.y,
                    node_map[current_node]->point.x - node_map[came_from[current_node]]->point.x);

                result_path.v.push_back({node_map[current_node]->point, current_node, target_theta});
                current_node = came_from[current_node];
            }

            if (remove_coax_nodes) {
                for (size_t i = 1; i < result_path.v.size();) {
                    if (result_path.v[i].theta == result_path.v[i - 1].theta) {
                        result_path.v.erase(result_path.v.begin() + i);
                    } else {
                        ++i;
                    }
                }
            }

            result_path.length = 0.0f;
            for (size_t i = 1; i < result_path.v.size(); ++i) {
                result_path.length += distance(&result_path.v[i - 1].point, &result_path.v[i].point);
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
