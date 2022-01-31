from PrintStream import PrintStream
from LowerState import LowerState

class Ident:
    def __init__(self, value) -> None:
        self.value = value

    def lower(self, state: LowerState):
        return self
    
    def str(self) -> str:
        return self.value

