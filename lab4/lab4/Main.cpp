#include <iostream>
#include <string>
using namespace std;
class Game
{
private:
	int gameFieldSizeX;
	int gameFieldSizeY;
	int** gameField;
	int variant;
	enum action
	{
		up, down, left,rigth,start
	};
	struct tree
	{
		action lastAction;
		int depth;
		int valI;
		int valJ;
		tree*up=NULL;
		tree*down=NULL;
		tree*left=NULL;
		tree*right=NULL;
	};
public:
	Game()//конструктор, создающий поле и заполняющий его значениями
	{
		cout << "Enter the size of game field" << endl;
		cout << "Enter the height of field: ";
		//проверка корректного ввода
		string str;
		bool t = true;
		while (t)
		{
			cin >> str;
			t = false;
			gameFieldSizeY = 0;
			for (int i = 0; i < str.length(); i++)
			{

				if ((str[i] >= '0') && (str[i] <= '9'))
				{
					gameFieldSizeY += (str[i] - '0') * pow(10, str.length() - i - 1);
				}
				else
				{
					cout << "it's not a number or it's irrational number" << endl;
					cout << "enter again: ";
					t = true;
					break;
				}
			}
		}
		cout << "Enter the weigth of field: ";
		t = true;
		while (t)
		{
			cin >> str;
			t = false;
			gameFieldSizeX = 0;
			for (int i = 0; i < str.length(); i++)
			{

				if ((str[i] >= '0') && (str[i] <= '9'))
				{
					gameFieldSizeX += (str[i] - '0') * pow(10, str.length() - i - 1);
				}
				else
				{
					cout << "it's not a number or it's irrational number" << endl;
					cout << "enter again: ";
					t = true;
					break;
				}
			}
		}
		//заполнение игрового поля
		cout << "Enter the game field, null position-any symbol,but not number" << endl;
		char c;
		gameField= new int*[gameFieldSizeY];
		for (int i = 0; i < gameFieldSizeY; i++)
		{
			gameField[i] = new int[gameFieldSizeX];
		}
		for (int i = 0; i < gameFieldSizeY; i++)
		{
			for (int j = 0; j < gameFieldSizeX; j++)
			{
				cin>>c;
				if ((c <= '9') && (c > '0'))
				{
					gameField[i][j] = c - '0';
				}
				else
				{
					gameField[i][j] = 0;
				}
			}
		}
	}
	~Game()//очищение памяти выделенной под игровое поле
	{
		for (int i = 0; i < gameFieldSizeY; i++)
		{
			delete[] gameField[i];
		}
		delete[] gameField;
	}
	void show()//вывод игрового поля на экран
	{
		for (int i = 0; i < gameFieldSizeY; i++)
		{
			for (int j = 0; j < gameFieldSizeX; j++)
			{
				if (gameField[i][j] > 0)
				{
					cout<<gameField[i][j];
				}
				else if(gameField[i][j] == -2)
				{
					cout<< '*';
				}
				else
				{
					cout << ' ';
				}
			}
			cout << endl;
		}
	}
	void complete()//функция рисующая пути для каждого ненулевого элемента
	{
		for(int r=1;r<9;r++)
		{ 
			for (int i = 0; i < gameFieldSizeY; i++)
			{
				for (int j = 0; j < gameFieldSizeX; j++)
				{
					if (gameField[i][j] ==  r)
					{
							if (gameField[i][j] == 1)
							{
								gameField[i][j] = -2;
							}
							else
							{
								tree*root = new tree;
								root->valI = i;
								root->valJ = j;
								root->depth = 1;
								root->lastAction = start;
								variant = 0;
								winNumber(root, root,"test");//означает, что будет построено дерево вариантов для нахождения количества "выигрывающих"
								free(root);
								if (variant == 1)
								{
									tree*root = new tree;
									root->valI = i;
									root->valJ = j;
									root->depth = 1;
									root->lastAction = start;
									winNumber(root, root, "game");//означает,что если выигрывает 1 вариант ,то будет построен путь для данной цифры
									free(root);
								}
								
							}
					
					}

				}
			}
		}
		while (1)//если после выполнения предыдущего цикла остались ненулевые положительные цифры на поле,выполнять построение путей для этих цифр,пока таковых на поле не останется
		{
			bool z = true;
			
			for (int i = 0; i < gameFieldSizeY; i++)
			{
				for (int j = 0; j < gameFieldSizeX; j++)
				{
					if (gameField[i][j] > 0)
					{
						z = false;
						tree*root = new tree;
						root->valI = i;
						root->valJ = j;
						root->depth = 1;
						root->lastAction = start;
						variant = 0;
						winNumber(root, root, "test");
						free(root);
						if (variant == 1)
						{
							tree*root = new tree;
							root->valI = i;
							root->valJ = j;
							root->depth = 1;
							root->lastAction = start;
							winNumber(root, root, "game");
							free(root);
						}
					}
				}
			}
			if (z)
			{
				break;
			}
		}		
	}
	bool winNumber(tree*node,tree*root,string str,bool win = false)//функция подсчета вариантов и построения путей для цифры
	{		
		if ((node->lastAction != start)&&(node->valI == root->valI)&&(node->valJ == root->valJ))
		{
			return false;
		}
		else
		{
			if (node->depth != gameField[root->valI][root->valJ])
			{
				if (node->lastAction != start)
				{
					if ((gameField[node->valI][node->valJ] != 0) or (gameField[node->valI][node->valJ] == -2))
					{
						return false;
					}
				}
				if ((node->valI - 1 >= 0) && (node->lastAction != up))//запускает ход "вверх"
				{
					node->up = new tree;
					node->up->depth = node->depth + 1;
					node->up->valI = node->valI - 1;
					node->up->valJ = node->valJ;
					node->up->lastAction = down;
					win = winNumber(node->up,root,str);
					if (win)//в случае если ветка выйгрышная помечать поля
					{
						if (str == "game")
						{
							gameField[node->valI][node->valJ] = -2;
							return win;
						}
					}
					//up
				}
				if ((node->valJ + 1 < gameFieldSizeX) && (node->lastAction != rigth))//запускает ход "вправо"
				{
					node->right = new tree;
					node->right->depth = node->depth + 1;
					node->right->valI = node->valI;
					node->right->valJ = node->valJ + 1;
					node->right->lastAction = left;
					win = winNumber(node->right,root,str);
					if (win)
					{
						if (str == "game")
						{
							gameField[node->valI][node->valJ] = -2;
							return win;
						}
					}
					//rigth
				}
				if ((node->valI + 1 < gameFieldSizeY) && (node->lastAction != down))//запускает ход "вниз"
				{
					node->down = new tree;
					node->down->depth = node->depth + 1;
					node->down->valI = node->valI + 1;
					node->down->valJ = node->valJ;
					node->down->lastAction = up;
					win = winNumber(node->down,root,str);
					if (win)
					{
						if (str == "game")
						{
							gameField[node->valI][node->valJ] = -2;
							return win;
						}
					}
					//down
				}
				if ((node->valJ - 1 >= 0) && (node->lastAction != left))//запускает ход "влево"
				{
					node->left = new tree;
					node->left->depth = node->depth + 1;
					node->left->valI = node->valI;
					node->left->valJ = node->valJ - 1;
					node->left->lastAction = rigth;
					win = winNumber(node->left,root,str);
					if (win)
					{
						if (str == "game")
						{
							gameField[node->valI][node->valJ] = -2;
							return win;
						}
					}
					//left
				}
				return win;
			}
			else //достигнута глубина
			{
				if (gameField[node->valI][node->valJ] == gameField[root->valI][root->valJ])
				{
					if(str == "game")
					gameField[node->valI][node->valJ] = -2;
					else
						variant++;
					win = true;					
				}
				return win;
			}
		}
		return win;
	}
	bool free(tree *head, bool del = false)//функция для очистки дерева используется в деструкторе
	{		
			if (head->up != NULL)
			{
				del=free(head->up);
				if (del)
				{
					delete head->up;
				}					
			}
			if (head->down != NULL)
			{
				del=free(head->down);
				if (del)
				{
					delete head->down;
				}
			}
			if (head->left != NULL)
			{
				del=free(head->left);
				if (del)
				{
					delete head->left;
				}
			}
			if (head->right != NULL)
			{
				del=free(head->right);
				if (del)
				{
					delete head->right;
				}
			}
			return true;
	}
};
int main()
{
	system("color F0");
	Game game;
	game.show();
	game.complete();
	game.show();
	system("pause");
	return 0;
	
}