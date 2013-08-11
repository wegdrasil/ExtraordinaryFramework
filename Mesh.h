#pragma once
#include <Windows.h>
#include <DirectXMath.h>
//--------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <cstring>
//--------------------------------------------------------------------------------
#include <vector>
//--------------------------------------------------------------------------------
#include "Vertex.h"
//--------------------------------------------------------------------------------
struct MeshData
{
	std::vector<Vertex> m_vVertices;
	std::vector<unsigned int> m_vIndices;
};
//--------------------------------------------------------------------------------
class Mesh
{
public:
	Mesh() {};
	~Mesh() {};

	MeshData m_MeshData;

	void GenQuad();
	void CreateMeshFromOBJFile(std::string filename);
	void GenerateNormals();
};
//--------------------------------------------------------------------------------
