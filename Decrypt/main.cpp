#include<iostream>
#include<string>
#include<vector>
#include<map>
using namespace std;

string caesar(string key, string cipher_text);
string playfair(string key, string cipher_text);
string vernam(string key, string cipher_text);
string row(string key, string cipher_text);
string rail_fence(string key, string cipher_text);

int main(int argc, char** argv) {
	if (argc != 4) {
		cout << "Arguments error!";
	}
	else {
		string cipher = argv[1];
		string key = argv[2];
		string cipher_text = argv[3];
		if (cipher == "caesar") {
			cout << caesar(key, cipher_text);
		}
		else if (cipher == "playfair") {
			cout << playfair(key, cipher_text);
		}
		else if (cipher == "vernam") {
			cout << vernam(key, cipher_text);
		}
		else if (cipher == "row") {
			cout << row(key, cipher_text);
		}
		else if (cipher == "rail_fence") {
			cout << rail_fence(key, cipher_text);
		}
		else {
			cout << "No such cipher \"" << cipher << "\"";
		}
	}
}

string caesar(string key, string cipher_text)
{
	int key_int = stoi(key);	//convert key into int
	for (int i = 0; i < cipher_text.size(); i++) {	//for each char in cipher_text
		cipher_text[i] -= key_int;	//char = char+key
		if (cipher_text[i] < 'A') {	//bound check
			cipher_text[i] += 26;
		}
		cipher_text[i] += 32;
	}
	return cipher_text;
}

string playfair(string key, string cipher_text)
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
	string plain_text;
	for (int i = 0; i < cipher_text.size(); i += 2) {
		char first, second;
		first = toupper(cipher_text[i]);
		second = toupper(cipher_text[i + 1]);
		int first_row, first_col, second_row, second_col;
		first_row = table.find(first).first;
		first_col = table.find(first).second;
		second_row = table.find(second).first;
		second_col = table.find(second).second;

		if (first_row == second_row) {	//same row
			if ((first_col - 1) < 0)
			{
				plain_text.push_back(table.table[first_row][4]);
			}
			else
			{
				plain_text.push_back(table.table[first_row][first_col - 1]);
			}
			if ((second_col - 1) < 0)
			{
				plain_text.push_back(table.table[second_row][4]);
			}
			else
			{
				plain_text.push_back(table.table[second_row][second_col - 1]);
			}
		}
		else if (table.find(first).second == table.find(second).second) {	//same column
			if ((first_row - 1)<0)
			{
				plain_text.push_back(table.table[4][first_col]);
			}
			else
			{
				plain_text.push_back(table.table[first_row - 1][first_col]);
			}
			if ((second_row - 1) < 0)
			{
				plain_text.push_back(table.table[4][second_col]);
			}
			else
			{
				plain_text.push_back(table.table[second_row - 1][second_col]);
			}
			
		}
		else {
			plain_text.push_back(table.table[first_row][second_col]);
			plain_text.push_back(table.table[second_row][first_col]);
		}
	}
	for (int i = 0; i < plain_text.size(); i++)
	{
		plain_text[i] += 32;
	}
	return plain_text;
}

string vernam(string key, string cipher_text)
{
	string plain_text;
	vector<unsigned char> cipher_text_telex(cipher_text.size()), key_telex(key.size()), plain_text_telex(cipher_text.size());	//vector for telex form
	for (int i = 0; i < cipher_text.size(); i++) {
		cipher_text_telex[i] = (unsigned char)(cipher_text[i] - 'A' & 0x1F);	//convert each char in cipher_text into telex form
	}
	for (int i = 0; i < key.size(); i++) {
		key_telex[i] = (unsigned char)(key[i] - 'A' & 0x1F);	//convert each char in cipher_text into telex form
	}
	for (int i = 0; i < cipher_text_telex.size(); i++) {
		plain_text_telex[i] = (cipher_text_telex[i] ^ key_telex[i]) & 0x1F;	//xor plaintext and key
		if (key_telex.size() < cipher_text_telex.size())	//auto key pushback
		{
			key_telex.push_back(plain_text_telex[i]);
		}
	}
	for (int i = 0; i < key_telex.size(); i++) {
		plain_text.push_back(plain_text_telex[i] + 'a');	//convert each char back from telex form
	}
	return plain_text;
}

string row(string key, string cipher_text)
{
	string plain_text;
	vector<int>key_int(key.size());	//vector for key in int type
	vector<string> table(key.size());	//cipher table
	vector<int>key_length(key.size());
	for (int i = 0; i < key_int.size(); i++) {	//convert key from string into int in vector
		key_int[i] = key[i] - '0';
	}
	for (int i = 0; i < key_length.size(); i++)
	{
		key_length[i] = (cipher_text.size() / key.size())+1;
	}
	for (int i = 0; i < key.size()-(cipher_text.size() % key.size()); i++)
	{
		key_length[key_int[key_int[key.size()-i-1]-1]-1]--;
	}
	int count = 0;
	for (int i = 0; i < key.size(); i++)
	{
		while (key_length[i] > 0)
		{
			key_length[i]--;
			table[i].push_back(cipher_text[count]);
			count++;
		}
	}

	for (int i = 0; i < cipher_text.size(); i++)
	{
		plain_text += table[key_int[i%key.size()] - 1][0];
		table[key_int[i%key.size()] - 1].erase(0,1);
	}

	for (int i = 0; i < plain_text.size(); i++)
	{
		plain_text[i] += 32;
	}
	return plain_text;
}

string rail_fence(string key, string cipher_text)
{
	int key_int = stoi(key);	//convert key into int type
	vector<string> table(key_int);	//table for cipher
	string plain_text="";
	int mod_nums=cipher_text.size()%key_int;
	vector<int> table_length(key_int);
	for (int i = 0; i < key_int; i++)
	{
		table_length[i] = cipher_text.size() / key_int;
	}
	int temp = 0;
	while (mod_nums > 0)
	{
		table_length[temp]++;
		temp++;
		mod_nums--;
	}
	int count = 0;
	for (int i = 0; i < key_int; i++)
	{
		while (table_length[i] > 0)
		{
			table[i] += cipher_text[count];
			count++;
			table_length[i]--;
		}
	}
	for (int i = 0; i < cipher_text.size(); i++)
	{
		plain_text += table[i%key_int][0];
		table[i%key_int].erase(0, 1);
	}
	for (int i = 0; i < plain_text.size(); i++)
	{
		plain_text[i] += 32;
	}
	return plain_text;
}
