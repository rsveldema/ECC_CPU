from abc import ABC, abstractmethod

from LowerState import LowerState


class Expr(ABC):
    
    def __init__(self) -> None:
        super().__init__()
        
    @abstractmethod
    def lower_ast(self, state: LowerState) -> 'Expr':
        pass
        
    @abstractmethod
    def str(self) -> str:
        pass