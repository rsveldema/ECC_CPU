from typing import List
from Type import Type
from PrintStream import PrintStream
from GlobalDecl import GlobalDecl
from LowerState import LowerState

class EnumDecl(GlobalDecl):
    def __init__(self, name:str, type:Type, vars: List[str]) -> None:
        self.vars = vars
        self.name = name
        self.vars = vars
    

    def lower_ast(self, state: LowerState): 
        return self
        
    def generate_decl(self, ps:PrintStream):
        ps.println("")
        ps.println("")
        ps.println("typedef enum byte {")
        comma = ""
        for v in self.vars:
            ps.println("\t" + comma + v)
            comma = ","
        ps.println("} " + self.name + ";")