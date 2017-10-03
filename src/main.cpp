#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

void loadWord(char* path, map<string, int>& word);
void getFullSubEmb(char* emb_path, const map<string, int>& full, const map<string, int>& sub);
void split_bychar(const string& str, vector<string>& vec, const char separator = ' ');

int main(int argc, char* argv[]){
	map<string, int> full, sub;
	loadWord(argv[1], full);
	loadWord(argv[2], sub);
	getFullSubEmb(argv[3], full, sub);
	return 0;
}

void loadWord(char* path, map<string, int>& word){
	ifstream inf(path);
	string line;
	while(inf>>line){
		word[line] = 0;
	}
	inf.close();
}

void getFullSubEmb(char* emb_path, const map<string, int>& full, const map<string, int>& sub) {
	ifstream inf(emb_path);
	string line;
	vector<string> info;
	vector<double> full_emb, sub_emb;
	long full_num = 0, sub_num = 0;
	getline(inf, line);
	split_bychar(line, info, ' ');
	int emb_size = info.size() - 1;
	full_emb.resize(emb_size);
	sub_emb.resize(emb_size);
	for(int idx = 0; idx < emb_size; idx++) { 
		full_emb[idx] = 0;
		sub_emb[idx] = 0;
	}
	string str = info[0];
	if (full.find(str) != full.end()) { 
		full_num++;
		for(int idx = 0; idx < emb_size; idx++) {
			full_emb[idx] += atof(info[idx + 1].c_str());
		}
	}
	if(sub.find(str) != sub.end()) { 
		sub_num++;
		for(int idx = 0; idx < emb_size; idx++) {
			sub_emb[idx] += atof(info[idx + 1].c_str());
		}
	}
	while (getline(inf, line)) {
		split_bychar(line, info, ' ');
		string str = info[0];
		if (full.find(str) != full.end()) {
			full_num++;
			for (int idx = 0; idx < emb_size; idx++) {
				full_emb[idx] += atof(info[idx + 1].c_str());
			}
		}
		if (sub.find(str) != sub.end()) {
			sub_num++;
			for (int idx = 0; idx < emb_size; idx++) {
				sub_emb[idx] += atof(info[idx + 1].c_str());
			}
		}
	}

	for (int idx = 0; idx < emb_size; idx++) {
		full_emb[idx] /= full_num;
		sub_emb[idx] /= sub_num;
	}
	cout << "--WORD--";
	for (int idx = 0; idx < emb_size; idx++) {
		cout << " " << full_emb[idx];
	}
	cout << endl;

	cout << "--SUBWORD--";
	for (int idx = 0; idx < emb_size; idx++) {
		cout << " " << sub_emb[idx];
	}
	inf.close();
}

void split_bychar(const string& str, vector<string>& vec, const char separator) {
	//assert(vec.empty());
	vec.clear();
	string::size_type pos1 = 0, pos2 = 0;
	string word;
	while ((pos2 = str.find_first_of(separator, pos1)) != string::npos) {
		word = str.substr(pos1, pos2 - pos1);
		pos1 = pos2 + 1;
		if (!word.empty())
			vec.push_back(word);
	}
	word = str.substr(pos1);
	if (!word.empty())
		vec.push_back(word);
}
