#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace sf;
using namespace std;
Sprite beads[5][5];
int beadPosition[5][5];
void writeFile(int turn,int victory,int beadPosition[5][5])
{
	ofstream outputfile ("game.txt");
	outputfile<<turn<<endl;
	outputfile<<victory<<endl;
	for(int i = 0;i<5;i++){
		for(int j= 0; j<5 ;j++){
			outputfile<<beadPosition[i][j]<<endl;
		}
	}
	outputfile.close();
}void readFile(int &turn,int &victory,int beadPosition[5][5])
{
	ifstream inputfile ("game.txt");
	inputfile>>turn;
	inputfile>>victory;
	for(int i = 0;i<5;i++){
		for(int j= 0; j<5 ;j++){
			inputfile>>beadPosition[i][j];
		}
	}
	inputfile.close();
}
Vector2i calculateClickPosition(Event::MouseButtonEvent mousebutton)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (beads[i][j].getGlobalBounds().contains(mousebutton.x, mousebutton.y))
			{
				return {i, j};
			}
		}
	}
	return {-1, -1};
}
void swapBeads(Vector2i firstChoice, Vector2i secondChoice, int &turn)
{
	beadPosition[secondChoice.x][secondChoice.y] = beadPosition[firstChoice.x][firstChoice.y];
	beadPosition[firstChoice.x][firstChoice.y] = 0;
	turn = turn * -1;
}
bool checkValidMove(Vector2i firstChoice, Vector2i secondChoice, int turn)
{
	if (beadPosition[firstChoice.x][firstChoice.y] != turn)
		return 0;

	if (beadPosition[secondChoice.x][secondChoice.y] != 0)
		return 0;

	if (!(
			((firstChoice.x*5 + firstChoice.y) %2 == 0) && 
			(
				(secondChoice.x - firstChoice.x) == (secondChoice.y - firstChoice.y) || 
				(secondChoice.x - firstChoice.x) == -(secondChoice.y - firstChoice.y)
			)  
			|| (secondChoice.x - firstChoice.x) == 0 
			|| (secondChoice.y - firstChoice.y) == 0 
		)
		)	
		return 0;

	if (secondChoice.x - firstChoice.x > 2 || secondChoice.x - firstChoice.x < -2)
		return 0;
	
	if (secondChoice.y - firstChoice.y > 2 || secondChoice.y - firstChoice.y < -2)
		return 0;	

	return 1;
}
bool isLongMove(Vector2i firstChoice, Vector2i secondChoice, int turn)
{
	if (((secondChoice.x - firstChoice.x) == 2 || (secondChoice.x - firstChoice.x) == -2) || ((secondChoice.y - firstChoice.y) == 2 || (secondChoice.y - firstChoice.y) == -2)){
		return 1;
	}
	return 0;
}
void doMove(Vector2i firstChoice, Vector2i secondChoice, int &turn)
{
	if (checkValidMove(firstChoice, secondChoice, turn)==1)
	{
		if (isLongMove(firstChoice, secondChoice, turn)==1)
		{
			if (beadPosition[firstChoice.x + (secondChoice.x - firstChoice.x) / 2][firstChoice.y + (secondChoice.y - firstChoice.y) / 2] == turn * -1)
			{
				beadPosition[firstChoice.x + (secondChoice.x - firstChoice.x) / 2][firstChoice.y + (secondChoice.y - firstChoice.y) / 2] = 0;
				swapBeads(firstChoice, secondChoice, turn);
			}
		}
		else
		{
			swapBeads(firstChoice, secondChoice, turn);
		}
	}
}
int main()
{
	bool isInMainMenu = 1;
	RenderWindow gameWindow(VideoMode(510, 510), "12 Beads Game-M&W Gaming Arena", Style::Close);
	Texture background;
	background.loadFromFile("bg.png");
	Sprite backgroundSprite(background);  

	Texture bluePopup;
	bluePopup.loadFromFile("blueWins.png");
	Sprite bluePopupSprite(bluePopup);

	Texture redPopup;
	redPopup.loadFromFile("redWins.png");
	Sprite redPopupSprite(redPopup);

	Texture redGemTexture;
	redGemTexture.loadFromFile("redgem.png");
	Texture blueGemTexture;
	blueGemTexture.loadFromFile("bluegem.png");
	Texture noGemTexture;
	noGemTexture.loadFromFile("521070.png");

	Vector2i firstChoice = {-1, -1};
	Vector2i secondChoice = {-1, -1};

	int turn = 1;
	int redCount = 0;
	int blueCount = 0;
	int victory = 0;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (i < 2 || (i == 2 && j < 2))
			{
				beadPosition[i][j] = 1;
			}
			else if (i != 2 || j != 2)
			{
				beadPosition[i][j] = -1;
			}
			else
			{
				beadPosition[i][j] = 0;
			}
			beads[i][j].setScale({60 / 300.f, 60 / 300.f});
			beads[i][j].setPosition({j * 113.f, i * 113.f});
		}
	}

	while (gameWindow.isOpen())
	{
		Event event;
		while (gameWindow.pollEvent(event))
		{
			if (event.type == event.Closed)
			{
				writeFile(turn,victory,beadPosition);
				gameWindow.close();
			}
			if (event.type == event.MouseButtonPressed && !isInMainMenu)
			{
				firstChoice = secondChoice;
				secondChoice = calculateClickPosition(event.mouseButton);

				if (secondChoice.x != -1 && firstChoice.x != -1 && secondChoice != firstChoice)
				{
					doMove(firstChoice, secondChoice, turn); 
				}
				if (secondChoice.x != -1 && beadPosition[secondChoice.x][secondChoice.y] == 0)
				{
					secondChoice = {-1, -1};
				}
			}
		}
		gameWindow.clear();
		
		if (!isInMainMenu)
		{	
			gameWindow.draw(backgroundSprite);
			if (victory == 0)
			{
				redCount = 0;
				blueCount = 0;
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 5; j++)
					{
						if (beadPosition[i][j] == 1)
							redCount++;
						if (beadPosition[i][j] == -1)
							blueCount++;

						if (beadPosition[i][j] == 0)
							beads[i][j].setTexture(noGemTexture);
						else if (beadPosition[i][j] == 1)	
							beads[i][j].setTexture(redGemTexture);
						else 
							beads[i][j].setTexture(blueGemTexture);
						gameWindow.draw(beads[i][j]);
					}
				}
				if (redCount == 0 || blueCount == 0)
				{
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 5; j++)
						{
							beadPosition[i][j] == 0;
						}
					}
					if (redCount == 0) victory = -1;
					else victory = 1;
				}
			}

			if (victory == 1) {
				gameWindow.draw(redPopupSprite);
			} else if (victory == -1) {
				gameWindow.draw(bluePopupSprite);
			}
		}
		else{
			Texture menu;
			menu.loadFromFile("gamestart.png");
			Sprite menuSprite(menu);
			gameWindow.draw(menuSprite);
			if (Mouse::isButtonPressed(Mouse::Button::Left)){
				//cout<<Mouse::getPosition(gameWindow).x<<" , "<<Mouse::getPosition(gameWindow).y<<endl;
				if ((Mouse::getPosition(gameWindow).x>=158)&&(Mouse::getPosition(gameWindow).x<=345)&&(Mouse::getPosition(gameWindow).y<=403)&&(Mouse::getPosition(gameWindow).y>=365)){
					gameWindow.close();
				}
				if ((Mouse::getPosition(gameWindow).x>=140)&&(Mouse::getPosition(gameWindow).x<=361)&&(Mouse::getPosition(gameWindow).y<=293)&&(Mouse::getPosition(gameWindow).y>=247)){
					readFile(turn,victory,beadPosition);
					isInMainMenu = 0;
				}else isInMainMenu = 0;
			}
		}
		gameWindow.display();
	}
	return 0;
}