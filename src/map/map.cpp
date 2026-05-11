#include "map/map.h"

map::map(/* args */)
{
    int start = LAST_POINT;
    m_Nodes = new std::vector<node_t>{STANDARD_NODES_ARRAY};
    m_Edges = new std::vector<edge_t>{STANDARD_EDGES_ARRAY};
    deposeCoord[7].push_back(add_a_point(20,DEPOSE_X_7,DEPOSE_Y_7,start+1,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    m_Edges->push_back({32,(uint8_t)(start+1)});

    //Midle Depose
    deposeCoord[8].push_back(add_a_point(21,DEPOSE_X_8,DEPOSE_Y_8,start+2,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[8].push_back(add_a_point(23,DEPOSE_X_8,DEPOSE_Y_8,start+3,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[8].push_back(add_a_point(28,DEPOSE_X_8,DEPOSE_Y_8,start+4,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    m_Edges->push_back({4,(uint8_t)(start+4)});
    deposeCoord[0].push_back(add_a_point(21,DEPOSE_X_0,DEPOSE_Y_0,start+5,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[0].push_back(add_a_point(23,DEPOSE_X_0,DEPOSE_Y_0,start+6,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[0].push_back(add_a_point(22,DEPOSE_X_0,DEPOSE_Y_0,start+7,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[0].push_back(add_a_point(14,DEPOSE_X_0,DEPOSE_Y_0,start+8,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[2].push_back(add_a_point(14,DEPOSE_X_2,DEPOSE_Y_2,start+9,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[2].push_back(add_a_point(22,DEPOSE_X_2,DEPOSE_Y_2,start+10,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    deposeCoord[6].push_back(add_a_point(18,DEPOSE_X_6,DEPOSE_Y_6,start+11,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    m_Edges->push_back({5,(uint8_t)(start+11)});

    m_Edges->push_back({24,19}); //YELLOW shortcut

    //BLUE shortcut
    deposeCoord[3].push_back(add_a_point(15,DEPOSE_X_3,DEPOSE_Y_3,start+12,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    m_Edges->push_back({33,(uint8_t)(start+12)});

    deposeCoord[4].push_back(add_a_point(8,DEPOSE_X_4,DEPOSE_Y_4,start+13,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    m_Edges->push_back({27,(uint8_t)(start+13)});
    m_Edges->push_back({13,(uint8_t)(start+13)});

    m_Edges->push_back({17,26});
    m_Edges->push_back({13,27});


    deposeCoord[2].push_back(add_a_point(8,DEPOSE_X_2,DEPOSE_Y_2,start+14,OFFSET_DEPOSE, OFFSET_DEPOSE_BIS));
    m_Edges->push_back({34,(uint8_t)(start+14)});
    m_Edges->push_back({16,(uint8_t)(start+14)});

}

map::~map()
{
}

point_angle_t map::point_at_distance(
    float x1, float y1,
    float x2, float y2,
    float dist, float dist_bis)
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

    // angle de la direction (en radians)
    p.theta = static_cast<int>((-std::atan2(dy, dx) * RAD_TO_DEG)+180);

    if (p.theta < 0)
        p.theta += 360;

    p.theta %= 360;

    return p;
}
point_angle_t map::add_a_point(uint8_t firstPointId, float x1, float y1, uint8_t pointId1, float dist, float distBis){
    point_t newPoint;
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
        returnPoint = point_at_distance(x1,y1,firstPoint.x,firstPoint.y,dist, distBis);
        newPoint.x = returnPoint.x;
        newPoint.y = returnPoint.y;
        node_t newNode = {pointId1,newPoint};
        m_Nodes->push_back(newNode);
        edge_t edges = {firstPointId,pointId1};
        m_Edges->push_back(edges);
    }
    return returnPoint;
}

std::vector<node_t>* map::get_node_arry(){
    return m_Nodes;
}

std::vector<edge_t>* map::get_edge_arry(){
    return m_Edges;
}