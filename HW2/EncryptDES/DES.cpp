#include "DES.h"
//#define DEBUGKEY
//#define DEBUGDATA



DES::DES()
{
}


DES::~DES()
{
}

string DES::encrypt(string plaintext_string, string key)
{
	Data data_hex = string_to_hex(plaintext_string);
	Data key_hex = string_to_hex(key);
#ifdef DEBUGDATA
	cout << "Data :\t";
	for (int i = 0; i < data_hex.size(); i++) {
		cout << data_hex[i];
	}
	cout << endl;
#endif
	data_hex = init_perm(data_hex);
	Subdata_56 key_56 = pc_1(key_hex);
#ifdef DEBUGKEY
	cout << "Key 0:\t";
	for (int i = 0; i < key_56.size(); i++) {
		cout << key_56[i];
		if (i == 27) {
			cout << " ";
		}
	}
	cout << endl;
#endif
#ifdef DEBUGDATA
	cout << "Data 0:\t";
	for (int i = 0; i < data_hex.size(); i++) {
		cout << data_hex[i];
	}
	cout << endl;
#endif
	for (int i = 1; i <= 16; i++) {
		key_56 = key_rotate(key_56, 1, i - 1);
		Subdata_48 key_48 = pc_2(key_56);
		data_hex = encrypt_round(data_hex, key_48);

#ifdef DEBUGKEY
		cout << "Key " << i << ":\t";
		for (int i = 0; i < key_56.size(); i++) {
			cout << key_56[i];
			if (i == 27) {
				cout << " ";
			}
		}
		cout << endl;
#endif
#ifdef DEBUGKEY
		cout << "K " << i << ":\t";
		for (int i = 0; i < key_48.size(); i++) {
			cout << key_48[i];
			if ((i + 1) % 6 == 0) {
				cout << " ";
			}
		}
		cout << endl;
#endif
#ifdef DEBUGDATA
		cout << "Data " << i << ":\t";
		for (int i = 0; i < data_hex.size(); i++) {
			cout << data_hex[i];
		}
		cout << endl;
#endif

	}
	data_hex = split_reverse(data_hex);
	data_hex = final_perm(data_hex);
	string ciphertext = hex_to_string(data_hex);
	return ciphertext;
}

string DES::decrypt(string ciphertext_string,string key)
{
	Data data_hex = string_to_hex(ciphertext_string);
	Data key_hex = string_to_hex(key);
#ifdef DEBUGDATA
	cout << "Data :\t";
	for (int i = 0; i < data_hex.size(); i++) {
		cout << data_hex[i];
	}
	cout << endl;
#endif
	data_hex = init_perm(data_hex);
	Subdata_56 key_56 = pc_1(key_hex);
#ifdef DEBUGKEY
	cout << "Key 0:\t";
	for (int i = 0; i < key_56.size(); i++) {
		cout << key_56[i];
		if (i == 27) {
			cout << " ";
		}
	}
	cout << endl;
#endif
#ifdef DEBUGDATA
	cout << "Data 0:\t";
	for (int i = 0; i < data_hex.size(); i++) {
		cout << data_hex[i];
	}
	cout << endl;
#endif
	for (int i = 16; i >= 1; i--) {
		Subdata_48 key_48 = pc_2(key_56);
		data_hex = encrypt_round(data_hex, key_48);
		key_56 = key_rotate(key_56, -1, i - 1);

#ifdef DEBUGKEY
		cout << "Key " << i << ":\t";
		for (int i = 0; i < key_56.size(); i++) {
			cout << key_56[i];
			if (i == 27) {
				cout << " ";
			}
		}
		cout << endl;
#endif
#ifdef DEBUGKEY
		cout << "K " << i << ":\t";
		for (int i = 0; i < key_48.size(); i++) {
			cout << key_48[i];
			if ((i + 1) % 6 == 0) {
				cout << " ";
			}
		}
		cout << endl;
#endif
#ifdef DEBUGDATA
		cout << "Data " << i << ":\t";
		for (int i = 0; i < data_hex.size(); i++) {
			cout << data_hex[i];
		}
		cout << endl;
#endif

	}
	data_hex = split_reverse(data_hex);
	data_hex = final_perm(data_hex);
	string plain_text = hex_to_string(data_hex);
	for (int i = 0; i < plain_text.size(); i++) {
		plain_text[i] = tolower(plain_text[i]);
	}
	return plain_text;
}

