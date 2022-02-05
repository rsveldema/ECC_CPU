from typing import List
from GlobalDecl import GlobalDecl
from PrintStream import PrintStream
from LowerState import LowerState
from MemberDecl import MemberDecl


class StructDecl(GlobalDecl):
    def __init__(self, name:str, members: List[MemberDecl]) -> None:
        self.name = name
        self.members = members
        
    def lower_ast(self, state: LowerState) -> GlobalDecl:
        pass
    
    def generate_decl(self, ps: PrintStream):
        ps.println("")
        ps.println("")
        ps.println("typedef struct {")
        ps.up()
        for m in self.members:
            m.generate_decl(ps)
        ps.down()
        ps.println("} " + self.name + ";")
