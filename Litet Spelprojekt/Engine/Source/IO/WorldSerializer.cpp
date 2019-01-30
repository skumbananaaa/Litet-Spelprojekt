#include <EnginePch.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <IO/WorldSerializer.h>

void WorldSerializer::Read(const char* const path, World& world)
{
	using namespace rapidjson;

	std::ifstream file(path);
	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	Document document;
	document.Parse(data.c_str(), data.length());
	std::cout << data << std::endl;
	int i = 0;
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
		writer.String(("level" + std::to_string(levelId)).c_str());
		writer.StartArray();

		for (uint32 x = 0; x < level->GetSizeX(); x++)
		{
			writer.String(("x" + std::to_string(x)).c_str());
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