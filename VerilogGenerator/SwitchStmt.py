from typing import List
from CaseStmt import CaseStmt
from GenerateContext import GenerateContext
from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from Block import Block
from Expr import Expr

    
    
class SwitchStmt(Statement):
    def __init__(self, expr, cases: List[CaseStmt]) -> None:
        self.expr = expr
        self.cases = cases
        
    def lower_ast(self, state:LowerState):
        newcases = []
        for c in self.cases:
            newcases.append(c.lower_ast(state))
        return SwitchStmt(self.expr.lower_ast(state), newcases)
        
        
    def getLocalDecls(self) -> List:
        ret = []
        for k in self.cases:
            ret.extend(k.getLocalDecls())
        return ret
    

    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        ps.println(f"case ({self.expr.str()})")
        ps.up()
        for p in self.cases:
            p.generate(ps, ctxt)
        ps.down()
        ps.println("endcase")

    def pretty(self):
        print(f"switch ({self.expr.str()})") 
        for p in self.cases:
            p.pretty()
            
