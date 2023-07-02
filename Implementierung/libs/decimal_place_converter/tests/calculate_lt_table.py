
decimal_range = 1001

msg = "This program calculates the lookup table for the lowest precision binary place needed for a given decimal place precision."

print(msg)

current = 1
binary_exp = 0

binary_lt_table = []

while current < decimal_range:
    while pow(2, binary_exp) < pow(10, current):
        binary_exp += 1
    binary_lt_table.append(binary_exp)
    current += 1

print("Binary LT Table: ", binary_lt_table)

