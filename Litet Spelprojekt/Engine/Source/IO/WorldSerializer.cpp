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
	const Value& object = document.GetObjectA();
	const Value& levels = object["levels"];
	
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

	World* world = new World(worldLevels, levels.Size());
	delete[] worldLevels;
	worldLevels = nullptr;
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
	//Loop through each world level
	for (uint32 levelId = 0; levelId < world.GetNumLevels(); levelId++)
	{
		const WorldLevel* level = world.GetLevel(levelId);
		//writer.String(("level" + std::to_string(levelId)).c_str());
		writer.StartArray();

		for (uint32 x = 0; x < level->GetSizeX(); x++)
		{
			//writer.String(("x" + std::to_string(x)).c_str());
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
	writer.EndObject();

	//std::cout << stringBuffer.GetString() << std::endl;

	FILE* file = fopen(path, "w");
	fputs(stringBuffer.GetString(), file);
	fclose(file);
}