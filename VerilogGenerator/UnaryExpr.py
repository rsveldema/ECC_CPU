from PrintStream import PrintStream
from LowerState import LowerState

class UnaryExpr:
    def __init__(self, op, value) -> None:
        self.op = op
        self.value = value

    def lower(self, state: LowerState):
        return UnaryExpr(self.op, self.value.lower(state))
    
    def str(self) -> str:
        return f"{self.op}({self.value.str()})"

