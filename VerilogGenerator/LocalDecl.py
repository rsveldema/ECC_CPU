from typing import List
from ArrayType import ArrayType
from GenerateContext import GenerateContext
from PrintStream import PrintStream
from LowerState import LowerState
from Type import Type
from Expr import Expr


class LocalDecl:
    def __init__(self, type: Type, var, init_expr: Expr) -> None:
        self.type = type
        self.var = var
        self.init_expr = init_expr

    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        op = "=" if ctxt.in_init else "<="

        if self.init_expr:
            ps.println(f"{self.var} {op} {self.init_expr.str()};")
        
     
    def lower_ast(self, state: LowerState):
        assert isinstance(self.var, str)
        lowered = None
        if self.init_expr:
            lowered = self.init_expr.lower_ast(state)
        return LocalDecl(self.type.lower_ast(state), 
                         self.var, 
                         lowered)
    
    def pretty(self):
        print(f"{self.str()}")
        
    def str(self):
        if self.init_expr:
            return f"{self.type.str()} {self.var} = {self.init_expr.str()}"
        else:
            return f"{self.type.str()} {self.var}"
            
    def generate_decl(self, ps: PrintStream):
        if (isinstance(self.type, ArrayType)):
            ps.println(f"{self.type.name} {self.var}[{self.type.num_elts.str()}];")
            return

        if self.init_expr:
            ps.println(f"{self.type.generate()} {self.var} = {self.init_expr.str()};")
        else:
            ps.println(f"{self.type.generate()} {self.var};")

    def getLocalDecls(self) -> List:
        return [self]
    
    def get_num_bits(self) -> int:
        return self.type.get_num_bits()