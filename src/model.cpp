#include <list>
#include <map>
#include <model.hpp>
#include <shaders.hpp>
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

static std::map<std::string, Texture> loaded_textures;

static glm::vec3 convertVector(const aiVector3D &v)
{
	return {v.x, v.y, v.z};
}

static glm::vec2 convertVector(const aiVector2D &v)
{
	return {v.x, v.y};
}

// https://learnopengl.com/Model-Loading/Model
static std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &name)
{
	std::vector<Texture> textures(mat->GetTextureCount(type));
	for (uint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		if (loaded_textures.count(std::string(str.C_Str())) > 0)
		{
			textures.push_back(loaded_textures[std::string(str.C_Str())]);
		}
		else
		{
			Texture tex;
			tex.texId = loadTexture((std::string("assets/textures/") + std::string(str.C_Str())).c_str());
			tex.type = name;
			textures.push_back(tex);
			loaded_textures[std::string(str.C_Str())] = tex;
		}
	}
	return textures;
}

static Mesh * processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.emplace_back(convertVector(mesh->mVertices[i]),
		                      convertVector(mesh->mNormals[i]),
		                      mesh->mTextureCoords[0] ? convertVector(mesh->mTextureCoords[0][i]) : glm::vec2(0.0f));
	}
	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse.begin(), diffuse.end());
		std::vector<Texture> specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular.begin(), specular.end());
	}
	return new Mesh(vertices, indices, textures);
}

static std::list<Mesh*> processNode(aiNode *node, const aiScene *scene)
{
	std::list<Mesh*> meshes;
	for (uint i = 0; i < node->mNumMeshes; i++)
		meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		std::list<Mesh*> childMeshes = processNode(node->mChildren[i], scene);
		meshes.insert(meshes.end(), childMeshes.begin(), childMeshes.end());
	}
	return meshes;
}

Material::Material() : ambient(0.1f), diffuse(0.6f), specular(0.3f), shine(32.0f)
{}

Material::Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shine)
		: ambient(ambient), diffuse(diffuse), specular(specular), shine(shine)
{}

Material::Material(const Material &m) = default;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
		: ComplexRenderable(),
		  vertices(std::move(vertices)),
		  indices(std::move(indices)),
		  textures(std::move(textures))
{
	internal = new IndexedRenderable(this->vertices, this->indices, GL_TRIANGLES);
}

Mesh::~Mesh()
{
	delete internal;
}

// https://learnopengl.com/Model-Loading/Mesh
void Mesh::render(GLuint shader)
{
	uint diffuseNum = 0, specularNum = 0;
	for (uint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		std::string name = textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNum++);
		else if (name == "texture_specular")
			number = std::to_string(specularNum++);
		setUniformInt(shader, (name + number).c_str(), i + 1);
		glBindTexture(GL_TEXTURE_2D, textures[i].texId);
	}
	glActiveTexture(GL_TEXTURE0);
	setUniformMat4(shader, MODEL, getHierarchicalWorldTransform());
	internal->render();
}

Model::Model(const char *path)
{
	Assimp::Importer in;
	const aiScene *scene = in.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		throw std::runtime_error("Failed to import model: " + std::string(in.GetErrorString()));
	std::list<Mesh*> m = processNode(scene->mRootNode, scene);
	meshes = std::vector(m.begin(), m.end());
	for (auto &mesh : meshes)
		mesh->setParent(this);
}

Model::~Model()
{
	for (auto &m : meshes)
		delete m;
}

void Model::render(GLuint shader)
{
	for (auto &m : meshes)
		m->render(shader);
}
