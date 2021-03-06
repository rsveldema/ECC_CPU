from ArrayType import ArrayType
from GlobalDecl import GlobalDecl
from LowerState import LowerState
from PrintStream import PrintStream

class TypeDef(GlobalDecl):
    def __init__(self, name, type):
        self.name = name
        self.type = type
        
    def lower_ast(self, state: 'LowerState') -> 'GlobalDecl':
        return self
    
    def generate_decl(self, ps:PrintStream):
        ps.println("")
        ps.println("typedef " + self.type.str() + " " + self.name + ";")
        