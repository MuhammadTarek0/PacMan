// PacMan_Demo.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "PacMan_Body.h"
#include <conio.h>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "ITI.h"

#define HEIGHT 1400
#define WIDTH 1200
#define Sizeofchar 45

using namespace std;

struct PressKeyReturn
{
	int KeyPressed;
	char TypeofKey;
}PressKeyR1;

struct HandleKeyReturn
{
	int VerticalFlag = 0;
	int TransitionFlag = 0;
	char TypeofFunction;
}HandleKeyR1;

struct MenuData
{
	char MainMenu[3][Sizeofchar] = { { "\n1. New Game" },{ "\n2. Leader Board" },{ "\n3. Exit" } };
}Menus;

struct Player
{
	int score;
	string Name;
	float time;

	struct Player *p_Prev;
	struct Player *p_Next;

};

struct Player *p_Head = NULL;
struct Player *p_Tail = NULL;

int z;

//Initializing the struct as a global variable to be able to access it in all scopes
//***********************************************************************************
struct Player x[100];

void GetKeyFunction(void);
void HandleKeyFunction(void);
void PrintFunction(void);
void AddData(int,float);
void PrintData(void);
void delay(void);
void Programterminate(void);
struct Player * CreatedNode(int, float);
void AddNodetoList(struct Player *);
void DisplayAll(void);
void FreeList(void);
void AddNodeSorted(struct Player *);
struct Player * SearchASC(int);
bool Start_Flag = false;





int main()
{
	
	do
	{

		PrintFunction();
		GetKeyFunction();
		HandleKeyFunction();

	} while (!Start_Flag);

	if (Start_Flag)
	{
		auto map = PM::Map();
		sf::Event event;
		PM::gamehandler Myagme;
		sf::RenderWindow window;
		sf::Texture texture;
		sf::Sprite image;

		if (!texture.loadFromFile("startup.png"))
		{
			cout << "Error while loading the picture";
		}

		window.create(sf::VideoMode(WIDTH, HEIGHT), "tarek");

		image.setPosition(0, 0);
		image.setTexture(texture);
		window.draw(image);
		window.display();

		do
		{
			while (window.pollEvent(event))
			{


				window.clear();
				Myagme.handlekeyfunction(event, window);
				Myagme.draw(window);
				AddData(Myagme.Get_Score(), Myagme.Get_Time());
				window.display();


			}
		} while (window.isOpen());
	}
	

	return 0;
}

void GetKeyFunction(void)
{
	int ch;
	ch = _getch();

	switch (ch)
	{
		//Extended Keys
	case NULL:
	case 224:
		ch = _getch();

		switch (ch)
		{
			//Home
		case 71:
			PressKeyR1.KeyPressed = 71;
			PressKeyR1.TypeofKey = 'e';
			break;

			//End
		case 79:
			PressKeyR1.KeyPressed = 79;
			PressKeyR1.TypeofKey = 'e';
			break;

			//Up
		case 72:
			PressKeyR1.KeyPressed = 72;
			PressKeyR1.TypeofKey = 'e';
			break;

			//PgUp
		case 73:
			PressKeyR1.KeyPressed = 73;
			PressKeyR1.TypeofKey = 'e';
			break;

			//Down
		case 81:
			PressKeyR1.KeyPressed = 81;
			PressKeyR1.TypeofKey = 'e';
			break;

			//PgDn
		case 80:
			PressKeyR1.KeyPressed = 80;
			PressKeyR1.TypeofKey = 'e';
			break;

			//Right
		case 77:
			PressKeyR1.KeyPressed = 77;
			PressKeyR1.TypeofKey = 'e';
			break;

			//Left
		case 75:
			PressKeyR1.KeyPressed = 75;
			PressKeyR1.TypeofKey = 'e';
			break;

		default:
			break;

		} break;

		//Enter Key
	case 13:_getch();
		PressKeyR1.KeyPressed = 13;
		PressKeyR1.TypeofKey = 'n';
		break;

		//Backspace Button
	case 8:_getch();
		PressKeyR1.KeyPressed = 8;
		PressKeyR1.TypeofKey = 'n';
		break;

		//quit ESC
	case 27:_getch();
		PressKeyR1.KeyPressed = 27;
		PressKeyR1.TypeofKey = 'n';
		break;

		//1
	case 49:_getch();
		PressKeyR1.KeyPressed = 49;
		PressKeyR1.TypeofKey = 'n';
		break;

		//2
	case 50:_getch();
		PressKeyR1.KeyPressed = 50;
		PressKeyR1.TypeofKey = 'n';
		break;

	default:
		_getch();

	}
}
void HandleKeyFunction(void)
{
	switch (PressKeyR1.TypeofKey)
	{
	case 'e':
		switch (PressKeyR1.KeyPressed)
		{
			//Home
		case 71:
			HandleKeyR1.VerticalFlag = 0;
			break;

			//End
		case 79:

			HandleKeyR1.VerticalFlag = 2;
			break;

			//Up
		case 72:
			HandleKeyR1.VerticalFlag -= 1;

			if (HandleKeyR1.VerticalFlag < 0)
			{
				HandleKeyR1.VerticalFlag += 3;	
			}

			break;

			//PgUp
		case 73:
			HandleKeyR1.VerticalFlag -= 1;

			if (HandleKeyR1.VerticalFlag < 0)
			{
				HandleKeyR1.VerticalFlag += 3;
			}

			break;

			//Down
		case 81:
			HandleKeyR1.VerticalFlag += 1;

				if (HandleKeyR1.VerticalFlag > 2)
				{
					HandleKeyR1.VerticalFlag -= 3;
				}

			break;

			//PgDn
		case 80:
			HandleKeyR1.VerticalFlag += 1;

			if (HandleKeyR1.VerticalFlag > 2)
			{
				HandleKeyR1.VerticalFlag -= 3;
			}
			break;
		default:break;
		}
		break;
	case 'n':
		switch (PressKeyR1.KeyPressed)
		{
			//Enter Key
		case 13://_getch();

			if (HandleKeyR1.VerticalFlag == 0)
			{
				Start_Flag = true;
			}

			else if (HandleKeyR1.VerticalFlag == 1)
			{
				DisplayAll();
			}

			else if (HandleKeyR1.VerticalFlag == 2)
			{
				Programterminate();
				
			}
			break;

			//Backspace Button
		case 8://_getch();
			HandleKeyR1.TransitionFlag = 0;
			break;

			//quit ESC
		case 27://_getch();
			HandleKeyR1.TransitionFlag = -1;
			HandleKeyR1.TransitionFlag = -1;
			break;

		default:break;
		}break;

	default:break;
	}

}

