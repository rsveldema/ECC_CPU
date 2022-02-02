
class PrintStream:
    def __init__(self, fp) -> None:
        self.fp = fp
        self.indent = 0

    def println(self, str) -> None:
        self.print_no_newline(str)
        self.fp.write("\n")
    
    def print_no_newline(self, str) -> None:
        for i in range(self.indent):
            str = "\t" + str
        self.fp.write(str)
        
        
    def print_same_line(self, str) -> None:
        self.fp.write(str)
        
    def up(self):
        self.indent += 1

    def down(self):
        self.indent -= 1
