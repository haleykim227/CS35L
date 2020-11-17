#!/usr/bin/python

# Haley Kim
# 405111152


import random, sys, argparse, string
from optparse import OptionParser

class shuf:

    def __init__(self, input_list, count, repeat):
        self.input_list = input_list
        self.repeat = repeat
        self.count = count

        # shuffle the lines of input
        random.shuffle(self.input_list)

    def shuffleline(self):
        # nothing done if length of input is 0
        if len(self.input_list) == 0:
            return
        
        # if repeat option used, pick randomly
        if self.repeat:
            while self.count > 0:
                sys.stdout.write(random.choice(self.input_list))
                self.count -= 1
                
        # If repeat option not used, pick in order
        if not self.repeat:
            for i in range(0, self.count):
                sys.stdout.write(self.input_list[i])


def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE
  or:  %prog -i LO-HI [OPTION]...
Write a random permutation of the input lines to standard output."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)

    # -n, --head-count
    # output at most COUNT lines
    parser.add_option("-n", "--head-count",
                     action="store", dest="count", default=sys.maxsize,
                     help="output at most COUNT lines")
    
    # -i, --input-range = LOW-HIGH
    # treat each number LOW through HIGH as input line
    parser.add_option("-i", "--input-range",
                      action="store", dest="inputRange", default="",
                      help="treat each number LO through HI as an input line")

    # -r, --repeat
    # output lines may be repeated
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines can be repeated")

    options, args = parser.parse_args(sys.argv[1:])
    
    # -n, --head-count
    try:
        count = int(options.count)
    except:
        parser.error("invalid COUNT: {0}".
                     format(options.count))
    if count < 0:
        parser.error("negative count {0}".
                     format(count))

    # -i, --input-range = LOW-HIGH
    inputRange = options.inputRange

    # -r, --repeat
    repeat = options.repeat

    # if option i
    if len(inputRange) > 0:

        # -i option cannot take arguments
        if len(args) != 0:
            parser.error("extra operand '{0}'".
                         format(args[0]))

        # input range cannot accept only one number
        try:
            dash = inputRange.index('-')
        except ValueError as e:
            parser.error("invalid input range: '{0}'".
                         format(options.inputRange))
        if dash == 0:
            parser.error("invalid input range: '{0}'".
                         format(options.inputRange))

        # check for integer values before and after '-'
        first, last = inputRange.split("-")
        try:
            firstNum = int(first)
        except ValueError as e:
            parser.error("invalid input range: '{0}'".
                         format(options.inputRange))
        try:
            lastNum = int(last)
        except ValueError as e:
            parser.error("invalid input range: '{0}'".
                         format(options.inputRange))

        # check that first value < last value of range
        if first > last:
            parser.error("invalid input range: '{0}'".
                         format(options.inputRange))

        # store number range into input
        input_list = list(range(firstNum,lastNum+1))

        # convert integer list into string list with newlines
        for i in range(len(input_list)):
            input_list[i] = str(input_list[i]) + "\n"

    else:
        # accept input from stdin if there are either zero non-option arguments or "-"
        if len(args) == 0 or (len(args) == 1 and args[0] == "-"):
            input_list = sys.stdin.readlines()
        
        # accept input from a file
        elif len(args) == 1:
            # check that input file is valid
            try:
                f = open(args[0], 'r')
                input_list = f.readlines()
                f.close()
            except IOError as e:
                errno, strerror = e.args
                parser.error("I/O error({0}): {1}".
                             format(errno,strerror))

        # cannot accept more than one argument
        else:
            parser.error("extra operand '{0}'".
                         format(args[1]))

    # check if count is greater than input, and repeat is not used
    if count > len(input_list) and not repeat:
        count = len(input_list)

    generator = shuf(input_list, count, repeat)
    generator.shuffleline()

if __name__ == "__main__":
    main()
