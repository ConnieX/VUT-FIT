import memory
import re
import unicodedata
import sys


# get frame where variable is saved
def get_frame(name):
    var = name.split("@")
    if var[0] == "GF":
        return "GF"
    elif var[0] == "LF":
        return "LF"
    else:
        return "TF"


# get value of variable, based of frame
def get_by_frame(frame, name, mem):
    if frame == "GF":
        return mem.global_frame.get(name)
    elif frame == "LF":
        if mem.frame_stack:
            return mem.frame_stack[0].get(name)
        else:
            exit(55)
    else:
        if mem.temp_frame is not None:
            return mem.temp_frame.get(name)
        else:
            exit(55)


# return true if variable is int type, false if not
def is_type_int(var_type, instruction, i, mem):
    if var_type == "":
        exit(56)
    if var_type != "int" and var_type != "var":
        exit(53)
    # if variable is in var - get it and check by regex
    if var_type == "var":
        var_name = instruction[i].text.split("@")
        frame = get_frame(var_name[0])
        var = get_by_frame(frame, var_name[1], mem)
        var = str(var)
        if var == "":
            exit(56)
        if not (re.match(r"^(-|\+)?[0-9]+$", var)):
            exit(53)
    else:
        var = instruction[i].text
    return var


# return type of variable
def return_type(variable, mem):
    frame = get_frame(variable)
    if frame == "GF":
        return mem.global_frame.get_type(variable.split("@")[1])
    elif frame == "LF":
        if mem.frame_stack:
            return mem.frame_stack[0].get_type(variable.split("@")[1])
        else:
            exit(55)
    elif frame == "TF":
        if mem.temp_frame:
            return mem.temp_frame.get_type(variable.split("@")[1])
        else:
            exit(55)


# convert string - escape sequences
def change_string(value):
    string = ""
    skip = 0
    value = str(value)
    for i in range(len(value)):
        if skip != 0:
            skip = skip - 1
        elif value[i] == "\\":
            try:
                numbers = value[i + 1] + value[i + 2] + value[i + 3]
                escape_seq = "\\" + numbers
                char = chr(int(numbers))
                value.replace(escape_seq, char)
                string = string + char
                skip = 3
            except:
                string = string + "\\"
        elif value[i] == "\n":
            skip = skip + 1
        else:
            string = string + value[i]
    return string


# move value to variable
def move(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions
    type1 = instruction[1].attrib['type']
    var1 = instruction[1].text
    if type1 == "var":
        type1 = return_type(var1, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    frame = get_frame(instruction[0].text)
    if var1 == "":
        exit(56)
    # save it to variable
    if frame == "GF":
        mem.global_frame.set(instruction[0].text.split("@")[1], var1, type1)
    elif frame == "LF":
        if mem.frame_stack:
            mem.frame_stack[0].set(instruction[0].text.split("@")[1], var1, type1)
        else:
            exit(55)
    elif frame == "TF":
        if mem.temp_frame:
            mem.temp_frame.set(instruction[0].text.split("@")[1], var1, type1)
        else:
            exit(55)
    return order + 1


# create temporary frame
def createframe(instruction, order, list, labels, mem):
    if mem.temp_frame is None:
        mem.create_frame()
    else:
        mem.temp_frame.variables.clear()
    return order + 1


# push temporary frame to local frame
def pushframe(instruction, order, list, labels, mem):
    if mem.temp_frame:
        mem.frame_stack.insert(0, mem.temp_frame)
        mem.push_frame()
    else:
        exit(55)
    return order + 1


# pop temporary frame from local frame
def popframe(instruction, order, list, labels, mem):
    if not mem.frame_stack:
        exit(55)
    else:
        if mem.temp_frame:
            mem.pop_frame(mem.frame_stack[0])
        else:
            # if temp frame does not exists, create it
            mem.create_frame()
            mem.pop_frame(mem.frame_stack[0])
    return order + 1


# define variable
def defvar(instruction, order, list, labels, mem):
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.add(var.split("@")[1])
    elif frame == "LF":
        if mem.frame_stack:
            mem.frame_stack[0].add(var.split("@")[1])
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.add(var.split("@")[1])
        else:
            exit(55)
    return order + 1


# call function (label)
def call(instruction, order, list, labels, mem):
    mem.call_stack.append(order + 1)
    label = instruction[0].text
    if label in labels:
        order = labels.get(label)  # get order of called instruction
        return int(order)
    else:
        exit(52)


# return from function
def returnOC(instruction, order, list, labels, mem):
    if mem.call_stack is None:
        exit(56)
    else:
        # try to get recent call, if successfull, pop it
        try:
            inst = mem.call_stack[-1]
        except:
            exit(56)
        mem.call_stack.pop()
        return int(inst)


# push value to data_stack
def pushs(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions
    type = instruction[0].attrib['type']
    var = instruction[0].text
    if type == "var":
        var = get_by_frame(get_frame(instruction[0].text), instruction[0].text.split("@")[1], mem)
    if var == "":
        exit(56)
    var = type + "@" + var  # save type@value of variable
    mem.data_stack.insert(0, var)
    return order + 1


# get value from data_stack to variable
def pops(instruction, order, list, labels, mem):
    if not mem.data_stack:
        exit(56)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    type = mem.data_stack[0].split("@")[0]
    value = mem.data_stack[0].split("@")[1]
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], value, type)
        mem.data_stack.pop(0)
    elif frame == "LF":
        if mem.frame_stack:
            mem.frame_stack[0].set(var.split("@")[1], value, type)
            mem.data_stack.pop(0)
        else:
            exit(54)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], value, type)
            mem.data_stack.pop(0)
        else:
            exit(55)
    return order + 1


