
from distutils.log import error
from typing import List

from Method import Method
from ConstantDecl import ConstantDecl
from EnumDecl import EnumDecl
from TypeDef import TypeDef
from LowerState import LowerState
from StructDecl import StructDecl
from GlobalDecl import GlobalDecl


class Namespace:
    def __init__(self, name, decls: List[GlobalDecl], methods: List[Method]):
        self.name = name
        self.methods = methods
        self.decls = decls
        
            
    def generate_decls(self, ps):
        state = LowerState()
        for k in self.decls:
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