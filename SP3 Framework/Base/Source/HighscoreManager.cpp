#include "HighscoreManager.h"


CHighscoreManager::CHighscoreManager(void):CurrentSize(0)
										  ,MAX_SIZE(10)
										  ,name("     ")
										  ,value(0)
{
	for (int i = 0; i <10; i++)
	{
		this->highscore_Menu[i] = CHighscore::CHighscore();
		cout << i << endl;
	}
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
			File >> this->highscore_Menu[i].Highscore_name;
			File >> this->highscore_Menu[i].Highscore_value;
			i++;
			CurrentSize++;
		}
		
	}
	cout << CurrentSize << endl;
	return i ;

}
	
int CHighscoreManager::WriteToFile(string Filename)
{
	ofstream File(Filename);
	int		 i=0; 

	if (File.is_open())
	{
		while(CurrentSize > 0)
		{
			File << this->highscore_Menu[i].Highscore_name << "     " << this->highscore_Menu[i].Highscore_value; 
			if(CurrentSize - 1 != 0)
			{
				File << endl;
			}
			i++; 
			CurrentSize--;
		}
	}
	return i;
}

bool CHighscoreManager::CheckIfInHighscore(CHighscore PlayerScore)
{
	for(int a = 0; a < CurrentSize; a++)
	{
		if(this->highscore_Menu[a].Highscore_name == PlayerScore.Highscore_name && this->highscore_Menu[a].Highscore_value == PlayerScore.Highscore_value)
		{
			return true;
		}
	}

	return false;
}

void CHighscoreManager::Add(CHighscore Highscore)
{
	bool inhighscore = false;
	for(int a = 0;a < CurrentSize; a++)
	{
		if(this->highscore_Menu[a].Highscore_name == Highscore.Highscore_name) 
		{
			inhighscore = true;
			if(this->highscore_Menu[a].Highscore_value == Highscore.Highscore_value)
			{
				return;
			}
			else
			{
				this->highscore_Menu[a].Highscore_value = Highscore.Highscore_value;
				MergeSort(this->highscore_Menu, 0, CurrentSize - 1);
				break;
			}
		}
		
	}
	if(inhighscore == false)
	{
		this->highscore_Menu[CurrentSize] = Highscore;
		CurrentSize++;
		MergeSort(this->highscore_Menu, 0, CurrentSize - 1);
	}
	
}

void CHighscoreManager::UpdateHighscore(CHighscore PlayerScore)
{
	if(CurrentSize < MAX_SIZE)
	{
		if(!CheckIfInHighscore(PlayerScore))
		{
			Add(PlayerScore);
			cout << 'q' << endl;
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
	}
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
		//if element from 1st list < 2nd list 
		if (temp[i].Highscore_value >= highscore[j].Highscore_value) 
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

CHighscoreManager::~CHighscoreManager(void)
{
}