# sum of two numbers
def add(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']

    var1 = is_type_int(type1, instruction, 1, mem)
    var2 = is_type_int(type2, instruction, 2, mem)

    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    if type1 != "int" or type2 != "int":
        exit(53)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], int(var1) + int(var2), "int")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], int(var1) + int(var2), "int")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], int(var1) + int(var2), "int")
        else:
            exit(55)
    return order + 1


# subtract of two numbers
def sub(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']

    var1 = is_type_int(type1, instruction, 1, mem)
    var2 = is_type_int(type2, instruction, 2, mem)

    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    if type1 != "int" or type2 != "int":
        exit(53)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], int(var1) - int(var2), "int")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], int(var1) - int(var2), "int")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], int(var1) - int(var2), "int")
        else:
            exit(55)
    return order + 1


# multiplication of two numbers
def mul(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']

    var1 = is_type_int(type1, instruction, 1, mem)
    var2 = is_type_int(type2, instruction, 2, mem)

    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    if type1 != "int" or type2 != "int":
        exit(53)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], int(var1) * int(var2), "int")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], int(var1) * int(var2), "int")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], int(var1) * int(var2), "int")
        else:
            exit(55)
    return order + 1


# division of two numbers
def idiv(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']

    var1 = is_type_int(type1, instruction, 1, mem)
    var2 = is_type_int(type2, instruction, 2, mem)

    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    if type1 != "int" or type2 != "int":
        exit(53)

    if int(var2) == 0:
        exit(57)
    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], int(int(var1) / int(var2)), "int")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], int(int(var1) / int(var2)), "int")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], int(int(var1) / int(var2)), "int")
        else:
            exit(55)
    return order + 1


# return bool value of first number is lower than second
def lt(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    # get types and values of attributes
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    # types must be the same
    if type1 != type2 or type1 != "int" and type1 != "string" and type1 != "bool" and type2 != "int" and type2 != "string" and type2 != "bool":
        exit(53)

    if var1 is None:
        var1 = ""
    if var2 is None:
        var2 = ""

    if type1 == "int":
        var1 = int(var1)
        var2 = int(var2)

    if type1 == "string":
        var1 = change_string(var1)
        var2 = change_string(var2)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    result = "false"
    if var1 is not None and var2 is not None and var1 < var2 and var1 != var2:
        result = "true"
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "bool")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "bool")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "bool")
        else:
            exit(55)
    return order + 1


# return bool value if first number is greater than second
def gt(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    # types must be the same
    if type1 != type2 or type1 != "int" and type1 != "string" and type1 != "bool" and type2 != "int" and type2 != "string" and type2 != "bool":
        exit(53)

    if var1 is None:
        var1 = ""
    if var2 is None:
        var2 = ""

    if type1 == "int":
        var1 = int(var1)
        var2 = int(var2)

    if type1 == "string":
        var1 = change_string(var1)
        var2 = change_string(var2)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    result = "false"
    if var1 is not None and var2 is not None and var1 > var2 and var1 != var2:
        result = "true"
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "bool")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "bool")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "bool")
        else:
            exit(55)
    return order + 1