Data DES::string_to_hex(string input)
{
	Data output;
	for (int i = 0; i < input.size(); i++) {
		unsigned char ch;
		ch = toupper(input[i]);
		if (ch >= '0'&&ch <= '9') {
			ch = ch - '0';
		}
		else if (ch >= 'A'&&ch <= 'F') {
			ch = ch - '0' - 7;
		}
		for (int j = 3; j >= 0; j--) {
			output.push_back((ch >> j) % 2 == 1);
		}
	}
	return output;
}

string DES::hex_to_string(Data input)
{
	string output;
	for (int i = 0; i < input.size(); i += 4) {
		unsigned char ch = 0x0;
		for (int j = 3; j >= 0; j--) {
			ch += input[i + j] << (3 - j);
		}
		if (ch >= 0 && ch <= 9) {
			ch = ch + '0';
		}
		else if (ch >= 10 && ch <= 15) {
			ch = ch + 'A' - 10;
		}
		output.push_back(ch);
	}
	return output;
}

Data DES::init_perm(Data input)
{
	Box box{ 58,50,42,34,26,18,10,2,
				60,52,44,36,28,20,12,4,
				62,54,46,38,30,22,14,6,
				64,56,48,40,32,24,16,8,
				57,49,41,33,25,17,9 ,1,
				59,51,43,35,27,19,11,3,
				61,53,45,37,29,21,13,5,
				63,55,47,39,31,23,15,7 };
	Data output;
	for (int i = 0; i < box.size(); i++) {
		output.push_back(input[box[i] - 1]);
	}
	return output;
}

Data DES::final_perm(Data input)
{
	Box box{ 40,8,48,16,56,24,64,32,
				39,7,47,15,55,23,63,31,
				38,6,46,14,54,22,62,30,
				37,5,45,13,53,21,61,29,
				36,4,44,12,52,20,60,28,
				35,3,43,11,51,19,59,27,
				34,2,42,10,50,18,58,26,
				33,1,41,9 ,49,17,57,25 };
	Data output;
	for (int i = 0; i < box.size(); i++) {
		output.push_back(input[box[i] - 1]);
	}
	return output;
}

Halfdata DES::round_perm(Halfdata input)
{
	Box box{ 16,7 ,20,21,
				29,12,28,17,
				1 ,15,23,26,
				5 ,18,31,10,
				2 ,8 ,24,14,
				32,27,3	,9 ,
				19,13,30,6 ,
				22,11,4	,25 };
	Halfdata output;
	for (int i = 0; i < box.size(); i++) {
		output.push_back(input[box[i] - 1]);
	}
	return output;
}

Data DES::encrypt_round(Data input, Subdata_48 key)
{
	Halfdata l, r;
	for (int i = 0; i < input.size() / 2; i++) {
		l.push_back(input[i]);
		r.push_back(input[i + input.size() / 2]);
	}
	l = XOR(l, f(r, key));
	Data output;
	for (int i = 0; i < r.size(); i++) {
		output.push_back(r[i]);
	}
	for (int i = 0; i < r.size(); i++) {
		output.push_back(l[i]);
	}
	return output;
}

Halfdata DES::f(Halfdata input, Subdata_48 key)
{
	Subdata_48 expanded = expansion(input);
	expanded = XOR(expanded, key);
	Halfdata output;
	vector<Subdata_6> bit6s = split_into_6(expanded);
	vector<Subdata_4> bit4s;
	for (int i = 0; i < bit6s.size(); i++) {
		bit4s.push_back(s_box(bit6s[i], i + 1));
	}
	output = join_into_32(bit4s);
	output = round_perm(output);
	return output;
}

Subdata_48 DES::expansion(Halfdata input)
{
	Box box = { 32,1,2	,3	,4,5 ,
				4,5	,6	,7	,8,9 ,
				8,9	,10	,11,12,13,
				12,13,14,15,16,17,
				16,17,18,19,20,21,
				20,21,22,23,24,25,
				24,25,26,27,28,29,
				28,29,30,31,32,1
	};
	Subdata_48 output;
	for (int i = 0; i < box.size(); i++) {
		output.push_back(input[box[i] - 1]);
	}
	return output;
}

