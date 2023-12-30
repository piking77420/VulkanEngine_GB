#pragma once
#include "VulkanConfig.h"


	class Vertex
	{
	public:
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;


		static VkVertexInputBindingDescription GetBindingDescription();

		constexpr Vertex(const Vector3& position, const Vector3& _normal,const Vector2& uv)
			: pos(position), normal(_normal) , texCoord(uv)
		{
		}
		constexpr Vertex() = default;

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
		}


	};

    namespace std {
        template <>
        struct hash<Vertex> {
            size_t operator()(const Vertex& vertex) const {
                // Combine the hashes of pos, normal, and texCoord using a hash function
                size_t hashValue = 0;
                hash_combine(hashValue, vertex.pos.x);
                hash_combine(hashValue, vertex.pos.y);
                hash_combine(hashValue, vertex.pos.z);
                hash_combine(hashValue, vertex.normal.x);
                hash_combine(hashValue, vertex.normal.y);
                hash_combine(hashValue, vertex.normal.z);
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
