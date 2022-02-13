from ast import Expression
from typing import List
from GenerateContext import GenerateContext
from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from CallExpr import CallExpr

class CallStmt(Statement):
    def __init__(self, func, args) -> None:
        self.call_expr = CallExpr(func, args)
   
        
    def lower_ast(self, state: LowerState):
        ce = self.call_expr.lower_ast(state)
        return CallStmt(ce.func, ce.args)
    
    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        c = self.call_expr.str()
        if c.find("CONTEXT_SWITCH") >= 0:
            ps.println("// " + c + ";")
        else:
            ps.println("" + c + ";")

        
    def pretty(self):
        print(f"{self.str()};")
        
    def str(self):
        return self.call_expr.str()
    
    def getLocalDecls(self) -> List:
        return []