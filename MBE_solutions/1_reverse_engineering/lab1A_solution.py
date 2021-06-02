
input_string = "!!! !!"
serial_key = (ord(input_string[3]) ^ 0x1337) + 0x5eeded

print ("Starting with seed value: " + str(serial_key))

for c in input_string:
    print("processing for character: " + c + ", ord -> " + str(hex(ord(c))))

    origin = ord(c) ^ serial_key
    eax = origin
    edx = (eax * 0x88233B2B) >> 32
    eax = (((((eax - edx) >> 1) + edx) >> 0xa) * 0x539)

    serial_key += (origin - eax)

print ("Serial Key for " + input_string + ": " + str(serial_key))     
