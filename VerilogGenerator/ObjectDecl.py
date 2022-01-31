from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from typing import List


class ObjectDecl(Statement):
    def __init__(self, type, var, args) -> None:
        self.type = type
        self.var = var
        self.args = args
        
    def lower(self, state: LowerState):
        new_args = []
        for p in self.args:
            new_args.append(p.lower(state))
        return ObjectDecl(self.type.lower(state), self.var, new_args)
    
    def generate(self, ps: PrintStream):
        ps.print("// " + self.str())
        
    def pretty(self):
        print(f"{self.str()}")
        
    def str(self):
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "

        return f"local_obj {self.type.str()} {self.var}({s})"

    def get_num_bits(self):
        return 10000

    def getLocalDecls(self) -> List:
        return [self]