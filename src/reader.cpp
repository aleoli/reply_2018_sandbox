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
    this->check_border(this->start);
    this->check_border(this->end);
    int n_obst = 5;
    //f >> n_obst;
    //cout << "Creating " << n_obst << " obstacles" << endl;
    while(n_obst > 0) {
        Point a;
        Point b;
        Point c;
        if(!(f >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y)) {
            break;
        }
        if(!this->check_limits(a) || !this->check_limits(b) || !this->check_limits(c)) {
            continue;
        }
        this->check_border(a);
        this->check_border(b);
        this->check_border(c);
        this->obstacles.push_back(new Obstacle(a, b, c));
        n_obst--;
    }
    
    f.close();
    
    cout << "x: " << this->min_x << " <-> " << this->max_x << endl;
    cout << "y: " << this->min_y << " <-> " << this->max_y << endl;
}

Reader::~Reader() {
    for(auto it=this->obstacles.begin(); it!=this->obstacles.end(); ++it) {
        delete *it;
    }
}

vector<Node *> Reader::get_nodes() const {
    //cout << "x: " << this->min_x << " <-> " << this->max_x << endl;
    //cout << "y: " << this->min_y << " <-> " << this->max_y << endl;
    //cout << "Creating " << (this->max_x-this->min_x)*(this->max_y-this->min_y) << " nodes" << endl;
    vector<Node *> res;
    unsigned long id=0;
    for(long a=this->min_y; a<=this->max_y; a++) {
        for(long b=this->min_x; b<=this->max_x; b++) {
            Point p;
            p.y = a;
            p.x = b;
            res.push_back(new Node(id, 1, p));
            id++;
        }
    }
    return res;
}

vector<Link> Reader::get_links() const {
    //cout << "x: " << this->min_x << " <-> " << this->max_x << endl;
    //cout << "y: " << this->min_y << " <-> " << this->max_y << endl;
    //cout << "Creating " << (this->max_x-this->min_x)*(this->max_y-this->min_y)*2 << " links" << endl;
    vector<Link> res;
    unsigned long id=0;
    long rows = this->max_y - this->min_y;
    for(long a=this->min_y; a<=this->max_y; a++) {
        for(long b=this->min_x; b<=this->max_x; b++) {
            if(a>0) {
                Link l;
                l.from = id;
                l.to = id-rows;
                l.weight = 1;
                if(l.from >= 0 && l.to >= 0)
                    res.push_back(l);
            }
            if(b>0) {
                Link l;
                l.from = id;
                l.to = id-1;
                l.weight = 1;
                if(l.from >= 0 && l.to >= 0)
                    res.push_back(l);
            }
            id++;
        }
    }
    return res;
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

bool Reader::check_limits(Point a) {
    return a.x >= -500 && a.x <= 2000 && a.y >= -500 && a.y <= 2000;
}
