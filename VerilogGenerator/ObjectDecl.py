from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement
from typing import List
from Expr import Expr


class ObjectDecl(Statement):
    def __init__(self, type, var, args: List[Expr]) -> None:
        self.type = type
        self.var = var
        self.args = args
        
    def lower_ast(self, state: LowerState):
        new_args = []
        for p in self.args:
            new_args.append(p.lower_ast(state))
        return ObjectDecl(self.type.lower_ast(state), self.var, new_args)
    
    def generate(self, ps: PrintStream):
        ps.println("// " + self.str())
        
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
        return self.type.get_num_bits()

    def getLocalDecls(self) -> List:
        return [self]