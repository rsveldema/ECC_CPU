
class PrintStream:
    def __init__(self, fp) -> None:
        self.fp = fp
        self.indent = 0

    def print(self, str) -> None:
        for i in range(self.indent):
            str = "\t" + str
        self.fp.write(str + "\n")
        
    def up(self):
        self.indent += 1

    def down(self):
        self.indent -= 1
