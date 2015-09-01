#pragma once
#include<vector>
#include<fstream>
#include "Highscore.h"

using std::vector;
using std::ifstream;
using std::ofstream;

class CHighscoreManager
{
private:
	CHighscore highscore_Menu[11];
	int MAX_SIZE;
	int CurrentSize;
	string name;
	int value;
	CHighscore Player;
	

public:
	CHighscoreManager(void);
	~CHighscoreManager(void);

	int ReadFromFile(string Filename);
	int WriteToFile(string Filename);

	void UpdateHighscore(CHighscore PlayerScore);

	
	void Add(CHighscore Highscore);
	void MergeSort(CHighscore highscore[], int first, int last);
	void Merge(CHighscore highscore[], int first, int middle, int last);

	void PrintAllHighscore();

	CHighscore GetAllHighscores(int pos);

	int GetCurrentSize();

	void SetPlayer(CHighscore Highscore);
	CHighscore GetPlayer();

};

