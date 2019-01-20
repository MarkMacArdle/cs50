from cs50 import get_int

card = str(get_int("Number: "))
reversed_card = card[::-1]
check_sum = 0

for i in range(len(reversed_card)):
    digit = int(reversed_card[i])

    if i % 2 == 0:
        check_sum += digit
    else:
        doubled_digit = digit * 2
        if doubled_digit >= 10:
            first_digit = doubled_digit % 10
            second_digit = (doubled_digit - first_digit) / 10
            check_sum += first_digit + second_digit
        else:
            check_sum += doubled_digit

# Default case
card_provider = 'INVALID'

if check_sum % 10 == 0:
    if card[0] == '4' and (len(card) == 13 or len(card) == 16):
        card_provider = 'VISA'
    elif (card[0:2] == '34' or card[0:2] == '37') and len(card) == 15:
        card_provider = 'AMEX'
    elif int(card[0:2]) >= 51 and int(card[0:2]) <= 55 and len(card) == 16:
        card_provider = 'MASTERCARD'

print(card_provider)
