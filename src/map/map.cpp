#include "map/map.h"

map::map(/* args */)
{
    int start = LAST_POINT;
    m_Nodes = new std::vector<node_t>{STANDARD_NODES_ARRAY};
    m_Edges = new std::vector<edge_t>{STANDARD_EDGES_ARRAY};
    deposeCoord[7].push_back(add_a_point(20,DEPOSE_X_7,DEPOSE_Y_7,start+1,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+2));
    m_Edges->push_back({12,(uint8_t)(start+2)});

    //Midle Depose
    deposeCoord[8].push_back(add_a_point(21,DEPOSE_X_8,DEPOSE_Y_8,start+3,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+4));
    deposeCoord[8].push_back(add_a_point(23,DEPOSE_X_8,DEPOSE_Y_8,start+5,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+6));
    deposeCoord[8].push_back(add_a_point(28,DEPOSE_X_8,DEPOSE_Y_8,start+7,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+8));
    m_Edges->push_back({4,(uint8_t)(start+8)});
    deposeCoord[0].push_back(add_a_point(21,DEPOSE_X_0,DEPOSE_Y_0,start+9,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+10));
    deposeCoord[0].push_back(add_a_point(23,DEPOSE_X_0,DEPOSE_Y_0,start+11,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+12));
    deposeCoord[0].push_back(add_a_point(22,DEPOSE_X_0,DEPOSE_Y_0,start+13,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+14));
    deposeCoord[0].push_back(add_a_point(14,DEPOSE_X_0,DEPOSE_Y_0,start+15,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+16));
    deposeCoord[2].push_back(add_a_point(14,DEPOSE_X_2,DEPOSE_Y_2,start+17,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+18));
    deposeCoord[2].push_back(add_a_point(22,DEPOSE_X_2,DEPOSE_Y_2,start+19,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+20));
    deposeCoord[6].push_back(add_a_point(18,DEPOSE_X_6,DEPOSE_Y_6,start+21,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+22));
    m_Edges->push_back({5,(uint8_t)(start+22)});

    m_Edges->push_back({24,19}); //YELLOW shortcut

    //BLUE shortcut
    deposeCoord[3].push_back(add_a_point(15,DEPOSE_X_3,DEPOSE_Y_3,start+23,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+24));
    m_Edges->push_back({33,(uint8_t)(start+24)});

    deposeCoord[4].push_back(add_a_point(8,DEPOSE_X_4,DEPOSE_Y_4,start+25,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+26));
    m_Edges->push_back({27,(uint8_t)(start+25)});
    m_Edges->push_back({13,(uint8_t)(start+26)});

    m_Edges->push_back({17,26});
    m_Edges->push_back({13,27});


    deposeCoord[2].push_back(add_a_point(8,DEPOSE_X_2,DEPOSE_Y_2,start+27,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS, OFFSET_DEPOSE_TRIS, start+28));
    m_Edges->push_back({6,(uint8_t)(start+28)});
    m_Edges->push_back({16,(uint8_t)(start+27)});

}

map::~map()
{
}

point_angle_t map::point_at_distance(
    float x1, float y1,
    float x2, float y2,
    float dist, float dist_bis, float dist_tris)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float length = std::sqrt(dx * dx + dy * dy);

    // éviter division par zéro
    assert(length != 0.0 && "Points identiques: direction indéfinie");

    point_angle_t p;
    p.x = x1 + (dx / length) * dist;
    p.y = y1 + (dy / length) * dist;

    p.x_bis = x1 + (dx / length) * dist_bis;
    p.y_bis = y1 + (dy / length) * dist_bis;

    p.x_tris = x1 + (dx / length) * dist_tris;
    p.y_tris = y1 + (dy / length) * dist_tris;

    // angle de la direction (en radians)
    p.theta = static_cast<int>((-std::atan2(dy, dx) * RAD_TO_DEG)+180);

    if (p.theta < 0)
        p.theta += 360;

    p.theta %= 360;

    return p;
}
point_angle_t map::add_a_point(uint8_t firstPointId, float x1, float y1, uint8_t pointId1, float dist, float distBis, float dist_tris, uint8_t pointIdtris){
    point_t newPoint;
    point_t newPointTris;
    point_angle_t returnPoint;
    if(dist == 0){
        newPoint.x = x1;
        newPoint.y = y1;
        node_t newNode = {pointId1,newPoint};
        m_Nodes->push_back(newNode);
        edge_t edges = {firstPointId,pointId1};
        m_Edges->push_back(edges);
    }
    else{
        point_t firstPoint;
        for (const node_t &node: *m_Nodes) {
            if(node.id == firstPointId){
                firstPoint = node.point;
                break;
            }
        }
        returnPoint = point_at_distance(x1,y1,firstPoint.x,firstPoint.y,dist, distBis, dist_tris);
        newPoint.x = returnPoint.x;
        newPoint.y = returnPoint.y;
        newPointTris.x = returnPoint.x_tris;
        newPointTris.y = returnPoint.y_tris;
        node_t newNode = {pointId1,newPoint};
        node_t newNodeTris = {pointIdtris,newPointTris};
        m_Nodes->push_back(newNodeTris);
        m_Nodes->push_back(newNode);
        edge_t edges = {firstPointId,pointId1};
        m_Edges->push_back(edges);
        edge_t edgesTris = {firstPointId,pointIdtris};
        m_Edges->push_back(edgesTris);
    }
    return returnPoint;
}

std::vector<node_t>* map::get_node_arry(){
    return m_Nodes;
}

std::vector<edge_t>* map::get_edge_arry(){
    return m_Edges;
}