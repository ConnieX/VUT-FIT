import re

# names of instructions of IPPcode20 and types of attributes that they use
grammar = {"MOVE": ["var", "symbol"],
           "CREATEFRAME": [],
           "PUSHFRAME": [],
           "POPFRAME": [],
           "DEFVAR": ["var"],
           "CALL": ["label"],
           "RETURN": [],
           "PUSHS": ["symbol"],
           "POPS": ["var"],
           "ADD": ["var", "symbol", "symbol"],
           "SUB": ["var", "symbol", "symbol"],
           "MUL": ["var", "symbol", "symbol"],
           "IDIV": ["var", "symbol", "symbol"],
           "LT": ["var", "symbol", "symbol"],
           "GT": ["var", "symbol", "symbol"],
           "EQ": ["var", "symbol", "symbol"],
           "AND": ["var", "symbol", "symbol"],
           "OR": ["var", "symbol", "symbol"],
           "NOT": ["var", "symbol"],
           "INT2CHAR": ["var", "symbol"],
           "STRI2INT": ["var", "symbol", "symbol"],
           "READ": ["var", "type"],
           "WRITE": ["symbol"],
           "CONCAT": ["var", "symbol", "symbol"],
           "STRLEN": ["var", "symbol"],
           "GETCHAR": ["var", "symbol", "symbol"],
           "SETCHAR": ["var", "symbol", "symbol"],
           "TYPE": ["var", "symbol"],
           "LABEL": ["label"],
           "JUMP": ["label"],
           "JUMPIFEQ": ["label", "symbol", "symbol"],
           "JUMPIFNEQ": ["label", "symbol", "symbol"],
           "EXIT": ["symbol"],
           "DPRINT": ["symbol"],
           "BREAK": [],
           }


# control if value of attribute match type of attribute
def control_types(type, value, instruction, labels):
    if type == 'int':
        if not (re.match(r"^(-|\+)?[0-9]+$", value)):
            exit(32)
    elif type == 'string':
        if value is None:
            pass
        elif not (re.match(r"^([^\r\n\t\f\v]|(\[0-9]{3}))*$", value)):
            exit(32)
    elif type == 'var':
        if not (re.match(r"(GF|LF|TF)@[ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z \-$&%*!?]+["r"ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z0-9_\-$&%*!?\S]*", value)):
            exit(32)
    elif type == 'bool':
        value = value.lower()
        if value != 'true' and value != 'false':
            exit(32)
    elif type == 'nil':
        if value != 'nil':
            exit(32)
    elif type == 'label':
        if not (re.match(r'^[ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z_\-$&%*!?]+[ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z0-9\S]*', value)):
            exit(32)
    else:
        exit(32)


# control if arguments are same type as expected
def control_args(opcode, arg_count, instruction, labels):
    # control of count of arguments of instructions
    if arg_count != len(grammar.get(opcode)):
        exit(32)

    # if opcode is LABEL, save name of label to dictionary
    if opcode == "LABEL":
        if instruction[0].text in labels:
            exit(52)
        else:
            labels[instruction[0].text] = instruction.attrib['order']

    # if there are some arguments, check them
    if arg_count > 0:
        instruction[:] = sorted(instruction, key=lambda child: (child.tag, child.tag))  # sort arguments
        # if instruction attribute is missing
        if instruction.tag != "instruction":
            exit(32)
        i = 0  # counter of arguments
        types = grammar.get(opcode)  # get expected types of arguments
        for argument in instruction:
            if argument.tag != ("arg" + str(i+1)) or 'type' not in argument.attrib:
                exit(32)
            else:
                type = argument.attrib['type']
                # if actual type doesn't match expected type of attribute
                if type != types[i]:
                    # if expected type is symbol - type can be var or bool or int or string or nil
                    # so if expected type is not symbol or (that means symbol is expected) type is not var or bool or int or string or nil -> error
                    if (types[i] != "symbol" or (type != "var" and type != "bool" and type != "int" and type != "string" and type != "nil")):
                        exit(32)
                    else:
                        control_types(argument.attrib['type'], argument.text, instruction, labels)
            i = i + 1


# control structure of given xml
def control_xml(root, labels):
    # control language
    if "language" in root.attrib:
        language = root.attrib['language']
        language = language.upper()
    else:
        exit(32)
    if language != "IPPCODE20":
        exit(32)

    counter = 0  # counter for controlling order of instructions
    number = 0  # variable for controlling value of instruction order
    for child in root:
        counter = counter + 1  # increase counter of instructions
        # if order element is missing
        if "order" not in child.attrib:
            exit(32)
        else:
            # try convert order attribute to int and save it
            try:
                number = (int(child.attrib['order']))
            except:
                exit(32)
        # if order of instruction is negative or zero -> error
        if number < 1:
            exit(32)
        # if one number in counter is missing, change it to missing number (if there are inst. 1, 2, 4 -> change 4 to 3)
        if number != counter:
            child.attrib['order'] = str(counter)

        # if opcode element is missing
        if "opcode" not in child.attrib:
            exit(32)
        else:
            name = child.attrib['opcode'].upper()
            # if instruction is in grammar, count number of arguments and check, if it matches expected count of args
            if name in grammar:
                arg_count = 0
                for arg in child:
                    arg_count = arg_count + 1
                # call function for checking actual value of argument
                control_args(name, arg_count, child, labels)
            # else instruction was not found in grammar -> error
            else:
                exit(32)
