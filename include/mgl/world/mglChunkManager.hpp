#ifndef MGL_CHUNK_MANAGER_HPP
#define MGL_CHUNK_MANAGER_HPP

#include <mgl/world/mglChunk.hpp>
#include <vector>

namespace mgl {
	class ChunkManager {
	public:
		/// <summary>
		/// Draws chunk's opaque voxels only
		/// </summary>
		void drawOpaque();

		/// <summary>
		/// Draws transaprent voxels only.
		/// This includes both water as well as transparent types such as glass
		/// </summary>
		void drawTransparent();

	private:
		std::vector<Chunk> chunks;
	};
}

#endif