#pragma once
#include "VulkanConfig.hpp"



struct Vertex
{
	Vector3 pos;
    Vector3 Normal;
	Vector3 color;
	Vector2 texCoord;

    static VkVertexInputBindingDescription GetBindingDescription();

	static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
    
   
     

};


namespace std {
    template <>
    struct hash<Vertex> {
        size_t operator()(const Vertex& vertex) const {
            // Combine the hashes of pos, color, and texCoord using a hash function
            size_t hashValue = 0;
            hash_combine(hashValue, vertex.pos.x);
            hash_combine(hashValue, vertex.pos.y);
            hash_combine(hashValue, vertex.pos.z);
            hash_combine(hashValue, vertex.color.x);
            hash_combine(hashValue, vertex.color.y);
            hash_combine(hashValue, vertex.color.z);
            hash_combine(hashValue, vertex.texCoord.x);
            hash_combine(hashValue, vertex.texCoord.y);
            return hashValue;
        }

        // Hash combine function to combine multiple hash values
        template <typename T>
        void hash_combine(size_t& seed, const T& value) const {
            std::hash<T> hasher;
            seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };
}