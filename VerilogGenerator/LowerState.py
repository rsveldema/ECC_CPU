from Label import Label

class LoopInfo:
    def __init__(self, start: Label, end:Label) -> None:
        self.start = start
        self.end = end

class LowerState:
    def __init__(self) -> None:
        self.stack = []
    
    def push_labels(self, loop: LoopInfo):
        self.stack.append(loop)
        
    def pop_labels(self):
        self.stack.pop()
        
    def get_labels(self) -> LoopInfo:
        return self.stack[len(self.stack) - 1]