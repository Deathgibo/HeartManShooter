#include "Model.h"

Model::Model()
{

}

Model::Model(const Model& obj)
{
	//cout << "model copy constructor!"<<endl;
	for (int i = 0; i < obj.meshList.size(); i++)
	{
		Mesh* tmp = new Mesh();
		tmp->LoadMeshIndex(obj.meshList[i]->meshdata, obj.meshList[i]->meshdatasize, obj.meshList[i]->indexdata, obj.meshList[i]->indexdatasize);
		tmp->physicscenter = obj.meshList[i]->physicscenter; tmp->velocity = obj.meshList[i]->velocity;

		meshList.push_back(tmp);
	}
	/*for (int i = 0; i < obj.textureList.size(); i++)
	{
		Texture* tmp = new Texture();
		tmp = obj.textureList[i];
		textureList.push_back(tmp);
	}*/
	textureList = obj.textureList;
	meshToTex = obj.meshToTex;

	copy = obj.copy;
}

/*Model& Model::operator=(const Model& obj)
{
	cout << "model assignment constructor!" << endl;
	for (int i = 0; i < obj.meshList.size(); i++)
	{
		Mesh* tmp = new Mesh();
		tmp->LoadMeshIndex(obj.meshList[i]->meshdata, obj.meshList[i]->meshdatasize, obj.meshList[i]->indexdata, obj.meshList[i]->indexdatasize);
		tmp->physicscenter = obj.meshList[i]->physicscenter; tmp->velocity = obj.meshList[i]->velocity;
		meshList.push_back(tmp);
	}
	textureList = obj.textureList;
	meshToTex = obj.meshToTex;

	return *this;
}*/

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)//meshList.size()
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::RenderModel3D(Texture3D& text)
{
	text.UseTexture3d();
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			//textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

void Model::LoadNode(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	//Utils::calcAverageNormals(&indices[0], indices.size(), &vertices[0], vertices.size(), 8, 3);
	newMesh->LoadMeshIndex(&vertices[0], vertices.size(), &indices[0], indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTextureA())
				{
					printf("Failed to load texture at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i])
		{
			//cout << "missing" << endl;
			textureList[i] = new Texture("Textures/missingtexture.png");
			textureList[i]->LoadTextureA();
		}
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i] != nullptr)
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i] != nullptr)
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
	meshList.clear();
	textureList.clear();

}

void Model::ClearMeshes()//clear for copies
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			textureList[i] = nullptr;
		}
	}
	meshList.clear();
	textureList.clear();
}

void Model::ChangeandUploadData(glm::mat4 matrix, bool scalenormal)
{
	//go through each mesh's data, extact vector vector mat multiply reinsert, then rebind data
	for (int i = 0; i < meshList.size(); i++)
	{
		meshList[i]->ChangeUplodaData(matrix, scalenormal);
	}
}

void Model::SetNormals()
{
	for (int i = 0; i < meshList.size(); i++)
	{
		meshList[i]->SetNormals();
	}
}

Model::~Model()
{	
	//ClearModel();
}