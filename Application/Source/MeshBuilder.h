#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateCircle(const std::string &meshName, int radius, Color color);
	static Mesh* GenerateRing(const std::string &meshName, int innner_radius, int outer_radius, Color color);
	static Mesh* GenerateSphere(const std::string &meshName, int inner_radius, Color color);
	static Mesh* GenerateHemiSphere(const std::string &meshName, float radius, Color color);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateCylinder(const std::string &meshName, Color color, int height, float topradius, float bottomradius, bool bottomcircle);
	static Mesh* GenerateCone(const std::string &meshName, Color color, int height, float radius);
};

#endif