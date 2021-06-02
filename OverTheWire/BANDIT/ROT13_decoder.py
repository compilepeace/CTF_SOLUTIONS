# Abhinav Thakur
# compilepeace@gmail.com
#
# ROT13_decoder.py - This script is built for bandit11 level. This script rotates all the upper case
#                    as well as lower case letters by 13 places.
#                    We will test the example : Gur cnffjbeq vf 5Gr8L4qetPEsPk8htqjhRK8XSP6x2RHh
#

string = raw_input("Please Enter the string to be rotated -> ")



rotated = []



for charecter in string:
    ordinal = ord(charecter)    


    # if charecter is a upper-case letter
    if ordinal in range(65, 91):
        
        # subtract 13 normally
        if ordinal > 78:
            rotated.append(chr(ordinal - 13))
        
        # subtract 13 such that result lies in range(65, 91)
        # i.e. the charecter is rotated 13 position left (or right does'nt matter)
        else:
            rotated.append(chr(78 + (ordinal - 65)))        


    # if charecter is a lower-case letter
    elif ordinal in range(97, 123):
        
        # subtract 13 normally
        if ordinal > 109:
            rotated.append(chr(ordinal - 13))
        
        # subtract 13 such that the result lies in range(97, 123)
        # i.e. the charecter is rotated 13 positions left/right
        else:
            rotated.append(chr(110 + (ordinal - 97)))

    else: 
        rotated.append(charecter)


print "%s" %(''.join(rotated))

