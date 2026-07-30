import random
import sys

def generate_trace():
    for i in range(0,9):
        print(f"{random.randrange(0,9)},", end='')
        
def generate_trace_locality():
    for i in range(0,9):
        num = random.randrange(0,99)
        
        if num < 40:
            print(f"{0},", end='')
        else: 
            print(f"{random.randrange(1,9)},", end='')
     
arg = None

if len(sys.argv) == 2:
    arg = sys.argv[1]

if arg != "-l" and arg is not None: 
    sys.exit("Unknown flag. Pass -l or nothing to get a trace with or without temporal locality.")

if arg is None:
    generate_trace()

else:
    generate_trace_locality()