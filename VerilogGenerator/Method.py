from re import template
from DotAccessExpr import DotAccessExpr
from GenerateContext import GenerateContext
from PrintStream import PrintStream
from LowerState import LowerState
from Block import Block
from LocalDecl import LocalDecl
from Goto import Goto
from Ident import Ident
from Label import Label
from LiteralExpr import LiteralExpr
from SwitchStmt import CaseStmt, SwitchStmt
from typing import List


class Method:
    def __init__(self, type, funcname, params, block: Block, template_args: List[LocalDecl]):
        self.type = type
        self.funcname = funcname
        self.params = params
        self.block = block 
        self.template_args = template_args

    def pretty(self):
        print("method " + self.funcname)
        self.block.pretty()
        
    def lower_ast(self):
        state = LowerState(not self.is_function())        
        self.block = self.block.lower_ast(state)
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
    
    def is_function(self):
        if isinstance(self.funcname, DotAccessExpr):
            return False
        return True


    def toStateSwitch(self):
        if self.is_function():
            return self
        
        self.block = self.create_case_stmt()
        return self
        
    def generate_local_vars(self, ps: PrintStream):
        ps.up()
        if not self.is_function():
            ps.println("reg[32:0] state = 0;")
        for k in self.block.getLocalDecls():
            #num_bits = k.get_num_bits()
            ps.println(f"{k.type.str()} {k.var};")
        ps.down()
        
    def generate(self, ps:PrintStream):
        if self.is_function():
            self.generate_function(ps)
        else:
            self.generate_module(ps)

    def get_param_str(self, prefix):
        params = ""
        comma = prefix
        for k in self.params:
            params += comma
            params += k.str()
            comma = ", " + prefix
        return params

    def generate_function(self, ps:PrintStream):
        ctxt = GenerateContext()
        ctxt.in_init = self.funcname.str().find("init") >= 0 or self.funcname.str().find("INITIAL") >= 0

        ps.println("")
        ps.println("")
        params = self.get_param_str("input ")
        ps.println(f"function {self.type.str()} {self.funcname.str()}({params});")       
        ps.println("begin")
        self.generate_local_vars(ps)         
        self.block.generate(ps, ctxt)
        ps.println("end")
        ps.println("endfunction")
        
        
    def generate_module(self, ps:PrintStream):
        da: DotAccessExpr = self.funcname
        module_name = da.left.str()        
        task_name = da.member
        
        params = self.get_param_str("")

        ps.println("")
        ps.println("")
        ps.println(f"module {module_name}({params});")
        

        ps.up()
        if self.template_args:
            for k in self.template_args:
                ps.println("parameter " + k.type.str() + " " + k.var + ";");
        ps.down()

        self.generate_local_vars(ps)
        
        #ps.print("initial")
        #ps.up()
        #ps.print("begin")
        #ps.up()
        
        ps.println("")
        ps.println("")
        ps.println("")
        ps.up()        
        
        ctxt = GenerateContext()

        ps.println("")
        ps.println("")
        ps.println(f"task {task_name}();")            
        self.block.generate(ps, ctxt)
        ps.println("endtask")
        ps.down()
        
        #ps.print("$finish;")
        #ps.down()
        #ps.print("end")
        ps.down()
        ps.println("endmodule")