vector<Subdata_6> DES::split_into_6(Subdata_48 input)
{
	vector<Subdata_6> output;
	for (int i = 0; i < input.size(); i += 6) {
		output.push_back({});
		for (int j = 0; j < 6; j++) {
			output[i / 6].push_back(input[i + j]);
		}
	}
	return output;
}

Halfdata DES::join_into_32(vector<Subdata_4> input)
{
	Halfdata output;
	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < 4; j++) {
			output.push_back(input[i][j]);
		}
	}
	return output;
}

Subdata_56 DES::pc_1(Data input)
{
	Box box{ 57	,49	,41	,33	,25	,17	,9 ,
				1	,58	,50	,42	,34	,26	,18,
				10	,2	,59	,51	,43	,35	,27,
				19	,11	,3	,60	,52	,44	,36,
				63	,55	,47	,39	,31	,23	,15,
				7	,62	,54	,46	,38	,30	,22,
				14	,6	,61	,53	,45	,37	,29,
				21	,13	,5	,28	,20	,12	,4 };
	Subdata_56 output;
	for (int i = 0; i < box.size(); i++) {
		output.push_back(input[box[i] - 1]);
	}
	return output;
}

Subdata_48 DES::pc_2(Subdata_56 input)
{
	Box box{ 14	,17	,11	,24	,1	,5 ,
				3	,28	,15	,6	,21	,10,
				23	,19	,12	,4	,26	,8 ,
				16	,7	,27	,20	,13	,2 ,
				41	,52	,31	,37	,47	,55,
				30	,40	,51	,45	,33	,48,
				44	,49	,39	,56	,34	,53,
				46	,42	,50	,36	,29	,32 };
	Subdata_48 output;
	for (int i = 0; i < box.size(); i++) {
		output.push_back(input[box[i] - 1]);
	}
	return output;
}

Subdata_56 DES::key_rotate(Subdata_56 input, int dir, int round)
{
	Subdata_56 output;
	vector<Subdata_28> halves = split_into_28(input);
	vector<int> times = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };
	for (int i = 0; i < times[round]; i++) {
		for (int j = 0; j < 2; j++) {
			if (dir == 1) {
				halves[j] = rotate_28_left(halves[j]);
			}
			else if (dir == -1) {
				halves[j] = rotate_28_right(halves[j]);
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < halves[i].size(); j++) {
			output.push_back(halves[i][j]);
		}
	}
	return output;
}

vector<Subdata_28> DES::split_into_28(Subdata_56 input)
{
	vector<Subdata_28> output(2);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 28; j++) {
			output[i].push_back(input[28 * i + j]);
		}
	}
	/*#ifdef DEBUGKEY
		cout << "input:\t";
		for (int i = 0; i < input.size(); i++) {
			cout << input[i];
			if (i == 27) {
				cout << " ";
			}
		}
		cout << endl;
		cout << "output:\t";
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < output[j].size(); i++) {
				cout << output[j][i];
			}
			cout << " ";
		}
		cout << endl;
	#endif*/
	return output;
}

Subdata_28 DES::rotate_28_left(Subdata_28 input)
{
	Subdata_28 output;
	for (int i = 1; i < input.size(); i++) {
		output.push_back(input[i]);
	}
	output.push_back(input[0]);
	return output;
}

Subdata_28 DES::rotate_28_right(Subdata_28 input)
{
	Subdata_28 output;
	output.push_back(input[input.size() - 1]);
	for (int i = 0; i < input.size() - 1; i++) {
		output.push_back(input[i]);
	}
	return output;
}

vector<bool> DES::XOR(vector<bool> input1, vector<bool> input2)
{
	vector<bool> output;
	for (int i = 0; i < input1.size(); i++) {
		output.push_back(input1[i] ^ input2[i]);
	}
	return output;
}

Data DES::split_reverse(Data input)
{
	Data output;
	for (int i = 32; i < 64; i++) {
		output.push_back(input[i]);
	}
	for (int i = 0; i < 32; i++) {
		output.push_back(input[i]);
	}
	return output;
}

void DES::print(vector<bool> input)
{
	for (int i = 0; i < input.size(); i++) {
		cout << input[i];
	}
	cout << endl;
}


