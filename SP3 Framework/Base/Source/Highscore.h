#pragma once
#include<iostream>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class CHighscore
{
private:
	int Highscore_value;
	string Highscore_name;

public:
	

	CHighscore(void);
	~CHighscore(void);

	void SetValue(int value);
	int GetValue(void);
	void SetName(string name);
	string GetName(void);

	void PrintAll();
};

