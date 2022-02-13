from typing import List
from Expr import Expr
from GenerateContext import GenerateContext
from LocalDecl import LocalDecl
from PrintStream import PrintStream
from LowerState import LowerState
from Label import Label
from Block import Block
from Goto import Goto
from UnaryExpr import UnaryExpr
from Statement import Statement

class ForStmt(Statement):
    def __init__(self, decl:LocalDecl, cond:Expr, inc:Expr, blk:Block) -> None:
        self.decl = decl
        self.cond = cond
        self.inc = inc
        self.blk = blk


    def getLocalDecls(self) -> List:
        return self.blk.getLocalDecls()

    
    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        ps.println(f"for ({self.decl.str()}; {self.cond.str()}; {self.inc.str()})")
        ps.up()
        self.blk.generate(ps, ctxt)
        ps.down()
        
    def lower_ast(self, state: LowerState):
        return ForStmt(self.decl.lower_ast(state), self.cond.lower_ast(state), self.inc.lower_ast(state), self.blk.lower_ast(state))
    
    def pretty(self):
        print(f"for ({self.decl.str()}; {self.cond.str()}; {self.inc.str()})")
        self.blk.pretty() 