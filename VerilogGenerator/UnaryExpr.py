from PrintStream import PrintStream
from LowerState import LowerState
from Expr import Expr

class UnaryExpr(Expr):
    def __init__(self, op, value:Expr) -> None:
        self.op = op
        self.value = value

    def lower_ast(self, state: LowerState) -> Expr:
        return UnaryExpr(self.op, self.value.lower_ast(state))
    
    def str(self) -> str:
        op = self.op + ""
        
        #print("OP = " + op)
        if op == "sizeof":
            return f"($bits({self.value.str()}) / 8)"
            
        return f"{self.op}({self.value.str()})"

