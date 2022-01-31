from abc import ABC, abstractmethod
from typing import List
from PrintStream import PrintStream

class Statement(ABC):
    
    @abstractmethod
    def generate(self, ps: PrintStream):
        pass
    
    @abstractmethod
    def lower_ast(self, state: 'LowerState') -> 'Statement':
        pass
    
    @abstractmethod
    def getLocalDecls(self) -> List:
        pass
    