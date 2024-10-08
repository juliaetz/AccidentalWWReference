#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <queue>

using namespace std;


class Song {
private:
	string file;
	string songName;
	string album;
	vector<string> songU;
	int lineCount = 0;
	//string toRemove = ",.!?'";
	Song() {}
	void buildSongU(ifstream &file);
	//string cleanS(string &toClean);
	//void SkipBOM(std::ifstream& in);
public:
	// song should not take in the file
	// should take in song name and album
	Song(string a, string n, ifstream &file);
	string getName();
	vector<string> getSong();
	void printLineU(int i);
	int totWordsU();
	int totLinesU();
	string printWordU(int num);
	bool findWordU(string w);
	int findWordUIndex(string w);
	void printSong();
	int findWordIndexH(string w, int startingIndex);
	void wwReference(string str);
};

class Playlist {
private:
	vector<Song> playlist;
	vector<string> files;
	Playlist() {}
public:
	// constructor that takes in file, makes new song element and calls buildSongU
	Playlist(vector<string> f);
	void wwReference(string str);
	vector<Song> getPlaylist();
};
