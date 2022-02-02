from typing import List
from PrintStream import PrintStream
from LowerState import LowerState
from Goto import Goto

class Continue:
    def __init__(self) -> None:
        pass

    def lower_ast(self, state: LowerState):
        k = state.get_labels()
        return Goto(k.start)
    
    
    def pretty(self):
        print("continue")
        
    def generate(self, ps: PrintStream):
        ps.println("continue;")


    def getLocalDecls(self) -> List:
        return []