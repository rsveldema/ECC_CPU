from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from Block import Block
from Expr import Expr

class CaseStmt(Statement):
    # blk can be None for a fallthrough
    def __init__(self, value: Expr, blk: Block) -> None:
        super().__init__()
        self.expr = value
        self.blk = blk
        
    def pretty(self):
        if self.expr == None:
            print("default:")
        else:
            print(f"case {self.expr.str()}:") 

        if self.blk:
            self.blk.pretty()            
            
    def lower_ast(self, state:LowerState):
        e = None
        if self.expr:
            e = self.expr.lower_ast(state)
            
        if self.blk != None:
            return CaseStmt(e, self.blk.lower_ast(state))
        return CaseStmt(e, None)
        
    def get_last_stmt(self):
        if self.blk == None:
            return None
        return self.blk.get_last_stmt()
    
    def generate(self, ps: PrintStream):
        if self.expr:
            ps.print(f"{self.expr.str()}:")
        else:
            ps.print("default:")
            
        if self.blk != None:
            ps.up()
            self.blk.generate(ps)
            ps.down()

    def getLocalDecls(self) -> List:
        if self.blk == None:
            return []
        return self.blk.getLocalDecls()
    
    
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
    

    def generate(self, ps: PrintStream):
        ps.print(f"case ({self.expr.str()})")
        ps.up()
        for p in self.cases:
            p.generate(ps)
        ps.down()
        ps.print("endcase")

    def pretty(self):
        print(f"switch ({self.expr.str()})") 
        for p in self.cases:
            p.pretty()
            
