#pragma once


#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <conio.h>
#include <ctime>
#include <string>
#include <algorithm>
#include "SFML/Graphics.hpp"

#define Number_of_Rows 7
#define Number_of_Columns 9
#define GAMETYPE 'S'

using namespace std;

char GameMap_M[Number_of_Rows][Number_of_Columns] = {
	{ "<...x..." },
{ "..x...x." },
{ "x...x..." },
{ "..x..x.x" },
{ ".x..x..." },
{ "..x...x." },
{ ".x..x..]" }
};
char GameMap_3[Number_of_Rows][Number_of_Columns] = {
{ "<...x..." },
{ "..x...x." },
{ "x...x..." },
{ "..x..x.x" },
{ ".x..x..." },
{ "..x...x." },
{ ".x..x..]" }
};
bool flag = false;


namespace PM {

class Object
{


protected:
	sf::Vector2f position;
	
	bool isDirty = true;

public:
	virtual	void draw(sf::RenderWindow &window) = 0;

	void setPosition(sf::Vector2f pos) {
		position = pos;
		isDirty = true;
	}
	sf::Vector2f getPosition() {
		return	position;
	}

	virtual void onCollision() {

	}

};
//+++++++++++++++++++++++++++++++++++++
class Body : public Object
{
	sf::Texture pTexture;
	sf::Sprite image;

protected:
	bool setTexture(sf::Color status, char D) {
		switch (D)
		{
		case 'R':
			if (pTexture.loadFromFile("RightDirection.png")) {
				image.setColor(status);
				image.setTexture(pTexture);
				image.setScale(0.7, 0.7);

				return true;
			}
			else {
				return false;
			}break;

		case 'L':
			if (pTexture.loadFromFile("LeftDirection.png")) {
				image.setColor(status);
				image.setTexture(pTexture);
				image.setScale(0.7, 0.7);
				return true;
			}
			else {
				return false;
			}break;

		case 'U':
			if (pTexture.loadFromFile("UpDirection.png")) {
				image.setColor(status);
				image.setTexture(pTexture);
				image.setScale(0.7, 0.7);
				return true;
			}
			else {
				return false;
			}break;

		case 'D':
			if (pTexture.loadFromFile("DownDirection.png")) {
				image.setColor(status);
				image.setTexture(pTexture);
				image.setScale(0.7, 0.7);
				return true;
			}
			else {
				return false;
			}break;
		}
		
	}

public:
	Body() {}
	~Body() {}

	void draw(sf::RenderWindow &window) override
	{
		if (isDirty) {
			image.setPosition(position.x, position.y);
			isDirty = false;
		}
		window.draw(image);
	}

};

//****************************
class Body_Normal : public Body
{
public:
	Body_Normal(char D = 'R')
	{
		setTexture(sf::Color::Yellow, D);
	}
};
class Body_Hollow : public Body
{
public:
	Body_Hollow(char D = 'R')
	{
		setTexture(sf::Color(255,255,255,100), D);
	}
};
class Body_Super : public Body
{
public:
	Body_Super(char D = 'R')
	{
		setTexture(sf::Color::Red, D);
	}
};
//****************************
class Food : public Object
{
	sf::Texture texture;
	sf::Sprite image;

public:
	Food(string Img = "Sugar.png")
	{

		setTexture(Img);

	}

	bool setTexture(string Img_URL)
	{
		bool Drawable = true;

		if (texture.loadFromFile(Img_URL))
		{
			isDirty = true;
		}

		if (!texture.loadFromFile(Img_URL))
		{
			cout << "Error while loading the picture";
			Drawable = false;
		}

		return Drawable;

	}

	void Set_Color(int m = 1)
	{
		image.setColor(sf::Color((rand() % 1 + 254)*m, (rand() % 1 + 254)*m, (rand() % 1 + 254)*m, 255));
	}

	void draw(sf::RenderWindow &window) override
	{
		if (isDirty) {
			image.setPosition(position.x, position.y);
			image.setTexture(texture);
			Set_Color();
			isDirty = false;
		}
		window.draw(image);
	}
};

class Obstacles : public Object
{
	int Random_Sides;
	float Obtascle_Radius;

	sf::CircleShape obstacle;
public:

	Obstacles(float x = 45)
	{
		Set_Obstacles_Radius(x);
		Set_Obstacles_Sides();
	}

	void Set_Obstacles_Radius(float X)
	{
		Obtascle_Radius = X;
		isDirty = true;
	}
	float Get_Obstacles_Radius()
	{
		return Obtascle_Radius;
	}

	void Set_Obstacles_Sides()
	{
		Random_Sides = rand() % 5 + 3;
		isDirty = true;
	}
	int Get_Obstacles_Sides()
	{
		return Random_Sides;
	}

	void draw(sf::RenderWindow &window) override
	{
		if (isDirty) {
			obstacle = sf::CircleShape(Obtascle_Radius, Random_Sides);
			obstacle.setPosition(position.x, position.y);
			obstacle.setFillColor(sf::Color::Magenta);
			isDirty = false;
		}
		window.draw(obstacle);
	}
};

class Gates : public Object
{
	sf::Texture texture;
	sf::Sprite image;

