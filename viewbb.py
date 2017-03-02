# Russley Shaw 2017

# Given a 64 bit integer,
# print out the corresponding bit chess board
# using Little-Endian Rank-File Mapping

# Usage: python3 viewbb.py 0x<hex>
# Usage: python3 viewbb.py <int>

# Example: python3 viewbb.py 0x8080808080808080

import sys

def help_msg():
    print("Usage: python3 {} 0x8080808080808080".format(sys.argv[0]))

def main():
    if(len(sys.argv) < 2):
        help_msg()
        exit(1)
    
    try:
        bb = int(sys.argv[1], 0)
    except:
        print("Error: Unable to parse '{}' as an integer".format(sys.argv[1]))
        exit(1)
    
    bblist = list("{0:064b}".format(bb))

    files = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
    pieces = {
        "0": "-",
        "1": "1"
    }

    print("Bit Board: {}".format(hex(bb)))
    print("=================")
    print("F a b c d e f g h ")
    print("R+===============")
    for r in range(7, -1, -1):
        print("{}|".format(r+1), end="")
        for f in range(7, -1, -1):
            print("{} ".format(pieces[bblist[r*8 + f]]), end="")
        print("")
    print("=================")   

if __name__ == "__main__":
    main()