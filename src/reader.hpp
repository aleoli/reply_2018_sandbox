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
        
        void get_nodes(Graph *g) const;
        void get_links(Graph *g) const;
        vector<Obstacle *> get_obstacles() const;
        Point get_start() const;
        Point get_end() const;
        
    private:
        void compute_limits();
        void check_border(Point a);
        bool check_limits(Point a);
        
        Point start;
        Point end;
        vector<Obstacle *> obstacles;
        long min_x, min_y, max_x, max_y;
        long l_min_x, l_min_y, l_max_x, l_max_y;
        
    };
    
}

#endif
