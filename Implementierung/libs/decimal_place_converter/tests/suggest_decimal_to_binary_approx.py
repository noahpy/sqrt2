

relative_precision = 0.001
range_limit = 30000

decimal_exp = 1

msg = """This program will calculate powers of 2, and suggest them as approximations for powers of 10,
if they are:\n\t- within a relative precision of {}.\n\t- bigger than the power of 10\n""".format(relative_precision)

print(msg)

decimal_lookup = "};"
binary_lookup = "};"

for binary_exp in range(1,range_limit):
    result = pow(2, binary_exp)
    number = pow(10, decimal_exp)
    lowest_cmp = abs(result - number) / number
    if result >= number and lowest_cmp < relative_precision:
        print("2^{} is approximately 10^{} with a precision of {}".format(binary_exp, decimal_exp, lowest_cmp))
        decimal_lookup = str(decimal_exp) + "," + decimal_lookup
        binary_lookup = str(binary_exp) + "," + binary_lookup
    number = pow(10, decimal_exp + 1)
    middle_cmp = abs(result - number) / number
    if result >= number and middle_cmp < relative_precision:
        print("2^{} is approximately 10^{} with precision of {}".format(binary_exp, decimal_exp + 1, middle_cmp))
        decimal_lookup = str(decimal_exp + 1) + "," + decimal_lookup
        binary_lookup = str(binary_exp) + "," + binary_lookup
    number = pow(10, decimal_exp + 2)
    if abs(result - number) / number < lowest_cmp:
        decimal_exp += 1

decimal_lookup = "{" + decimal_lookup
binary_lookup = "{" + binary_lookup

print("Decimal lookup: {}".format(decimal_lookup))
print("Binary lookup: {}".format(binary_lookup))
