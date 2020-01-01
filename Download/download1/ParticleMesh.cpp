#include "ParticleMesh.h"

ParticleMesh::ParticleMesh()
{

}

void ParticleMesh::LoadParticleIndex(int particlesize, float width, float height)
{
	GLfloat data[20]{
		-width, height,0,0,1,	//tl  0
		-width,-height,0,0,0,	//bl  1
		 width, height,0,1,1,	//tr  2
		 width,-height,0,1,0,	//br  3
	};

	unsigned int indices[6] = {
		0,2,1,
		2,3,1
	};
	int datasize = 20;
	int theindexsize = 6;

	meshdatasize = datasize;
	indexdatasize = theindexsize;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * datasize, data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(data[0]) * 3));
	
	glGenBuffers(1, &positionbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * particlesize, NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * particlesize, NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &anglebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, anglebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * particlesize, NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, theindexsize * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	trianglessize = datasize / 3;
	indexsize = theindexsize;
}

void ParticleMesh::UpdateBind(int particlesize, GLfloat* positiondata, GLfloat* colordata, GLfloat* angledata)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * particlesize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * particlesize, positiondata);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * particlesize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * particlesize, colordata);

	glBindBuffer(GL_ARRAY_BUFFER, anglebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * particlesize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * particlesize, angledata);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, anglebuffer);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glBindVertexArray(0);
}

void ParticleMesh::RenderMesh(int particlecount)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElementsInstanced(GL_TRIANGLES, indexsize, GL_UNSIGNED_INT, 0, particlecount);
	//glDrawArraysInstanced(GL_TRIANGLES, 0, trianglessize, particlecount);
	//glDrawArrays(GL_TRIANGLES, 0, trianglessize);
	glBindVertexArray(0);
}

ParticleMesh::~ParticleMesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &positionbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &anglebuffer);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}
