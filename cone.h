#ifndef CONE_H
#define CONE_H

#include "object3d.h"
#include "operate.h"

class _cone : public _object3D
{
public:
    _cone(float Size=1.0,unsigned int N=128,bool upper=true,bool lower=true);
    vector<_vertex3ui> _triangles(vector<_vertex3f> P1,unsigned int N=4);
};

#endif // CONE_H
