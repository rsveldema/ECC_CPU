from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Expr import Expr

class DotAccessExpr: 
    def __init__(self, left:Expr, right:List[str]) -> None:
        self.left = left
        self.member_list = right

    def lower_ast(self, state: LowerState):
        return DotAccessExpr(self.left.lower_ast(state), self.member_list)
    
    def const_expr(self):
        return self.left.const_expr() + self.get_member_list_str()

    def str(self) -> str:  
        return  self.left.str() + self.get_member_list_str()

    def get_member_list_str(self):
        k = ""
        for p in self.member_list:
            k += "." + p
        return k


    def pretty(self):
        print(self.str())

    def getLocalDecls(self) -> List:
        return []