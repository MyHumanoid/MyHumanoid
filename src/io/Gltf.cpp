/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "Gltf.h"

#include <vector>
#include <glm/glm.hpp>
#include <physfs.h>
#include "Vfs.h"
#include <limits>

#include "tiny_gltf.h"
#include "animorph/Mesh.h"
#include "Logger.h"
#include "Version.h"

namespace gltf = tinygltf;

union uint_union {
	uint32_t value;
	std::array<unsigned char, 4> bytes;
};
static_assert(sizeof(uint_union) == 4, "foo");

union vec3_union {
	glm::vec3 value;
	std::array<unsigned char, 12> bytes;
};
static_assert(sizeof(vec3_union) == 12, "foo");

template<typename T>
int vec_push(std::vector<T> & vec, T & val) {
	vec.emplace_back(val);
	return vec.size() - 1;
}

template<typename T>
void push_bytes(std::vector<unsigned char> & vec, T & val) {
	std::copy(val.bytes.begin(), val.bytes.end(), std::back_inserter(vec));
}




static auto getRange(const std::vector<glm::vec3> & in) {
	using T = glm::vec3;
	using ET = T::value_type;
	
	T min = T(std::numeric_limits<ET>::max());
	T max = T(std::numeric_limits<ET>::min());
	for(glm::vec3 v : in) {
		min = glm::min(min, v);
		max = glm::max(max, v);
	}
	return std::make_pair(min, max);
}

static auto getRange(const std::vector<Animorph::Face> & in) {
	using ET = int;
	
	ET min = std::numeric_limits<ET>::max();
	ET max = std::numeric_limits<ET>::min();
	auto foo = [&](auto v) {
		min = glm::min(min, v);
		max = glm::max(max, v);
	};
	
	for(Animorph::Face f : in) {
		foo(f.vertices[0]);
		foo(f.vertices[1]);
		foo(f.vertices[2]);
	}
	return std::make_pair(min, max);
}


template <
	glm::length_t L,
	glm::qualifier Q = glm::defaultp,
    typename CONTAINER,
	typename F
>
void updateRange(gltf::Accessor & acc, const CONTAINER & c, F && functor) {
	
	using T = double;
	using FOO = glm::vec<L, T, Q>;
	
	FOO min = FOO(std::numeric_limits<T>::max());
	FOO max = FOO(std::numeric_limits<T>::min());
	
	std::for_each(c.begin(), c.end(), [&](auto & t){
		FOO v = functor(t);
		min = glm::min(min, v);
		max = glm::max(max, v);
	});
	
	for(int i = 0; i < L; ++i) {
		acc.minValues.emplace_back(min[i]);
		acc.maxValues.emplace_back(max[i]);
	}
}



void convertVertices(std::vector<unsigned char> & out, const std::vector<glm::vec3> & in) {
	for(glm::vec3 v : in) {
		vec3_union u;
		u.value = v;
		std::copy(u.bytes.begin(), u.bytes.end(), std::back_inserter(out));
	};
}

int convertFaces(std::vector<unsigned char> & out, const std::vector<Animorph::Face> & in)
{
	int elements = 0;
	
	union U {
		std::array<int, 3> ints;
		std::array<unsigned char, 12> chars;
	};
	static_assert(sizeof(U) == 12, "foo");
	
	auto pushTriangle1 = [&](auto f) {
		U u;
		u.ints[0] = f.vertices[0];
		u.ints[1] = f.vertices[1];
		u.ints[2] = f.vertices[2];
		elements += 3;
		std::copy(u.chars.begin(), u.chars.end(), std::back_inserter(out));
	};
	auto pushTriangle2 = [&](auto f) {
		U u;
		u.ints[0] = f.vertices[0];
		u.ints[1] = f.vertices[2];
		u.ints[2] = f.vertices[3];
		elements += 3;
		std::copy(u.chars.begin(), u.chars.end(), std::back_inserter(out));
	};
	
	for(Animorph::Face f : in) {
		pushTriangle1(f);
		if(f.getSize() == 4) {
			pushTriangle2(f);
		}
	};
	return elements;
}



