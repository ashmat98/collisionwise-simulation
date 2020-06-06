
#include <box.h>

#include "default.h"

struct Box{
    eg::Vector3d sides;
    
    Box(){};
    Box(double a, double b, double c){sides << a,b,c;}
};

eg::Vector3d Box::min_difference() {
    return Eigen::Vector3d();
}
