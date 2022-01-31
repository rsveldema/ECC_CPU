from typing import List
from PrintStream import PrintStream
from Statement import Statement


counter = 0


class Label(Statement):
    def __init__(self) -> None:
        global counter
        counter += 1
        self.label = counter
        
    def lower_ast(self, state: 'LowerState') -> 'Statement':
        return self
        
    def str(self):
        return f"{self.label}"
        
    def generate(self, ps: PrintStream):
        ps.print(f"{self.label}: ;")

    def getLocalDecls(self) -> List:
        return []
    