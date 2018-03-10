#include "obstacle.hpp"
using reply::Obstacle;

Obstacle::Obstacle(Point a, Point b, Point c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

Obstacle::~Obstacle() {
    
}

string Obstacle::to_string() const {
    return std::to_string(this->a.x)+" "+std::to_string(this->a.y)+" "+std::to_string(this->b.x)+" "+std::to_string(this->b.y)+" "+std::to_string(this->c.x)+" "+std::to_string(this->c.y);
}

void Obstacle::occ_points(Graph *g) {
    this->min_x = this->min_y = this->max_x = this->max_y = this->a.x;
    this->check_border(this->a);
    this->check_border(this->b);
    this->check_border(this->c);
    
    bool error = false;
    string err = "";
    
    for(float a=this->min_y; a<=this->max_y; a+=1) {
        for(float b=this->min_x; b<=this->max_x; b+=1) {
            if(this->is_inside(a, b)) {
                try {
                    Node *n = g->getNode(a, b);
                    n->crossable(Node::NOT_CROSSABLE);
                } catch(A_star_exception &e) {
                    error = true;
                    err = e.what();
                }
            }
        }
    }
    if(error) {
        throw A_star_exception(err);
    }
}

bool Obstacle::is_inside(float x, float y) {
    float as_x = x-this->a.x;
    float as_y = y-this->a.y;
    bool s_ab = (this->b.x-this->a.x)*as_y-(this->b.y-this->a.y)*as_x > 0;
    if(((this->c.x-this->a.x)*as_y-(this->c.y-this->a.y)*as_x > 0) == s_ab)
        return false;
    if(((this->c.x-this->b.x)*(y-this->b.y)-(this->c.y-this->b.y)*(x-this->b.x) > 0) != s_ab)
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
