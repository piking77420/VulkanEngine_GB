#include "Resource/Model.hpp"
#include <External/ExternalLoader.h>



#define OBJ ".obj"
#define GLFT ".gltf"

void Model::LoadResource(const std::string& path)
{
    
    std::string format = std::filesystem::path(path).extension().generic_string();

    if(format == OBJ)
    {
        LoadObj(path);

    }else if(format == GLFT)
    {
        LoadGltf(path);
    }

    Ebo.LoadEBO(indices);
    Vbo.LoadVBO(vertices);
    
}

void Model::Destroy()
{
    Ebo.FreeEBO();
    Vbo.FreeVBO();


}

void Model::OnChange()
{
}

void Model::LoadObj(const std::string& path)
{
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint16_t> uniqueVertices{};


    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };


            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
    
}

void Model::LoadGltf(const std::string& path)
{

    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    tinygltf::Model model;


    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
        std::cout << "Failed to load glTF: " << path << std::endl;
    else
        std::cout << "Loaded glTF: " << path << std::endl;

    const auto& meshes = model.meshes[0];
    std::vector<Vector3> normals;
    std::vector<Vector3> positions;
    std::vector<Vector2> texs;


    for (size_t i = 0; i < meshes.primitives.size(); ++i)
    {
        tinygltf::Primitive primitive = meshes.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        for (auto& attrib : primitive.attributes) {
            tinygltf::Accessor accessor = model.accessors[attrib.second];
            /*
            int byteStride =
                accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);
            */
            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                size = accessor.type;
            }

            std::cout << "Accessor Type: " << accessor.type << ", Component Type: " << accessor.componentType << std::endl;
            std::cout << "Attribute Name: " << attrib.first << std::endl;


            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0)
            {
                const int posAccessorIndex = primitive.attributes.at("NORMAL");
                const tinygltf::Accessor& posAccessor = model.accessors[posAccessorIndex];

                // Assuming the accessor component type is FLOAT and the type is VEC3
                const tinygltf::BufferView& bufferView = model.bufferViews[posAccessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                const int numComponents = 3; // Assuming VEC3
                const int numElements = posAccessor.count;

                const int byteStride = (bufferView.byteStride != 0) ? bufferView.byteStride : numComponents * sizeof(float);

                const float* posData = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + posAccessor.byteOffset]);

                for (int k = 0; k < numElements; ++k) {
                    Vector3 position;
                    position.x = posData[k * numComponents + 0];
                    position.y = posData[k * numComponents + 1];
                    position.z = posData[k * numComponents + 2];
                    positions.push_back(position);
                }
            }
            if (attrib.first.compare("NORMAL") == 0)
            {
                const int posAccessorIndex = primitive.attributes.at("NORMAL");
                const tinygltf::Accessor& posAccessor = model.accessors[posAccessorIndex];

                // Assuming the accessor component type is FLOAT and the type is VEC3
                const tinygltf::BufferView& bufferView = model.bufferViews[posAccessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                const int numComponents = 3; // Assuming VEC3
                const int numElements = posAccessor.count;

                const int byteStride = (bufferView.byteStride != 0) ? bufferView.byteStride : numComponents * sizeof(float);

                const float* posData = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + posAccessor.byteOffset]);

                for (int k = 0; k < numElements; ++k) {
                    Vector3 normal;
                    normal.x = posData[k * numComponents + 0];
                    normal.y = posData[k * numComponents + 1];
                    normal.z = posData[k * numComponents + 2];
                    normals.push_back(normal);
                }

            };
            if (attrib.first.compare("TEXCOORD_0") == 0)
            {
                const int posAccessorIndex = primitive.attributes.at("NORMAL");
                const tinygltf::Accessor& posAccessor = model.accessors[posAccessorIndex];

                // Assuming the accessor component type is FLOAT and the type is VEC3
                const tinygltf::BufferView& bufferView = model.bufferViews[posAccessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                const int numComponents = 2; // Assuming VEC2
                const int numElements = posAccessor.count;

                const int byteStride = (bufferView.byteStride != 0) ? bufferView.byteStride : numComponents * sizeof(float);

                const float* posData = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + posAccessor.byteOffset]);

                for (int k = 0; k < numElements; ++k) {
                    Vector2 tex;
                    tex.x = posData[k * numComponents + 0];
                    tex.y = posData[k * numComponents + 1];
                    texs.push_back(tex);
                }
            }


        }

        if (primitive.indices >= 0) {
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

            const int numElements = indexAccessor.count;
            const int byteStride = indexAccessor.ByteStride(model.bufferViews[indexAccessor.bufferView]);

            const tinygltf::BufferView& bufferView = model.bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

            const void* indexData = &buffer.data[bufferView.byteOffset + indexAccessor.byteOffset];

            for (int k = 0; k < numElements; ++k) {
                if (indexAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT) {
                    const unsigned int* indicesDataUint = reinterpret_cast<const unsigned int*>(indexData);
                    std::uint32_t index = indicesDataUint[k];
                    indices.push_back(index);
                }
                else if (indexAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT) {
                    const unsigned short* indicesDataUShort = reinterpret_cast<const unsigned short*>(indexData);
                    std::uint32_t index = indicesDataUShort[k];
                    indices.push_back(index);
                }
                // Add support for other component types as needed
            }
        }
    }



    vertices.resize(normals.size());
    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i] = Vertex(positions[i], normals[i], Vector2::One());
    }

}

    