	//string Img_URL;

public:
	Gates(string Img = "G_Right.png")
	{
		setTexture(Img);
	}

	bool setTexture(string Img_URL)
	{
		bool Drawable = true;

		if (texture.loadFromFile(Img_URL))
		{
			isDirty = true;
		}

		if (!texture.loadFromFile(Img_URL))
		{
			cout << "Error while loading the picture";
			Drawable = false;
		}

		return Drawable;

	}

	void draw(sf::RenderWindow &window) override
	{
		if (isDirty) {
			image.setPosition(position.x, position.y);
			image.setTexture(texture);
			isDirty = false;
		}
		window.draw(image);
	}
};
//+++++++++++++++++++++++++++++++++++++

class Map //has a aggregation realtion with object class
{
	vector<Object*> objects;

protected:
	
	char *ptr[Number_of_Rows];
	char *ptr_Moving;

	
public:

	Map(char C = 'N')
	{
		if (flag == true)
		{
			objects.clear();
			flag = false;

			for (int i = 0; i < Number_of_Rows; i++)
			{
				ptr[i] = nullptr;
			}

		}

			for (int i = 0; i < Number_of_Rows; i++)
			{
				ptr[i] = GameMap_3[i];
			}
		
		Set_Level(C);
	}

	void Set_Level(char character = 'N', char gametype = GAMETYPE)
	{

		for (int R = 0; R < Number_of_Rows; R++)
		{
			ptr_Moving = &ptr[R][0];

			for (int C = 0; C < Number_of_Columns - 1; C++, ptr_Moving++)
			{

				if (*ptr_Moving == 'x')
				{
					Set_Obstacles(sf::Vector2f(70 + (135 * C), 110 + (143 * R)));
				}
				else if (*ptr_Moving == ']')
				{
					Set_Gate(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)));
				}
				else if (*ptr_Moving == '.')
				{
					Set_Food(sf::Vector2f(55 + (135 * C), 97 + (143 * R)));
				}
				else if (*ptr_Moving == '<' || *ptr_Moving == '>' || *ptr_Moving == 'v' || *ptr_Moving == '^')
				{
					switch (character)
					{
					case 'N':
						if (*ptr_Moving == '<')
						{
							Set_Player_Normal(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)));
						}
						else if (*ptr_Moving == '>')
						{
							Set_Player_Normal(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'L');
						}
						else if (*ptr_Moving == 'v')
						{
							Set_Player_Normal(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'U');
						}
						else if (*ptr_Moving == '^')
						{
							Set_Player_Normal(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'D');
						}break;
					case 'H':
						if (*ptr_Moving == '<')
						{
							Set_Player_Hollow(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)));
						}
						else if (*ptr_Moving == '>')
						{
							Set_Player_Hollow(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'L');
						}
						else if (*ptr_Moving == 'v')
						{
							Set_Player_Hollow(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'U');
						}
						else if (*ptr_Moving == '^')
						{
							Set_Player_Hollow(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'D');
						}break;
					case 'S':
						if (*ptr_Moving == '<')
						{
							Set_Player_Super(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)));
						}
						else if (*ptr_Moving == '>')
						{
							Set_Player_Super(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'L');
						}
						else if (*ptr_Moving == 'v')
						{
							Set_Player_Super(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'U');
						}
						else if (*ptr_Moving == '^')
						{
							Set_Player_Super(sf::Vector2f(60 + (135 * C), 99.5 + (143 * R)), 'D');
						}break;
					}
				}

			}
		}

		if (gametype == 'M')
		{

			for (int R2 = 0; R2 < 7; R2++)
			{
				for (int C2 = 0; C2 < 8; C2++)
				{
					float c = (float)C2;
					float r = (float)R2;
					if (GameMap_M[R2][C2] == 'x')
					{
						Set_Obstacles(sf::Vector2f(70 + (135 * c) + 1250, 110 + (143 * r)));
					}
					else if (GameMap_M[R2][C2] == ']')
					{
						Set_Gate(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)));
					}
					else if (GameMap_M[R2][C2] == '.')
					{
						Set_Food(sf::Vector2f(55 + (135 * c) + 1250, 97 + (143 * r)));


					}
					else if (GameMap_M[R2][C2] == '<' || GameMap_M[R2][C2] == '>' || GameMap_M[R2][C2] == 'v' || GameMap_M[R2][C2] == '^')
					{
						switch (character)
						{
						case 'N':
							if (GameMap_M[R2][C2] == '<')
							{
								Set_Player_Normal(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)));
							}
							else if (GameMap_M[R2][C2] == '>')
							{
								Set_Player_Normal(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'L');
							}
							else if (GameMap_M[R2][C2] == 'v')
							{
								Set_Player_Normal(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'U');
							}
							else if (GameMap_M[R2][C2] == '^')
							{
								Set_Player_Normal(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'D');
							}break;
						case 'H':
							if (GameMap_M[R2][C2] == '<')
							{
								Set_Player_Hollow(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)));
							}
							else if (GameMap_M[R2][C2] == '>')
							{
								Set_Player_Hollow(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'L');
							}
							else if (GameMap_M[R2][C2] == 'v')
							{
								Set_Player_Hollow(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'U');
							}
							else if (GameMap_M[R2][C2] == '^')
							{
								Set_Player_Hollow(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'D');
							}break;
						case 'S':
							if (GameMap_M[R2][C2] == '<')
							{
								Set_Player_Super(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)));
							}
							else if (GameMap_M[R2][C2] == '>')
							{
								Set_Player_Super(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'L');
							}
							else if (GameMap_M[R2][C2] == 'v')
							{
								Set_Player_Super(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'U');
							}
							else if (GameMap_M[R2][C2] == '^')
							{
								Set_Player_Super(sf::Vector2f(60 + (135 * c) + 1250, 99.5 + (143 * r)), 'D');
							}break;
						}
					}

				}
			}
		}


	}
		
	void Set_Gate(sf::Vector2f pos)
	{
		Object *gate = new Gates();
		gate->setPosition(pos);
		objects.push_back(gate);
	}
	
	void Set_Food(sf::Vector2f pos)
	{
		Object *food = new Food();
		food->setPosition(pos);
		objects.push_back(food);
	}
	
	void Set_Obstacles(sf::Vector2f pos)
	{
		Object * obstacle = new Obstacles();
		obstacle->setPosition(pos);
		objects.push_back(obstacle);
	}
	
	void Set_Player_Normal(sf::Vector2f pos, char D ='R')
	{
		Object *player = new Body_Normal(D);
		player->setPosition(pos);
		objects.push_back(player);
		
	}
	
	void Set_Player_Hollow(sf::Vector2f pos, char D = 'R')
	{
		Object *player = new Body_Hollow(D);
		player->setPosition(pos);
		objects.push_back(player);
	}
	
	void Set_Player_Super(sf::Vector2f pos, char D = 'R')
	{
		Object *player = new Body_Super(D);
		player->setPosition(pos);
		objects.push_back(player);
	}
	
	void Draw_Map(sf::RenderWindow &window)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->draw(window);
		}

	}

	void Clear_Map(void)
	{
		for (int r = 0; r < Number_of_Rows; r++)
		{
			for (int c = 0; c < Number_of_Columns - 1; c++)
			{
				ptr[r][c] = ' ';
			}
		}
	}
	
};

