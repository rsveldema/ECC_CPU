from lark import Lark
from MyTransformer import MyTransformer
from PrintStream import PrintStream


output_path = "test/"

def preprocess(lines):
    result = ""
    for l in lines:
        k = l.strip()
        if k.startswith("#"):
            continue
        
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

def parseInclude(parser, file):
    with open(file) as fp:
        lines = fp.readlines()
        data = preprocess(lines)
        tree = parser.parse(data)
        return tree
                

def main():
    fp = open("grammar.lark")
    grammar = fp.read()
    fp.close()

    parser = Lark(grammar)
    
    module_name = "global_decls"
    with open(output_path + module_name + ".sv", "w") as fp: 
        ps = PrintStream(fp)

        includes = ["../CoreLib/include/Defines.h"]
        for file in includes:
            tree = parseInclude(parser, file)
            ast = createAST(tree)
            ast.generate_decls(ps)
        
            


    files = ["../CoreLib/FetchStage.cpp"]
    for file in files:
        
        last_slash = file.rfind("/")
        module_name = file[last_slash+1:]
        module_name = module_name.replace(".cpp", "")
        
        with open(output_path + module_name + ".sv", "w") as fp: 
            ps = PrintStream(fp)

            with open(file) as fp:
                lines = fp.readlines()
                data = preprocess(lines)
                tree = parser.parse(data)
                #print(tree)

                ast = createAST(tree)
                #print(f"AST = ")
                #ast.pretty()

                methods = ast.lower_ast()

                methods = create_state_switch(methods)
                
                generate(methods, ps)
                

main()