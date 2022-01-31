from ast import Return
from lark import Lark, Transformer
from Type import Type
from Break import Break
from Continue import Continue
from IfStmt import IfStmt
from SwitchStmt import CaseStmt, SwitchStmt
from ReturnStmt import ReturnStmt
from WhileStmt import WhileStmt
from LiteralExpr import LiteralExpr
from BinaryExpr import BinaryExpr
from UnaryExpr import UnaryExpr
from Ident import Ident
from CallExpr import CallExpr
from CallStmt import CallStmt
from LocalDecl import LocalDecl
from ObjectDecl import ObjectDecl
from Block import Block
from Method import Method
from Namespace import Namespace
from CastExpr import CastExpr

class MyTransformer(Transformer):

    def __init__(self) -> None:
        super().__init__()
        self.methods = []      

    def args(self, tree):
        ret = []
        for x in tree:
            ret.append(x)
        #args: (expr ("," expr)*)?
        return ret

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
        if len(tree) > 1:
            return CallExpr(tree[0], tree[1])
        return tree[0]

    def qual_ident(self, tree):
        return Ident(tree[0] + "")

    def array_access(self, tree):
        return BinaryExpr("[]", tree[0], tree[1])

    def string(self, tree):
        t = tree[0] + ""
        return LiteralExpr(t)

    def true_expr(self, tree):
        return LiteralExpr("true")

    def false_expr(self, tree):
        return LiteralExpr("false")
    
    def return_stmt(self, tree):
        return ReturnStmt(tree[0])
    
    def case_list(self, tree):
        ret = []
        for p in tree:
            ret.append(p)
        return ret
    
    def case_stmt(self, tree):
        if len(tree) == 1:
            return CaseStmt(tree[0], None)
        return CaseStmt(tree[0], tree[1])

    def default_stmt(self, tree):
        return CaseStmt(None, tree[0])

    def switch_stmt(self, tree):
        return SwitchStmt(tree[0], tree[1])

    def NUMBER(self, tree):
        t = tree + ""
        return LiteralExpr(t)
 
    def HEX_NUMBER(self, tree):
        t = tree + ""
        return LiteralExpr(t)

    def term(self, tree):
        return tree[0]

    def static_cast(self, tree):
        return CastExpr(tree[0], tree[1])

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
        func = tree[1]
        params = tree[2]
        block = tree[3]
        return Method(type, func, params, block)

    def namespace(self, tree):
        return Namespace(tree[1:])

    def stmt(self, tree):
        return tree

    def start(self, tree):
        return tree[1]