void PrintFunction(void)
{
	clrscr();


	for (int i = 0; i < 3; i++)
	{

		if (i == HandleKeyR1.VerticalFlag)
		{
			printHighlighted(Menus.MainMenu[i]);

			printf("\n");
		}

		else
		{
			puts(Menus.MainMenu[i]);
		}

	}
}

void AddData(int score = 0, float time = 0)
{
	struct Player *p;

	p = CreatedNode(score, time);

		AddNodeSorted(p);

}
void PrintData(void)
{
		DisplayAll();
}

void delay(void)
{
	for (int i = 0; i < 1000000000; i++);
}
void Programterminate(void)
{
	exit(0);
}

//Return the p_Head of the created node 
//**************************************
struct Player * CreatedNode(int score = 0, float time = 0)
{
	struct Player *p;

	p = (struct Player *)malloc(sizeof(struct Player));

	if (!p)
	{
		printf("Out of Memory");
		_getch();
		Programterminate();
	}	

	p->score = score;

	p->time = time;

	p->p_Prev = NULL;
	p->p_Next = NULL;

	return p;

}

void AddNodetoList(struct Player *p_Node)
{

	//if it's the first node in the tree
	//**********************************
	if (!p_Head)
	{
		p_Head = p_Node;

		p_Tail = p_Node;

		p_Node->p_Next = NULL;

		p_Node->p_Prev = NULL;

	}

	//general case node in the tree
	//**********************************
	else
	{

		p_Node->p_Prev = p_Tail;

		p_Node->p_Next = NULL;

		p_Tail->p_Next = p_Node;

		p_Tail = p_Node;
	}

}

void DisplayAll(void)
{
	struct Player *p_Display;

	p_Display = p_Head;

	while (p_Display)
	{
		cout << "Name: " << p_Display->Name;
		cout << "Score: " << p_Display->score;
		cout << "Time: " << p_Display->time;
		cout << "************************************";
		p_Display = p_Display->p_Next;
	}
}

//Return the p_Current of the node that have this code 
//****************************************************
struct Player * SearchListbyCode(int Employee_Code)
{
	struct Player *p_Current;

	p_Current = p_Head;

	while (!p_Current && p_Current->score != Employee_Code)
	{
		p_Current = p_Current->p_Next;
	}

	return p_Current;

}

void FreeList(void)
{
	struct Player *p_Free;

	p_Free = p_Head;

	while (p_Free)
	{
		p_Free = p_Head->p_Next;

		free(p_Free);
	}

	p_Head = NULL;

	p_Tail = NULL;
}

void AddNodeSorted(struct Player *p_Node)
{
	struct Player *p_AddSorted;

	p_AddSorted = SearchASC(p_Node->score);

	if (!p_AddSorted)
	{
		AddNodetoList(p_Node);
	}

	else if (p_AddSorted == p_Head)
	{
		p_Node->p_Next = p_Head;

		p_Node->p_Prev = NULL;

		p_Head->p_Prev = p_Node;

		p_Head = p_Node;
	}

	else
	{

		p_Node->p_Next = p_AddSorted;

		p_Node->p_Prev = p_AddSorted->p_Prev;

		p_AddSorted->p_Prev->p_Next = p_Node;

		p_Node->p_Next = p_AddSorted;
	}


}

struct Player * SearchASC(int Employee_Score)
{
	//Minimum code will be more than 0 beacuse no code will be zero won't validate the rules
	//***************************************************************************************

	int biggest = 0;

	struct Player *p_SearchingASC;

	p_SearchingASC = p_Head;

	if (!p_Head)
	{
		p_Head = CreatedNode();

		p_SearchingASC = p_Head;
	}
	else
	{
		while (!p_SearchingASC)
		{
			if (p_SearchingASC->score > Employee_Score)
			{
				if (p_SearchingASC->score < biggest)
				{
					biggest = p_SearchingASC->score;
				}
			}

			p_SearchingASC = p_SearchingASC->p_Next;
		}

		p_SearchingASC = SearchListbyCode(biggest);
	}

	return p_SearchingASC;
}
