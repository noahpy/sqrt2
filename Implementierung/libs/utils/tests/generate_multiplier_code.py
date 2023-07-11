
c = 1024

dec_string = "1" + c*4*"0"

hex_string = hex(int(dec_string))[2:]

i = len(hex_string)
count = 0

while(i > 0):
    print(f"digits[{count}] = 0x{hex_string[max(0,i-8): i]};")
    i -= 8
    count += 1
