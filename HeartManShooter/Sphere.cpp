#include "Sphere.h"

Sphere::Sphere()
{
	center = glm::vec3(0, 0, 0);
}


void Sphere::Move(float x, float y, float z)
{
	center.x += x;
	center.y += y;
	center.z += z;
}

void Sphere::GenerateSphere(float sectorCount, float stackCount,float theradius)
{
	radius = theradius;

	// clear memory of prev arrays
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * 3.14f / sectorCount;
	float stackStep = 3.14f / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = 3.14f / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi
			if (j == sectorCount)
			{
				sectorAngle = 0;
			}
			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(-x);
			vertices.push_back(z);
			vertices.push_back(y);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	std::vector<unsigned int> indices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	vector<float> buffer;
	int j = 0;
	for (int i = 0; i < vertices.size(); i+=3)
	{
		buffer.insert(buffer.end(), {vertices[i],vertices[i+1],vertices[i+2],normals[i],normals[i+1],normals[i+2],texCoords[j],texCoords[j+1]});
		j += 2;
	}

	LoadMeshIndex(&buffer[0], buffer.size(),&indices[0],indices.size());
}
