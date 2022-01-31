from abc import ABC, abstractmethod


class Expr(ABC):
    
    def __init__(self) -> None:
        super().__init__()
        
        
    @abstractmethod
    def str(self) -> str
        pass