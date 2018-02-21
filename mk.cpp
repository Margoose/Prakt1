#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "windows.h"
using namespace std;

char bufRus[256];
char* Rus(const char* text)
{
	AnsiToOem(text, bufRus);
	return bufRus;
}

int n=0;

struct card{char name[14]; int value; bool use; int player;};
struct result{int i; int k;};

class file
{
public: void write(struct result *newresult)
		{
			ofstream file("result.txt");
			for(int i=0; i<n; i++)
			{
				file<<"Раунд "<<newresult[i].k+1;
				if ((newresult[i].i!=0)&&(newresult[i].i!=3)) file<<".\nВыиграл игрок №"<<newresult[i].i;
				if (newresult[i].i==0) file<<".\nОба проиграли";
				if (newresult[i].i==3) file<<".\nНичья";
				file<<"\n---------------------------------------------\n";
			}
			file.close();
		}
};
class operation: public file
{
protected: 
	void read(char* str)
	{
		char ch;
		int i = 0;
		while((ch = getchar()) != '\n')
		{
			if (i<49)
			{
				str[i]=ch;
				if (ch==' ') str[i]='_';
				i++;
			}
		}
		str[i]='\0';
	}
public:
	int converter(int a, int b)
	{
		int k;
		char s[50];
		while (true)
		{
			s[0]='\0';
			read(s);
			k=atoi(s);
			if ((k>=a)&&(k<=b)) break;
			cout<<Rus("Неправильно указан номер. Диапазон [")<<a<<";"<<b<<"]\n";
		}
		return k;
	}
	virtual void creat(struct card *newcard) = 0;
	virtual void down(struct card *newcard) = 0;
	virtual void play(struct card *newcard, struct result *newresult) = 0;
	virtual void show(struct card *newcard, int p) = 0;
	virtual int control(struct card *newcard, int p) = 0;
	virtual void randz(struct card *newcard, int p) = 0;
	virtual void winner(int i1, int i2, struct result *newresult) = 0;
};
class game: public operation
{
public:
	void creat(struct card *newcard)//генерация карт
	{
		for(int i=0; i<52; i++)
		{
			newcard[i].use=false;
			newcard[i].player=0;
		}

		for(int i=0; i<4; i++)
		{
			for(int j=0; j<12; j++)
			{
				if (j<9) {
					newcard[j+i*13].value=j+2;
				}
				else newcard[j+i*13].value=10;
			}
			newcard[12+i*13].value=1;
		}

		for(int i=0;i<4;i++)
		{
			strncpy_s(newcard[i*13].name,"2 ",13);		
			strncpy_s(newcard[1+i*13].name,"3 ",13);
			strncpy_s(newcard[2+i*13].name,"4 ",13);
			strncpy_s(newcard[3+i*13].name,"5 ",13);
			strncpy_s(newcard[4+i*13].name,"6 ",13);
			strncpy_s(newcard[5+i*13].name,"7 ",13);
			strncpy_s(newcard[6+i*13].name,"8 ",13);
			strncpy_s(newcard[7+i*13].name,"9 ",13);
			strncpy_s(newcard[8+i*13].name,"10 ",13);
			strncpy_s(newcard[9+i*13].name,"Jack ",13);
			strncpy_s(newcard[10+i*13].name,"Queen ",13);
			strncpy_s(newcard[11+i*13].name,"King ",13);
			strncpy_s(newcard[12+i*13].name,"Ace ",13);
			for(int j=0;j<13; j++)
			{
				if (i==0)
					strcat_s(newcard[j+i*13].name,"Heart");
				if (i==1)
					strcat_s(newcard[j+i*13].name,"Diamond");
				if (i==2)
					strcat_s(newcard[j+i*13].name,"Club");
				if (i==3)
					strcat_s(newcard[j+i*13].name,"Spade");

			}
		}
	}
	void down(struct card *newcard)//сброс игровых данных
	{
		for(int i=0; i<52; i++)
		{
			newcard[i].use=false;
			newcard[i].player=0;
		}
	}
	void play(struct card *newcard, struct result *newresult)//сама игра
	{
		system("cls");
		system("pause");
		system("cls");

		cout<<Rus("Ход первого игрока:\n");
		bool round = true;
		int k;
		randz(newcard, 1);
		randz(newcard, 1);
		show(newcard, 1);
		cout<<Rus("Сумма карт: ")<<control(newcard, 1)<<"\n\n";
		while (round)
		{
			cout<<Rus("1. Еще карту\n2. Пасс\n");
			k=converter(1,2);
			if (k==1) {
				randz(newcard, 1);
				show(newcard, 1);
				cout<<Rus("Сумма карт: ")<<control(newcard, 1)<<"\n\n";
				if (control(newcard,1)>21) {
					round=false;
					cout<<Rus("Перебор!\n");
				}
			} else round = false;
		};

		round = true;
		system("pause");
		system("cls");
		system("pause");
		system("cls");
		cout<<Rus("Ход второго игрока: \n");
		randz(newcard, 2);
		randz(newcard, 2);
		show(newcard, 2);
		cout<<Rus("Сумма карт: ")<<control(newcard, 2)<<"\n\n";
		while (round)
		{
			cout<<Rus("1. Еще карту\n2. Пасс\n");
			k=converter(1,2);
			if (k==1) {
				randz(newcard, 2);
				show(newcard, 2);
				cout<<Rus("Сумма карт: ")<<control(newcard, 2)<<"\n\n";
				if (control(newcard, 2)>21) {
					round=false;
					cout<<Rus("Перебор!\n");
				}
			} else round = false;
		}
		system("pause");
		system("cls");
		winner(control(newcard,1),control(newcard,2), newresult);
	}
	void winner(int i1, int i2, struct result *newresult) //кто победил
	{
		if ((i1==i2)&&(i1<=21))
		{
			cout<<Rus("Ничья.\n");
			newresult[n].i=3;
			newresult[n].k=n;
		}
		else if ((i1>21)&&(i2>21))
		{
			cout<<Rus("Оба проиграли.\n");
			newresult[n].i=0;
			newresult[n].k=n;		
		}
		else if ((i1>i2)&&(i1>21)||(i1<i2)&&(i2<=21))
		{
			cout<<Rus("Победил второй игрок!\n");
			newresult[n].i=2;
			newresult[n].k=n;
		}
		else if ((i2>i1)&&(i2>21)||(i2<i1)&&(i1<=21))
		{
			cout<<Rus("Победил первый игрок!\n");
			newresult[n].i=1;
			newresult[n].k=n;
		}
		system("pause");
	}
	void randz(struct card *newcard, int p) //рандом
	{
		int i;
		while (true)
		{
			i =abs(rand()%26-rand()%52);
			if (newcard[i].use==false)
			{
				newcard[i].player = p;
				newcard[i].use = true;
				break;
			}
		}
	}
	void show(struct card *newcard, int p) //вывод карт
	{
		cout<<Rus("\nВаши карты: ");
		for(int i=0; i<52; i++)
		{
			if (newcard[i].player == p)
			{
				cout<<newcard[i].name<<"; ";
			}
		}
		cout<<"\n";
	}
	int control(struct card *newcard, int p) //подсчет очков
	{
		int k,s=0;
		k=0;
		for(int i=0; i<52; i++)
		{
			if (newcard[i].player==p)
			{
				if ((i+1)%13!=0)
					s+=newcard[i].value;
				else k+=1;
			}
		}
		if (k>0)
			if ((s+k+10)>21)
				return (s+k);
			else return (s+k+10);
			return s;
	}
};

void main()
{
	card newcard[52];
	result newresult[250];
	operation *ptr;
	ptr = new game;
	int k=0;
	ptr->creat(newcard);
	while (n<250)
	{
		cout<<Rus("Выберите: \n1.Начать игру\n2.Вывести результат в файл\n3.Выход\n");
		k=ptr->converter(1,3);
		if (k==1) {
			cout<<"\n";
			ptr->play(newcard, newresult);
			ptr->down(newcard);
			system("cls");
			n++;
		}
		if (k==2) {
			ptr->write(newresult);
			cout<<"\n";
			system("cls");
		}
		if (k==3) break;
	}
	system("cls");
	if (n==250) cout<<Rus("Сыграно максимальное число партий, перезапустите программу!\n");
	system("pause");
}