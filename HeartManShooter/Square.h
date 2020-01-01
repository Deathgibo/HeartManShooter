#pragma once
#include "Mesh.h"
#include "Utils.h"

class Square : public Mesh
{
public:
	Square();
	void createsquare(float width, float height);
	void createsquareflat(float thewidth, float thedepth);
	void createsquarenox(float theheight, float thedepth);
	~Square();

private:

};

