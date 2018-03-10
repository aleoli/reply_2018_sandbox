#ifndef obstacle_hpp
#define obstacle_hpp

#include "dependencies.hpp"

namespace reply {
    
    class Obstacle {
        
    public:
        Obstacle(Point a, Point b, Point c);
        ~Obstacle();
        
        void occ_points(Graph *g);
        string to_string() const;
        
    private:
        void check_border(Point a);
        bool is_inside(float x, float y);
        
        Point a, b, c;
        float min_x, min_y, max_x, max_y;
        
    };
    
}

#endif
