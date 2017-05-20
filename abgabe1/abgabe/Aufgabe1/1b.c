#include <iostream>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fstream>

#define NUMBER_OF_RUNS 1000000

using namespace std;



int panini()																// Funktion zum Zählen der Anzahl der benötigten Päckchen um Album zu füllen
{
	vector<int> panini(535,0);
	vector<int> packs(5);
	int p=0;																//Anzahl der Päckchen
	for (int i=0; i<535;)	
	{			
		for (int i=0; i<5; i++)												// Päckchen zufällig erstellen
		{
			int k;
			k=rand() % 1049+1; 												// doppelte Wahrscheinlichkeit für 514 Karten (21 Deutsche)
			if (k < 536)													// Füllen der iten Stelle des Päckchens
			{
				packs[i]=k;
			}
			else
			{
				k=k-514;
				packs[i]=k;
			}
		}
		
		
		for (int k=0; k<5; k++)
		{
			int n;
			n=packs[k];					
			if (panini[n-1]==0)												// Wenn Stelle im Album des Stickers aus dem Päckchen 0, Sticker einkleben -> 1
			{
				panini[n-1]=1;
				i=i+1;														// Kontrollzähler um 1 erhöhen bis 535 Karten eingeklebt sind
			}		
		}
		p=p+1;
	}
	return p;
}

vector<int> histo()															// Funktion zur Erstellung des Histogramms
{
	
	vector<int> data(NUMBER_OF_RUNS);
	for (int i=0; i<NUMBER_OF_RUNS; i++)									// 10^6 fache Wiederholung
	{
		data[i]=panini();													// In Data an die Stelle i jeweils die Anzahl der benötigten Päckchen p 
		cout << "Wiederholung " << i << ":" << data[i] << "Päckchen" << endl;
	}
	sort(data.begin(), data.end());											// Data sortieren
	vector<int> histo(data[NUMBER_OF_RUNS-1]+1, 0); 						//Erzeuge Histogramm mit maximaler Päckchenzahl und Startwert 0
	
	for (int i = 0; i < NUMBER_OF_RUNS; ++i) 								// Häufigkeit der Anzahl der Päckchen zählen
	{				
		histo[data[i]]++;				
	}
	return histo;
}


int main()
{		
	ofstream myfile;														// Daten in einem Textdokument "data.txt" speichern
	srand(time(NULL));														// seed setzen
	myfile.open("1b.txt");
	vector<int> hist = histo();
	cout << "Histogramm:" << endl;
	for (int i=0; i < hist.size();i++)
	{
		cout << i << "\t" << hist[i] << endl;
		myfile << i << "\t" << hist[i] << endl;
	}
	myfile.close();
	return 0;
}

