import sys


class Frame:
    variables = dict()

    def __init__(self):
        self.variables = dict()

    def add(self, name):
        if name not in self.variables:
            var = Variable()
            var.name = name
            self.variables[name] = ''
        else:
            sys.exit(52)

    def get(self, name):
        if name in self.variables:
            value = self.variables.get(name);
            if value == "":
                return ""
            else:
                return value.split("@", 1)[1]
        else:
            sys.exit(54)

    def set(self, name, value, type):
        if name in self.variables:
            self.variables[name] = type + "@" + str(value)
        else:
            sys.exit(54)

    def get_type(self, name):
        if name in self.variables:
            value = self.variables.get(name);
            if value == "":
                return ""
            else:
                type = value.split("@", 1)[0]
                if type == "":
                    exit(56)
                else:
                    return type
        else:
            sys.exit(54)

class Memory:
    global_frame: Frame
    temp_frame: Frame
    frame_stack = []
    data_stack = []
    call_stack = []
    file = ""

    def __init__(self, read_input):
        self.global_frame = Frame()
        self.temp_frame = None
        self.frame_stack = list()
        self.data_stack = list()
        self.call_stack = list()
        if read_input != "sys.stdin":
            self.file = read_input


    def push_frame(self):
        self.temp_frame = None

    def pop_frame(self, frame):
        self.temp_frame = frame
        self.frame_stack.pop(0)

    def create_frame(self):
        self.temp_frame = Frame()

class Variable:
    name = "name"
    value = "value"
