#include <mgl/world/mglChunkManager.hpp>
#include <mgl/world/mglChunk.hpp>

namespace mgl {
	void ChunkManager::drawOpaque() {
		for each (Chunk chunk& in chunks)
		{
			chunk.renderOpaque();
		}
	}

	// TODO
	void ChunkManager::drawTransparent() {
		std::count << "not implemented" << std::endl;
		exit(1);
	}
}