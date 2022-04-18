/*! \file
 * Copyright Brian Sena Simons
 * email: dmartin@ugr.es
 */


#include "cube.h"


_cube::_cube(float Size)
{
    Vertices.clear();
    float A,B,C;
    A = B = C = Size/2.0;
    Vertices.push_back(_vertex3f(A,B,C));
    Vertices.push_back(_vertex3f(A,B,-C));
    Vertices.push_back(_vertex3f(A,-B,C));
    Vertices.push_back(_vertex3f(A,-B,-C));
    Vertices.push_back(_vertex3f(-A,B,C));
    Vertices.push_back(_vertex3f(-A,B,-C));
    Vertices.push_back(_vertex3f(-A,-B,C));
    Vertices.push_back(_vertex3f(-A,-B,-C));

    Triangles.clear();
    Triangles.push_back(_vertex3ui(0,2,1));//Left B
    Triangles.push_back(_vertex3ui(1,2,3));
    Triangles.push_back(_vertex3ui(6,2,4)); //Back B
    Triangles.push_back(_vertex3ui(2,0,4));
    Triangles.push_back(_vertex3ui(4,5,7));// Right
    Triangles.push_back(_vertex3ui(7,6,4));
    Triangles.push_back(_vertex3ui(3,7,1)); //Front
    Triangles.push_back(_vertex3ui(7,5,1));
    Triangles.push_back(_vertex3ui(0,1,4)); //Above
    Triangles.push_back(_vertex3ui(1,5,4));
    Triangles.push_back(_vertex3ui(2,7,3)); //Under
    Triangles.push_back(_vertex3ui(2,6,7));

}

/******************************************************************************/
