#pragma once
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Graphics.hpp>
#include <vector>

namespace SFML_utils
{	
	bool onKeyReleased(sf::Keyboard::Key key);	
	bool onKeyPress(sf::Keyboard::Key key);
	bool onMouseButtonReleased(sf::Mouse::Button button);

	float			vecfLength(const sf::Vector2f & v);
	sf::Vector2f	normaliseVecf(const sf::Vector2f & v);
	sf::Vector2f	vecfMul(const sf::Vector2f & a, const sf::Vector2f & b);
	sf::Vector2f	quantiseVecf(const sf::Vector2f & vec, int quantise_size);
	std::string		vecfToString(const sf::Vector2f & v);
	sf::Vector2f	vecfFromString(const std::string & s);

	int				vecLength(const sf::Vector2i & v);
	sf::Vector2i	normaliseVec(const sf::Vector2i & v);
	sf::Vector2i	vecMul(const sf::Vector2i & a, const sf::Vector2i & b);
	sf::Vector2i	quantiseVec(const sf::Vector2i & vec, int quantise_size);
	std::string		vecToString(const sf::Vector2i & v);
	sf::Vector2i	vecFromString(const std::string & s);

	std::string sub_string(const std::string string, sf::Vector2i positions);
	std::string sub_string(const std::string substr1, const std::string substr2, const std::string data_string);

	sf::Vector2i string_dataposition(const std::string substr1, const std::string substr2, const std::string data_string);

	bool string_contains(std::string string, char character);
	bool string_contains(std::string string, std::string  characters);

	bool file_exists(std::string file_name);

	void fileout(std::string filename, std::string data);
	void fileout(std::string filename, unsigned char* data,int length);
	std::vector<std::string>* filein(std::string filename);

	class Text {
	public:
		static void		initialise(std::string fontPath);
		static void		draw(sf::Vector2f pos, unsigned int size, std::string text, sf::RenderWindow & window, sf::Color color = sf::Color::White,bool drawCenter = false);

	private:
		static sf::Font				font;
		static sf::Text				text;
	};
}
	
