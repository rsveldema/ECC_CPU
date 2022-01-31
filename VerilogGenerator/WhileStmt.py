from PrintStream import PrintStream
from Label import Label
from Block import Block
from IfStmt import IfStmt
from Goto import Goto
from LowerState import LoopInfo, LowerState
from LiteralExpr import LiteralExpr
from Statement import Statement
from typing import List

        
class WhileStmt(Statement):
    def __init__(self, expr, code:Block) -> None:
        self.expr = expr
        self.code = code

    def is_one(self, cond):
        if isinstance(cond, LiteralExpr):
            return cond.value == "1"
        return False

    def lower_ast(self, state: LowerState):       
        start_label = Label()
        end_label = Label()
        
        state.push_labels(LoopInfo(start_label, end_label))
        
        
        ret = Block()
        ret.insns.append(start_label)
        ret.insns.append(self.code.lower_ast(state))        
        
        cond = self.expr.lower_ast(state)
        if self.is_one(cond):
            ret.insns.append(Goto(start_label))
        else:
            if_code = Block()
            if_code.insns.append(Goto(start_label))
            ret.insns.append(IfStmt(cond, if_code, None))
                    
        ret.insns.append(end_label)
        
        state.pop_labels()
        return ret
        
        #return WhileStmt(self.expr.lower(), self.code.lower())
    
    def pretty(self):
        print(f"while ({self.expr.str()})")
        self.code.pretty()

    def generate(self, ps: PrintStream):
        ps.print(f"while ({self.expr.str()})")
        self.code.generate(ps)
        
    def getLocalDecls(self) -> List:
        return self.code.getLocalDecls()