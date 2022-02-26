from typing import List
from Method import Method
from MyTransformer import MyTransformer
from PrintStream import PrintStream
from lark import Lark, logger
import logging


logger.setLevel(logging.DEBUG)

output_path = "test/"

def preprocess(lines):
    result = ""
    for l in lines:
        k = l.strip()
        if k.startswith("#"):
            l = "// " + l
        
        result += l
    return result


def createAST(tree):
    ast = MyTransformer().transform(tree)
    return ast


def create_state_switch(methods: List[Method]):
    switched = []
    for m in methods:
        k = m.toStateSwitch()
        switched.append(k)
    return switched

def generate(methods: List[Method], ps: PrintStream):
    for m in methods:
        m.generate(ps)

def parseFile(parser, file):
    with open(file) as fp:
        lines = fp.readlines()
        data = preprocess(lines)
        print("PARSING: " + file)
        tree = parser.parse(data)
        return tree

def get_file_name(file: str) -> str:
    last_slash = file.rfind("/")
    module_name = file[last_slash+1:]     
    for remove_ext in [".cpp", ".h"]:
        module_name = module_name.replace(remove_ext, "")
    return module_name + ".sv"

def main():
    fp = open("grammar.lark")
    grammar = fp.read()
    fp.close()

    parser = Lark(grammar, parser='lalr', debug=True)


    inc_path = "../CoreLib/include/"
    includes = ["Defines.h", "VectorValue.h", "Packets.h", "RegisterFile.h", "DecodeToExecuteBus.h", "FetchToDecodeBus.h", "StoreToFetchBus.h"]
    for file in includes:
        tree = parseFile(parser, inc_path + file)
        ast = createAST(tree)
        output_filename = get_file_name(file)
        with open(output_path + output_filename, "w") as fp: 
            ps = PrintStream(fp)
            ast.generate_decls(ps)
            methods = ast.lower_ast()
            generate(methods, ps)


    files = ["../CoreLib/FetchStage.cpp", "../CoreLib/DecodeStage.cpp", "../CoreLib/ExecuteStage.cpp", "../CoreLib/DRAM.cpp", "../CoreLib/include/MemoryBus.h"]
    for file in files:
        module_name = get_file_name(file)

        
        tree = parseFile(parser, file)
        #print(tree)

        ast = createAST(tree)
        #print(f"AST = ")
        #ast.pretty()

        methods = ast.lower_ast()

        methods = create_state_switch(methods)

        print("\tGEN: " + module_name)
        with open(output_path + module_name, "w") as fp: 
        
            ps = PrintStream(fp)
            ast.generate_decls(ps)
            
            generate(methods, ps)
            

main()