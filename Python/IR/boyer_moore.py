#!/usr/bin/env python

"""
Simple implementation of boyer-moore string search algorithm.

I computed delta1 and delta2 by hand on paper first, this reduced
the complexity significantly.

Author: Shuo Yang
"""

pat = 'PICNIC'
string = 'PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC'

delta1 = {
    'P' : 5,
    'I' : 1,
    'C' : 0,
    'N' : 2
    }

delta2 = {
    6 : 1,
    5 : 7,
    4 : 5,
    3 : 9,
    2 : 10,
    1 : 11
    }

def print_scenario(pat, string, num_shift, pos_of_pointer):
    if num_shift == 0:
        print pat
    else:
        print (num_shift-1) * ' ', pat
    print string
    print (pos_of_pointer-2) * ' ', '^'
    print (pos_of_pointer-2) * ' ', '|'
    
def search(pat, string):
    patlen = len(pat)
    strlen = len(string)
    ## i indicates the position of 'string'
    i = patlen
    ## j indicates the position of 'pat' 
    j = patlen

    num_matched = 0
    num_cmp = 0
    total_shift = 0

    while (total_shift + patlen) <= strlen:
        print_scenario(pat, string, total_shift, i)
        while pat[j-1] == string[i-1]:
            num_cmp += 1
            num_matched += 1
            if j == 1: ## found a match
                print 'Found a match'
                print 'Total comparison:', num_cmp
                print 'sublinear coefficient(efficiency matrix): ',
                print float(num_cmp) / (patlen + strlen)
                return True
            j -= 1
            i -= 1
        num_cmp += 1

        ## once run out of the above loop, we have found a mismatch.
        mismatch = string[i-1]
        print 'mismatched char:', mismatch
        
        if delta1.has_key(mismatch): ## 'mismatch' is in 'pat'
            d1 = delta1[mismatch]
        else:
            d1 = patlen
        print 'delta1(%s) = %d' % (mismatch, d1)
        print '#matched = ', num_matched

        d2 = delta2[j]
        print 'delta2(%s) = %d' % (mismatch, d2)

        num_shift = max(d1, d2) - num_matched
        total_shift += num_shift
        print 'shift pat down by', num_shift

        ## update i
        i = i + num_shift + num_matched
        ## reset j
        j = patlen
        num_matched = 0
        print '# of comparison:', num_cmp

    return False

if __name__ == '__main__':
    result = search(pat, string)
    print 'Searching result:', result

"""
Program output:

imsure@ubuntu:~/Programming/py/IR/HW8$ ./boyer_moore.py
PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
     ^
     |
mismatched char: P
delta1(P) = 5
#matched =  0
delta2(P) = 1
shift pat down by 5
# of comparison: 1
     PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
          ^
          |
mismatched char: P
delta1(P) = 5
#matched =  0
delta2(P) = 1
shift pat down by 5
# of comparison: 2
          PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
               ^
               |
mismatched char: D
delta1(D) = 6
#matched =  0
delta2(D) = 1
shift pat down by 6
# of comparison: 3
                PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
                     ^
                     |
mismatched char: O
delta1(O) = 6
#matched =  0
delta2(O) = 1
shift pat down by 6
# of comparison: 4
                      PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
                           ^
                           |
mismatched char: L
delta1(L) = 6
#matched =  0
delta2(L) = 1
shift pat down by 6
# of comparison: 5
                            PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
                                 ^
                                 |
mismatched char: P
delta1(P) = 5
#matched =  0
delta2(P) = 1
shift pat down by 5
# of comparison: 6
                                 PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
                                      ^
                                      |
mismatched char: T
delta1(T) = 6
#matched =  0
delta2(T) = 1
shift pat down by 6
# of comparison: 7
                                       PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
                                            ^
                                            |
mismatched char: P
delta1(P) = 5
#matched =  2
delta2(P) = 5
shift pat down by 3
# of comparison: 10
                                          PICNIC
PETERPIPERPICKEDAPECKOFPICKLEDPEPPERSATTHEPICNIC
                                               ^
                                               |
Found a match
Total comparison: 16
sublinear coefficient(efficiency matrix):  0.296296296296
Searching result: True
imsure@ubuntu:~/Programming/py/IR/HW8$ 

"""
