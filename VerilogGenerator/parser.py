from ast import Assign
from lark import Lark, Transformer

output_path = "test/"

class PrintStream:
    def __init__(self, fp) -> None:
        self.fp = fp
        self.indent = 0

    def print(self, str) -> None:
        for i in range(self.indent):
            str = "\t" + str
        self.fp.write(str + "\n")
        
    def up(self):
        self.indent += 1

    def down(self):
        self.indent -= 1

class Method:
    def __init__(self, type, ns, funcname, block):
        self.type = type
        self.ns = ns
        self.funcname = funcname
        self.block = block 

    def pretty(self):
        print("method " + self.funcname)
        self.block.pretty()
        
    def lower(self):
        self.block = self.block.lower()
        return self
        
    def generate(self):
        with open(output_path + self.ns + ".v", "w") as fp: 
            ps = PrintStream(fp)
            ps.print("module " + self.ns + ";")
            ps.print("initial")
            ps.up()
            ps.print("begin")
            ps.up()
            
            self.block.generate(ps)
            
            ps.print("$finish;")
            ps.down()
            ps.print("end")
            ps.down()
            ps.print("endmodule")

class Namespace:
    def __init__(self, method):
        self.method = method

    def pretty(self):
        print("namespace ")
        self.method.pretty()

    def lower(self):
        return self.method.lower()


class Block:
    def __init__(self) -> None:
        self.insns = []

    def pretty(self):
        print("{")        
        for insn in self.insns:
            insn.pretty()
        print("}")
        
    def lower(self):
        b = Block()        
        for insn in self.insns:
            k = insn.lower()
            b.insns.append(k)
        return b
        
    def generate(self, ps: PrintStream):
        ps.print("begin")
        ps.up()
        for insn in self.insns:
            insn.generate(ps)
        ps.down()
        ps.print("end")

class ObjectDecl:
    def __init__(self, type, var, args) -> None:
        self.type = type
        self.var = var
        self.args = args
        
    def lower(self):
        new_args = []
        for p in self.args:
            new_args.append(p.lower())
        return ObjectDecl(self.type.lower(), self.var, new_args)
    
    def generate(self, ps: PrintStream):
        ps.print("// " + self.str())
        
    def pretty(self):
        print(f"{self.str()}")
        
    def str(self):
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "

        return f"local_obj {self.type.str()} {self.var}({s})"

class LocalDecl:
    def __init__(self, type, var, init_expr) -> None:
        self.type = type
        self.var = var
        self.init_expr = init_expr

    def generate(self, ps: PrintStream):
        ps.print("// " + self.str())
        
    def lower(self):
        return LocalDecl(self.type.lower(), self.var.lower(), self.init_expr.lower())
    
    def pretty(self):
        print(f"{self.str()}")
        
    def str(self):
        return f"local {self.type.str()} {self.var} {self.init_expr.str()}"
        
class WhileStmt:
    def __init__(self, expr, code) -> None:
        self.expr = expr
        self.code = code

    def lower(self):
        return WhileStmt(self.expr.lower(), self.code.lower())
    
    def pretty(self):
        print(f"while ({self.expr.str()})")
        self.code.pretty()

    def generate(self, ps: PrintStream):
        ps.print(f"while ({self.expr.str()})")
        self.code.generate(ps)

class IfStmt:
    def __init__(self, expr, if_code, else_code) -> None:
        self.expr = expr
        self.if_code = if_code
        self.else_code = else_code

    def generate(self, ps: PrintStream):
        ps.print(f"if ({self.expr.str()})")
        self.if_code.generate(ps)
        if self.else_code:
            ps.print("else")
            self.else_code.generate(ps)

        
    def lower(self):
        ec = None
        if self.else_code:
            ec = self.else_code.lower()
        return IfStmt(self.expr.lower(), self.if_code.lower(), ec)
    
    def pretty(self):
        print(f"if ({self.expr.str()})")
        self.if_code.pretty()
        if self.else_code:
            print("else")
            self.else_code.pretty()

class CallStmt:
    def __init__(self, func, args) -> None:
        self.func = func
        self.args = args
    
        
    def lower(self):
        new_args = []
        for p in self.args:
            new_args.append(p.lower())
        return CallStmt(self.func, new_args)
    
    def generate(self, ps: PrintStream):
        ps.print("// " + self.str())
        
    def pretty(self):
        print(f"{self.str()};")
        
    def str(self):
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "
        return f"call {self.func.str()}({s})"

class NumberExpr:
    def __init__(self, value) -> None:
        self.value = value

    def lower(self):
        if self.value == "true":
            return NumberExpr("1")
        if self.value == "false":
            return NumberExpr("0")
        return self
    
    def str(self) -> str:
        return self.value


class UnaryExpr:
    def __init__(self, op, value) -> None:
        self.op = op
        self.value = value

    def lower(self):
        return UnaryExpr(self.op, self.value.lower())
    
    def str(self) -> str:
        return f"{self.op}({self.value})"

class Ident:
    def __init__(self, value) -> None:
        self.value = value

    def lower(self):
        return self
    
    def str(self) -> str:
        return self.value


