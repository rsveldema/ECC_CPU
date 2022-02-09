from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Expr import Expr

class DotAccessExpr: 
    def __init__(self, left:Expr, right:str) -> None:
        self.left = left
        self.member = right

    def lower_ast(self, state: LowerState):
        return DotAccessExpr(self.left.lower_ast(state), self.member)
    
    def const_expr(self):
        return self.left.const_expr() + "." + self.member

    def str(self) -> str:  
        return  self.left.str() + "."  + self.member

    def pretty(self):
        print(self.str())

    def getLocalDecls(self) -> List:
        return []