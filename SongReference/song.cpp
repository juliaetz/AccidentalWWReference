#include "song.h"

using namespace std;


void SkipBOM(std::ifstream& in)
{
	char test[3] = { 0 };
	in.read(test, 3);
	if ((unsigned char)test[0] == 0xEF &&
		(unsigned char)test[1] == 0xBB &&
		(unsigned char)test[2] == 0xBF)
	{
		return;
	}
	in.seekg(0);
}

static string toRemove = ",.!?'";
string cleanS(string &toClean) {
	bool addToClean;
	string cleanString = "";
	for (char let : toClean) {
		addToClean = true;
		for (char i : toRemove) {
			if (let == i) {
				addToClean = false;
			}
		}
		if (addToClean) {
			cleanString = cleanString + let;
		}
	}
	return cleanString;
}


Song::Song(string a, string n, ifstream &f) {
	album = a;
	songName = n;
	buildSongU(f);
}

string Song::getName() {
	return songName;
}

vector<string> Song::getSong() {
	return songU;
}


void Song::buildSongU(ifstream &songFile) {
	SkipBOM(songFile);
	string songInput = "";
	string songTot = "";

	if (!songFile.is_open()) {
		cout << "closed";
	}

	getline(songFile, songInput);
	getline(songFile, songInput);
	while (getline(songFile, songInput)) {
		lineCount++;
		songTot = songTot + songInput + " " + to_string(lineCount) + " ";
	}

	stringstream s(songTot);
	songInput = "";
	while (s >> songInput) {
		songU.push_back(cleanS(songInput));
	}
}


void Song::printLineU(int i) {
	if (i < 1 || i > lineCount) {
		cout << "invalid range";
	}

	int index = findWordUIndex(to_string(i - 1));
	index++;

	while (songU[index] != to_string(i)) {
		cout << songU[index] << " ";
		index++;
	}
}



int Song::totLinesU() {
	return lineCount;
}


int Song::totWordsU() {
	return songU.size();
}


string Song::printWordU(int num) {
	if (num < totWordsU()) {
		return songU[num -1];
	}
	else {
		throw "not valid num";
	}
}


bool Song::findWordU(string w) {
	transform(w.begin(), w.end(), w.begin(), ::toupper);
	for (string word : songU) {
		transform(word.begin(), word.end(), word.begin(), ::toupper);
		if(word == w) {
			return true;
		}
	}

	return false;
}

int Song::findWordUIndex(string w) {	
	return findWordIndexH(w, 0);
}


int Song::findWordIndexH(string w, int startingIndex) {
	transform(w.begin(), w.end(), w.begin(), ::toupper);
	for (int i = startingIndex; i < totWordsU(); i++) {
		transform(songU[i].begin(), songU[i].end(), songU[i].begin(), ::toupper);
		if (songU[i] == w) {
			return i;
		}
	}

	return -1;
}