class BinaryExpr:
    def __init__(self, op, left, right) -> None:
        self.op = op
        self.left = left
        self.right = right

    def generate(self, ps: PrintStream):
        ps.print(f"{self.str()};")
        
    def lower(self):
        return BinaryExpr(self.op, self.left.lower(), self.right.lower())
    
    def str(self) -> str:
        if self.op == "[]":
            return self.left.str() + "[" + self.right.str() + "]"
            
        
        if self.op == "=" or self.op == "+=" or self.op == "-=" or self.op == "|=" or self.op == "&=":
            return self.left.str() + self.op + self.right.str() 
            
        return "(" + self.left.str() + self.op + self.right.str() + ")"

    def pretty(self):
        print(self.str())

class Break:
    def __init__(self) -> None:
        pass

    def lower(self):
        return self
    
    def pretty(self):
        print("break")
        
    def generate(self, ps: PrintStream):
        ps.print("break;")

class Continue:
    def __init__(self) -> None:
        pass

    def lower(self):
        return self
    
    def pretty(self):
        print("continue")
        
    def generate(self, ps: PrintStream):
        ps.print("continue;")


class Type:
    def __init__(self, name) -> None:
        self.name = name

    def lower(self):
        return self
    
    def str(self) -> str:
        if isinstance(self.name, str):
            return self.name
        return self.name.str()


class MyTransformer(Transformer):

    def __init__(self) -> None:
        super().__init__()
        self.methods = []      

    def booltype(self, tree):
        return Type("bool")

    def autotype(self, tree):
        return Type("auto")

    def qual_ident_type(self, tree):
        return Type(tree[0])

    def type(self, tree):
        return tree[0]

    def break_stmt(self, tree):
        return Break()

    def continue_stmt(self, tree):
        return Continue()

    def access(self, tree):
        return tree[0]

    def access_expr(self, tree):
        return tree[0]

    def qual_ident(self, tree):
        return Ident(tree[0] + "")

    def array_access(self, tree):
        return BinaryExpr("[]", tree[0], tree[1])

    def string(self, tree):
        t = tree[0] + ""
        return NumberExpr(t)

    def true_expr(self, tree):
        return NumberExpr("true")

    def false_expr(self, tree):
        return NumberExpr("false")

    def NUMBER(self, tree):
        t = tree[0] + ""
        return NumberExpr(t)
 
    def HEX_NUMBER(self, tree):
        t = tree[0] + ""
        return NumberExpr(t)

    def term(self, tree):
        return tree[0]

    def static_cast(self, tree):
        return UnaryExpr("static_cast", tree[0])

    def sizeof_term(self, tree):
        return UnaryExpr("sizeof", tree[0])

    def binary_op(self, tree):
        return tree[0]

    def cmp_eq(self, tree):
        return "=="

    def cmp_neq(self, tree):
        return "!="

    def plus(self, tree):
        return "+"

    def bit_and(self, tree):
        return "&"

    def bit_or(self, tree):
        return "|"

    def expr(self, tree):
        if len(tree) == 1:
            return tree[0]
        return BinaryExpr(tree[1], tree[0], tree[2])
 
    def while_stmt(self, tree):
        expr = tree[0]
        code = tree[1]
        return WhileStmt(expr, code)

    def args(self, tree):
        ret = []
        for a in tree:
            ret.append(a)
        return ret

    def call_stmt(self, tree):
        return CallStmt(tree[0], tree[1])

    def assign_op(self, tree):
        return "="

    def assign_add(self, tree):
        return "+="

    def some_assign_op(self, tree):
        return tree[0]

    def assign_stmt(self, tree):
        var = tree[0]
        op = tree[1]
        expr = tree[2]
        return BinaryExpr(op, var, expr)

    def bit_not_expr(self, tree):
        return UnaryExpr("~", tree[0])

    def hex_number_expr(self, tree):
        return tree[0]
    
    def number_expr(self, tree):
        return tree[0]

    def if_stmt(self, tree):
        expr = tree[0]
        if_code = tree[1]
        else_code = None
        if len(tree) == 3:
            else_code = tree[2]
        return IfStmt(expr, if_code, else_code)

    def local_decl(self, tree):
        return LocalDecl(tree[0], tree[1] + "", tree[2])

    def object_init(self, tree):
        return ObjectDecl(tree[0], tree[1] + "", tree[2])



    def block(self, tree):
        b = Block()
        for k in tree:
            for p in k:
                b.insns.append(p)
        return b

    def method(self, tree):
        type = tree[0] 
        ns = tree[1]
        func = tree[2]
        block = tree[3]
        return Method(type, ns, func, block)

    def namespace(self, tree):
        return Namespace(tree[1])

    def stmt(self, tree):
        return tree

    def start(self, tree):
        return tree[1]


def preprocess(lines):
    result = ""
    for l in lines:
        result += l
    return result


def createAST(tree):
    ast = MyTransformer().transform(tree)
    return ast


def main():
    fp = open("grammar.lark")
    grammar = fp.read()
    fp.close()

    parser = Lark(grammar)

    files = ["../CoreLib/FetchStage.cpp"]
    for file in files:
        with open(file) as fp:
            lines = fp.readlines()
            data = preprocess(lines)
            tree = parser.parse(data)
            #print(tree)

            ast = createAST(tree)
            print(f"AST = ")
            ast.pretty()

            ast = ast.lower()

            ast.generate()
            

main()