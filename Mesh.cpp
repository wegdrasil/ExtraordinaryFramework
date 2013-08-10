#include "Mesh.h"
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
			std::cout << "position: " << position[0] << " " << position[1] << " "  << position[2] << std::endl;
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
			std::cout << "texcoord: " << texcoord[0] << " " << texcoord[1] <<  std::endl;
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
			std::cout << "normal: " << normal[0] << " " << normal[1] << " "  << normal[2] << std::endl;
		}
		
		if(buffer[0] == 'f')
		{
			float face[3];

			p_char = strtok(buffer, " f");
				
			for(int i = 0; i < 3; i++)
			{
				face[i] = atof(p_char);
				p_char = strtok(nullptr, " ");
			}
			std::cout << "face: " << face[0] << " " << face[1] << " "  << face[2] << std::endl;
		}
	}
	file.close();
}
//--------------------------------------------------------------------------------
//while(p_char != nullptr)
//{
//	std::cout << p_char << std::endl;
//	if(p_char == "v")
//		continue;
//
//	p_char = strtok(nullptr, " ");
//}