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
        self.pixels_bin = bytes()
        lines = ppm_bin.readlines()
        for line in lines:
            self.pixels_bin += line
    
    def __bytes__(self):
        return bytes("P6\n","utf-8")+bytes(str(self.length),'utf-8')+bytes(" ","utf-8")+bytes(str(self.width),'utf-8')+bytes("\n","utf-8")+bytes(str(self.depth),'utf-8')+bytes("\n","utf-8")+bytes(self.pixels_bin)

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
    input_jpg_path = argv[1]
    en_de = argv[2]
    mode = argv[3]
    key = argv[4]

    with Image.open(input_jpg_path) as input_jpg_file :
        input_ppm_path = input_jpg_path[:-4]+".ppm"
        input_jpg_file.save(input_ppm_path)
    with open(input_ppm_path,'rb') as input_ppm_bin :
        ppm_file = AESppm(input_ppm_bin)
        ppm_file.ecb(key,0)
        output_ppm_path = '/'.join(input_ppm_path.split('/')[:-1])+"/output.ppm"
        with open(output_ppm_path,'wb') as output_ppm :
            output_ppm.write(bytes(ppm_file))
        
        #with Image.open(output_jpg_path)
