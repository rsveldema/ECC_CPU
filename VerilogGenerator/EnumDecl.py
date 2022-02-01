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
        ps.print("")
        ps.print("")
        ps.print("typedef enum {")
        comma = ""
        for v in self.vars:
            ps.print("\t" + comma + v)
            comma = ","
        ps.print("} " + self.name + ";")