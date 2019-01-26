from nltk.tokenize import sent_tokenize
from nltk import ngrams


def lines(a, b):
    """Return lines in both a and b"""
    matches = []
    lines_a = a.split('\n')
    lines_b = b.split('\n')

    for line in lines_a:
        if line in lines_b:
            matches.append(line)

    return matches


def sentences(a, b):
    """Return sentences in both a and b"""
    matches = []
    sent_a = set(sent_tokenize(a))
    sent_b = set(sent_tokenize(b))

    for sent in sent_a:
        if sent in sent_b:
            matches.append(sent)

    return matches


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    matches = []
    grams_tuple_list = ngrams(a, n)

    for gram_tuple in set(grams_tuple_list):
        gram = ''.join(gram_tuple)

        if gram in b:
            matches.append(gram)

    return matches
