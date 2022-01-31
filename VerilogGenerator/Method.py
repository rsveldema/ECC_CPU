from PrintStream import PrintStream
from LowerState import LowerState
from Block import Block
from LocalDecl import LocalDecl
from Goto import Goto
from Ident import Ident
from Label import Label
from LiteralExpr import LiteralExpr
from SwitchStmt import CaseStmt, SwitchStmt


class Method:
    def __init__(self, type, funcname, params, block: Block):
        self.type = type
        self.funcname = funcname
        self.params = params
        self.block = block 

    def pretty(self):
        print("method " + self.funcname)
        self.block.pretty()
        
    def lower(self):
        state = LowerState()        
        self.block = self.block.lower(state)
        return self
                
    def create_case_stmt(self):
        cases = []
        value = LiteralExpr(0)
        current_case = CaseStmt(value, Block())
        cases.append(current_case)
        
        for k in self.block.insns:
            if isinstance(k, Label):
                last = current_case.get_last_stmt()
                if last == None or not isinstance(last, Goto):
                    current_case.blk.append(Goto(k))
                
                current_case = CaseStmt(LiteralExpr(k.label), Block())
                cases.append(current_case)
            else:
                current_case.blk.append(k)
        sw = SwitchStmt(Ident("state"), cases)
        return sw
    
    
    def toStateSwitch(self):
        self.block = self.create_case_stmt()
        return self
        
    def generate_local_vars(self, ps: PrintStream):
        ps.up()
        ps.print("reg[32:0] state = 0;")
        for k in self.block.getLocalDecls():
            #num_bits = k.get_num_bits()
            ps.print(f"{k.type.str()} {k.var} = 0;")
        ps.down()
        
    def generate(self, ps:PrintStream):
        if self.funcname.find("::") > 0:
            self.generate_module(ps)
        else:
            self.generate_function(ps)

    def generate_function(self, ps:PrintStream):
        ps.print("function " + self.funcname + ";")
        ps.print("endfunction")
        
        
    def generate_module(self, ps:PrintStream):
        module_name = self.funcname.split("::")[0]
        task_name = self.funcname.split("::")[1]
                
        ps.print("module " + module_name + ";")
        
        self.generate_local_vars(ps)
        
        #ps.print("initial")
        #ps.up()
        #ps.print("begin")
        #ps.up()
        
        ps.print("")
        ps.print("")
        ps.print("")
        ps.up()
        ps.print(f"task {task_name};")            
        self.block.generate(ps)
        ps.print("endtask")
        ps.down()
        
        #ps.print("$finish;")
        #ps.down()
        #ps.print("end")
        ps.down()
        ps.print("endmodule")
