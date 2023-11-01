#include "TextureManager.h"


unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string file) {
	string path = "images/";
	path += file + ".png";

	textures[file].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName) {
	if (textures.find(textureName) == textures.end()) {
		LoadTexture(textureName);
	}
		return textures[textureName];
}

void TextureManager::Clear() {
	textures.clear();
}