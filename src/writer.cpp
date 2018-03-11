#include "writer.hpp"
using reply::Writer;

#include <fstream>
using std::ofstream;

Writer::Writer(string file, vector<Node *> path) {
    ofstream f(file, ofstream::out);
    if(path.size() > 0) {
        f << path.size() << endl;
        for(auto it=path.begin(); it!=path.end(); ++it) {
            Point p = (*it)->getPosition();
            f << p.x << " " << p.y << endl;
        }
    } else {
        f << "IMPOSSIBLE" << endl;
    }
    f.close();
}

Writer::~Writer() {
    
}
