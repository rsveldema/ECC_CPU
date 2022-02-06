
from distutils.log import error
from typing import List

from Method import Method
from ConstantDecl import ConstantDecl
from EnumDecl import EnumDecl
from TypeDef import TypeDef
from LowerState import LowerState
from StructDecl import StructDecl
from GlobalDecl import GlobalDecl


class InterfaceDecl:
    def __init__(self, name, decls: List[GlobalDecl], methods: List[Method]):
        self.name = name
        self.methods = methods
        self.decls = decls
                  

    def generate_decl(self, ps):
        ps.println("interface " + self.name + ";")
        ps.up()
        for k in self.decls:
            k.generate_decl(ps)            
        for k in self.methods:
            k.generate(ps)
        ps.down()
        ps.println("endinterface;")
            
    def pretty(self):
        print("interface " + self.name + "{")
        for m in self.methods:
            m.pretty()
        print("}")

    def lower_ast(self, state: LowerState):        
        ret_methods = []
        for p in self.methods:
            ret_methods.append(p.lower_ast())
        
        self.methods = ret_methods    
        
        return self
    
    