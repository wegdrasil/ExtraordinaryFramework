#include "Mesh.h"
//--------------------------------------------------------------------------------
void Mesh::GenQuad(float size)
{
	m_MeshData.m_vVertices.resize(4);
	m_MeshData.m_vVertices[0] = Vertex(-size, size, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	m_MeshData.m_vVertices[1] = Vertex(size, size, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	m_MeshData.m_vVertices[2] = Vertex(size, -size, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	m_MeshData.m_vVertices[3] = Vertex(-size, -size, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

	m_MeshData.m_vIndices.resize(6);
	m_MeshData.m_vIndices[0] = 0;
	m_MeshData.m_vIndices[1] = 1;
	m_MeshData.m_vIndices[2] = 2;
	m_MeshData.m_vIndices[3] = 0;
	m_MeshData.m_vIndices[4] = 2;
	m_MeshData.m_vIndices[5] = 3;
}
//--------------------------------------------------------------------------------
void Mesh::CreateMeshFromOBJFile(std::string filename)
{
	const int MAX_CHARS_PER_LINE = 512;
	const int MAX_TOKENS_PER_LINE = 20;

	std::ifstream file;
	file.open(filename);

	char* p_char;

	if(!file.good())
	{
		std::cout << "File not found" << std::endl;
		return;
	}

	while(!file.eof())
	{
		char buffer[MAX_CHARS_PER_LINE];
		file.getline(buffer, MAX_CHARS_PER_LINE);

		if(buffer[0] == '#')
			continue;
		
		//Get vertex positions
		if(buffer[0] == 'v' && buffer[1] == ' ')
		{
			float position[3];

			p_char = strtok(buffer, " v");
				
			for(int i = 0; i < 3; i++)
			{
				position[i] = atof(p_char);
				p_char = strtok(nullptr, " ");
			}
			//std::cout << "position: " << position[0] << " " << position[1] << " "  << position[2] << std::endl;
			m_MeshData.m_vVertices.push_back(Vertex(position[0], position[1], position[2], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		}

		//Get texture coordinates
		if(buffer[0] == 'v' && buffer[1] == 't')
		{			
			float texcoord[2];

			p_char = strtok(buffer, " vt");
				
			for(int i = 0; i < 2; i++)
			{
				texcoord[i] = atof(p_char);
				p_char = strtok(nullptr, " ");
			}
			//std::cout << "texcoord: " << texcoord[0] << " " << texcoord[1] <<  std::endl;
		}

		//Get vertex normals
		if(buffer[0] == 'v' && buffer[1] == 'n')
		{			
			float normal[3];

			p_char = strtok(buffer, " vn");
				
			for(int i = 0; i < 3; i++)
			{
				normal[i] = atof(p_char);
				p_char = strtok(nullptr, " ");
			}
			//std::cout << "normal: " << normal[0] << " " << normal[1] << " "  << normal[2] << std::endl;
		}
		
		if(buffer[0] == 'f')
		{
			float face[3];

			p_char = strtok(buffer, " f");
				
			for(int i = 0; i < 3; i++)
			{
				face[i] = atof(p_char) -1;     //tricky bug: obj counts from 1, but programmers count from 0, so we need subtract 1
				p_char = strtok(nullptr, " ");
			}
			//std::cout << "face: " << face[0] << " " << face[1] << " "  << face[2] << std::endl;

			m_MeshData.m_vIndices.push_back(face[0]);
			m_MeshData.m_vIndices.push_back(face[1]);
			m_MeshData.m_vIndices.push_back(face[2]);
		}
	}
	file.close();
}
//--------------------------------------------------------------------------------
void Mesh::GenerateNormals()
{
	unsigned int numTris = m_MeshData.m_vIndices.size() / 3;
	for(unsigned int i = 0; i < numTris; ++i)
	{
		unsigned int i0 = m_MeshData.m_vIndices[(i*3)+0];
		unsigned int i1 = m_MeshData.m_vIndices[(i*3)+1];
		unsigned int i2 = m_MeshData.m_vIndices[(i*3)+2];

		DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&m_MeshData.m_vVertices[i0].mPosition);
		DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&m_MeshData.m_vVertices[i1].mPosition);
		DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&m_MeshData.m_vVertices[i2].mPosition);

		DirectX::XMVECTOR u = DirectX::XMVectorSubtract(p1, p0);
		DirectX::XMVECTOR v = DirectX::XMVectorSubtract(p2, p0);
		DirectX::XMVECTOR faceNormal = DirectX::XMVector3Cross(u, v);
		
		DirectX::XMVECTOR n0 = DirectX::XMLoadFloat3(&m_MeshData.m_vVertices[i0].mNormal);
		DirectX::XMVECTOR n1 = DirectX::XMLoadFloat3(&m_MeshData.m_vVertices[i1].mNormal);
		DirectX::XMVECTOR n2 = DirectX::XMLoadFloat3(&m_MeshData.m_vVertices[i2].mNormal);

		n0 = DirectX::XMVectorAdd(n0, faceNormal);
		n1 = DirectX::XMVectorAdd(n1, faceNormal);
		n2 = DirectX::XMVectorAdd(n2, faceNormal);

		DirectX::XMStoreFloat3(&m_MeshData.m_vVertices[i0].mNormal, n0);
		DirectX::XMStoreFloat3(&m_MeshData.m_vVertices[i1].mNormal, n1);
		DirectX::XMStoreFloat3(&m_MeshData.m_vVertices[i2].mNormal, n2);
	}

	for(unsigned int i = 0; i < m_MeshData.m_vVertices.size(); ++i)
	{
		DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&m_MeshData.m_vVertices[i].mNormal);
		DirectX::XMVector3Normalize(normal);
		DirectX::XMStoreFloat3(&m_MeshData.m_vVertices[i].mNormal, normal);
	}
}