# return bool value if are two values equal
def eq(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)
    # types must be the same
    if (type1 != type2 and type1 != "nil" and type2 != "nil") or (
            type1 != "int" and type1 != "string" and type1 != "bool" and type2 != "int" and type2 != "string" and type2 != "bool" and type1 != "nil" and type2 != "nil"):
        exit(53)

    if var1 is None:
        var1 = ""
    if var2 is None:
        var2 = ""

    if type1 == "int" and var1 != "nil":
        var1 = int(var1)

    if type2 == "int" and var2 != "nil":
        var2 = int(var2)

    if type1 == "string":
        var1 = change_string(var1)
        var2 = change_string(var2)

    var1 = change_string(var1)
    var2 = change_string(var2)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    result = "false"
    if var1 == var2:
        result = "true"
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "bool")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack.insert(var.split("@")[1], result, "bool")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "bool")
        else:
            exit(55)
    return order + 1


# and two bool values
def andOC(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    # types must be the same
    if type1 != "bool" or type2 != "bool":
        exit(53)

    result = "false"
    if var1 == "true" and var2 == "true":
        result = "true"

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "bool")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "bool")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "bool")
        else:
            exit(55)
    return order + 1


# or of two bool values
def orOC(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    # types must be the same
    if type1 != "bool" or type2 != "bool":
        exit(53)

    result = "false"
    if var1 == "true" or var2 == "true":
        result = "true"

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "bool")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "bool")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "bool")
        else:
            exit(55)
    return order + 1


# not of bool value
def notOC(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    var1 = instruction[1].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)

    if type1 == "":
        exit(56)

    # types must be the same
    if type1 != "bool":
        exit(53)

    result = "true"
    if var1 == "true":
        result = "false"
    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "bool")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "bool")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame is not None:
            mem.temp_frame.set(var.split("@")[1], result, "bool")
        else:
            exit(55)
    return order + 1


# convert int to char
def int2char(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    var1 = instruction[1].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)

    if type1 == "":
        exit(56)

    # types must be the same
    if type1 != "int":
        exit(53)

    try:
        result = chr(int(var1))
    except:
        exit(58)
    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "string")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "string")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "string")
        else:
            exit(55)
    return order + 1


# convert string to int
def str2int(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    var1 = instruction[1].text
    type2 = instruction[2].attrib['type']
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)

    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    # types must be the same
    if type1 != "string":
        exit(53)

    # types must be the same
    if type2 != "int":
        exit(53)

    var2 = int(var2)

    if var2 > len(var1) - 1 or var2 < 0:
        exit(58)
    unicodedata.name(var1[var2])
    result = ord(var1[var2])
    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "int")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "int")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "int")
        else:
            exit(55)
    return order + 1


# read value from input
def read(instruction, order, list, labels, mem):
    if mem.file == "":
        value = input()
    else:
        value = mem.file.readline(50)
    type = instruction[1].text
    if type != "int" and type != "string" and type != "bool":
        exit(53)
    if value == "":
        type = "nil"
    acq_type = "nil"
    if value == '':
        value = "nil"
    elif type == "bool":
        if value == "":
            value = "nil"
            type = "nil"
        if value.lower() == "true" or value.lower() == "true\n":
            value = "true"
        else:
            value = "false"
    else:
        if re.match(r"^([-+])?[0-9]+$", value):
            acq_type = "int"
        elif re.match(r"^([^\r\n\t\f\v]|(\[0-9]{3}))*$", value):
            acq_type = "string"
        else:
            exit(53)
        if acq_type != instruction[1].text:
            value = "nil"
            type = "nil"

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], value, type)
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], value, type)
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], value, type)
        else:
            exit(55)
    return order + 1


# write value
def write(instruction, order, list, labels, mem):
    type = instruction[0].attrib['type']
    value = instruction[0].text
    if type == "var":
        type = return_type(value, mem)
        frame = get_frame(value)
        if frame == "GF":
            value = mem.global_frame.get(value.split("@", 1)[1])
        elif frame == "LF":
            if mem.frame_stack:
                value = mem.frame_stack[0].get(value.split("@", 1)[1])
            else:
                exit(55)
        else:
            if mem.temp_frame:
                value = mem.temp_frame.get(value.split("@", 1)[1])
            else:
                exit(55)

    if type == "":
        exit(56)

    if value == "nil":
        if type != "nil":
            print(value, end='')
        else:
            print('', end='')
        return order + 1
    elif type == "bool" and value.lower() == "true":
        print("true", end='')
        return order + 1
    elif type == "bool" and value.lower() != "true":
        print("false", end='')
        return order + 1
    elif type == "":
        print("", end='')
    else:
        value = change_string(value)
        print(value, end='')
    return order + 1


