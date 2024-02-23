import xml.etree.ElementTree as ET
import sys
from control_tree import control_xml
from realize_code import realize_code

# arguments
xml_input = sys.stdin
read_input = sys.stdin
for argument in sys.argv:
    arg = argument.split("=")
    if arg[0] == "--help":
        print(
            "Help me if you can, I'm feeling down\nAnd I do appreciate you being 'round\nHelp me get my feet back on the ground\nWon't you please, please help me?\n")
        exit(0)
    elif arg[0] == "--source":
        xml_input = arg[1]
    elif arg[0] == "--input":
        read_input = arg[1]

if xml_input == sys.stdin and read_input == sys.stdin:
    exit(10)

# try to open given files
if read_input != sys.stdin:
    try:
        read_input = open(read_input, "r")
    except:
        exit(11)

if xml_input != sys.stdin:
    try:
        xml_input = open(xml_input, "r")
    except:
        exit(11)

# get tree from xml file
try:
    tree = ET.parse(xml_input)
except:
    exit(31)

try:
    root = tree.getroot()
except:
    exit(32)

# sorting instructions by their order
try:
    root[:] = sorted(root, key=lambda child: (child.tag, int(child.get('order'))))
except:
    exit(32)

# tags for control
root_tags = ["language", "name", "description"]
tags = ["opcode", "order"]

# check attributes in root
if "language" not in root.attrib:
    exit(32)

for attrib in root.attrib:
    if attrib not in root_tags:
        exit(32)

# control duplicity of instruction order
last_order = 0  # variable for inst. order of last inst.
for child in root:
    # if it's first inst.
    if last_order == 0:
        last_order = child.attrib['order']
    else:
        current_order = child.attrib['order']  # variable for inst. order of current inst.
        if last_order == current_order:
            exit(32)
        else:
            last_order = current_order
    # control attributes in child of root
    for attrib in child.attrib:
        if attrib not in tags:
            exit(32)

labels = dict()
control_xml(root, labels)  # control xml input
realize_code(root, labels, read_input)  # do program
xml_input.close()
read_input.close()
