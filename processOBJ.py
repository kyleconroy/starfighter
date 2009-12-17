import sys
import re
import os

def processOBJ(path, npath):
    f = open(path, 'r')
    fo = open(npath, 'w')
    
    for line in f:
        vertex = "v -?\d\.\d+ -?\d\.\d+ -?\d\.\d+"
        face = "f \d+ \d+ \d+$"
        tri = "f \d+ \d+ \d+ \d+$"
        line = re.sub(r'(?P<num>\d+)\/\d+', r'\1', line)
        if re.match(vertex,line):
            fo.write(line)
        elif re.match(tri,line):
            fo.write(line)
        elif re.match(face,line):
            fo.write(line)
            
        

if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(1) 
        print "NOT ENOUGH ARGUMENTS"
    processOBJ(sys.argv[1], sys.argv[2])
    # for i in range(1,19):
    #     processOBJ("tmpobj_stone%d.obj" % i, "asteroid%d.obj" % i)