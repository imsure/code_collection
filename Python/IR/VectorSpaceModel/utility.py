#!/usr/bin/env python

import re

def getTerms(content):
    """
    Get all the terms from 'content'. Each term should only
    contain lowercase letters.
    
    Note:
    Do not support stemming, stopwords or other
    normalization, like 'state-of-the-art' or 'O'Neil.
    """
    # Split 'content' by the occrrences of any non-alphanumeric character.
    items = re.split('\W+', content)
    # Elimate empty string ''.
    words = filter(lambda x : x.isalpha(), items)
    return map(lambda x : x.lower(), words)

