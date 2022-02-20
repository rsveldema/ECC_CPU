from xmlrpc.client import Boolean
from Label import Label

class LoopInfo:
    def __init__(self, start: Label, end:Label) -> None:
        self.start = start
        self.end = end

class LowerState:
    def __init__(self, is_a_task: Boolean) -> None:
        self.stack = []
        self._is_task = is_a_task

    def is_task(self) -> Boolean:
        return self._is_task

    
    def push_labels(self, loop: LoopInfo):
        self.stack.append(loop)
        
    def pop_labels(self):
        self.stack.pop()
        
    def get_labels(self) -> LoopInfo:
        return self.stack[len(self.stack) - 1]