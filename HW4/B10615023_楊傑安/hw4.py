from sys import argv
from random import choice, randint,random
from math import gcd

def is_prime(n):
    for i in range(2,n//2+1):
        if n%i == 0:
            return False
    return True

def generate_big_prime(bits):
    binary=""
    for i in range(bits-2):
        if(random()>0.8):
            binary+="1"
        else:
            binary+="0"
    binary="1"+binary+"1"
    return int(binary,base=2)
    

def generate_prime(min,max):
    to_return = randint(min,max)
    while not is_prime(to_return):
        to_return = randint(min,max)
    return to_return

def phi(n):
    if is_prime(n):
        return n-1
    else:
        count=0
        for i in range(0,n):
            if gcd(n,i)==1:
                count+=1
        return count

def square_and_multiply(base,exponent,mod):
    to_return = base
    exponent_binary=[int(i) for i in bin(exponent)[2:]]
    for i in exponent_binary[1:]:
        to_return=(to_return**2)%mod
        if i == 1:
            to_return=(to_return*base)%mod
    return to_return

def exponent_mod(base,exponent,mod):
    return square_and_multiply(base,exponent,mod)

def inverse(e,n):
    return exponent_mod(e,phi(n)-1,n)


def init(mode,argv) :
    p = generate_big_prime(6)
    while not is_prime(p):
        p = generate_big_prime(6)
    q = generate_big_prime(5)
    while not is_prime(q):
        q=  generate_big_prime(5)
    n=p*q
    phi_of_n=(p-1)*(q-1)
    e = randint(2,phi_of_n)
    while gcd(e,phi_of_n) != 1:
        e = randint(2,phi_of_n)
    d = inverse(e,phi_of_n)
    print(p,q,n,e,d)

def encrypt(mode,argv):
    text=[]
    for ch in argv[2]:
        text.append(ord(ch))
    n=int(argv[3])
    e=int(argv[4]) 
    cipher=[]
    for ch in text:
        cipher.append(str(exponent_mod(ch,e,n))) 
    print(",".join(cipher))

def decrypt(mode,argv):
    text=argv[2].split(",")
    cipher=[]
    for ch in text:
        cipher.append(int(ch))
    n=int(argv[3])
    d=int(argv[4]) 
    plain=[]
    for ch in cipher:
        plain.append(chr(exponent_mod(ch,d,n))) 
    print("".join(plain))

def exception(mode,argv):
    print("No such mode:"+mode)

def caller(mode,argv):
    mode_map = {
        "-i" : init,
        "-e" : encrypt,
        "-d" : decrypt
    }
    function=mode_map.get(mode,exception)
    if function:
        function(mode,argv)
    else:
        print("arguments error")
        
if __name__ == "__main__":
    caller(argv[1],argv)