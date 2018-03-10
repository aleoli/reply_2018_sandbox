#ifndef reader_hpp
#define reader_hpp

#include "dependencies.hpp"

#include "obstacle.hpp"
using reply::Obstacle;

namespace reply {
    
    class Reader {
        
    public:
        Reader(string file);
        ~Reader();
        
        vector<Node *> get_nodes() const;
        vector<Link> get_links() const;
        vector<Obstacle *> get_obstacles() const;
        Point get_start() const;
        Point get_end() const;
        
    private:
        void check_border(Point a);
        
        Point start;
        Point end;
        vector<Obstacle *> obstacles;
        long min_x, min_y, max_x, max_y;
        
    };
    
}

#endif
