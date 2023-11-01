#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

/*
Texture Class to manage textures used in SFML
Based on code from Professor Fox's video
*/

class TextureManager
{
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string textureName);
public:
	static sf::Texture& GetTexture(string textureName);
	static void Clear();

};