# concatenation of two strings
def concat(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    if type1 != "string" or type2 != "string":
        exit(53)

    if var1 is None:
        var1 = ""
    if var2 is None:
        var2 = ""
    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], var1 + var2, "string")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack.append(var.split("@")[1], var1 + var2, "string")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], var1 + var2, "string")
        else:
            exit(55)
    return order + 1


# return length of string
def strlen(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    var1 = instruction[1].text

    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)

    if type1 == "":
        exit(56)

    if type1 != "string":
        exit(53)

    if var1 is None:
        var_len = 0
    else:
        var_len = len(var1)

    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], var_len, "int")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], var_len, "int")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], var_len, "int")
        else:
            exit(55)
    return order + 1


# get char in the index of given string
def getchar(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type1 = instruction[1].attrib['type']
    var1 = instruction[1].text
    type2 = instruction[2].attrib['type']
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)

    if type1 == "":
        exit(56)

    # types must be the same
    if type1 != "string":
        exit(53)

    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type2 == "":
        exit(56)

    # types must be the same
    if type2 != "int":
        exit(53)

    var2 = int(var2)
    if var2 > len(var1) - 1 or var2 < 0:
        exit(58)
    result = var1[var2]
    # save it to variable
    var = instruction[0].text
    frame = get_frame(var)
    if frame == "GF":
        mem.global_frame.set(var.split("@")[1], result, "string")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(var.split("@")[1], result, "string")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame:
            mem.temp_frame.set(var.split("@")[1], result, "string")
        else:
            exit(55)
    return order + 1


# change char of string at given index
def setchar(instruction, order, list, labels, mem):
    # get type and value of arguments of instructions and control them
    type = return_type(instruction[0].text, mem)
    if type == "":
        exit(56)
    if type != "string":
        exit(53)
    var = get_by_frame(get_frame(instruction[0].text), instruction[0].text.split("@")[1], mem)

    type1 = instruction[1].attrib['type']
    var1 = instruction[1].text
    type2 = instruction[2].attrib['type']
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)

    if type1 == "":
        exit(56)

    if type1 != "int":
        exit(53)

    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type2 == "":
        exit(56)

    # types must be the same
    if type2 != "string":
        exit(53)

    var1 = int(var1)
    if var2 is None:
        exit(58)
    var2 = change_string(var2)
    if var2[0] == "\\":
        var2 = var2[1:]
    else:
        var2 = var2[0]
    if var1 > len(var) - 1 or var1 < 0:
        exit(58)

    # save it to variable
    var = var[:var1] + var2 + var[var1 + 1:]
    name_of_var = instruction[0].text.split("@")[1]
    frame = get_frame(instruction[0].text)
    if frame == "GF":
        mem.global_frame.set(name_of_var, var, "string")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(name_of_var, var, "string")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame is not None:
            mem.temp_frame.set(name_of_var, var, "string")
        else:
            exit(55)
    return order + 1


# save type of value to variable
def typeOC(instruction, order, list, labels, mem):
    type = instruction[1].attrib['type']
    if type == "var":
        type = return_type(instruction[1].text, mem)
    if type == "type":
        type = "string"
    frame = get_frame(instruction[0].text)
    if frame == "GF":
        mem.global_frame.set(instruction[0].text.split("@")[1], type, "type")
    elif frame == "LF":
        if mem.frame_stack:
            try:
                mem.frame_stack[0].set(instruction[0].text.split("@")[1], type, "type")
            except:
                exit(54)
        else:
            exit(55)
    else:
        if mem.temp_frame is not None:
            mem.temp_frame.set(instruction[0].text.split("@")[1], type, "type")
        else:
            exit(55)
    return order + 1


# function for saving label - I save in on another place, just return number of next instruction
def label(instruction, order, list, labels, mem):
    return order + 1


# jump to label
def jump(instruction, order, list, labels, mem):
    label = instruction[0].text
    if label in labels:
        order = labels.get(label)
        return int(order)
    else:
        exit(52)


# jump to label if two vaules are equal
def jumpifeq(instruction, order, list, labels, mem):
    label = instruction[0].text
    if label not in labels:
        exit(52)
    # get types and values of attributes
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)
    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    # types must be the same
    if type1 != type2 and type1 != "nil" and type2 != "nil":
        exit(53)

    var1 = change_string(var1)
    var2 = change_string(var2)

    if str(var1) == str(var2) or (type1 == "nil" and type2 == "nil"):
        order = labels.get(label)
        return int(order) - 1
    else:
        return order + 1


