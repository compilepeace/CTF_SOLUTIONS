#!/usr/bin/python

#   
#   Abhinav Thakur
#   compilepeace@gmail.com
#
#   base64_decoder.py - A script to decode base64 encoding string into ASCII encoding
#
#   This program first converts the string's (file content) each charecter into corresponding charecter
#   value (refer to Base64 encoding table). Then that value is converted into binary representation 
#   of 6 bits each and stored into a list. Then the list is converted into a string so that group of
#   8 bits can be formed which are (binary 8 bits) converted into ordinal values which have 
#   corresponding charecter set according to ASCII encoding. All Charecters are at last converted into
#   a string and printed out.
#   
#   The script was made as an attempt to find the passcode for level11 of overthewire bandit challenge.



import sys 


script = sys.argv[0]
filename = sys.argv[1]


base64_table = {'A': 0, 'B': 1, 'C': 2, 'D': 3, 'E': 4, 'F': 5, 'G': 6, 'H': 7, 'I': 8, 'J': 9, 'K': 10,
                'L': 11, 'M': 12, 'N': 13, 'O': 14, 'P': 15, 'Q': 16, 'R': 17, 'S': 18, 'T': 19, 'U': 20,
                'V': 21, 'W': 22, 'X': 23, 'Y': 24, 'Z': 25, 'a': 26, 'b': 27, 'c': 28, 'd': 29, 'e': 30,
                'f': 31, 'g': 32, 'h': 33, 'i': 34, 'j': 35, 'k': 36, 'l': 37, 'm': 38, 'n': 39, 'o': 40,
                'p': 41, 'q': 42, 'r': 43, 's': 44, 't': 45, 'u': 46, 'v': 47, 'w': 48, 'x': 49, 'y': 50,
                'z': 51, '0': 52, '1': 53, '2': 54, '3': 55, '4': 56, '5': 57, '6': 58, '7': 59, '8': 60,
                '9': 61, '+': 62, '/': 63 }



fo = open(filename, 'r')                    # open file in read mode
string = fo.read().rstrip('\n')             # remove if any '\n' charecter is present at end of file                     



# list will contain the binary representation of each charecter
ls = []



# Iterating through each charecter and converting a binary string for whole base64_string
#
for char in string:
    value = base64_table[char]              # converting each Charecter to its equivalent integer 
    binary = format(value, '06b')           # converting the above integer to binary 
    ls.append(binary)                       # adding the 6 digit binary to a list



# Converting the list into a string and making 8 digit groups
bin_str = ''.join(ls)       


print ""
print "Base64 encoded String :", string
print ""


ls_decoded = []                             # will store the decoded charecters


for i in range(0, len(bin_str), 8):
    b = bin_str[i:i+8]                      # group 8 bits together
    s = '0b' + b                            # adds '0b' in front of each group so that it can be converted 
                                            # later into integer
    ordinal = int(s, 2)                     # converts binary string into integer
    ls_decoded.append(chr(ordinal))         # chr() - converts ordinal number to corresponding ASCII char
    


# Finally printing the Decoded String
print "The decoded string is : ", ''.join(ls_decoded)            

    

