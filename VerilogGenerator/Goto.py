from typing import List
from PrintStream import PrintStream

class Goto:
    def __init__(self, label) -> None:
        self.label = label
        
    
    def generate(self, ps: PrintStream):
        ps.print(f"state = {self.label.str()}; // GOTO")
        ps.print(f"return;")
        

    def getLocalDecls(self) -> List:
        return []