from Crypto.Cipher import AES
from PIL import Image
from sys import argv

class AESppm():
    def __init__(self,ppm_bin):
        string = ppm_bin.readline()
        string = ppm_bin.readline()
        self.length = int(string.split()[0])
        self.width = int(string.split()[1])
        self.depth = int(ppm_bin.readline())
        self.pixels_bin = ppm_bin.readline()
        
    def ecb(self,key,en_de) : #0=encrypt 1=decrypt
        pass
    def cbc(self,key,en_de) :
        pass
    def my(self,key,en_de) :
        pass



#cipher = AES.new(key, AES.MODE_ECB)
#ciphertext = cipher.encrypt(one_block_text)

if __name__ == "__main__":
    # argv=[hw3.py, filepath, encrypt/decrypt, mode, key]
    if(len(argv)!=5):
        print("Arguments error")
        exit()
    filepath = argv[1]
    en_de = argv[2]
    mode = argv[3]
    key = argv[4]

    with Image.open(filepath) as image_file :
        ppm_path = filepath[:-4]+".ppm"
        image_file.save(ppm_path)
    with open(ppm_path,'rb') as ppm_bin :
        ppm_file = AESppm(ppm_bin)
        ppm_file.ecb(key,0)
        output_path = 
        with 

