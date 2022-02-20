from typing import List
from GenerateContext import GenerateContext
from LowerState import LowerState
from PrintStream import PrintStream
from Statement import Statement

class Goto(Statement):
    def __init__(self, label) -> None:
        self.label = label
        
    
    def lower_ast(self, state: LowerState) -> 'Statement':
        return self
        
    def generate(self, ps: PrintStream, ctxt:GenerateContext):
        ps.println(f"state <= {self.label.str()}; // GOTO")
        ps.println(f"return;")
        

    def getLocalDecls(self) -> List:
        return []