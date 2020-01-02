import numpy as np
import sys
import random as rd
from hashlib import sha1

def create_length(lengthp,lengthq):
    q = rd.getrandbits(lengthq)
    while(miller_rabin(q)==0):
        q = rd.getrandbits(lengthq)
    temp = rd.getrandbits(lengthp-lengthq)
    p = temp*q+1
    while((miller_rabin(p)==0) or (p.bit_length()!=lengthp)):
        temp = rd.getrandbits(lengthp-lengthq)
        p = temp*q+1
#     if(p.bit_length()==lengthp):
    return(p,q,temp)
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
def find_nums(p, q, times):
    h = rd.randint(1, p)
    d = rd.randint(1, q)
    alpha = square_and_multiply(h, times, p)
    beta = square_and_multiply(alpha, d, p)
    return(h, d, alpha, beta)
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
if __name__ == '__main__':
    messagex = "suckmy"
    (p, q, times)= create_length(1024, 160)
    (h, d, alpha, beta) = find_nums(p, q, times)
    kE = rd.randint(1, q)
    r = square_and_multiply(alpha, kE, p)
    r = r % q
    tempr, tempx, tempy = ext_gcd(kE, q)
    kEinverse = tempx
    SHA = sha1(messagex.encode('utf-8')).hexdigest()
    print('SHA1:',SHA)
    SHA_int = int(SHA, 16)
    s = SHA_int + d * r
    s = s * kEinverse
    s = s % q
    print("r=",r, "s=",s)
    # verify
    tempr, tempx, tempy = ext_gcd(s, q)
    w = tempx
    u1 = w * SHA_int % q
    u2 = w * r % q
    v = square_and_multiply(alpha, u1, p) * square_and_multiply(beta, u2, p) % p % q
    print("v=",v)
      
