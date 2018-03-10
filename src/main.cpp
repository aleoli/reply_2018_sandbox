#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;

using std::cout;
using std::endl;
using std::thread;

#include "reply.hpp"
using namespace reply;

void get_nodes(vector<Node *> *nodes, Reader *r);
void get_links(vector<Link> *links, Reader *r);

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
    
    cout << "Creating graph" << endl;
    Graph g(nodes, links);
    
    cout << "Computing obstacles" << endl;
    for(auto it=obstacles.begin(); it!=obstacles.end(); ++it) {
        (*it)->occ_points(&g);
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
