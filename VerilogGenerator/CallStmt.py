from ast import Expression
from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from CallExpr import CallExpr

class CallStmt(Statement):
    def __init__(self, func, args) -> None:
        self.call_expr = CallExpr(func, args)
   
        
    def lower(self, state: LowerState):
        ce = self.call_expr.lower(state)
        return CallStmt(ce.func, ce.args)
    
    def generate(self, ps: PrintStream):
        ps.print("// " + self.call_expr.str())
        
    def pretty(self):
        print(f"{self.str()};")
        
    def str(self):
        return self.call_expr.str()
    
    def getLocalDecls(self) -> List:
        return []