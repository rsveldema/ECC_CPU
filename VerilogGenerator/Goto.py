from typing import List
from GenerateContext import GenerateContext
from PrintStream import PrintStream

class Goto:
    def __init__(self, label) -> None:
        self.label = label
        
    
    def generate(self, ps: PrintStream, ctxt:GenerateContext):
        ps.println(f"state <= {self.label.str()}; // GOTO")
        ps.println(f"return;")
        

    def getLocalDecls(self) -> List:
        return []