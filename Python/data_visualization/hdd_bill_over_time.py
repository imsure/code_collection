#!/usr/bin/python env

import pylab
import numpy as np
from matplotlib.ticker import MultipleLocator, FormatStrFormatter

DEBUG = False

date_col = 0
hdd_col = 6
typ_hdd_col = 7
energy_bill_col = 3

date = []
hdd = []
typ_hdd = []
energy_bill = []

def read_data(fname='energy_bill.txt'):
    try:
        fobj = open(fname)
    except IOError:
        print 'open file failed!'
        return

    fobj.readline()
    for line in fobj.readlines():
        items = line.split()
        date.append(items[date_col])
        hdd.append(items[hdd_col])
        typ_hdd.append(items[typ_hdd_col])
        energy_bill.append(items[energy_bill_col])

    if DEBUG:
        print date
        print hdd
        print typ_hdd
        print energy_bill

def draw_chart():
    fig = pylab.figure() # Make a new figure.
    
    # Make a new axis on the 'fig'.
    # This says one row, one column and first subplot.
    ax = fig.add_subplot(1,1,1)
    
    xticks = np.arange(len(date)) # x locations
    width = 0.35 # width of the bar
    
    rects1 = ax.bar(xticks, map(float, hdd), width, color='b', label='HDD')
    rects2 = ax.bar(xticks+width, map(float, typ_hdd), width, color='r', label='Typical HDD')

    # Add a secondary axis line chart.
    ax2 = ax.twinx()
    get_bill = lambda m : float(m.strip('$'))
    if DEBUG:
        print map(get_bill, energy_bill)
    line1 = ax2.plot(xticks, map(get_bill, energy_bill), label='Energy Bill', color='y', linewidth=5)

    majorLocator   = MultipleLocator(4)
    minorLocator   = MultipleLocator(1)    
    ax.xaxis.set_major_locator(majorLocator)
    ax.xaxis.set_minor_locator(minorLocator)

    # We only display each other date because space is too narrow to display all.
    ax.set_xticklabels([d for (index, d) in enumerate(date) if index % 4 == 0])
    
    # Extremely nice function to auto-rotate the x axis labels.
    # It was made for dates (hence the name) but it works
    # for any long x tick labels.
    fig.autofmt_xdate()

    charts = [rects1[0], rects2[0], line1[0]]
    labels = [chart.get_label() for chart in charts]
    ax.legend(charts, labels, loc=0)

    ax.grid()
    ax.set_xlabel("Date")
    ax.set_ylabel("HDD")
    ax2.set_ylabel("Dollar($)")
    ax.set_title("Assignment 1-b: HDD over time and energy bills over time")

    pylab.show()
    fig.savefig('hdd_bill_over_time.png')

def main():
    read_data()
    draw_chart()

if __name__ == '__main__':
    main()
    
