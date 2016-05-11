from sys import stdin

# read map
lines = [line.rstrip() for line in stdin.readlines()]
height = len(lines)
width = len(lines[0])

def make_cell(c, pos):
    return {
        'neighbors': [],
        'contents': c,
        'dist': -1,
        'pos': pos,
    }

# transform it into a graph (cells have contents)
graph = []
for y in range(height):
    row = []
    for x in range(width):
        row.append(make_cell(lines[y][x], (x, y)))
    graph.append(row)

def try_connect(here, x, y):
    if (0 <= x < width) and (0 <= y < height): # in bounds
        there = graph[y][x]
        if there['contents'] != '#':
            # print("from {} to {} {}".format(here['pos'], x, y))
            here['neighbors'].append(there)

# hook up neighbors
for y in range(height):
    for x in range(width):
        here = graph[y][x]
        try_connect(here, x, y-1)
        try_connect(here, x, y+1)
        try_connect(here, x-1, y)
        try_connect(here, x+1, y)

# find start and end nodes
start, end = {}, {}
for y in range(height):
    for x in range(width):
        here = graph[y][x]
        if here['contents'] == 's':
            start = here
            print("start {} {}".format(x, y))
        if here['contents'] == 'e':
            print("end {} {}".format(x, y))
            end = here

# # print graph
# for row in graph:
#     for col in row:
#         print(len(col['neighbors']), end="")
#     print("")

# do bfs
start['dist'] = 0
q = [start]
nxt = []
while q or nxt:
    # print("@@", len(q))
    while q:
        here = q.pop()
        # print("at {} with dist={}".format(here['pos'], here['dist']))
        if here['contents'] == 'e':
            # print('>>>>>>>>>>>>>>> found the exit!')
            q = nxt = [] # break
        else:
            for there in here['neighbors']:
                if there['dist'] == -1: # unexplored
                    there['dist'] = here['dist'] + 1
                    nxt.append(there)

    q = nxt
    nxt = []

def colorize(c):
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    if c == ' ':
        c = '*'
    return OKGREEN + c + ENDC

# mark path back
if end['dist'] != -1:
    here = end
    while here['contents'] != 's':
        here['contents'] = colorize(here['contents'])
        for n in here['neighbors']:
            d = n['dist']
            if d == here['dist'] - 1:
                here = n

# print solution
if end['dist'] == -1:
    print('impossible')
else:
    # print graph
    for row in graph:
        for col in row:
            print(col['contents'], end="")
        print("\n", end="")