void Playlist::wwReference(string str) {
	// so I can later output the exact string the user inputed if a reference is found
	string strcpy = str;
	strcpy = cleanS(strcpy);
	vector<string> strToSearch;
	string word = "";
	stringstream s(strcpy);
	// seperating the string by word and cleaning up
	while (s >> word) {
		strToSearch.push_back(word);
	}

	// starting searching at index 0
	int strToSearchPos = 0;
	bool foundString = false;
	int songNum = 0;
	while (!foundString && songNum < playlist.size()) {
		// only search if there are four words ahead of the word we are searching
		// if there are less then four words ahead and no reference has been found thusfar, then none will			
		// be found since we cannot get a string of five words
		strToSearchPos = 0;
		while (strToSearchPos + 4 < strToSearch.size() && !foundString) {
			queue<int> posOfWordInSong;
			// start searching for a match to the string word at the first index of the song
			int songWordPos = 0;

			while (songWordPos < playlist[songNum].totWordsU()) { // there are still words to search
				// search starting at the indicated song index
				int i = playlist[songNum].findWordIndexH(strToSearch[strToSearchPos], songWordPos);
				songWordPos = i;
				if (songWordPos != -1) { // word was found at some index
					posOfWordInSong.push(songWordPos);
					songWordPos++;
				}
				else { // no word was found, set song index equal to the end of the song to break the loop
					songWordPos = playlist[songNum].totWordsU();
				}
				// adding one so the find function doesn't start at the same song index as where the
				// match was found
			}
			//cout << "word is " << strToSearch[strToSearchPos] << " and size is " << posOfWordInSong.size() << endl;


			int cycles = 0;
			// if a string moves from one line in the song to the next, bypass the line counter element
			int lineCountCorrection = 0;
			while (!posOfWordInSong.empty() && (cycles < 4)) {
				// cycles count the number of consecutive words matched, adds 1 initially for the words found
				// in the prev step
				cycles++;
				//cout << "I've made it back here again" << endl;
				int i = 0;
				int size = posOfWordInSong.size();
				// for every time the word to search for in the string matched a word in the song
				while (i < size) {
					int nextWordInStr = strToSearchPos + cycles;
					int nextWordInSong = posOfWordInSong.front() + cycles + lineCountCorrection;
					//cout << cycles;
					//cout << lineCountCorrection;

					// if the next word in the song is a line indicater I added (ints to count line #)
					// add one to the next line and update the lineCountCorrection int for future iterations
				
					vector<string> songy = playlist[songNum].getSong();
					if (all_of(songy[nextWordInSong].begin(), songy[nextWordInSong].end(), ::isdigit)) {
						lineCountCorrection++;
						nextWordInSong++;
					}

					transform(strToSearch[nextWordInStr].begin(), strToSearch[nextWordInStr].end(), strToSearch[nextWordInStr].begin(), ::toupper);
					transform(songy[nextWordInSong].begin(), songy[nextWordInSong].end(), songy[nextWordInSong].begin(), ::toupper);
					if (strToSearch[nextWordInStr] != playlist[songNum].getSong()[nextWordInSong]) {
						// remove the word if it doesn't match
						//cout << strToSearch[nextWordInStr] << " doesnt match " << songU[nextWordInSong] << endl;
						posOfWordInSong.pop();
						lineCountCorrection = 0;
					}
					else {
						// if they match, move the word to the end for the next cycle
						//cout << strToSearch[nextWordInStr] << " does match " << songU[nextWordInSong] << endl;
						int temp = posOfWordInSong.front();
						posOfWordInSong.pop();
						posOfWordInSong.push(temp);
					}
					i++;
				}
			}
			// ends when posOfWordInSong is empty (the word to search in the string did not have four more
			// words that exactly matched the song) or cycles >= 4 (there were four more words that matched
			// the song, so a reference was found)


			if (posOfWordInSong.empty()) { // case 1: no reference
				if (cycles == 0) { // the word from the string was not in the song
					cycles++;
				}
				// search a new word in the string, bypassing the consecutive matching words before the 
				// word that broke the chain
				strToSearchPos = strToSearchPos + cycles;
			}
			else { // case 2: loop broke because 5 matches were found (reference)
				foundString = true;
			}
		}

		// all words in the string were searched and no reference was found
		if (!foundString) {
			songNum++;
		}
		else { // reference found
			// outputs the phrase that was the reference, would like to add more things like the song line
			// associated with the reference

			string thePhrase = "";
			for (int i = strToSearchPos; i < strToSearchPos + 5; i++) {
				thePhrase = thePhrase + " " + strToSearch[i];
			}
			cout << thePhrase << "." << thePhrase << "?";
		}
	}
}

void Song::printSong() {
	for (int i = 0; i < songU.size(); i++) {
		cout << songU[i] << " ";
	}
}


Playlist::Playlist(vector<string> f) {
	for (int i = 0; i < f.size(); i++) {
		files.push_back(f[i]);
	}
	for (string file: files) {
		ifstream songFile;
		songFile.open(file);
		string album, name;
		getline(songFile, album);
		getline(songFile, name);
		Song currSong(album, name, songFile);
		playlist.push_back(currSong);
		songFile.close();
	}
}

vector<Song> Playlist::getPlaylist() {
	return playlist;
}
