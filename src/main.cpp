#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std::chrono;

using std::cout;
using std::endl;
using std::thread;

#include "reply.hpp"
using namespace reply;

void get_nodes(vector<Node *> *nodes, Reader *r);
void get_links(vector<Link> *links, Reader *r);
void load_obstacles(Graph *g, vector<Obstacle *> obstacles, int n, unsigned long *err);

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cout << "Invalid command number" << endl;
        exit(1);
    }
    
    auto started = high_resolution_clock::now();
    
    cout << "Reading file" << endl;
    Reader r((string(argv[1])));
    vector<Node *> nodes;
    vector<Link> links;
    cout << "Getting nodes" << endl;
    thread n(get_nodes, &nodes, &r);
    cout << "Getting links" << endl;
    thread l(get_links, &links, &r);
    vector<Obstacle *> obstacles = r.get_obstacles();
    
    n.join();
    l.join();
    
    cout << "Creating graph with " << nodes.size() << " nodes and " << links.size() << " links" << endl;
    Graph g(nodes, links);
    
    cout << "Computing obstacles" << endl;
    unsigned long err=0;
    unsigned long errors[4];
    thread th[4];
    
    for(int a=0; a<4; a++) {
        errors[a] = 0;
        th[a] = thread(load_obstacles, &g, obstacles, a, &(errors[a]));
    }
    
    Point s = r.get_start();
    Node *start = g.getNode(s.x, s.y);
    Point e = r.get_end();
    Node *end = g.getNode(e.x, e.y);
    
    for(int a=0; a<4; a++) {
        th[a].join();
        err += errors[a];
    }
    cout << "Errors: " << err << "/" << obstacles.size() << " - " << (float)err/(float)obstacles.size()*100 << "%" << endl;
    
    cout << "Path from " << start->to_string() << " to " << end->to_string() << endl;
    Path p(&g, start, end);
    vector<Node *> path = p.get();
    for(auto it=path.begin(); it!=path.end(); ++it) {
        cout << (*it)->to_string() << endl;
    }
    
    auto done = high_resolution_clock::now();
    cout << "Time: " << duration_cast<milliseconds>(done-started).count() << " ms" << endl;
    return 0;
}

void get_nodes(vector<Node *> *nodes, Reader *r) {
    *nodes = r->get_nodes();
}

void get_links(vector<Link> *links, Reader *r) {
    *links = r->get_links();
}

void load_obstacles(Graph *g, vector<Obstacle *> obstacles, int n, unsigned long *err) {
    unsigned long count = 0;
    for(auto it=obstacles.begin(); it!=obstacles.end(); ++it) {
        count++;
        if(count%4 != n) {
            continue;
        }
        try {
            (*it)->occ_points(g);
        } catch(A_star_exception &e) {
            //cout << e.what() << " in obstacle " << (*it)->to_string() << endl;
            (*err)++;
        }
    }
}
