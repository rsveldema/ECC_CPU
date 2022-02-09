
from PrintStream import PrintStream
from Type import Type
from Expr import Expr
from LowerState import LowerState
from GlobalDecl import GlobalDecl


class ConstantDecl(GlobalDecl):
    def __init__(self, name:str, type:Type, expr:Expr):
        self.type = type
        self.name = name
        self.expr = expr
        
    def lower_ast(self, state: LowerState): 
        self.expr = self.expr.lower_ast(state)
        return self
        
    def generate_decl(self, ps:PrintStream):
        ps.println("")
        ps.println("parameter " + self.name + " = " + self.expr.str() + ";")
        
        