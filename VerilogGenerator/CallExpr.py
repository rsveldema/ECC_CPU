from ast import Expression
from typing import List
from PrintStream import PrintStream
from LowerState import LowerState


class CallExpr(Expression):
    def __init__(self, func, args) -> None:
        self.func = func
        self.args = args
    
        
    def lower(self, state: LowerState):
        new_args = []
        for p in self.args:
            z = p.lower(state)
            new_args.append(z)
        return CallExpr(self.func, new_args)
    
    def generate(self, ps: PrintStream):       
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "
        ps.print(f"{self.func({s})}")
        
    def pretty(self):
        print(f"{self.str()};")
        
    def str(self):
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "
        return f"call {self.func.str()}({s})"
