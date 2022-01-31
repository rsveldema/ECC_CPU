from PrintStream import PrintStream
from LowerState import LowerState
from Expr import Expr


class CastExpr(Expr):
    def __init__(self, type, expr: Expr) -> None:
        self.type = type
        self.expr = expr

    def lower_ast(self, state: LowerState):
        return CastExpr(self.type, self.expr.lower_ast(state))
    
    def str(self) -> str:
        return f"({self.type.str()}'({self.expr.str()}))"

