from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from Block import Block
from Expr import Expr

class ReturnStmt(Statement):
    def __init__(self, value: Expr) -> None:
        super().__init__()
        self.expr = value 
        
    def pretty(self):
        print(f"return {self.expr.str()};")
        
    def lower_ast(self, state: LowerState) -> Statement:
        return ReturnStmt(self.expr.lower_ast(state))
            
    def generate(self, ps: PrintStream):
        ps.print(f"return {self.expr.str()};")

    def getLocalDecls(self) -> List:
        return []