// SongReference.cpp : Defines the entry point for the application.
//

#include "SongReference.h"

using namespace std;

int main()
{
	vector<string> songs;
	songs.push_back("C://Users//julia//source//repos//songReference//songReference//WWFiles//2econd-2ight-2eer.txt");
	songs.push_back("C://Users//julia//source//repos//songReference//songReference//WWFiles//wbtta.txt");
	Playlist ww(songs);

	string w = "plz tell me wanted to im cut know lab rat girls and pretty um";
	ww.wwReference(w);

	return 0;
}
