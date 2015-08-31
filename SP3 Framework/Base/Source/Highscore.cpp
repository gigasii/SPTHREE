#include "Highscore.h"


CHighscore::CHighscore(void)
{
	this->Highscore_name = "NULL";
	this->Highscore_value = 0;
}

void CHighscore::SetValue(int value)
{
	this->Highscore_value = value;
}
	
int CHighscore::GetValue(void)
{
	return this->Highscore_value;
}
	
void CHighscore::SetName(string name)
{
	this->Highscore_name = name;
}
	
string CHighscore::GetName(void)
{
	return this->Highscore_name;
}

void CHighscore::PrintAll()
{
	cout << this->Highscore_name << "          " << this->Highscore_value << endl;
}


CHighscore::~CHighscore(void)
{
}
