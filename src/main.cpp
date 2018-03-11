#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

#define _N_CORES_ 8

using namespace std::chrono;

using std::cout;
using std::endl;
using std::thread;

#include "reply.hpp"
using namespace reply;

void get_links(Graph *g, Reader *r);
void load_obstacles(Graph *g, vector<Obstacle *> obstacles, int n, unsigned long *err);

int main(int argc, char *argv[]) {
    if(argc != 3) {
        cout << "Invalid command number" << endl;
        exit(1);
    }
    
    auto started = high_resolution_clock::now();
    
    cout << "Reading file" << endl;
    Reader r((string(argv[1])));
    
    Graph g;
    
    cout << "Getting nodes" << endl;
    r.get_nodes(&g);
    
    cout << "Getting links" << endl;
    thread l(get_links, &g, &r);
    
    cout << "Computing obstacles" << endl;
    unsigned long err=0;
    unsigned long errors[_N_CORES_];
    thread th[_N_CORES_];
    vector<Obstacle *> obstacles = r.get_obstacles();
    for(int a=0; a<_N_CORES_; a++) {
        errors[a] = 0;
        th[a] = thread(load_obstacles, &g, obstacles, a, &(errors[a]));
    }
    
    Point s = r.get_start();
    Node *start = g.getNode(s);
    Point e = r.get_end();
    Node *end = g.getNode(e);
    
    for(int a=0; a<_N_CORES_; a++) {
        th[a].join();
        err += errors[a];
    }
    cout << "Errors: " << err << "/" << obstacles.size() << " - " << (float)err/(float)obstacles.size()*100 << "%" << endl;
    
    l.join();
    
    cout << "Path from " << start->to_string() << " to " << end->to_string() << endl;
    Path p(&g, start, end);
    vector<Node *> path = p.get();
    cout << path.size() << " steps" << endl;
    
    cout << "Writing output file" << endl;
    Writer w((string(argv[2])), path);
    
    auto done = high_resolution_clock::now();
    cout << "Time: " << duration_cast<milliseconds>(done-started).count() << " ms" << endl;
    return 0;
}

void get_links(Graph *g, Reader *r) {
    r->get_links(g);
}

void load_obstacles(Graph *g, vector<Obstacle *> obstacles, int n, unsigned long *err) {
    unsigned long count = 0;
    for(auto it=obstacles.begin(); it!=obstacles.end(); ++it) {
        count++;
        if(count%_N_CORES_ != n) {
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
