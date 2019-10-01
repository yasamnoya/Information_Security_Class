#include<iostream>
#include<string>
#include<vector>
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
	int key_int = stoi(key);	//convert key into int
	for (int i = 0; i < plain_text.size(); i++) {	//for each char in plain_text
		plain_text[i] = toupper(plain_text[i]);	//convert char into upper
		plain_text[i] += key_int;	//char = char+key
		if (plain_text[i] > 'Z') {	//bound check
			plain_text[i] -= 26;
		}
	}
	return plain_text;
}

string playfair(string key, string plain_text)
{
	return string();
}

string vernam(string key, string plain_text)
{
	string cipher_text;
	vector<unsigned char> plain_text_telex(plain_text.size()), key_telex(key.size()),cipher_text_telex(plain_text.size());	//vector for telex form
	for (int i = 0; i < plain_text.size(); i++) {
		plain_text_telex[i] = (unsigned char)(plain_text[i] - 'a' & 0x1F);	//convert each char in plain_text into telex form
	}
	for (int i = 0; i < key.size(); i++) {
		key_telex[i] = (unsigned char)(key[i] - 'a' & 0x1F);	//convert each char in plain_text into telex form
	}
	for (int i = 0; key_telex.size() < plain_text_telex.size(); i++) {
		key_telex.push_back(plain_text_telex[i]);	//expand key with autokey
	}
	for (int i = 0; i < plain_text_telex.size(); i++) {
		cipher_text_telex[i] = (plain_text_telex[i] ^ key_telex[i])&0x1F;	//xor plaintext and key
	}
	for (int i = 0; i < key_telex.size(); i++) {
		cipher_text.push_back(cipher_text_telex[i] + 'A');	//convert each char back from telex form
	}
	return cipher_text;
}

string row(string key, string plain_text)
{
	string cipher_text;
	vector<int>key_int(key.size());	//vector for key in int type
	vector<string> table(key.size());	//cipher table
	for (int i = 0; i < key_int.size(); i++) {	//convert key from string into int in vector
		key_int[i] = key[i] - '0';
	}
	for (int i = 0; i < plain_text.size(); i++) {
		table[key_int[i%key_int.size()]-1].push_back(toupper(plain_text[i]));	//fiiling chars with the order of key
	}
	for (int i = 0; i < table.size(); i++) {
		cipher_text += table[i];
	}
	return cipher_text;
}

string rail_fence(string key, string plain_text)
{
	return string();
}
