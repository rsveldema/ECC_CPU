from PrintStream import PrintStream
from LowerState import LowerState

class CastExpr:
    def __init__(self, type, expr) -> None:
        self.type = type
        self.expr = expr

    def lower(self, state: LowerState):
        return CastExpr(self.type, self.expr.lower(state))
    
    def str(self) -> str:
        return f"({self.type.str()}'({self.expr.str()}))"

