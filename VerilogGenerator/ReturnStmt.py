from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from Block import Block

class ReturnStmt(Statement):
    def __init__(self, value) -> None:
        super().__init__()
        self.expr = value 
        
    def pretty(self):
        print(f"return {self.expr.str()};")
        
    def lower(self, state):
        return ReturnStmt(self.expr.lower(state))
            
    def generate(self, ps: PrintStream):
        ps.print(f"return {self.expr.str()};")

    def getLocalDecls(self) -> List:
        return []