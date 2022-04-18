/*! \file
 * Copyright Brian Sena Simons
 * email: dmartin@ugr.es
 */

#include "plymodel.h"
#include "file_ply_stl.h"

_plymodel::_plymodel(string filename){
    _file_ply file;
    //string path = "./ply_models/";
    file.open(filename);
    vector<GLfloat> coords;
    vector<GLuint> pos;
    file.read(coords,pos);
	Vertices.clear();
    for(unsigned int i=0;i<coords.size();i+=3){
		Vertices.push_back(_vertex3f(coords[i],coords[i+1],coords[i+2]));
    }
    Triangles.clear();
    for(unsigned int i=0;i<pos.size();i+=3){
		Triangles.push_back(_vertex3ui(pos[i],pos[i+1],pos[i+2]));
    }

}
