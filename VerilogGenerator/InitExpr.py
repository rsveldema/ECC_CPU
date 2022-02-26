from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Expr import Expr

class InitExpr(Expr):
    def __init__(self, exprs:List[Expr]) -> None:
        self.exprs = exprs

    def lower_ast(self, state: LowerState) -> Expr:
        exprs_copy = []
        for k in self.exprs:
            exprs_copy.append(k.lower_ast(state))
        return InitExpr(exprs_copy)
    
    def str(self) -> str:            
        s = "{"
        comma = ""
        for k in self.exprs:
            s += comma
            s += k.str()
            comma = ", "
        s += "}"
        return s

