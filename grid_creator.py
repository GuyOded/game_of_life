import sys
from math import isnan

def main(argv):
    file_name = "grid.txt"

    l = len(argv)
    if l != 2:
        print 'Wrong number of arguments ({0} for 2)'.format(l)
        sys.exit(1)
    
    death_char = argv[0]
    if len(death_char) != 1:
        print 'You must provide a single character for the first argument'
        sys.exit(1)

    grid_size = argv[1]
    try:
        int(argv[1])
    except ValueError:
        print 'The size you have provided \"{0}\" is not a number'.format(grid_size)
        print 'Use the following line to run the program:\npython <program_name>.py <char> <size>'
        sys.exit(1)

    grid_size = int(argv[1])

    create_grid_file(grid_size, death_char, file_name)

    print "The file {0} now contains a grid that his size is {1}".format(file_name, grid_size)

def create_grid_file(size, death, file_name):
    temp_file = open(file_name, 'w')
    for i in range(0, size):
        temp_file.write(death*size)
        if i != (size-1):
            temp_file.write('\n')
    temp_file.close()




if __name__ == "__main__":
    main(sys.argv[1:])