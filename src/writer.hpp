#ifndef writer_hpp
#define writer_hpp

#include "dependencies.hpp"

namespace reply {
    
    class Writer {
        
    public:
        Writer(string file, vector<Node *> path);
        ~Writer();
        
    };
    
}

#endif