class gamehandler : public Object  , public Map
{
	
	sf::RenderWindow window;
	sf::Vector2f position;
	sf::Vector2f positionM;

	sf::Font myfont;

	sf::Text mytext;
	sf::Text mytext2;
	sf::Text mytext3;
	sf::Text mytextM;
	sf::Text mytextM2;

	char status = 'N';
	clock_t time_taken;
	bool isUsed = true;
	bool superpower = false;
	bool sflag = true;
	int scounter = 0;
	bool lost = false;
	int X_Pos = 0;
	int Y_Pos = 0;
	int score = 0;
	int N_moves = 0;

	char statusM = 'N';
	clock_t time_takenM;
	bool isUsedM = true;
	bool lostM = false;
	int X_PosM = 0;
	int Y_PosM = 0;
	int scoreM = 0;

public:
	
	

	gamehandler() { 

		mytext.setFont(myfont);
		mytext.setCharacterSize(64);
		

		mytext2.setFont(myfont);
		mytext2.setCharacterSize(45);

		mytext3.setFont(myfont);
		mytext3.setCharacterSize(45);

		mytextM.setFont(myfont);
		mytextM.setCharacterSize(64);
		mytextM.setPosition(sf::Vector2f(1850, 1150));

		mytextM2.setFont(myfont);
		mytextM2.setCharacterSize(45);

		if (!myfont.loadFromFile("BLKCHCRY.TTF"))
		{
			cout << "Error in loading the font";
		}
	};

