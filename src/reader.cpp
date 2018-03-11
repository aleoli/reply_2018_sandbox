#include "reply.hpp"
using reply::Reader;

#include <fstream>
using std::ifstream;

Reader::Reader(string file) {
    this->min_x = 0;
    this->min_y = 0;
    this->max_x = 0;
    this->max_y = 0;
    
    ifstream f(file);
    
    f >> this->start.x >> this->start.y >> this->end.x >> this->end.y;
    //this->check_border(this->start);
    //this->check_border(this->end);
    this->compute_limits();
    int n_obst;
    f >> n_obst;
    //cout << "Creating " << n_obst << " obstacles" << endl;
    while(n_obst > 0) {
        Point a;
        Point b;
        Point c;
        if(!(f >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y)) {
            break;
        }
        if(!this->check_limits(a) && !this->check_limits(b) && !this->check_limits(c)) {
            continue;
        }
        //this->check_border(a);
        //this->check_border(b);
        //this->check_border(c);
        this->obstacles.push_back(new Obstacle(a, b, c));
        n_obst--;
    }
    
    f.close();
    
    cout << "x: " << this->l_min_x << " <-> " << this->l_max_x << endl;
    cout << "y: " << this->l_min_y << " <-> " << this->l_max_y << endl;
}

Reader::~Reader() {
    for(auto it=this->obstacles.begin(); it!=this->obstacles.end(); ++it) {
        delete *it;
    }
}

void Reader::get_nodes(Graph *g) const {
    //cout << "x: " << this->min_x << " <-> " << this->max_x << endl;
    //cout << "y: " << this->min_y << " <-> " << this->max_y << endl;
    //cout << "Creating " << (this->max_x-this->min_x)*(this->max_y-this->min_y) << " nodes" << endl;
    unsigned long id=0;
    for(long a=this->l_min_y; a<=this->l_max_y; a++) {
        for(long b=this->l_min_x; b<=this->l_max_x; b++) {
            Point p;
            p.y = a;
            p.x = b;
            g->addNode(new Node(id, 1, p));
            id++;
        }
    }
}

void Reader::get_links(Graph *g) const {
    //cout << "x: " << this->min_x << " <-> " << this->max_x << endl;
    //cout << "y: " << this->min_y << " <-> " << this->max_y << endl;
    //cout << "Creating " << (this->max_x-this->min_x)*(this->max_y-this->min_y)*2 << " links" << endl;
    unsigned long id=0;
    long rows = this->l_max_x - this->l_min_x + 1;
    for(long a=this->l_min_y; a<=this->l_max_y; a++) {
        for(long b=this->l_min_x; b<=this->l_max_x; b++) {
            if(a>this->l_min_y) {
                Link l;
                l.from = id;
                l.to = id-rows;
                l.weight = 1;
                g->addLink(l);
            }
            if(b>this->l_min_x) {
                Link l;
                l.from = id;
                l.to = id-1;
                l.weight = 1;
                g->addLink(l);
            }
            id++;
        }
    }
}

vector<Obstacle *> Reader::get_obstacles() const {
    return this->obstacles;
}

Point Reader::get_start() const {
    return this->start;
}

Point Reader::get_end() const {
    return this->end;
}

void Reader::check_border(Point a) {
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

void Reader::compute_limits() {
    this->l_max_x = (this->start.x > this->end.x ? this->start.x : this->end.x) + 500;
    this->l_max_y = (this->start.y > this->end.y ? this->start.y : this->end.y) + 500;
    this->l_min_x = (this->start.x < this->end.x ? this->start.x : this->end.x) - 500;
    this->l_min_y = (this->start.y < this->end.y ? this->start.y : this->end.y) - 500;
}

bool Reader::check_limits(Point a) {
    return a.x >= this->l_min_x && a.x <= this->l_max_x && a.y >= this->l_min_y && a.y <= this->l_max_y;
}
