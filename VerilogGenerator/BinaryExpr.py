from typing import List
from PrintStream import PrintStream
from LowerState import LowerState

class BinaryExpr:
    def __init__(self, op, left, right) -> None:
        self.op = op
        self.left = left
        self.right = right

    def generate(self, ps: PrintStream):
        ps.print(f"{self.str()};")
        
    def lower(self, state: LowerState):
        return BinaryExpr(self.op, self.left.lower(state), self.right.lower(state))
    
    def str(self) -> str:
        if self.op == "[]":
            return self.left.str() + "[" + self.right.str() + "]"
            
        
        if self.op == "=" or self.op == "+=" or self.op == "-=" or self.op == "|=" or self.op == "&=":
            return self.left.str() + self.op + self.right.str() 
            
        return "(" + self.left.str() + " " + self.op + " " + self.right.str() + ")"

    def pretty(self):
        print(self.str())

    def getLocalDecls(self) -> List:
        return []