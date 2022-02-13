from typing import List
from GenerateContext import GenerateContext
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
    
    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        if self.expr:
            if self.blk != None:
                ps.println(f"{self.expr.const_expr()}:")
                ps.up()
                self.blk.generate(ps, ctxt)
                ps.down()
            else:
                ps.println(f"{self.expr.const_expr()},")
        else:
            ps.println("default:")
            if self.blk != None:
                ps.up()
                self.blk.generate(ps, ctxt)
                ps.down()
            else:
                ps.print_same_line(",")
            

    def getLocalDecls(self) -> List:
        if self.blk == None:
            return []
        return self.blk.getLocalDecls()
