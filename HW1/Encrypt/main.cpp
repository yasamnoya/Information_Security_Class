#include<iostream>
#include<string>
#include<vector>
#include<map>
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
	class Table {
	public:
		map<char, bool>is_used;	//check if char is used in table
		vector<string> table;
		Table();
		Table(string key) {	//Table constructor
			string table_in_line;	//connect each row in table in a line (helper)
			for (char i = 'A'; i <= 'Z'; i++) {	//initialize is_used
				is_used[i] = false;
			}
			is_used['J'] = true;	//'J' is not used
			for (int i = 0; i < key.size(); i++) {	//push char in key into table_in_line
				key[i] = toupper(key[i]);
				if (!is_used[key[i]]) {
					is_used[key[i]] = true;
					table_in_line.push_back(key[i]);
				}
			}
			for (char i = 'A'; i <= 'Z'; i++) {	//push the rest chars into table_in_line
				if (!is_used[i]) {
					is_used[i] = true;
					table_in_line.push_back(i);
				}
			}
			for (int i = 0; i < 5; i++) {	//break table_in_line into rows and form the table
				table.push_back(table_in_line.substr(5 * i, 5));
			}
		}
		pair<int, int> find(char ch) {	//return position of ch in table
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					if (table[i][j] == ch)
						return pair<int, int>(i, j);
				}
			}
		}
	};
	Table table(key);	//construct key table
	string cipher_text;
	for (int i = 0; i < plain_text.size(); i += 2) {
		char first, second;
		first = toupper(plain_text[i]);
		second = toupper(plain_text[i + 1]);
		int first_row, first_col, second_row, second_col;
		first_row = table.find(first).first;
		first_col = table.find(first).second;
		second_row = table.find(second).first;
		second_col = table.find(second).second;

		if (first_row == second_row) {	//same row
			cipher_text.push_back(table.table[first_row][(first_col + 1) % 5]);
			cipher_text.push_back(table.table[second_row][(second_col + 1) % 5]);
		}
		else if (table.find(first).second == table.find(second).second) {	//same column
			cipher_text.push_back(table.table[(first_row + 1) % 5][first_col]);
			cipher_text.push_back(table.table[(second_row + 1) % 5][second_col]);
		}
		else {
			cipher_text.push_back(table.table[first_row][second_col]);
			cipher_text.push_back(table.table[second_row][first_col]);
		}
	}
	return cipher_text;
}

string vernam(string key, string plain_text)
{
	string cipher_text;
	vector<unsigned char> plain_text_telex(plain_text.size()), key_telex(key.size()), cipher_text_telex(plain_text.size());	//vector for telex form
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
		cipher_text_telex[i] = (plain_text_telex[i] ^ key_telex[i]) & 0x1F;	//xor plaintext and key
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
		table[key_int[i%key_int.size()] - 1].push_back(toupper(plain_text[i]));	//fiiling chars with the order of key
	}
	for (int i = 0; i < table.size(); i++) {
		cipher_text += table[i];	//connect rows
	}
	return cipher_text;
}

string rail_fence(string key, string plain_text)
{
	int key_int = stoi(key);	//convert key into int type
	vector<string> table(key_int);	//table for cipher
	string cipher_text;
	for (int i = 0, row = 0; i < plain_text.size();) {
		for (; i < plain_text.size() && row < key_int; i++, row++) {	//filling chars into table
			table[row].push_back(toupper(plain_text[i]));
		}
		row -= 2;	//out of bound
		for (; i < plain_text.size() && row >= 0; i++, row--) {
			table[row].push_back(toupper(plain_text[i]));
		}
		row += 2;	//out of bound
	}
	for (int i = 0; i < table.size(); i++) {
		cipher_text += table[i];	//connect rows
	}
	return cipher_text;
}
