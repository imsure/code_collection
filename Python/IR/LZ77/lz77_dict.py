#!/usr/bin/env python

"""
Implementation of LZ77 compression algorithm using
a lookup dictionary.

Author: Shuo Yang
"""

from operator import itemgetter

text = 'AABABBAA'
#text = 'ABBAABBAABABBAAAABAABBA'

def init_dict(text):
    """
    Initialize the dictionary to contain all blocks
    of length one.
    """
    d = {}
    index = 0
    for c in text:
        if not d.has_key(c):
            d[c] = index
            index += 1
    return d, index

## Global variable to store final coding list.
code = []

def lz77_encode(table, index, text):
    """
    Implementation of LZ77 encoding algorithm.
    
    Thinking recursively:
    1. Start from the beginning of the 'text', find
       the longest block W that has appeared in the 'table'.
    2. Encode W by its index in the 'table'.
    3. Add W followed by the first symbol of the next block to
       the table with 'index' as its value, then update 'index'.
    4. Relocate the beginning of the 'text' to the next block,
       then repeat 1,2,3 until we reach to the end of 'text'.

    Args:
    table --> lookup table.
    index --> new index need to be added to 'table'.
    text --> text to be compressed.

    Return: None
    """
    if len(text) != 0:
        ## Look up for the longest block W which
        ## has appeared in the lookup table.
        position = 1 ## indicate current position.
        while table.has_key(text[0 : position]):
            current_code = table[ text[0 : position] ]
            position += 1
            ## See if we have reached to the end of the text.
            if position > len(text):
                break
        ## Add new entry only when we have not reached to the end of the 'text'.
        if position <= len(text):
            ## Add entry for the block of W followed by the first symbol.
            table[ text[0 : position] ] = index
        ## Encode
        code.append(current_code)
        print '\nencode', text[0 : position-1], '--->', current_code
        print '\ncurrent table:'
        print_table(table)
        
        ## update index for the next iteration.
        index += 1
        ## Move one block ahead.
        lz77_encode(table, index, text[position-1 : ])

def print_table(table):
    """
    Print a table(dictionry) sorted by value.
    """
    items = table.items()
    items.sort(key=itemgetter(1))
    print items


def main():
    lookup_table, next_index = init_dict(text)
    print 'original table:'
    print_table(lookup_table)

    print '\ntext needed to be compressed:'
    print text

    lz77_encode(lookup_table, next_index, text)
    print '\nfinal code:'
    print code

if __name__ == '__main__':
    main()


"""
Output:

imsure@ubuntu:~/Programming/py/IR/HW5/LZ77$ ./lz77_dict.py
original table:
[('A', 0), ('B', 1)]

text needed to be compressed:
AABABBAA

encode A ---> 0

current table:
[('A', 0), ('B', 1), ('AA', 2)]

encode A ---> 0

current table:
[('A', 0), ('B', 1), ('AA', 2), ('AB', 3)]

encode B ---> 1

current table:
[('A', 0), ('B', 1), ('AA', 2), ('AB', 3), ('BA', 4)]

encode AB ---> 3

current table:
[('A', 0), ('B', 1), ('AA', 2), ('AB', 3), ('BA', 4), ('ABB', 5)]

encode BA ---> 4

current table:
[('A', 0), ('B', 1), ('AA', 2), ('AB', 3), ('BA', 4), ('ABB', 5), ('BAA', 6)]

encode A ---> 0

current table:
[('A', 0), ('B', 1), ('AA', 2), ('AB', 3), ('BA', 4), ('ABB', 5), ('BAA', 6)]

final code:
[0, 0, 1, 3, 4, 0]

"""
