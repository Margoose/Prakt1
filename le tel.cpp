#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "windows.h"

using namespace std;
struct data{char name[33]; char number[16];};

char bufRus[256];
char* Rus(const char* text)
{
	AnsiToOem(text, bufRus);
	return bufRus;
}

class correct
{
public:
	int diap(int a, int b)
	{
		int k;
		char s[50];
		while (true)
		{
			number(s);
			k=atoi(s);
			if ((k>=a)&&(k<=b)) break;
			cout<<Rus("Введите число в диапозоне [")<<a<<";"<<b<<"]\n";
		}
		return k;
	}
protected:
	void read(char* str)
	{
		char ch;
		int i = 0;
		str[0]='\0';
		while((ch = getchar()) != '\n')
		{
			if (i<49)
			{
				str[i]=ch;
				i++;
			}
			if (ch==' ') str[i]='_';
		}
		str[i]='\0';
	}
	void name(char* s)
	{
		int i;
		bool flag = true;
		while (flag)
		{
			read(s);
			i=strlen(s);
			if (i<33)
			{
				int j = 0;
				while (true)
				{
					if ((int)(s[j])>31) 
					{
						cout<<Rus("Допустимые символы: а-я, А-Я!\n");
						break;
					}
					j++;
					if (i==j)
					{
						flag = false;
						break;
					}
				}
			}
			else cout<<Rus("Максимальное число символов - 32!\n");
		}
		toRus(s);
	}
	void number(char* s)
	{
		int i;
		bool flag = true;
		while (flag)
		{
			read(s);
			i=strlen(s);
			if (i<16)
			{
				int j = 0;
				while (true)
				{
					if ((int)(s[j])<=47 || (int)(s[j])>=59)
					{
						cout<<Rus("Допустимые символы: 0-9!\n");
						break;
					}
					j++;
					if (i==j)
					{
						flag = false;
						break;
					}
				}
			}
			else cout<<Rus("Максимальное число символов - 15!\n");
		}
	}
	void correctName(char *s)
	{
		int j = 0;
		char str[50];
		int k = strlen(s);
		if (strlen(s)>50) k = 50;
		for(int i = 0; i<k; i++)
		{
			if ((int)(s[i])>-65&&(int)(s[i])<0)
			{
				str[j] = s[i];
				j++;
			}
		}
		if (j>32) j = 32;
		for(int i = 0; i<j; i++)
		{
			s[i] = str[i];
		}
		if (j>0) s[j]='\0'; else s[j]='\0';
	}
	void correctNumber(char *s)
	{
		int j = 0;
		char str[50];
		int k = strlen(s);
		if (strlen(s)>50) k = 50;
		for(int i = 0; i<k; i++)
		{
			if ((int)(s[i])>47 && (int)(s[i])<58) 
			{
				str[j] = s[i];
				j++;
			}
		}
		if (j>32) j = 32;
		for(int i = 0; i<j; i++)
		{
			s[i] = str[i];
		}
		if (j>0) s[j]='\0'; else s[j]='\0';
	}
	void toRus(char* s)
	{
		int j;
		int k = strlen(s);
		for(int i=0;i<k;i++)
		{
			j=abs((int)(s[i]));
			convert(j);
			s[i]=(char)(j);
		}
	}
	void convert(int &j)
	{
		int i=0;
		if ((j>=81)&&(j<=128))
		{
			j=320-j;
		}
		else if ((j>=17)&&(j<=32))
		{
			j=272-j;
		}
		else if (j==16)
		{
			j=197;
		}
		else j=229;	
	}
};
class file: public correct
{
protected:
	void dbcor(ifstream &file, char* s)
	{
		int i=0;
		char ch;
		while (((ch = file.get()) != ' ')&&!file.eof())
		{
			if (ch=='\n') break;
			if (i<49)
			{
				s[i]=ch;
				i++;
			}
		}
		s[i]='\0';
	}
public:
	void open(struct data *base)
	{
		int n = 0;
		ifstream file("1.txt");
		if (!file.is_open())
		{
			cout<<Rus("БАЗА ДАННЫХ ОТСУТСТВУЕТ!\n");
			for(int i=0; i<255; i++)
			{
				base[i].name[0]='\0';
				base[i].number[0]='\0';				
			}
		}
		else {
			char s[50];
			while (!file.eof() && n<255)
			{
				dbcor(file, s);
				correctName(s);
				strncpy_s(base[n].name, s, 32);
				dbcor(file, s);
				correctNumber(s);
				strncpy_s(base[n].number, s, 15);
				n++;
			}
			for(int i=n; i<255; i++)
			{
				base[i].name[0]='\0';
				base[i].number[0]='\0';
			}
			file.close();
		}
	}
	void close(struct data *base)
	{
		{
			ofstream file("1.txt");
			for(int i=0; i<255; i++)
			{
				if (i!=0) file<<endl;
				if (strcmp(base[i].name,"\0")==0) strcpy_s(base[i].name,"NoName");
				if (strcmp(base[i].number,"\0")==0) strcpy_s(base[i].number, "NoNumber");
				file<<base[i].name<<" "<<base[i].number;
			}
			file.close();
		}
	}
};
class operation: public file
{
public:
	void review(struct data *base)
	{
		int k;
		cout<<Rus("\n1. Вывод данных (1 - весь список, 2 - опр. номер): ");
		k=diap(1,2);
		if (k==1)
		{
			cout<<endl;
			cout.width(3);
			cout<<""<<"|";
			cout.width(32);
			cout.fill('.');
			cout<<left<<Rus("Имя")<<" | ";
			cout.width(15);
			cout<<Rus("Номер")<<"\n";
			cout.fill('-');
			cout.width(54);
			cout<<""<<endl;
			for(int i=0; i<255; i++)
			{
				cout.fill('.');
				cout.width(3);
				cout<<i+1<<"|";
				cout.width(32);
				cout<<Rus(base[i].name)<<" | ";
				cout.width(15);
				cout<<Rus(base[i].number)<<endl;
			}
			cout.fill('-');
			cout.width(54);
			cout<<""<<"\n\n";
		} else {
			cout<<Rus("Введите номер выводимой строки: ");
			k=diap(1,255);
			cout<<endl;
			cout.width(32);
			cout.fill('.');
			cout<<left<<Rus("Имя")<<" | ";
			cout.width(15);
			cout<<Rus("Номер")<<"\n";
			cout.fill('-');
			cout.width(50);
			cout<<""<<endl;
			cout.fill('.');
			cout.width(32);
			cout<<left<<Rus(base[k-1].name)<<" | ";
			cout.width(15);
			cout<<Rus(base[k-1].number)<<"\n\n";
		}
	}
	void edit(struct data *base)
	{
		cout<<Rus("\n2. Ввдите номер изменяемой строки: ");
		int k=diap(1,255);
		char s[50];
		cout<<Rus("Имя = ");
		name(s);
		strncpy_s(base[k-1].name,s,32);
		s[0]='\0';
		cout<<Rus("Номер = ");
		number(s);
		strncpy_s(base[k-1].number,s,15);
		cout<<endl;
	}
	void search(struct data *base)
	{
		cout<<Rus("\n3. Введите тип для поиска (1 - по имени, 2 - по номеру): ");
		int k,j=0;
		k=diap(1,2);
		if (k==2)
		{
			char str[50];
			cout<<Rus("Введите номер: ");
			number(str);
			for(int i=0; i<255;i++)
			{
				if (strcmp(str,base[i].number) == 0) 
				{
					cout.fill('.');
					cout<<left<<i+1<<")";
					cout.width(32);
					cout<<Rus(base[i].name)<<" | ";
					cout.width(15);
					cout<<Rus(base[i].number)<<"\n";
					j=1;
				}
			}
			cout<<endl;
		} else {
			cout<<Rus("Введите имя: ");
			char str[50];
			name(str);
			for(int i=0; i<255;i++)
			{
				if (strcmp(str,base[i].name) == 0)
				{
					cout.fill('.');
					cout<<left<<i+1<<")";
					cout.width(32);
					cout<<Rus(base[i].name)<<" | ";
					cout.width(15);
					cout<<Rus(base[i].number)<<"\n";
					j=1;
				}
			}
			cout<<endl;
		}
		if (j==0) cout<<Rus("Запись не найдена!\n");
	}
	void del(struct data *base)
	{
		cout<<Rus("\n4.Введите номер удалемой строки: ");
		int k = diap(1,255);
		base[k-1].name[0]='\0';
		base[k-1].number[0]='\0';
		cout<<endl;
	}
};
class program: public operation
{

};

void main()
{
	data base[255];
	int k;
	program pr;
	pr.open(base);
	while (true)
	{
		cout<<Rus("Выберите режим работы программмы:\n•1 - просмотр базы данных;\n•2 - редактирование базы данных;\n•3 - поиск по базе данных;\n•4 - удаление записи;\n•5 - выход;\nРежим: ");
		k = pr.diap(1,5);
		if (k==1)
		{
			pr.review(base);
		}
		else if (k==2)
		{
			pr.edit(base);
		}
		else if (k==3)
		{
			pr.search(base);
		}
		else if (k==4)
		{
			pr.del(base);
		}
		else if (k==5)
		{
			break;
		}
	}
	pr.close(base);
	system("pause");
}