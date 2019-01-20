from cs50 import get_string
from sys import argv, exit


def main():

    if len(argv) != 2:
        print('Usage: python bleep.py dictionary')
        exit(1)

    # Read in banned word list
    with open(argv[1]) as f:
        banned = f.read().split('\n')

    mess_in = get_string("What message would you like to censor?\n")
    mess_in_words = mess_in.split()

    # Create output list with words censored as needed
    mess_out_words = []
    for word in mess_in_words:
        if word.lower() in banned or word.upper() in banned:
            mess_out_words.append('*' * len(word))
        else:
            mess_out_words.append(word)

    print(' '.join(mess_out_words))


if __name__ == "__main__":
    main()
