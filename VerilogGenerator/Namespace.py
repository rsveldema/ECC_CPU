
from distutils.log import error
from typing import List

from Method import Method
from ConstantDecl import ConstantDecl
from EnumDecl import EnumDecl
from TypeDef import TypeDef
from LowerState import LowerState


class Namespace:
    def __init__(self, name, decls, methods: List[Method]):
        self.name = name
        self.methods = methods
        self.enums = []
        self.constants = []
        self.typedefs = []
        
        for k in decls:
            if isinstance(k, EnumDecl):
                self.enums.append(k)
            elif isinstance(k, TypeDef):
                self.typedefs.append(k)
            elif isinstance(k, ConstantDecl):
                self.constants.append(k)
            else:
                error("unimplemented global decl")
            
    def generate_decls(self, ps):
        state = LowerState()
        for k in self.constants:
            k.lower_ast(state)
            k.generate_decl(ps)
            
        for k in self.typedefs:
            k.lower_ast(state)
            k.generate_decl(ps)
            
        for k in self.enums:
            k.lower_ast(state)
            k.generate_decl(ps)

    def pretty(self):
        print("namespace ")
        for m in self.methods:
            m.pretty()

    def lower_ast(self):
        ret = []
        for p in self.methods:
            ret.append(p.lower_ast())
        return ret