import numpy as np
import sys
import random as rd
def create_key(length):
    (p,q)=create_length(length)
    n = p * q 
    fyn = (p - 1) * (q - 1) 
    e = 3889
    a = e 
    b = fyn
    r, x, y = ext_gcd(a, b)
    d = x
    return p,q,d,e,n
def create_length(length):
    p = rd.getrandbits(length)
    while(miller_rabin(p)==0):
        p = rd.getrandbits(length)
    q = rd.getrandbits(length)
    while(miller_rabin(q)==0 or q==p):
        q = rd.getrandbits(length)
    return(p,q)
def ext_gcd(a, b):
    if b == 0:
        x1 = 1 
        y1 = 0 
        x = x1 
        y = y1 
        r = a 
        return r, x, y 
    else: 
        r, x1, y1 = ext_gcd(b, a % b)
        x = y1 
        y = x1 - a // b * y1 
        return r, x, y
def square_and_multiply(base, exponent, n):
    exponent_bin = bin(exponent)[2:]
    exponent_len = len(exponent_bin)
    ans = 1
    for ii in range(exponent_len):
        ans *= ans
        ans = ans % n
        if(exponent_bin[ii]=='1'):
            ans *= base
            ans = ans % n
#     return pow(base,exponent,n)
    return ans
def miller_rabin(n):
    if (n < 2):
        return False
    a = [2,3,5,7,11,13,17,19]
    for a2 in a:
        u = n-1
        t = 0
        while (u % 2 == 0):
            u >>= 1
            t += 1
        if(t == 0):
            return False
        x = square_and_multiply(a2, n - 1, n)
        if (x == 1 or x ==n-1):
            continue
        x = square_and_multiply(a2, u, n)
        if (x == 1 or x == n-1):
            continue
        for i in range (t-1):
            x = x * x % n
            if (x == 1):
                return False
            if (x == n-1):
                continue
        return False
    return True
def encrypt(plain, e, n):
    cipher = square_and_multiply(plain, e, n)
    return cipher
def decrypt(cipher, d, n):
    plain = square_and_multiply(cipher, d, n)
    return plain
if __name__ == '__main__':
    if(sys.argv[1] == "init"):
        p,q,d,e,n = create_key(int(sys.argv[2]))
        print("p =",p,"\n")
        print("q =",q,"\n")
        print("d =",d,"\n")
        print("e =",e,"\n")
        print("n =",n,"\n")
    elif(sys.argv[1] == "-e"):
        plain = sys.argv[2]
        n = sys.argv[3]
        n = int(n)
        e = sys.argv[4]
        e = int(e)
        plain_intstr = ""
        for ch in plain:
            plain_intstr += (str(ord(ch)).zfill(3))
        plain_int = int(plain_intstr)
        cipher = encrypt(plain_int,e,n)
        print("cipher text = ",cipher)
    elif(sys.argv[1] == "-d"):
        cipher = sys.argv[2]
        n = sys.argv[3]
        n = int(n)
        d = sys.argv[4]
        d = int(d)
        cipher_intstr = ""
        #for ch in cipher:
        #    cipher_intstr += (str(ord(ch)).zfill(3))
        #cipher_int = int(cipher_intstr)
        cipher_int = int(cipher)
        plain = decrypt(cipher_int,d,n)
        plain = str(plain)
        while (len(plain) %3 != 0):
            plain = '0' + plain
        plain_str = ""
        for i in range(len(plain)//3):
            threeinarow = int(plain[i*3:i*3+3])
            plain_str += str(chr(threeinarow))
        print("plain text = ",plain_str)
    