from Type import Type
from Expr import Expr

class ArrayType(Type):
    
    def __init__(self, elt_type: str, num_elts: Expr) -> None:
        super().__init__(elt_type)
        self.num_elts = num_elts
        
    def str(self) -> str:
        return "ARRAYTYPE"