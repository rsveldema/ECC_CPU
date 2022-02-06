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


def create_state_switch(methods):
    switched = []
    for m in methods:
        k = m.toStateSwitch()
        switched.append(k)
    return switched

def generate(methods, ps: PrintStream):
    for m in methods:
        m.generate(ps)

def parseFile(parser, file):
    with open(file) as fp:
        lines = fp.readlines()
        data = preprocess(lines)
        print("PARSING: " + file)
        tree = parser.parse(data)
        return tree


def main():
    fp = open("grammar.lark")
    grammar = fp.read()
    fp.close()

    parser = Lark(grammar, parser='lalr', debug=True)

    with open(output_path + "global_decls.sv", "w") as fp: 
        ps = PrintStream(fp)

        includes = ["../CoreLib/include/Defines.h", "../CoreLib/include/Packets.h"]
        for file in includes:
            tree = parseFile(parser, file)
            ast = createAST(tree)
            ast.generate_decls(ps)
        
            


    files = ["../CoreLib/FetchStage.cpp", "../CoreLib/include/MemoryBus.h"]
    for file in files:
        
        last_slash = file.rfind("/")
        module_name = file[last_slash+1:]
     
        for remove_ext in [".cpp", ".h"]:
            module_name = module_name.replace(remove_ext, "")
        
            tree = parseFile(parser, file)
            #print(tree)

            ast = createAST(tree)
            #print(f"AST = ")
            #ast.pretty()

            methods = ast.lower_ast()

            methods = create_state_switch(methods)
    
            with open(output_path + module_name + ".sv", "w") as fp: 
          
                ps = PrintStream(fp)
                ast.generate_decls(ps)
                
                generate(methods, ps)
                

main()