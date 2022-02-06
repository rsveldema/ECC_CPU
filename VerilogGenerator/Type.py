from PrintStream import PrintStream
from LowerState import LowerState
from Config import Config

class Type:
    def __init__(self, name) -> None:
        self.name = name

    def lower_ast(self, state: LowerState):
        return self
    
    def str(self) -> str:
        ret = ""
        if isinstance(self.name, str):
            ret = self.name
        else:
            ret = self.name.str()
        ret = ret.replace("::", "__")
        return ret

    def generate(self):
        return self.str()

    def get_num_bits(self) -> int:
        if self.name == "bool":
            return 1
        return 8 * int(Config.getInstance().get_size(self.name.str()))
    
    