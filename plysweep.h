#ifndef PLYSWEEP_H
#define PLYSWEEP_H

#include "object3d.h"

class _plysweep : public _object3D
{
public:
    _plysweep(string filename="./ply_sweep/peon.ply", unsigned int N=32, bool upper=true, bool lower=true);
    void _rotate(vector<_vertex3f>& Temp,vector<_vertex3ui> &tri, vector<char> Axis, unsigned int N=4, bool upper=false,bool lower=false);
};

#endif // PLYSWEEP_H
