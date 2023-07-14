
from numpy import log
from math import ceil

test_inputs = [0, 1, 7, 11, 99, 200, 293, 299, 539, 777, 987, 1021, 2855, 3366, 5782, 13456, 294859322]

test_msg_1 = "test_decimal_conv({},{});"
test_msg_2 = "test_hex_conv({},{});"

for i in test_inputs:
    result = ceil(i * log(10)/log(2))
    print(test_msg_1.format(i, result))

print("\n"*3)

for i in test_inputs:
    result = i * 4
    print(test_msg_2.format(i, result))


