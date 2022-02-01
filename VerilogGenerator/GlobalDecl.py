from abc import ABC, abstractmethod
from PrintStream import PrintStream


class GlobalDecl(ABC):
    def __init__(self) -> None:
        super().__init__()
        
    @abstractmethod
    def lower_ast(self, state: 'LowerState') -> 'GlobalDecl':
        pass
    
    @abstractmethod
    def generate_decl(self, ps:PrintStream):
        pass