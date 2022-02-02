from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Statement import Statement

class Block(Statement):
    def __init__(self) -> None:
        self.insns : List[Statement] = []

    def pretty(self):
        print("{")        
        for insn in self.insns:
            insn.pretty()
        print("}")
        
    def append_recurs(self, b, k):
        if isinstance(k, Block):
            for p in k.insns:
                self.append_recurs(b, p)
        else:
            b.insns.append(k)

    def append(self, insn):
        self.append_recurs(self, insn)
        
    def getLocalDecls(self):
        ret = []
        for k in self.insns:
            r = k.getLocalDecls()
            ret.extend(r)
        return ret
        
    def lower_ast(self, state: LowerState):
        b = Block()        
        for insn in self.insns:
            k = insn.lower_ast(state)
            self.append_recurs(b, k)
        return b
    
    
    def get_last_stmt(self):
        if len(self.insns) == 0:
            return None
        return self.insns[len(self.insns) - 1]
        
    def generate(self, ps: PrintStream):
        ps.println("begin")
        ps.up()
        for insn in self.insns:
            insn.generate(ps)
        ps.down()
        ps.println("end")
