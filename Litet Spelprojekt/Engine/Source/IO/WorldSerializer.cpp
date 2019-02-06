#include <EnginePch.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <IO/WorldSerializer.h>

World* WorldSerializer::Read(const char* const path)
{
	using namespace rapidjson;

	std::ifstream file(path);
	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	Document document;
	document.Parse(data.c_str(), data.length());
	const Value& jsonObject = document.GetObjectA();
	const Value& levels = jsonObject["levels"];
	
	WorldLevel** worldLevels = new WorldLevel*[levels.Size()];

	for (uint32 levelId = 0; levelId < levels.Size(); levelId++)
	{
		const Value& level = levels[levelId];
		uint32 xSize = level.Size();
		uint32 zSize = level[0].Size();
		uint32* levelIndexes = new uint32[xSize * zSize];

		for (uint32 xId = 0; xId < xSize; xId++)
		{
			const Value& x = level[xId];

			for (uint32 zId = 0; zId < zSize; zId++)
			{
				levelIndexes[xId *  zSize + zId] = x[zId].GetUint();
			}
		}

		worldLevels[levelId] = new WorldLevel(levelIndexes, xSize, zSize);
		delete[] levelIndexes;
		levelIndexes = nullptr;
	}

	const Value& objects = jsonObject["objects"];
	WorldObject* worldObjects = new WorldObject[objects.Size()];

	for (uint32 objectId = 0; objectId < objects.Size(); objectId++)
	{
		worldObjects[objectId].TileId.x = objects[objectId]["tileIdX"].GetUint();
		worldObjects[objectId].TileId.y = objects[objectId]["tileIdY"].GetUint();
		worldObjects[objectId].TileId.z = objects[objectId]["tileIdZ"].GetUint();
		worldObjects[objectId].MeshId = objects[objectId]["meshId"].GetUint();
		worldObjects[objectId].MaterialId = objects[objectId]["materialId"].GetUint();
	}

	const Value& stairs = jsonObject["stairs"];
	glm::ivec3* worldStairs = new glm::ivec3[stairs.Size()];

	for (uint32 stairId = 0; stairId < stairs.Size(); stairId++)
	{
		const Value& stair = stairs[stairId];
		worldStairs[stairId] = glm::ivec3(stair[0].GetUint(), stair[1].GetUint(), stair[2].GetUint());
	}

	World* world = new World(worldLevels, levels.Size(), worldObjects, objects.Size());
	world->SetStairs(worldStairs, stairs.Size());
	delete[] worldLevels;
	worldLevels = nullptr;
	delete[] worldObjects;
	worldObjects = nullptr;
	delete[] worldStairs;
	worldStairs = nullptr;
	return world;
}

void WorldSerializer::Write(const char* const path, const World& world)
{
	using namespace rapidjson;

	StringBuffer stringBuffer;
	PrettyWriter<StringBuffer> writer(stringBuffer);
	
	writer.StartObject();
	writer.String("levels");
	writer.StartArray();
	for (uint32 levelId = 0; levelId < world.GetNumLevels(); levelId++)
	{
		const WorldLevel* level = world.GetLevel(levelId);
		writer.StartArray();

		for (uint32 x = 0; x < level->GetSizeX(); x++)
		{
			writer.StartArray();

			for (uint32 z = 0; z < level->GetSizeZ(); z++)
			{
				writer.Uint(level->GetLevel()[x][z]);
			}

			writer.EndArray();
		}

		writer.EndArray();
	}
	writer.EndArray();

	writer.String("objects");
	writer.StartArray();

	for (uint32 objectId = 0; objectId < world.GetNumWorldObjects(); objectId++)
	{
		const WorldObject& currentObject = world.GetWorldObject(objectId);
		writer.StartObject();
		writer.String("tileIdX");
		writer.Uint(currentObject.TileId.x);
		writer.String("tileIdY");
		writer.Uint(currentObject.TileId.y);
		writer.String("tileIdZ");
		writer.Uint(currentObject.TileId.z);
		writer.String("meshId");
		writer.Uint(currentObject.MeshId);
		writer.String("materialId");
		writer.Uint(currentObject.MaterialId);
		writer.EndObject();
	}

	writer.EndArray();

	writer.String("stairs");
	writer.StartArray();
	for (uint32 stairId = 0; stairId < world.GetNumStairs(); stairId++)
	{
		const glm::ivec3* stairs = world.GetStairs();
		writer.StartArray();

		writer.Uint(stairs[stairId].x);
		writer.Uint(stairs[stairId].y);
		writer.Uint(stairs[stairId].z);

		writer.EndArray();
	}
	writer.EndArray();

	writer.EndObject();

	//std::cout << stringBuffer.GetString() << std::endl;

	FILE* file = fopen(path, "w");
	fputs(stringBuffer.GetString(), file);
	fclose(file);
}