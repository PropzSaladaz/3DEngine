#include <glm/glm.hpp>

namespace mgl {
	
	#define VOXEL_SIZE 0.25f
	#define CHUNK_SIZE 50
	#define WORLD_HEIGHT_CHUNKS 10

	#define NUMBER_OF_LODS 2

	class World;
	
	struct WorldRenderSettings {
		int chunkRenderDistance;
		int chunkRenderDistanceForLOD[NUMBER_OF_LODS];

		WorldRenderSettings(int renderDistance, int renderDistanceForLOD) :
			chunkRenderDistance(renderDistance), chunkRenderDistance(chunkRenderDistanceForLOD) {}
	};

	class World {
	public:
		WorldRenderSettings RenderSettings(5, 10);
		glm::vec3 CurrentPlayerChunkIdx;

		static World& getInstance();

		init();
	};
}