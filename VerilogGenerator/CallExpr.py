from ast import Expression
from typing import List
from PrintStream import PrintStream
from LowerState import LowerState


def is_macro(fn):
    return fn in ["PACK2", "PACK4", "PACK8_BYTES", "SET_BIT", "COUNT_BITS64", "PACK8_BITS"]


class CallExpr(Expression):
    def __init__(self, func, args) -> None:
        self.func = func
        self.args = args
    
        
    def lower_ast(self, state: LowerState):
        new_args = []
        for p in self.args:
            z = p.lower_ast(state)
            new_args.append(z)
        return CallExpr(self.func, new_args)
    
    def generate(self, ps: PrintStream):               
        ps.println(f"{self.str()}")
        
    def pretty(self):
        print(f"{self.str()};")
        
    def str(self):
        s = ""
        comma = ""
        for k in self.args:
            s += comma
            s += k.str()
            comma = ", "

        fn = self.func.str()
        if is_macro(fn):
            fn = '`' + fn
        return f"{fn}({s})"
