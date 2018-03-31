#include "save_load_gamedata.h"

std::vector<Entity_data> load_entities_data(const char* filepath)
{
	std::vector<Entity_data> entity_data;
	std::string line;
	std::ifstream file_reader(filepath);

	if (file_reader.is_open())
	{
		while (getline(file_reader, line))
		{
			std::string elements[6];
			int counter = 0;
			
			for (int i = 0; i < line.size(); i++)
			{
				if (counter > 5)
					break;
				else if (line.at(i) == ';')
					counter++;
				else
					elements[counter] += line.at(i);
			}

			entity_data.emplace_back(Entity_data(
				elements[0],
				elements[1],
				stof(elements[2]),
				stof(elements[3]),
				stof(elements[4]),
				stof(elements[5])
			));
		}
		file_reader.close();

		console::print("Successfully processed file: ");
		console::print(filepath);
		console::print("\n");
	}
	else
	{
		console::print("Failed to open file: ");
		console::print(filepath);
		console::print("\n");
	}

	return entity_data;
}

void save_entities_data(std::vector<Entity_data> entities_data)
{
	
}

std::map<std::string, sf::Texture> load_textures(std::string filepath)
{
	std::map<std::string, sf::Texture> textures;
	std::string line;
	std::ifstream file_reader(filepath);

	if (file_reader.is_open())
	{
		while (getline(file_reader, line))
		{
			sf::Texture texture;
			if (!texture.loadFromFile("resources/textures/" + line))
			{
				console::print("Failed to load texture: ");
				console::print(line);
				console::print("\n");
			}
			
			textures[line] = texture;
		}
		file_reader.close();

		console::print("Successfully processed file: ");
		console::print(filepath);
		console::print("\n");
	}
	else
	{
		console::print("Failed to open file: ");
		console::print(filepath);
		console::print("\n");
	}

	return textures;
}