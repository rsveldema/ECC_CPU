from GlobalDecl import GlobalDecl
from PrintStream import PrintStream
from Type import Type
from LowerState import LowerState


class MemberDecl(GlobalDecl):
    def __init__(self, type: Type, name: str) -> None:
        self.name = name
        self.type = type
        
    def lower_ast(self, state: LowerState) -> GlobalDecl:
        pass
    
    def generate_decl(self, ps: PrintStream):
        ps.print(self.type.str() + " " + self.name + ";")
        
        