from PrintStream import PrintStream
from LowerState import LowerState
from Expr import Expr

class Ident(Expr):
    def __init__(self, value) -> None:
        self.value = value

    def lower_ast(self, state: LowerState):
        return self
    
    def const_expr(self):
        s = self.str()
        ix = s.find("::")
        if ix > 0:
            return s[ix+2:]
        return s
    
    def str(self) -> str:
        return self.value

