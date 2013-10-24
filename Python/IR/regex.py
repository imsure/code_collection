#!/usr/bin/env python

"""
Python code for regular expressions as appeared in HW4.

Regular expression that matches "Tom" or "Thomas":
\bTom\b|\bThomas\b

Regular expression that mathces a 5 or 9 digits zip code:
(^|\s+|:)\d{5}(-\d{4})?($|\s+)
"""
import re
import sys

while True:
    line = raw_input('Enter a string for exercise-1')
    match = re.findall(r'\bTom\b|\bThomas\b', line)
    if match:
        print 'match found:', match
    else:
        print 'match not found'

    print 'Enter a string for exercise-2:'
    line = raw_input()
    match = re.search(r'(^|\s+|:)\d{5}(-\d{4})?($|\s+)', line)
    if match:
        print 'match found:', match.group()
    else:
        print 'match not found'

"""
Test Results:

imsure@ubuntu:~/Programming/py/IR$ ./regex.py 
Enter a string for exercise-1:
I am Tom, and this is Thomas.
match found: ['Tom', 'Thomas']
Enter a string for exercise-2:
zip code: 55347
match found:  55347
Enter a string for exercise-1:
Tomas
match not found
Enter a string for exercise-2:
  55347-1098
match found:   55347-1098
Enter a string for exercise-1:
Thom
match not found
Enter a string for exercise-2:
6543210 
match not found
Enter a string for exercise-1:
thomas
match not found
Enter a string for exercise-2:
abc509123
match not found
Enter a string for exercise-1:
TomThomas
match not found
Enter a string for exercise-2:
55105-104134
match not found
Enter a string for exercise-1:
  tom-Thomas
match found: ['Thomas']
Enter a string for exercise-2:
zip-code: 55104-2345 
match found:  55104-2345
Enter a string for exercise-1:
Tom
match found: ['Tom']
Enter a string for exercise-2:
zipcode:55523
match found: :55523
Enter a string for exercise-1:
f
match not found
Enter a string for exercise-2:
zip code:551052
match not found
Enter a string for exercise-1:
a
match not found
Enter a string for exercise-2:
zip code:55105-9876
match found: :55105-9876

"""