# jump to label if two values are not equal
def jumpifneq(instruction, order, list, labels, mem):
    label = instruction[0].text
    if label not in labels:
        exit(52)
    # get types and values of attributes
    type1 = instruction[1].attrib['type']
    type2 = instruction[2].attrib['type']
    var1 = instruction[1].text
    var2 = instruction[2].text

    # if type is var - find what type is value and get that value
    if type1 == "var":
        type1 = return_type(instruction[1].text, mem)
        var1 = get_by_frame(get_frame(instruction[1].text), instruction[1].text.split("@")[1], mem)

    if type2 == "var":
        type2 = return_type(instruction[2].text, mem)
        var2 = get_by_frame(get_frame(instruction[2].text), instruction[2].text.split("@")[1], mem)

    if type1 == "" or type2 == "":
        exit(56)

    if type1 == "int" and type2 == "int":
        var1 = int(var1)
        var2 = int(var2)
    else:
        var1 = change_string(var1)
        var2 = change_string(var2)

    # types must be the same
    if type1 != type2 and type1 != "nil" and type2 != "nil":
        exit(53)

    if var1 != var2:
        label = instruction[0].text
        order = labels.get(label)
        return int(order) - 1
    else:
        return order + 1


# exit from program with givet exiting code
def exitOC(instruction, order, list, labels, mem):
    # get types and values of attributes
    type = instruction[0].attrib['type']
    var = instruction[0].text

    # if type is var - find what type is value and get that value
    if type == "var":
        type = return_type(instruction[0].text, mem)
        var = get_by_frame(get_frame(instruction[0].text), instruction[0].text.split("@")[1], mem)
    if type == "":
        exit(56)
    if type != "int":
        exit(53)
    if int(var) < 0 or int(var) > 49:
        exit(57)
    else:
        exit(int(var))


# print to stderr
def dprint(instruction, order, list, labels, mem):
    # get types and values of attributes
    type = instruction[0].attrib['type']
    var = instruction[0].text

    # if type is var - find what type is value and get that value
    if type == "var":
        var = get_by_frame(get_frame(instruction[0].text), instruction[0].text.split("@")[1], mem)
    sys.stderr.write(var + "\n")
    return order + 1


# break
def breakOC(instruction, order, list, labels, mem):
    sys.stderr.write("Prints state of GF, LF and TF; order of this instruction etc.\n")
    return order + 1


do_opcodes = {"MOVE": move,
              "CREATEFRAME": createframe,
              "PUSHFRAME": pushframe,
              "POPFRAME": popframe,
              "DEFVAR": defvar,
              "CALL": call,
              "RETURN": returnOC,
              "PUSHS": pushs,
              "POPS": pops,
              "ADD": add,
              "SUB": sub,
              "MUL": mul,
              "IDIV": idiv,
              "LT": lt,
              "GT": gt,
              "EQ": eq,
              "AND": andOC,
              "OR": orOC,
              "NOT": notOC,
              "INT2CHAR": int2char,
              "STRI2INT": str2int,
              "READ": read,
              "WRITE": write,
              "CONCAT": concat,
              "STRLEN": strlen,
              "GETCHAR": getchar,
              "SETCHAR": setchar,
              "TYPE": typeOC,
              "LABEL": label,
              "JUMP": jump,
              "JUMPIFEQ": jumpifeq,
              "JUMPIFNEQ": jumpifneq,
              "EXIT": exitOC,
              "DPRINT": dprint,
              "BREAK": breakOC,
              }


def realize_code(root, labels, read_input):
    # after control
    order_to_do = 0
    instruction_arr = []
    mem = memory.Memory(read_input)
    # save order and opcode of instructions
    for child in root:
        if child.attrib['opcode'].upper() in do_opcodes:
            instruction_arr.append(child.attrib['order'] + "@" + child.attrib['opcode'])
    count_of_inst = len(instruction_arr)  # count of instructions
    if count_of_inst == 0:
        exit(0)
    while True:
        name = instruction_arr[order_to_do].split("@")[1].upper()  # get name of instruction
        order_to_do = do_opcodes[name](root[order_to_do], order_to_do, instruction_arr, labels, mem)  # do instruction
        order_2_do = order_to_do  # because type conversion
        # control index of instruction
        if int(order_2_do) >= count_of_inst:
            break
