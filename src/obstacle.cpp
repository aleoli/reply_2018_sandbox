#include "obstacle.hpp"
using reply::Obstacle;

Obstacle::Obstacle(Point a, Point b, Point c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

Obstacle::~Obstacle() {
    
}

void Obstacle::occ_points(Graph *g) {
    this->min_x = this->min_y = this->max_x = this->max_y = this->a.x;
    this->check_border(this->a);
    this->check_border(this->b);
    this->check_border(this->c);
    
    for(float a=this->min_y; a<=this->max_y; a+=1) {
        for(float b=this->min_x; b<=this->max_x; b+=1) {
            if(this->is_inside(a, b)) {
                Node *n = g->getNode(a, b);
                n->crossable(Node::NOT_CROSSABLE);
            }
        }
    }
}

bool Obstacle::is_inside(float x, float y) {
    float as_x = x-this->a.x;
    float as_y = y-this->a.y;
    bool s_ab = (this->b.x-this->a.x)*as_y-(this->b.y-this->a.y)*as_x > 0;
    if((this->c.x-this->a.x)*as_y-(this->c.y-this->a.y)*as_x > 0 == s_ab)
        return false;
    if((this->c.x-this->b.x)*(y-this->b.y)-(this->c.y-this->b.y)*(x-this->b.x) > 0 != s_ab)
        return false;
    return true;
}

void Obstacle::check_border(Point a) {
    if(a.x < this->min_x) {
        this->min_x = a.x;
    }
    if(a.x > this->max_x) {
        this->max_x = a.x;
    }
    if(a.y < this->min_y) {
        this->min_y = a.y;
    }
    if(a.y > this->max_y) {
        this->max_y = a.y;
    }
}
