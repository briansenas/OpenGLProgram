#include "plysweep.h"
#include "file_ply_stl.h"
#include "operate.h"

_plysweep::_plysweep(string filename,unsigned int N,bool upper, bool lower)
{
    _file_ply file;
    file.open(filename);
    vector<GLfloat> coords;
    vector<GLuint> pos;
    file.read(coords,pos);
	Vertices.clear();
	Triangles.clear();
	_vertex3f toAppend;
    for(unsigned int i=0;i<coords.size();i+=3){
        toAppend._0 = coords[i];
        toAppend._1 = coords[i+1];
        toAppend._2 = coords[i+2];
		Vertices.push_back(toAppend);
    }
    vector<char> Axis;
    Axis.push_back('Y');
    this->_rotate(Vertices,Triangles,Axis,N,upper,lower);
	this->steps = N;
}

void _plysweep::_rotate(vector<_vertex3f>& vert,vector<_vertex3ui>& tri,vector<char> Axis,unsigned int N,bool upper, bool lower){
    for(unsigned int i=0;i<Axis.size();i++){
        switch(Axis[i]){
           case 'X':
                _operate::_rotateX(vert,tri,N,360,upper,lower);
            break;
            case 'Y':
               _operate::_rotateY(vert,tri,N,360,upper,lower);
            break;
            case 'Z':
               _operate::_rotateZ(vert,tri,N,360,upper,lower);
               break;
        }
    }
}
