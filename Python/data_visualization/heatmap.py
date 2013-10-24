#!/usr/bin/env python

import pylab

DEBUG = True

years = ['08', '09', '10', '11', '12']
months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
hdd_col = 6

def fill_matrix(y2r, m2c, m):
    try:
        fobj = open('energy_bill.txt')
    except IOError:
        print 'Open file failed!'

    fobj.readline() # Just to ignore the first line.
    for line in fobj.readlines():
        items = line.split()
        month, year = items[0].split('-')
        m[y2r[year]][m2c[month]] = items[6]

    if DEBUG:
        print m

    f = pylab.figure()
    pylab.subplots_adjust(top=0.85)
    ax = f.add_subplot(111)
    cax = ax.matshow(m, interpolation='nearest')
    f.colorbar(cax)
    pylab.xticks(range(len(months)), months)
#    pylab.xticks(range(len(months)), months, rotation=90)
    pylab.yticks(range(len(years)), years)
    ax.set_xticklabels(months)
    ax.set_yticklabels(years)
    f.suptitle('Assignment 1-c: Heat Map (White color means no data available)', fontsize=20)
    ax.xaxis.tick_top() 
    #ax.set_xlabel('Month', ha='center', va='top')
    ax.set_xlabel('HDD (Heating Degree Days)', ha='right')
    ax.set_ylabel('Year')
    pylab.show()
    f.savefig('heatmap.png')

def main():
    num_rows = len(years)
    num_cols = len(months)

    # Year represents row in matrix.
    year_to_row = dict(zip(years, range(num_rows)))
    # Month represents column in matrix.
    month_to_col = dict(zip(months, range(num_cols)))

    if DEBUG:
        print year_to_row
        print month_to_col

    matrix = pylab.zeros((num_rows, num_cols))
    for row_num, row in enumerate(matrix):
        for col_num, col in enumerate(row):
            matrix[row_num][col_num] = None

    if DEBUG:
        print 'type of matrix is %s.' % type(matrix)
        print matrix

    fill_matrix(year_to_row, month_to_col, matrix)

if __name__ == '__main__':
    main()

