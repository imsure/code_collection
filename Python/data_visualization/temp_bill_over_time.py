#!/usr/bin/python env

import pylab
import numpy as np
from matplotlib.ticker import MultipleLocator, FormatStrFormatter

DEBUG = False

date_col = 0
avg_temp_col = 4
typ_temp_col = 5
energy_bill_col = 3

date = []
avg_temp = []
typical_temp = []
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
        avg_temp.append(items[avg_temp_col])
        typical_temp.append(items[typ_temp_col])
        energy_bill.append(items[energy_bill_col])

    if DEBUG:
        print date
        print avg_temp
        print typical_temp
        print energy_bill

def draw_chart():
    fig = pylab.figure() # Make a new figure.
    
    # Make a new axis on the 'fig'.
    # This says one row, one column and first subplot.
    ax = fig.add_subplot(1,1,1)
    
    xticks = np.arange(len(date)) # x locations
    width = 0.35 # width of the bar
    
    rects1 = ax.bar(xticks, map(float, avg_temp), width, color='b', label='Average Temp')
    rects2 = ax.bar(xticks+width, map(float, typical_temp), width, color='r', label='Typical Temp')

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
    ax.set_ylabel("Temperature(F)")
    ax2.set_ylabel("Dollar($)")
    ax.set_title("Assigment 1-a: Temperatures over time and energy bills over time")

    pylab.show()
    fig.savefig('temp_bill_over_time.png')

def main():
    read_data()
    draw_chart()

if __name__ == '__main__':
    main()
    
