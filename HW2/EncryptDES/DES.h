#pragma once

using namespace std;
#include<iostream>
#include<string>
#include<vector>
typedef vector<bool> Data;	//8bytes
typedef vector<bool> Halfdata;	//4bytes
typedef vector<bool> Subdata_48;	//6bytes
typedef vector<bool> Subdata_56;	//7bytes
typedef vector<bool> Subdata_28; //28bits
typedef vector<bool> Subdata_6;	//6bits
typedef vector<bool> Subdata_4;	//4bits
typedef vector<int> Box;

class DES
{
public:
	DES();
	~DES();
	static string encrypt(string plaintext_string,string key);
	static string decrypt(string ciphertext_string,string key);
private:
	static Data string_to_hex(string input);
	static string hex_to_string(Data input);
	static Data init_perm(Data input);
	static Data final_perm(Data input);
	static Halfdata round_perm(Halfdata input);
	static Data encrypt_round(Data input,Subdata_48 key);
	static Halfdata f(Halfdata input,Subdata_48 key);
	static Subdata_48 expansion(Halfdata input);
	static vector<Subdata_6> split_into_6(Subdata_48 input);
	static Subdata_4 s_box(Subdata_6 input,int interval);	//1~8
	static Halfdata join_into_32(vector<Subdata_4> input);
	static Subdata_56 pc_1(Data input);
	static Subdata_48 pc_2(Subdata_56 input);
	static Subdata_56 key_rotate(Subdata_56 input,int dir,int times);	//1=encrypt -1=decrypt
	static vector<Subdata_28> split_into_28(Subdata_56 input);
	static Subdata_28 rotate_28_left(Subdata_28 input);
	static Subdata_28 rotate_28_right(Subdata_28 input);
	static vector<bool> XOR(vector<bool> input1, vector<bool> input2);
	static Data split_reverse(Data input);
	static void print(vector<bool> input);
};


