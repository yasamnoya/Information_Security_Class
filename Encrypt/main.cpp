#include<iostream>
#include<string>
using namespace std;

string caesar(string key, string plain_text);
string playfair(string key, string plain_text);
string vernam(string key, string plain_text);
string row(string key, string plain_text);
string rail_fence(string key, string plain_text);

int main(int argc, char** argv) {
	if (argc != 4) {
		cout << "Arguments error!";
	}
	else {
		string cipher = argv[1];
		string key = argv[2];
		string plain_text = argv[3];
		if (cipher == "caesar") {
			cout << caesar(key, plain_text);
		}
		else if (cipher == "playfair") {
			cout << playfair(key, plain_text);
		}
		else if (cipher == "vernam") {
			cout << vernam(key, plain_text);
		}
		else if (cipher == "row") {
			cout << row(key, plain_text);
		}
		else if (cipher == "rail_fence") {
			cout << rail_fence(key, plain_text);
		}
		else {
			cout << "No such cipher \"" << cipher << "\"";
		}

	}
}

string caesar(string key, string plain_text)
{
	for (int i = 0; i < plain_text.size(); i++) {
		int key_int = stoi(key);
		plain_text = toupper(plain_text[i]);
		plain_text[i] += key_int;
		if (plain_text[i] > 'Z') {
			plain_text[i] -= 26;
		}
	}
	return string();
}

string playfair(string key, string plain_text)
{
	return string();
}

string vernam(string key, string plain_text)
{
	return string();
}

string row(string key, string plain_text)
{
	return string();
}

string rail_fence(string key, string plain_text)
{
	return string();
}
