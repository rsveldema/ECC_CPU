from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Type import Type

class LocalDecl:
    def __init__(self, type: Type, var, init_expr) -> None:
        self.type = type
        self.var = var
        self.init_expr = init_expr

    def generate(self, ps: PrintStream):
        ps.print(f"{self.var} = {self.init_expr.str()};")
        
    def lower(self, state: LowerState):
        return LocalDecl(self.type.lower(state), self.var.lower(), self.init_expr.lower(state))
    
    def pretty(self):
        print(f"{self.str()}")
        
    def str(self):
        return f"local {self.type.str()} {self.var} {self.init_expr.str()}"

    def getLocalDecls(self) -> List:
        return [self]
    
    def get_num_bits(self) -> int:
        return self.type.get_num_bits()