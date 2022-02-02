from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Label import Label
from Block import Block
from Goto import Goto
from UnaryExpr import UnaryExpr
from Statement import Statement

class IfStmt(Statement):
    def __init__(self, expr, if_code:Block, else_code:Block) -> None:
        self.expr = expr
        self.if_code = if_code
        self.else_code = else_code

    def getLocalDecls(self) -> List:
        ret = self.if_code.getLocalDecls()
        if self.else_code:
            ret.extend(self.else_code.getLocalDecls())
        return ret
    
    def generate(self, ps: PrintStream):
        ps.println(f"if ({self.expr.str()})")
        self.if_code.generate(ps)
        if self.else_code:
            ps.println("else")
            self.else_code.generate(ps)

        
    def lower_ast(self, state: LowerState):
        ret = Block()       
        
        end_label = Label()
        
        if_code = self.if_code.lower_ast(state)
        cond = self.expr.lower_ast(state)


        if self.else_code:
            else_label = Label()
            else_code = self.else_code.lower_ast(state)
            
            jmp_if = Block()
            jmp_if.append(Goto(end_label))
            jmp_empty = Block()

            ret.append(IfStmt(cond, jmp_empty, jmp_if))
            
            ret.append(if_code)
            ret.append(Goto(end_label))
            ret.append(else_label)
            ret.append(else_code)
        else:
            jmp_if = Block()
            jmp_if.append(Goto(end_label))
            jmp_empty = Block()

            ret.append(IfStmt(UnaryExpr("!", cond), jmp_if, None))
            ret.append(if_code)
        ret.append(end_label)        
        return ret
    
    def pretty(self):
        print(f"if ({self.expr.str()})")
        self.if_code.pretty()
        if self.else_code:
            print("else")
            self.else_code.pretty()
            