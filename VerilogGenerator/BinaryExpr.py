from typing import List
from GenerateContext import GenerateContext
from PrintStream import PrintStream
from LowerState import LowerState
from Expr import Expr

class BinaryExpr: 
    def __init__(self, op, left:Expr, right:Expr) -> None:
        self.op = op
        self.left = left
        self.right = right

    def generate(self, ps: PrintStream, ctxt: GenerateContext):
        ps.println(f"{self.stmt_str(ctxt)};")
        
    def lower_ast(self, state: LowerState):
        return BinaryExpr(self.op, self.left.lower_ast(state), self.right.lower_ast(state))
    
    def str(self) -> str:
        if self.op == "[]":
            return self.left.str() + "[" + self.right.str() + "]"
              
        if self.op == "=" or self.op == "+=" or self.op == "-=" or self.op == "|=" or self.op == "&=":
            return self.left.str() + self.op + self.right.str() 
            
        return "(" + self.left.str() + " " + self.op + " " + self.right.str() + ")"

    def stmt_str(self, ctxt: GenerateContext) -> str:
        if self.op == "[]":
            return self.left.str() + "[" + self.right.str() + "]"

        asg = " = " if ctxt.in_init else " <= "
                
        if self.op == "=":
            return self.left.str() + asg + self.right.str() 

        if self.op == "+=" or self.op == "-=" or self.op == "|=" or self.op == "&=":
            op = self.op[0]
            return self.left.str() + asg + self.left.str() + " " + op + " " + self.right.str() 
            
        return "(" + self.left.str() + " " + self.op + " " + self.right.str() + ")"

    def pretty(self):
        print(self.str())

    def getLocalDecls(self) -> List:
        return []