auto convertMorphTargets(gltf::Model & model, const std::string & name,
                         const std::vector<Animorph::TargetData> & in)
{
	gltf::Buffer buffer;
	buffer.uri = fmt::format("morph-{}.bin", model.buffers.size());
	
	for(Animorph::TargetData t : in) {
		uint_union u;
		u.value = t.vertex_number;
		push_bytes(buffer.data, u);
	};
	int indexSize = buffer.data.size();
	
	for(Animorph::TargetData t : in) {
		vec3_union u;
		u.value = t.morph_vector;
		push_bytes(buffer.data, u);
	}
	int bufferIdx = vec_push(model.buffers, buffer);
	
	gltf::BufferView indexView;
	indexView.buffer = bufferIdx;
	indexView.byteOffset = 0;
	indexView.byteLength = in.size() * (4);
	int indexBVidx = vec_push(model.bufferViews, indexView);
	
	gltf::BufferView posView;
	posView.buffer = bufferIdx;
	posView.byteOffset = indexSize;
	posView.byteLength = in.size() * (4 * 3);
	int posBVidx = vec_push(model.bufferViews, posView);
	
	gltf::Accessor acc;
	acc.name = name;
	acc.type = TINYGLTF_TYPE_VEC3;
	acc.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
	acc.sparse.isSparse = true;
	acc.sparse.count = in.size();
	acc.sparse.indices.bufferView = indexBVidx;
	acc.sparse.indices.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
	acc.sparse.values.bufferView = posBVidx;
	
	auto mapper = [](const auto & td) -> glm::vec3 {
		return td.morph_vector;
	};
	updateRange<3>(acc, in, mapper);
	
	int foo = vec_push(model.accessors, acc);
	
	return foo;
}



void IoGltfWrite(const Animorph::Mesh & mhMesh, const string & fileName) {
	
	using namespace tinygltf;
	Model model;
	model.asset.version = "2.0";
	model.asset.generator = "MyHumanoid";
	
	{
		{
			const auto & faces = mhMesh.faces();
			
			Buffer b;
			b.uri = "mesh-indices.bin";
			int elements = convertFaces(b.data, faces);
			model.buffers.emplace_back(b);
			
			BufferView bv;
			bv.buffer = 0;
			bv.byteLength = b.data.size();
			model.bufferViews.emplace_back(bv);
			
			Accessor acc;
			acc.name = "indices";
			acc.bufferView = 0;
			acc.byteOffset = 0;
			acc.type = TINYGLTF_TYPE_SCALAR;
			acc.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
			acc.count = elements;
			
			auto range = getRange(faces);
			acc.minValues = {
				double(range.first)
			};
			acc.maxValues = {
				double(range.second)
			};
			model.accessors.emplace_back(acc);
		}
		{
			auto & rawVerts = mhMesh.rawVertices();
			
			Buffer b;
			b.uri = "mesh-vertices.bin";
			convertVertices(b.data, rawVerts);
			model.buffers.emplace_back(b);
			
			BufferView bv;
			bv.buffer = 1;
			bv.byteLength = b.data.size();
			model.bufferViews.emplace_back(bv);
			
			Accessor acc;
			acc.name = "vertices";
			acc.bufferView = 1;
			acc.byteOffset = 0;
			acc.type = TINYGLTF_TYPE_VEC3;
			acc.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
			acc.count = rawVerts.size();
			
			auto range = getRange(rawVerts);
			acc.minValues = {
				range.first.x,
				range.first.y,
				range.first.z
			};
			acc.maxValues = {
				range.second.x,
				range.second.y,
				range.second.z
			};
			model.accessors.emplace_back(acc);
		}
		
		Primitive prim;
		prim.mode = 4;
		prim.indices = 0;
		prim.attributes = {
			{"POSITION", 1}
		};
		
		
		{
			// Morph targets
			auto & targets = mhMesh.targets();
			for(auto & t : targets) {
				auto & name = t.first;
				auto & target = *t.second;
				
				if(target.size() == 0) {
					continue;
				}
				
				
				auto accIdx = convertMorphTargets(model, name, target);
				
				model.accessors[accIdx].count = mhMesh.rawVertices().size();
				
				prim.targets.push_back({
					{"POSITION", accIdx}
				});
			}
		}
		
		Mesh mesh;
		mesh.name = "Humanoid";
		mesh.primitives.emplace_back(prim);
		model.meshes.emplace_back(mesh);
		
		Node node;
		node.mesh = 0;
		model.nodes.emplace_back(node);
		
		Scene scene;
		scene.name = "Test",
		scene.nodes = {0},
		model.scenes.emplace_back(scene);
	}
	
	log_info("Writing gltf to: {}", fileName);
	
	TinyGLTF loader;
	loader.WriteGltfSceneToFile(&model,
	                            fileName,
	                            false, // embedImages
	                            false, // embedBuffers
	                            true, // prettyPrint
	                            false // writeBinary
	);
}
