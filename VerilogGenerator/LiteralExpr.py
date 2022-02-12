from PrintStream import PrintStream
from LowerState import LowerState

# can be a string, a char, or a number
class LiteralExpr:
    def __init__(self, value) -> None:
        self.value = value
        
        #if value.find('x'):
        #    print(f"SAW: {value}")

    def const_expr(self):
        return self.str()
        
    def lower_ast(self, state: LowerState):
        if self.value == "true":
            return LiteralExpr("1")
        if self.value == "false":
            return LiteralExpr("0")
        
        if isinstance(self.value, str):
            if self.value.startswith("0x"):                
                bits = 32
                digits = self.value[2:]
                if len(digits) > 8:
                    bits = 64
                return LiteralExpr(f"{bits}'h{digits}")
        
        return self
    
    def str(self) -> str:
        return self.value

