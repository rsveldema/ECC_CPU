from ast import Assign
from lark import Lark, Transformer

class PrintStream:
    def __init__(self, fp) -> None:
        self.fp = fp

    def print(self, str) -> None:
        self.fp.write(str + "\n")

class Method:
    def __init__(self, type, ns, funcname, block):
        self.type = type
        self.ns = ns
        self.funcname = funcname
        self.block = block

    def pretty(self):
        print("method " + self.funcname)
        self.block.pretty()
        
    def generate(self, ps):
        ps.print("module " + self.ns + "{")
        ps.print("}")

class Namespace:
    def __init__(self, method):
        self.method = method

    def pretty(self):
        print("namespace ")
        self.method.pretty()

    def generate(self, ps):
        self.method.generate(ps)

class Block:
    def __init__(self) -> None:
        self.insns = []

    def pretty(self):
        print("{")
        for insn in self.insns:
            insn.pretty()
        print("}")

class ObjectDecl:
    def __init__(self, type, var, args) -> None:
        self.type = type
        self.var = var
        self.args = args

    def pretty(self):
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "

        print(f"local_obj {self.type.str()} {self.var}({s})")

class LocalDecl:
    def __init__(self, type, var, init_expr) -> None:
        self.type = type
        self.var = var
        self.init_expr = init_expr

    def pretty(self):
        print(f"local {self.type.str()} {self.var} {self.init_expr.str()}")

class WhileStmt:
    def __init__(self, expr, code) -> None:
        self.expr = expr
        self.code = code

    def pretty(self):
        print(f"while ({self.expr.str()})")
        self.code.pretty()


class IfStmt:
    def __init__(self, expr, if_code, else_code) -> None:
        self.expr = expr
        self.if_code = if_code
        self.else_code = else_code

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
    
    def pretty(self):
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "
        print(f"call {self.func.str()}({s})")

class NumberExpr:
    def __init__(self, value) -> None:
        self.value = value

    def str(self) -> str:
        return self.value


class UnaryExpr:
    def __init__(self, op, value) -> None:
        self.op = op
        self.value = value

    def str(self) -> str:
        return f"{self.op}({self.value})"

class Ident:
    def __init__(self, value) -> None:
        self.value = value

    def str(self) -> str:
        return self.value


class BinaryExpr:
    def __init__(self, op, left, right) -> None:
        self.op = op
        self.left = left
        self.right = right

    def str(self) -> str:
        return self.left.str() + self.op + self.right.str()

    def pretty(self):
        print(self.str())

class Break:
    def __init__(self) -> None:
        pass

    def pretty(self):
        print("break")

class Continue:
    def __init__(self) -> None:
        pass

    def pretty(self):
        print("continue")

class Type:
    def __init__(self, name) -> None:
        self.name = name

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

            with open("generated_verilog.v", "w") as fp: 
                ps = PrintStream(fp)
                ast.generate(ps)
            

main()