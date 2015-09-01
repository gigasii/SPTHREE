#include "HighscoreManager.h"

CHighscoreManager::CHighscoreManager(void):CurrentSize(0)
										  ,MAX_SIZE(11)
										  ,name("     ")
										  ,value(-1)
{
	for (int i = 0; i < MAX_SIZE; i++)
	{
		this->highscore_Menu[i] = CHighscore::CHighscore();
		//cout << i << endl;
	}

	this->Player.SetName(name);
	this->Player.SetValue(value);
}

int CHighscoreManager::ReadFromFile(string Filename)
{
	ifstream File(Filename);
	int		 i=0; 
	string line;
	if (File.is_open())
	{
		while(!File.eof())
		{	
			string tempName;
			int tempScore;
			File >> tempName;
			File >> tempScore;

			this->highscore_Menu[i].SetName(tempName);
			this->highscore_Menu[i].SetValue(tempScore);

			i++;
			CurrentSize++;
		}
		
	}
	//cout << CurrentSize << endl;
	return i ;

}
	
int CHighscoreManager::WriteToFile(string Filename)
{
	ofstream File(Filename);
	int		 i=0; 

	if (File.is_open())
	{
		while(i < MAX_SIZE - 1)
		{
			File << this->highscore_Menu[i].GetName() << "     " << this->highscore_Menu[i].GetValue(); 
			if(i < MAX_SIZE - 2)
			{
				File << endl;
			}
			i++; 
			CurrentSize--;
		}
	}
	return i;
}

void CHighscoreManager::Add(CHighscore Highscore)
{
	
	if(CurrentSize < MAX_SIZE)
	{
		this->highscore_Menu[CurrentSize] = Highscore;
		cout << this->Player.GetName() << endl;
		CurrentSize++;
		MergeSort(this->highscore_Menu, 0, CurrentSize - 1);
	}
	else
	{
		this->highscore_Menu[MAX_SIZE - 1] = Highscore;
		MergeSort(this->highscore_Menu, 0, CurrentSize - 1);
	}
	
}

void CHighscoreManager::UpdateHighscore(CHighscore PlayerScore)
{
	if(	this->Player.GetValue() != PlayerScore.GetValue())
	{
		this->Player.SetValue(PlayerScore.GetValue());
		Add(this->Player);
		MergeSort(this->highscore_Menu, 0, MAX_SIZE - 1);
	}
	/*if(MAX_SIZE )	
	if(this->Player.GetValue() == )*/

	

	/*if(CurrentSize < MAX_SIZE)
	{
		if(!CheckIfInHighscore(PlayerScore))
		{
			Add(PlayerScore);
			cout << "Added" << endl;
		}
	}
	else
	{
		if(PlayerScore.GetValue() > this->highscore_Menu[MAX_SIZE-1].GetValue())
		{
			if(!CheckIfInHighscore(PlayerScore))
			{
				Add(PlayerScore);
			}
			else
			{
				MergeSort(this->highscore_Menu, 0, MAX_SIZE - 1);
			}
		}
	}*/
}

void CHighscoreManager::MergeSort(CHighscore highscore[], int first, int last)
{
	if(first < last) 
	{  
		int middle = (first + last)/2;  
		MergeSort(highscore, first, middle);  
		MergeSort(highscore, middle+1, last); 
		Merge(highscore, first, middle, last); 
	}
}

void CHighscoreManager::Merge(CHighscore highscore[], int first, int middle, int last)
{
	//temp array to hold 1st list
	CHighscore *temp = new CHighscore[middle-first+1];   
	//i is index for temp array, 
	//j is index for 2nd list, 
	//k is index for combine list
	int i, j, k;  
	for(j = first, i = 0; j <= middle; i++, j++)
	{ 
		temp[i] = highscore[j]; //duplicate 1st list
	}
	i = 0; k = first; 
	while (k < j && j <= last) 
	{ 
		//if element from 1st list > 2nd list 
		if (temp[i].GetValue() >= highscore[j].GetValue()) 
		{
			highscore[k++] = temp[i++]; //copy from 1st list 
		}
		else 
		{
			highscore[k++] = highscore[j++]; //copy from 2nd list 
		}
	}
	while (k < j) //copy remaining elements in temp, if any 
	{ 
		highscore[k++] = temp[i++];  
	}
	delete [] temp; //remove temp array 
}

void CHighscoreManager::PrintAllHighscore()
{
	for(int a = 0; a < CurrentSize; a++)
	{
		this->highscore_Menu[a].PrintAll();
	}
}

CHighscore CHighscoreManager::GetAllHighscores(int pos)
{
	return this->highscore_Menu[pos];
}

int CHighscoreManager::GetCurrentSize()
{
	return this->CurrentSize;
}

void CHighscoreManager::SetPlayer(CHighscore Highscore)
{
	this->Player = Highscore;
}

CHighscore CHighscoreManager::GetPlayer()
{
	return this->Player;
}

CHighscoreManager::~CHighscoreManager(void)
{
}
