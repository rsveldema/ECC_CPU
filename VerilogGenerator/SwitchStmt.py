from typing import List
from BinaryExpr import BinaryExpr
from CaseStmt import CaseStmt
from GenerateContext import GenerateContext
from IfStmt import IfStmt
from Label import Label
from PrintStream import PrintStream
from LowerState import LoopInfo, LowerState
from Statement import Statement
from Block import Block

    
    
class SwitchStmt(Statement):
    def __init__(self, expr, cases: List[CaseStmt]) -> None:
        self.expr = expr
        self.cases = cases
        
    # lower switch stmt down to a sequence of if-else stmts
    def lower_ast(self, state:LowerState):
        if state.is_task():
            return self.lower_to_if_cascade(state)
        else:
            return self.lower_simple(state)

    def lower_simple(self, state:LowerState):
        newcases = []
        for c in self.cases:
            newcases.append(c.lower_ast(state))
        return SwitchStmt(self.expr.lower_ast(state), newcases)


    def lower_to_if_cascade(self, state:LowerState):
        root = Block()
        root_blk = root
  
        start_label = Label()
        end_label = Label()
        
        state.push_labels(LoopInfo(start_label, end_label))

        ix = 0
        while ix < len(self.cases):
            k = self.cases[ix]
            if k.expr:
                cond = BinaryExpr("==", self.expr.lower_ast(state), k.expr.lower_ast(state))

                while not self.cases[ix].blk:
                    ix += 1
                    k = self.cases[ix]
                    other_cond = BinaryExpr("==", self.expr.lower_ast(state), k.expr.lower_ast(state))
                    cond = BinaryExpr("||", cond, other_cond)

                if_part = k.blk.lower_ast(state)
                ifstmt = IfStmt(cond, if_part, root)
                root = ifstmt
                ix += 1
            else:
                root_blk.append(k.blk.lower_ast(state))
                ix += 1

        #return SwitchStmt(self.expr.lower_ast(state), newcases)
        new_blk = Block()
        new_blk.append(start_label)
        new_blk.append(root.lower_ast(state))
        new_blk.append(end_label)
        return new_blk

    def getLocalDecls(self) -> List:
        ret = []
        for k in self.cases:
            ret.extend(k.getLocalDecls())
        return ret
    

    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        ps.println(f"case ({self.expr.str()})")
        ps.up()
        for p in self.cases:
            p.generate(ps, ctxt)
        ps.down()

        ps.println("endcase")

    def pretty(self):
        print(f"switch ({self.expr.str()})") 
        for p in self.cases:
            p.pretty()
            
