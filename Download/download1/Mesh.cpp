#include "Mesh.h"

Mesh::Mesh()
{
	usingindexbuffer = false;
	physicscenter = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	meshdata = nullptr;
	indexdata = nullptr;

	meshdata = new GLfloat[10];
}

Mesh::Mesh(const Mesh& obj)
{
	//cout << "mesh copy!" << endl;
	physicscenter = obj.physicscenter;
	velocity = obj.velocity;
	if (obj.usingindexbuffer)
	{
		LoadMeshIndex(obj.meshdata, obj.meshdatasize, obj.indexdata, obj.indexdatasize);
	}
	else
	{
		LoadMesh(obj.meshdata, obj.meshsize);
	}
}

void Mesh::LoadMeshIndex(GLfloat * data, int datasize, unsigned int* indices, int theindexsize)
{
	usingindexbuffer = true;
	meshdata = new GLfloat[datasize];
	for (int i = 0; i < datasize; i++)
	{
		meshdata[i] = data[i];
	}
	meshdatasize = datasize;

	indexdata = new unsigned int[theindexsize];
	for (int i = 0; i < theindexsize; i++)
	{
		indexdata[i] = indices[i];
	}
	indexdatasize = theindexsize;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * datasize, data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(data[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(data[0]) * 6));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, theindexsize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	trianglessize = datasize / 8;
	indexsize = theindexsize;
}

void Mesh::SetNormals()
{
	Utils::calcAverageNormals(indexdata, indexdatasize, meshdata, meshdatasize,8,3);

	ReBindData(meshdata, meshdatasize);
}

void Mesh::ChangeUplodaData(glm::mat4 matrix, bool scalenormal)//*make sure matrix isn't non-uniform scale for normal
{
	int stride = 8;
	for (int i = 0; i < meshdatasize; i += stride)
	{
		glm::vec4 extractvector(meshdata[i], meshdata[i + 1], meshdata[i + 2],1.0f);
		extractvector = matrix * extractvector;
		meshdata[i] = extractvector.x; meshdata[i + 1] = extractvector.y; meshdata[i + 2] = extractvector.z;

		glm::vec4 extractnormalvector(meshdata[i + 3], meshdata[i + 4], meshdata[i + 5], 0.0f);
		if(!scalenormal)
			extractnormalvector = glm::normalize(matrix * extractnormalvector);
		else
			extractnormalvector = matrix * extractnormalvector;
		meshdata[i+3] = extractnormalvector.x; meshdata[i + 4] = extractnormalvector.y; meshdata[i + 5] = extractnormalvector.z;
	}
	ReBindData(meshdata, meshdatasize);
}

void Mesh::ReBindData(GLfloat * data, int size)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(data[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(data[0]) * 6));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Mesh::LoadMesh(GLfloat * data, int datasize, bool stillusingindex)
{
	if(!stillusingindex)
		usingindexbuffer = false;
	meshdata = new GLfloat[datasize];
	for (int i = 0; i < datasize; i++)
	{
		meshdata[i] = data[i];
	}
	meshdatasize = datasize;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * datasize, data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(data[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(data[0]) * 6));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	trianglessize = datasize / 8;
}
void Mesh::RenderMesh()
{
	if (usingindexbuffer)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexsize, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
	else
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, trianglessize);
		glBindVertexArray(0);
	}

	/*method one with using 1 vao
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);*/

	/*without indexing
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, trianglessize);
	glBindVertexArray(0);
	*/
}

void Mesh::UpdatePhysics(float percent)
{
	physicscenter = physicscenter + velocity * percent;
}

Mesh::~Mesh()
{
	if(meshdata != nullptr)
		delete meshdata;
	if(indexdata != nullptr)
		delete indexdata;
	meshdata = nullptr;
	indexdata = nullptr;

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}