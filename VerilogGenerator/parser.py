from lark import Lark


fp = open("grammar.lark")
grammar = fp.read()
fp.close()

parser = Lark(grammar)

files = ["../CoreLib/FetchStage.cpp"]


def preprocess(lines):
    result = ""
    for l in lines:
        result += l
    return result

for file in files:
    with open(file) as fp:
        lines = fp.readlines()
        data = preprocess(lines)
        tree = parser.parse(data)
        print(tree)