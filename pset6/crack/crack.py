from sys import argv
import crypt

if len(argv) != 2:
    print('Usage: python crack.py hash')
    quit()

possible_hash_chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
possible_hash_chars_len = len(possible_hash_chars)

passed_hash = argv[1]
passed_salt = passed_hash[0:2]
pw_check = [''] * 5

for letter1 in possible_hash_chars:
    pw_check[0] = letter1

    for letter2 in possible_hash_chars:
        pw_check[1] = letter2

        for letter3 in possible_hash_chars:
            pw_check[2] = letter3

            for letter4 in possible_hash_chars:
                pw_check[3] = letter4

                for letter5 in possible_hash_chars:
                    pw_check[4] = letter5

                    pw_to_test = ''.join(pw_check).strip()
                    if crypt.crypt(pw_to_test, passed_salt) == passed_hash:
                        print(pw_to_test)
                        quit()

print("Password not found")
