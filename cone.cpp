#include "cone.h"

_cone::_cone(float Size,unsigned int N,bool upper_,bool lower_)
{
	this->upper = upper_;
	this->lower = lower_;
    Vertices.clear();
    Vertices.push_back(_vertex3f(0,-Size/2.0,0));
    Vertices.push_back(_vertex3f(Size,-Size/2.0,0));
    Vertices.push_back(_vertex3f(0,Size/2.0,0));

    _operate::_rotateY(Vertices,Triangles,N,360,upper,lower);
	this->steps = N;
    //_operate::_rrotateY(Vertices,Triangles,N);
}

