#ifndef OPERATE_H
#define OPERATE_H

#include "vertex.h"
#include <vector>

class _operate
{
private:
    void static closure(vector<_vertex3f>& temp, vector<_vertex3ui>& T1, vector<_vertex3f> axis,unsigned int N,
                                      unsigned int t_sz,bool slope,bool upper=true, bool lower=true);
public:
    vector<_vertex3ui> static _triangles(vector<_vertex3f> fixed, unsigned int N=4);

	_vertex3f static normalize(_vertex3f v);
    _vertex3f static cross_product(_vertex3f a, _vertex3f b);
    void static _rotateX(vector<_vertex3f>& P1, vector<_vertex3ui>& T1, unsigned int N=4,
            unsigned int angle=360, bool upper=true, bool lower=true);
    void static _rotateY(vector<_vertex3f>& P1, vector<_vertex3ui>& T1, unsigned int N=4,
            unsigned int angle=360, bool upper=true, bool lower=true);
    void static _rotateZ(vector<_vertex3f>& P1, vector<_vertex3ui>& T1, unsigned int N=4,
            unsigned int angle=360, bool upper=true, bool lower=true);

};

#endif // OPERATE_H
