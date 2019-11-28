from Crypto.Cipher import AES
from PIL import Image
from sys import argv
import copy

def pad(text):
	padding = 16 - (len(text) % 16)
	return text + bytes([padding] * padding)

def unpad(text):
	padding = text[-1]
	return text[:-padding]

def AES_encrypt_one_block(plain,key):
    cipher = AES.new(key, AES.MODE_ECB)
    ciphertext = cipher.encrypt(plain)
    return ciphertext

def AES_decrypt_one_block(cipher,key):
    plain = AES.new(key, AES.MODE_ECB)
    plaintext = plain.decrypt(cipher)
    return plaintext

def bytes_xor(b1, b2): # use xor for bytes
    parts = []
    for b1, b2 in zip(b1, b2):
        parts.append(bytes([b1 ^ b2]))
    return b''.join(parts)

class AESppm():
    def __init__(self,ppm_bin):
        string = ppm_bin.readline()
        string = ppm_bin.readline()
        self.length = int(string.split()[0])
        self.width = int(string.split()[1])
        self.depth = int(ppm_bin.readline()) 
        self.pixels_bin = ppm_bin.read()
    
    def __bytes__(self):
        to_return = bytes("P6\n","utf-8")+bytes(str(self.length),'utf-8')+bytes(" ","utf-8")+bytes(str(self.width),'utf-8')+bytes("\n","utf-8")+bytes(str(self.depth),'utf-8')+bytes("\n","utf-8")+self.pixels_bin
        return to_return

    def ecb(self,key,en_de) : #0=encrypt 1=decrypt
        self.pixels_bin = pad(self.pixels_bin)
        to_return = self
        pixels_bin_origin = self.pixels_bin
        to_return.pixels_bin=bytes()
        if en_de==0:
            cipherblock = bytes()
            for i in range(int(len(pixels_bin_origin)/16)) :
                block = pixels_bin_origin[16*i:16*i+16]
                cipherblock += AES_encrypt_one_block(block,key)
            to_return.pixels_bin += cipherblock
        elif en_de==1:
            plainblock = bytes()
            for i in range(int(len(pixels_bin_origin)/16)) :
                block = pixels_bin_origin[16*i:16*i+16]
                plainblock += AES_decrypt_one_block(block,key)
            to_return.pixels_bin += plainblock
        return to_return

    def cbc(self,key,en_de) :
        self.pixels_bin = pad(self.pixels_bin)
        to_return = copy.copy(self)
        to_return.pixels_bin=bytes()
        processed_block=bytes()
        vector = key[0:16]
        if(en_de==0) :
            for i in range(int(len(self.pixels_bin)/16)) :
                block=self.pixels_bin[16*i:16*i+16]
                block = bytes_xor(block,vector)
                processed_block = AES_encrypt_one_block(block,key)
                vector = processed_block
                to_return.pixels_bin+=processed_block
        else :
            for i in range(int(len(self.pixels_bin)/16)) :
                block=self.pixels_bin[16*i:16*i+16]
                processed_block = AES_decrypt_one_block(block,key)
                processed_block = bytes_xor(processed_block,vector)
                vector = block
                to_return.pixels_bin+=processed_block
        return to_return
            

    def cool(self,key,en_de) :
        self.pixels_bin = pad(self.pixels_bin)
        to_return = copy.copy(self)
        to_return.pixels_bin=bytes()
        processed_block=bytes()
        vector = key[0:16]
        for i in range(int(len(self.pixels_bin)/16)) :
            block=self.pixels_bin[16*i:16*i+16]
            xored_block = bytes_xor(block,vector)
            if(en_de==0) :
                processed_block = AES_encrypt_one_block(xored_block,key)
            else :
                processed_block = AES_decrypt_one_block(xored_block,key)
            processed_block = bytes_xor(processed_block,vector)
            to_return.pixels_bin+=processed_block
            vector = bytes_xor(processed_block,block)
        return to_return
    


#cipher = AES.new(key, AES.MODE_ECB)
#ciphertext = cipher.encrypt(one_block_text)

if __name__ == "__main__":
    # argv=[hw3.py, input filepath, output filepath, encrypt/decrypt, mode, key]
    if(len(argv)!=6):
        print("Arguments error")
        exit()
    input_png_path = argv[1]
    output_png_path = argv[2]
    if(argv[3].lower()=="encrypt") :
        en_de=0;
    elif(argv[3].lower()=="decrypt") :
        en_de=1;
    else :
        print("encyrpt or decrypt")
        exit()
    mode = argv[4].lower()
    key = bytes.fromhex(argv[5])

    with Image.open(input_png_path) as input_png_file :
        input_ppm_path = input_png_path[:-4]+".ppm"
        input_png_file.save(input_ppm_path)
    with open(input_ppm_path,'rb') as input_ppm_bin :
        ppm_file = AESppm(input_ppm_bin)
        if(mode=="ecb") :
            ppm_file=ppm_file.ecb(key,en_de)
        elif(mode=="cbc") :
            ppm_file=ppm_file.cbc(key,en_de)
        elif(mode=="cool") :
            ppm_file=ppm_file.cool(key,en_de)
        output_ppm_path = '/'.join(input_ppm_path.split('/')[:-1])+"/output.ppm"
        with open(output_ppm_path,'wb') as output_ppm_bin :
            output_ppm_bin.write(bytes(ppm_file))
        with Image.open(output_ppm_path) as output_ppm :
            output_ppm.save(output_png_path,output_png_path[-3:])