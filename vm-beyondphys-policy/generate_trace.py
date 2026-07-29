import random
import sys


def generate_trace():
    for i in range(0,9):
        print(f"{random.randrange(0,9)},", end='')

generate_trace()
