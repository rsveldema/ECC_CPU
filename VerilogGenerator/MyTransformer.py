from lark import Lark, Token, Transformer
from ForStmt import ForStmt
from InitExpr import InitExpr
from Type import Type
from Break import Break
from Continue import Continue
from IfStmt import IfStmt
from SwitchStmt import CaseStmt, SwitchStmt
from ReturnStmt import ReturnStmt
from EnumDecl import EnumDecl
from ConstantDecl import ConstantDecl
from DotAccessExpr import DotAccessExpr
from TypeDef import TypeDef
from MemberDecl import MemberDecl
from StructDecl import StructDecl
from UnionDecl import UnionDecl
from ArrayType import ArrayType
from InterfaceDecl import InterfaceDecl
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
    


    def type(self, tree):
        return tree[0]

    def user_defined_type(self, tree):
        if len(tree) == 1:
            return Type(tree[0])
        else:
            return ArrayType(tree[1]+"", tree[2])

    def int8_t(self, tree):
        return Type("int8_t")

    def int16_t(self, tree):
        return Type("int16_t")

    def int32_t(self, tree):
        return Type("int32_t")

    def int64_t(self, tree):
        return Type("int64_t")

    def uint8_t(self, tree):
        return Type("uint8_t")

    def uint16_t(self, tree):
        return Type("uint16_t")

    def uint32_t(self, tree):
        return Type("uint32_t")

    def uint64_t(self, tree):
        return Type("uint64_t")

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
        if len(tree) == 1:
            return tree[0]
        else:
            ident_list = []
            for p in tree[1:]:
                ident_list.append(p + "")
            return DotAccessExpr(tree[0], ident_list)

    def init_expr(self, tree):
        return InitExpr(tree[0:])

    def lhs(self, tree):
        if len(tree) == 1:
            return tree[0]
        else:
            b = BinaryExpr("[]", tree[0], tree[1])
            if len(tree) > 2:
                b = DotAccessExpr(b, tree[2:])
            return b;


    def access_expr(self, tree):        
        if len(tree) > 1:
            return CallExpr(tree[0], tree[1])
        return tree[0]

    def method_name_qual_ident(self, tree):
        base = Ident(tree[0] + "")        
        if len(tree) == 2:
            return DotAccessExpr(base, [tree[1] + ""])
        if len(tree) == 3:
            return DotAccessExpr(base, [tree[2] + ""])
        return base


    def qual_ident(self, tree):
        base = Ident(tree[0] + "")

        if len(tree) == 2:
            return DotAccessExpr(base, [tree[1] + ""])

        return base

    def array_access(self, tree):
        b = BinaryExpr("[]", tree[0], tree[1])
        if len(tree) > 2:
            b = DotAccessExpr(b, tree[2:])
        return b;

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

    def subtract(self, tree):
        return "-"

    def bit_and(self, tree):
        return "&"

    def bit_or(self, tree):
        return "|"

    def mult(self, tree):
        return "*"
    
    def divide(self, tree):
        return "/"

    def bit_and(self, tree):
        return "&"

    def bit_or(self, tree):
        return "|"
    
    def bit_shl(self, tree):
        return "<<"
    
    def bit_shr(self, tree):
        return ">>"

    def cmp_lt(self, tree):
        return "<"

    def cmp_gt(self, tree):
        return ">"

    def cmp_le(self, tree):
        return "<="

    def cmp_ge(self, tree):
        return ">="
    
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

    def assign_or(self, tree):
        return "|="

    def some_assign_op(self, tree):
        return tree[0]

    def assign_stmt(self, tree):
        var = tree[0]
        op = tree[1]
        expr = tree[2]
        return BinaryExpr(op, var, expr)


    def logical_not_expr(self, tree):
        return UnaryExpr("!", tree[0])

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
        if len(tree) == 3:
            return LocalDecl(tree[0], tree[1] + "", tree[2])
        return LocalDecl(tree[0], tree[1] + "", None)

    def object_init(self, tree):
        return ObjectDecl(tree[0], tree[1] + "", tree[2])



    def block(self, tree):
        b = Block()
        for k in tree:
            for p in k:
                b.insns.append(p)
        return b

    def is_token(k, s):
        if isinstance(k, Token):
            return s == (k + "")
        return False

    def method(self, tree):
        start = 0

        if MyTransformer.is_token(tree[start], "static"):
            start += 1

        if MyTransformer.is_token(tree[start], "inline"):
            start += 1

        template_args = None
        if isinstance(tree[start], list):
            template_args = tree[start]
            start += 1

        type = tree[start] 
        func = tree[start + 1]
        params = tree[start + 2]
        block = tree[start + 3]
        return Method(type, func, params, block, template_args)
    

    def template_arg(self, tree):
        type = tree[0]
        var = tree[1]
        return LocalDecl(type, var, None)

    def template_params(self, tree):
        return tree[0:]

    def params(self, tree):
        return tree[0:]
    
    def param(self, tree):
        return LocalDecl(tree[0], tree[1] + "", None)
    
    def for_stmt(self, tree):
        decl = tree[0]
        expr = tree[1]
        inc = tree[2]
        blk = tree[3]
        return ForStmt(decl, expr, inc, blk)

    def inc_stmt(self, tree):
        lhs = Ident(tree[0] + "")
        lhs2 = Ident(tree[0] + "")
        return BinaryExpr("=", lhs, BinaryExpr("+", lhs2, LiteralExpr("1")))

    def enum_decl(self, tree):
        name = tree[1] + ""
        type = tree[2]
        vars = []
        for k in tree[3:]:
            vars.append(k + "")
        return EnumDecl(name, type, vars)
    
    def static_decl(self, tree):
        type = tree[1]
        name = tree[2] + ""
        expr = tree[3]
        return ConstantDecl(name, type, expr)
    
    def global_decl(self, tree):
        return tree[0]

    def global_methods(self, tree):
        return tree[0:]

    def global_decls(self, tree):
        return tree[0:]


    def interface_decl(self, tree):
        name = tree[0]
        global_decls = tree[1]
        methods = tree[2]
        return InterfaceDecl(name, global_decls, methods)

    def namespace(self, tree):
        if len(tree) == 2:
            name = tree[0]
            global_decls = tree[1]
            methods = None
        else:
            name = tree[0]
            global_decls = tree[1]
            methods = tree[2]

        return Namespace(name, global_decls, methods)
        
    def using_decl(self, tree):
        name = tree[1] + ""
        type = tree[2]
        return TypeDef(name, type)

    def struct_decl(self, tree):
        name = tree[0]
        members = tree[1:]
        return StructDecl(name, members)
    
    def union_decl(self, tree):
        name = tree[0]
        members = tree[1:]
        return UnionDecl(name, members)

    def member_decl(self, tree):
        type = tree[0]
        name = tree[1]
        return MemberDecl(type, name)

    def stmt(self, tree):
        return tree

    def start(self, tree):
        return tree[0]
