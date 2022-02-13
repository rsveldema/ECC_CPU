from typing import List
from GenerateContext import GenerateContext
from PrintStream import PrintStream
from LowerState import LowerState
from Goto import Goto

class Break:
    def __init__(self) -> None:
        pass

    def lower_ast(self, state: LowerState):
        k = state.get_labels()
        return Goto(k.end)
    
    def pretty(self):
        print("break")
        
    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        ps.println("break;")


    def getLocalDecls(self) -> List:
        return []