	void handlekeyfunction(sf::Event Event, sf::RenderWindow &window)
	{
		
		switch (Event.type)
		{
		case sf::Event::KeyPressed:

			 if (Event.key.code == sf::Keyboard::Right)
			{
				 if (lost == false)
				 {
					 Text_Clear();
					 N_moves++;
					if (ptr[Y_Pos][X_Pos] != '\0')
					{
						if (status == 'N')
						{
							if (ptr[Y_Pos][X_Pos + 1] == '.' || ptr[Y_Pos][X_Pos + 1] == ' ')
							{
								if (ptr[Y_Pos][X_Pos + 1] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos][X_Pos + 1] = '<';
								X_Pos++;
							
							}
							else if (ptr[Y_Pos][X_Pos + 1] == ']')
							{
								if (score == 40 || !isUsed)
								{
									End_Game('R', 'W');
								
								}
								else
								{
									mytext.setPosition(sf::Vector2f(400, 1250));
									mytext.setString("Please finish your food...");
								}
							
							}
							else
							{
								End_Game('R');
							}
						
						}
						else if (status == 'H')
						{
							if (ptr[Y_Pos][X_Pos + 1] == '.' || ptr[Y_Pos][X_Pos + 1] == ' ')
							{
								if (ptr[Y_Pos][X_Pos + 1] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos][X_Pos + 1] = '<';
								X_Pos++;

							}
							else if (ptr[Y_Pos][X_Pos + 1] == 'x' && X_Pos < (Number_of_Columns - 3))
							{
								ptr[Y_Pos][X_Pos] = ' ';
								if (ptr[Y_Pos][X_Pos + 2] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos + 2] = '<';
								X_Pos += 2;
							}
							else if (ptr[Y_Pos][X_Pos + 1] == ']')
							{
								if (score == 40 || !isUsed)
								{
									End_Game('R', 'W');
								}
								else
								{
									mytext.setPosition(sf::Vector2f(400, 1250));
									mytext.setString("Please finish your food...");
								}

							}
						}
						else if (status == 'S')
						{
							if (ptr[Y_Pos][X_Pos + 1] == '.' || ptr[Y_Pos][X_Pos + 1] == ' ' || ptr[Y_Pos][X_Pos + 1] == 'x')
							{
								if (ptr[Y_Pos][X_Pos + 1] == '.' || ptr[Y_Pos][X_Pos + 1] == 'x')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos][X_Pos + 1] = '<';
								X_Pos++;

							}
							else if (GameMap_3[Y_Pos][X_Pos + 1] == ']')
							{
								if (score >= 44 || !isUsed)
								{
									End_Game('R', 'W');
								}
								else
								{
									mytext.setString(" ");
								
									mytext.setPosition(sf::Vector2f(400, 1250));
									mytext.setString("Please finish your food...");
								}

							}
							else
							{
								ptr[Y_Pos][X_Pos] = '<';
							}
						}
					}
				 }
			}
			else if (Event.key.code == sf::Keyboard::Left)
			{
				if (lost == false)
				{
					Text_Clear();
					N_moves++;
					if (X_Pos >= 0)
					{
						if (status == 'N')
						{
							if (ptr[Y_Pos][X_Pos - 1] == '.' || ptr[Y_Pos][X_Pos - 1] == ' ')
							{
								if (ptr[Y_Pos][X_Pos - 1] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos][X_Pos - 1] = '>';
								X_Pos--;
							}
							else
							{
								End_Game('L');
							}
						}
						else if (status == 'H')
						{
							if (ptr[Y_Pos][X_Pos - 1] == '.' || ptr[Y_Pos][X_Pos - 1] == ' ')
							{
								if (ptr[Y_Pos][X_Pos - 1] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos][X_Pos - 1] = '>';
								X_Pos--;
							}
							else if (ptr[Y_Pos][X_Pos - 1] == 'x' && X_Pos >= 2)
							{
								ptr[Y_Pos][X_Pos] = ' ';
								if (ptr[Y_Pos][X_Pos - 2] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos - 2] = '>';
								X_Pos -= 2;
							}
						}
						else if (status == 'S')
						{
							if (ptr[Y_Pos][X_Pos - 1] == '.' || ptr[Y_Pos][X_Pos - 1] == ' ' || ptr[Y_Pos][X_Pos - 1] == 'x')
							{
								if (ptr[Y_Pos][X_Pos - 1] == '.' || ptr[Y_Pos][X_Pos - 1] == 'x')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos][X_Pos - 1] = '>';
								X_Pos--;
							}
							else
							{
								ptr[Y_Pos][X_Pos] = '>';
							}
						}
					}
				}
			}
			else if (Event.key.code == sf::Keyboard::Up)
			{
				if (lost == false)
				{
					Text_Clear();
					N_moves++;
					if (Y_Pos >= 0)
					{
						if (status == 'N')
						{
							if (ptr[Y_Pos - 1][X_Pos] == '.' || ptr[Y_Pos - 1][X_Pos] == ' ')
							{
								if (ptr[Y_Pos - 1][X_Pos] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos - 1][X_Pos] = 'v';
								Y_Pos--;
							
							}
							else
							{
								End_Game('U');
							}
						}
						else if (status == 'H')
						{
							if (ptr[Y_Pos - 1][X_Pos] == '.' || ptr[Y_Pos - 1][X_Pos] == ' ')
							{
								if (ptr[Y_Pos - 1][X_Pos] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos - 1][X_Pos] = 'v';
								Y_Pos--;

							}
							else if(ptr[Y_Pos - 1][X_Pos] == 'x' && Y_Pos >= 2)
							{
								ptr[Y_Pos][X_Pos] = ' ';
								if (ptr[Y_Pos - 2][X_Pos] == '.')
								{
									score++;
								}
								ptr[Y_Pos - 2][X_Pos] = 'v';
								Y_Pos -= 2;

							}
						}
						else if (status == 'S')
						{
							if (ptr[Y_Pos - 1][X_Pos] == '.' || ptr[Y_Pos - 1][X_Pos] == ' ' || ptr[Y_Pos - 1][X_Pos] == 'x')
							{
								if (ptr[Y_Pos - 1][X_Pos] == '.' || ptr[Y_Pos - 1][X_Pos] == 'x')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos - 1][X_Pos] = 'v';
								Y_Pos--;
							}
							else
							{
								ptr[Y_Pos][X_Pos] = 'v';
							}
						}
					}
				}
			}
			else if (Event.key.code == sf::Keyboard::Down)
			{
				Text_Clear();
				N_moves++;
				if (lost == false)
				{
					if (Y_Pos < Number_of_Rows)
					{
						if (status == 'N')
						{
							if (ptr[Y_Pos + 1][X_Pos] == '.' || ptr[Y_Pos + 1][X_Pos] == ' ')
							{
								if (ptr[Y_Pos + 1][X_Pos] == '.')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos + 1][X_Pos] = '^';
								Y_Pos++;
							}
							else if (ptr[Y_Pos + 1][X_Pos] == ']')
							{

								mytext.setPosition(sf::Vector2f(400, 1250));
								mytext.setString("Not the right opening -_- !!");
							}
							else
							{
								End_Game('D');
							}
						}
						else if (status == 'H')
						{
							if (ptr[Y_Pos + 1][X_Pos] == '.' || ptr[Y_Pos + 1][X_Pos] == ' ')
							{
								if (ptr[Y_Pos + 1][X_Pos] == '.')
								{
									score++;
								}

								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos + 1][X_Pos] = '^';
								Y_Pos++;
							}
							else if(ptr[Y_Pos + 1][X_Pos] == 'x' && Y_Pos < (Number_of_Rows - 2))
							{
								ptr[Y_Pos][X_Pos] = ' ';
								if (ptr[Y_Pos + 2][X_Pos] == '.')
								{
									score++;
								}
								ptr[Y_Pos + 2][X_Pos] = '^';
								Y_Pos += 2;
							}
						}
						else if (status == 'S')
						{
							if (ptr[Y_Pos + 1][X_Pos] == '.' || ptr[Y_Pos + 1][X_Pos] == ' ' || ptr[Y_Pos + 1][X_Pos] == 'x')
							{
								if (ptr[Y_Pos + 1][X_Pos] == '.' || ptr[Y_Pos + 1][X_Pos] == 'x')
								{
									score++;
								}
								ptr[Y_Pos][X_Pos] = ' ';
								ptr[Y_Pos + 1][X_Pos] = '^';
								Y_Pos++;
							}
							else
							{
								ptr[Y_Pos][X_Pos] = '^';
							}
						}
					}
				}
			}

			else if (Event.key.code == sf::Keyboard::D)
			{
				//minordelay();
				if (lostM == false)
				{
					if (X_PosM < Number_of_Columns - 1)
					{
						if (statusM == 'N')
						{
							if (GameMap_M[Y_PosM][X_PosM + 1] == '.' || GameMap_M[Y_PosM][X_PosM + 1] == ' ')
							{
								if (GameMap_M[Y_PosM][X_PosM + 1] == '.')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM][X_PosM + 1] = '<';
								X_PosM++;

							}
							else if (GameMap_M[Y_PosM][X_PosM + 1] == ']')
							{
								if (scoreM < 40 || !isUsedM)
								{
									GameMap_M[Y_PosM][X_PosM] = '<';
									time_takenM = clock() - time_takenM;

									mytextM.setPosition(sf::Vector2f(1650, 1150));
									mytextM.setString("Now, we have a winner ");

									mytextM2.setPosition(sf::Vector2f(1650, 1250));
									mytextM2.setString("Press 'R' to replay");


									cout << "\nScore " << scoreM;
									cout << "\nTime taken" << (float)time_takenM / CLOCKS_PER_SEC;

								}
								else
								{

									mytextM.setPosition(sf::Vector2f(1650, 1250));
									mytextM.setString("Please finish your portion...");
								}

							}
							else
							{
								lostM = true;

								GameMap_M[Y_PosM][X_PosM] = '<';
								time_takenM = clock() - time_takenM;

								mytextM.setString("Try again....");

								mytextM2.setPosition(sf::Vector2f(1650, 1250));
								mytextM2.setString("Press 'R' to replay");

								cout << "\nScore " << scoreM;
								cout << "\nTime taken" << (float)time_takenM / CLOCKS_PER_SEC;

							}
						}
						/*else if (status == 'H')
						{
						if (GameMap_M[Y_PosM][X_PosM + 1] == '.' || GameMap_M[Y_PosM][X_PosM + 1] == ' ')
						{
						if (GameMap_M[Y_PosM][X_PosM + 1] == '.')
						{
						scoreM++;
						}
						GameMap_M[Y_PosM][X_PosM] = ' ';
						GameMap_M[Y_PosM][X_PosM + 1] = '<';
						X_PosM++;

						}
						else if (GameMap_M[Y_PosM][X_PosM + 1] == 'x' && X_PosM < (Number_of_Columns - 3))
						{
						GameMap_M[Y_PosM][X_PosM] = ' ';
						if (GameMap_M[Y_PosM][X_PosM + 2] == '.')
						{
						scoreM++;
						}
						GameMap_M[Y_PosM][X_PosM + 2] = '<';
						X_PosM += 2;
						}
						else if (GameMap_M[Y_PosM][X_PosM + 1] == ']')
						{
						if (scoreM == 40 || !isUsedM)
						{
						GameMap_M[Y_PosM][X_PosM] = '<';
						time_taken = clock() - time_taken;

						mytext.setPosition(sf::Vector2f(400, 1250));
						mytext.setString("Now, we have a winner \0/");

						cout << "\nScoreM " << scoreM;
						cout << "\nTime taken" << (float)time_taken / CLOCKS_PER_SEC;

						}
						else
						{

						mytext.setPosition(sf::Vector2f(400, 1250));
						mytext.setString("Please finish your portion...");
						}

						}
						}
						else if (status == 'S')
						{
						if (GameMap_M[Y_PosM][X_PosM + 1] == '.' || GameMap_M[Y_PosM][X_PosM + 1] == ' ' || GameMap_M[Y_PosM][X_PosM + 1] == 'x')
						{
						if (GameMap_M[Y_PosM][X_PosM + 1] == '.' || GameMap_M[Y_PosM][X_PosM + 1] == 'x')
						{
						scoreM++;
						}
						GameMap_M[Y_PosM][X_PosM] = ' ';
						GameMap_M[Y_PosM][X_PosM + 1] = '<';
						X_PosM++;

						}
						else if (GameMap_M[Y_PosM][X_PosM + 1] == ']')
						{
						if (scoreM > 40 || !isUsedM)
						{
						GameMap_M[Y_PosM][X_PosM] = '<';
						time_taken = clock() - time_taken;

						mytext.setString(" ");

						mytext.setPosition(sf::Vector2f(400, 1250));
						mytext.setString("Now, we have a winner \0/");

						cout << "\nScoreM " << scoreM;
						cout << "\nTime taken" << (float)time_taken / CLOCKS_PER_SEC;


						}
						else
						{
						mytext.setString(" ");

						mytext.setPosition(sf::Vector2f(400, 1250));
						mytext.setString("Please finish your portion...");
						}

						}
						else
						{
						GameMap_M[Y_PosM][X_PosM] = '<';
						}
						}*/
					}
				}
			}
			else if (Event.key.code == sf::Keyboard::A)
			{
				//minordelay();
				if (lostM == false)
				{
					if (X_PosM >= 0)
					{
						if (statusM == 'N')
						{
							if (GameMap_M[Y_PosM][X_PosM - 1] == '.' || GameMap_M[Y_PosM][X_PosM - 1] == ' ')
							{
								if (GameMap_M[Y_PosM][X_PosM - 1] == '.')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM][X_PosM - 1] = '>';
								X_PosM--;
							}
							else
							{
								lostM = true;

								GameMap_M[Y_PosM][X_PosM] = '>';
								time_takenM = clock() - time_takenM;

								mytextM.setString("Try again....");

								cout << "\nScore " << scoreM;
								cout << "\nTime taken" << (float)time_takenM / CLOCKS_PER_SEC;


							}
						}
						/*else if (status == 'H')
						{
						if (GameMap_M[Y_PosM][X_PosM - 1] == '.' || GameMap_M[Y_PosM][X_PosM - 1] == ' ')
						{
						if (GameMap_M[Y_PosM][X_PosM - 1] == '.')
						{
						scoreM++;
						}
						GameMap_M[Y_PosM][X_PosM] = ' ';
						GameMap_M[Y_PosM][X_PosM - 1] = '>';
						X_PosM--;
						}
						else if (GameMap_M[Y_PosM][X_PosM - 1] == 'x' && X_PosM >= 2)
						{
						GameMap_M[Y_PosM][X_PosM] = ' ';
						if (GameMap_M[Y_PosM][X_PosM - 2] == '.')
						{
						scoreM++;
						}
						GameMap_M[Y_PosM][X_PosM - 2] = '>';
						X_PosM -= 2;
						}
						}
						else if (status == 'S')
						{
						if (GameMap_M[Y_PosM][X_PosM - 1] == '.' || GameMap_M[Y_PosM][X_PosM - 1] == ' ' || GameMap_M[Y_PosM][X_PosM - 1] == 'x')
						{
						if (GameMap_M[Y_PosM][X_PosM - 1] == '.' || GameMap_M[Y_PosM][X_PosM - 1] == 'x')
						{
						scoreM++;
						}
						GameMap_M[Y_PosM][X_PosM] = ' ';
						GameMap_M[Y_PosM][X_PosM - 1] = '>';
						X_PosM--;
						}
						else
						{
						GameMap_M[Y_PosM][X_PosM] = '>';
						}
						}*/
					}
				}
			}
			else if (Event.key.code == sf::Keyboard::W)
			{
				//minordelay();
				if (lostM == false)
				{
					if (Y_PosM >= 0)
					{
						if (statusM == 'N')
						{
							if (GameMap_M[Y_PosM - 1][X_PosM] == '.' || GameMap_M[Y_PosM - 1][X_PosM] == ' ')
							{
								if (GameMap_M[Y_PosM - 1][X_PosM] == '.')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM - 1][X_PosM] = 'v';
								Y_PosM--;

							}
							else
							{
								lostM = true;

								GameMap_M[Y_PosM][X_PosM] = 'v';
								time_takenM = clock() - time_takenM;

								mytextM.setString("Try again....");

								cout << "\nScore " << scoreM;
								cout << "\nTime taken" << (float)time_takenM / CLOCKS_PER_SEC;
							}
						}
						/*else if (status == 'H')
						{
							if (GameMap_M[Y_PosM - 1][X_PosM] == '.' || GameMap_M[Y_PosM - 1][X_PosM] == ' ')
							{
								if (GameMap_M[Y_PosM - 1][X_PosM] == '.')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM - 1][X_PosM] = 'v';
								Y_PosM--;

							}
							else if (GameMap_M[Y_PosM - 1][X_PosM] == 'x' && Y_PosM >= 2)
							{
								GameMap_M[Y_PosM][X_PosM] = ' ';
								if (GameMap_M[Y_PosM - 2][X_PosM] == '.')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM - 2][X_PosM] = 'v';
								Y_PosM -= 2;

							}
						}
						else if (status == 'S')
						{
							if (GameMap_M[Y_PosM - 1][X_PosM] == '.' || GameMap_M[Y_PosM - 1][X_PosM] == ' ' || GameMap_M[Y_PosM - 1][X_PosM] == 'x')
							{
								if (GameMap_M[Y_PosM - 1][X_PosM] == '.' || GameMap_M[Y_PosM - 1][X_PosM] == 'x')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM - 1][X_PosM] = 'v';
								Y_PosM--;
							}
							else
							{
								GameMap_M[Y_PosM][X_PosM] = 'v';
							}
						}*/
					}
				}
			}
			else if (Event.key.code == sf::Keyboard::S)
			{
				//minordelay();
				if (lostM == false)
				{
					if (Y_PosM < Number_of_Rows)
					{
						if (statusM == 'N')
						{
							if (GameMap_M[Y_PosM + 1][X_PosM] == '.' || GameMap_M[Y_PosM + 1][X_PosM] == ' ')
							{
								if (GameMap_M[Y_PosM + 1][X_PosM] == '.')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM + 1][X_PosM] = '^';
								Y_PosM++;
							}
							else if (GameMap_M[Y_PosM + 1][X_PosM] == ']')
							{
								mytextM.setPosition(sf::Vector2f(1650, 1250));
								mytextM.setString("Not the right opening -_- !!");
							}
							else
							{
								lostM = true;

								GameMap_M[Y_PosM][X_PosM] = '^';
								time_takenM = clock() - time_takenM;

								mytextM.setString("Try again....");

								cout << "\nScore " << scoreM;
								cout << "\nTime taken" << (float)time_takenM / CLOCKS_PER_SEC;

							}
						}
						/*else if (status == 'H')
						{
							if (GameMap_M[Y_PosM + 1][X_PosM] == '.' || GameMap_M[Y_PosM + 1][X_PosM] == ' ')
							{
								if (GameMap_M[Y_PosM + 1][X_PosM] == '.')
								{
									scoreM++;
								}

								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM + 1][X_PosM] = '^';
								Y_PosM++;
							}
							else if (GameMap_M[Y_PosM + 1][X_PosM] == 'x' && Y_PosM < (Number_of_Rows - 2))
							{
								GameMap_M[Y_PosM][X_PosM] = ' ';
								if (GameMap_M[Y_PosM + 2][X_PosM] == '.')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM + 2][X_PosM] = '^';
								Y_PosM += 2;
							}
						}
						else if (status == 'S')
						{
							if (GameMap_M[Y_PosM + 1][X_PosM] == '.' || GameMap_M[Y_PosM + 1][X_PosM] == ' ' || GameMap_M[Y_PosM + 1][X_PosM] == 'x')
							{
								if (GameMap_M[Y_PosM + 1][X_PosM] == '.' || GameMap_M[Y_PosM + 1][X_PosM] == 'x')
								{
									scoreM++;
								}
								GameMap_M[Y_PosM][X_PosM] = ' ';
								GameMap_M[Y_PosM + 1][X_PosM] = '^';
								Y_PosM++;
							}
							else
							{
								GameMap_M[Y_PosM][X_PosM] = '^';
							}
						}*/
					}
				}
			}
			
			else if (Event.key.code == sf::Keyboard::PageUp)
			{
				if (isUsed && sflag)
				{
					status = 'H';
					superpower = true;
					sflag = false;
				}
				
			}
			else if (Event.key.code == sf::Keyboard::PageDown)
			{
				if (isUsed && sflag)
				{
					status = 'S';
					superpower = true;
					sflag = false;
					
				}
				
			}
			else if (Event.key.code == sf::Keyboard::Home)
			{
				status = 'N';
			}

			else if (Event.key.code == sf::Keyboard::E)
			{
				sflag = true;
			}
			else if (Event.key.code == sf::Keyboard::R)
			{
				flag = true;
				lost = false;
				window.clear();
				mytext.setString(" ");
				mytext2.setString(" ");
				mytext3.setString(" ");
				cout << "\nR";
				
				char GameMap_R[Number_of_Rows][Number_of_Columns] = {
					{ "<...x..." },
				{ "..x...x." },
				{ "x...x..." },
				{ "..x..x.x" },
				{ ".x..x..." },
				{ "..x...x." },
				{ ".x..x..]" }
				};

				for (int r = 0; r < 7; r++)
				{
					for (int c = 0; c < 8; c++)
					{
						GameMap_3[r][c] = GameMap_R[r][c];
						GameMap_M[r][c] = GameMap_R[r][c];

					}
				}

				X_Pos = 0;
				Y_Pos = 0;
				score = 0;
				time_taken = clock();
				scounter = 0;
				N_moves = 0;
				sflag = true;

				X_PosM = 0;
				Y_PosM = 0;
				scoreM = 0;
				time_takenM = clock();

				auto map = Map('N');
				map.Set_Level();
				map.Draw_Map(window);
			}
			else if (Event.key.code == sf::Keyboard::M)
			{

				flag = true;
				lost = false;
				lostM = false;
				window.clear();
				mytext.setString(" ");
				mytext2.setString(" ");
				mytextM.setString(" ");
				mytextM2.setString(" ");
				cout << "\nR";
				for (int r = 0; r < 7; r++)
				{
					for (int c = 0; c < 8; c++)
					{
						GameMap_3[r][c] = ' ';
					}
				}
				char GameMap_R[Number_of_Rows][Number_of_Columns] = {
					{ "<...x..." },
				{ "..x...x." },
				{ "x...x..." },
				{ "..x..x.x" },
				{ ".x..x..." },
				{ "..x...x." },
				{ ".x..x..]" }
				};

				for (int r = 0; r < 7; r++)
				{
					for (int c = 0; c < 8; c++)
					{
						GameMap_3[r][c] = GameMap_R[r][c];
						GameMap_M[r][c] = GameMap_R[r][c];

					}
				}

				X_Pos = 0;
				Y_Pos = 0;
				score = 0;

				X_PosM = 0;
				Y_PosM = 0;
				scoreM = 0;

				auto map = Map('N');
				map.Set_Level('N','M');
				map.Draw_Map(window);
			}

			else if (Event.key.code == sf::Keyboard::Space)
			{
				lost = false;

				time_taken = clock();
				time_takenM = clock();
			}

			if (superpower == true && scounter <= 10)
			{
				std::stringstream sc;
				sc << 10 - scounter;
				scounter++;
				
				mytext3.setPosition(sf::Vector2f(50, 1250));
				mytext3.setString(sc.str().c_str());
			}
			if (scounter == 11)
			{
				mytext3.setString("");
				status = 'N';
			}

			break;

			/*case sf::Event::MouseMoved:
				cout << Event.mouseMove.x << "\t" << Event.mouseMove.y << "\n";

			break;*/

		default:
			break;
		}
		

	}

	int Get_Score(void)
	{
		return score;
	}

	float Get_Time(void)
	{
		return (float)time_taken / CLOCKS_PER_SEC;
	}

	void End_Game(char direction, char status = 'L')
	{

		lost = true;

		Clear_Map();

		switch (direction)
		{
		case 'R':ptr[Y_Pos][X_Pos] = '<';break;
		case 'U':ptr[Y_Pos][X_Pos] = 'v'; break;
		case 'L':ptr[Y_Pos][X_Pos] = '>'; break;
		case 'D':ptr[Y_Pos][X_Pos] = '^'; break;
		}
		
		time_taken = clock() - time_taken;

		if (status == 'L')
		{
			mytext.setPosition(sf::Vector2f(300, 550));
			mytext.setCharacterSize(120);
			mytext.setString("Game Over");
			Score_Display();
		}
		else if (status == 'W')
		{
			mytext.setPosition(sf::Vector2f(50, 550));
			mytext.setCharacterSize(120);
			mytext.setString("Now, we have a winner");
			Score_Display();

		}
	
		mytext2.setPosition(sf::Vector2f(400, 1250));
		mytext2.setString("Press 'R' to replay");

		cout << "\nScore " << score;
		cout << "\nTime taken" << (float)time_taken / CLOCKS_PER_SEC;
	}

	void Text_Clear(void)
	{
		mytext.setString(" ");
		mytext2.setString(" ");
		mytextM.setString(" ");
		mytextM2.setString(" ");
	}

	void Score_Display(void)
	{
		std::ostringstream scr;
		float Final_Score = (200 - (N_moves * 100 / score));
		float FF_Score = ((float)time_taken / CLOCKS_PER_SEC)*(Final_Score / 100);
		scr << FF_Score;
		cout << "\n" << Final_Score << endl;
		cout << FF_Score << endl;
		mytext3.setPosition(sf::Vector2f(200, 900));
		mytext3.setCharacterSize(120);
		mytext3.setString(scr.str());
	}

	void draw(sf::RenderWindow &window) override
	{
		
		auto map = Map(status);
		map.Draw_Map(window);

		window.draw(mytext);
		window.draw(mytext2);
		window.draw(mytext3);
		window.draw(mytextM);
		window.draw(mytextM2);
		
	}
private:
};
}
