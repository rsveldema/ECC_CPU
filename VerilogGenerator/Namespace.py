
from typing import List

from Method import Method


class Namespace:
    def __init__(self, methods: List[Method]):
        self.methods = methods

    def pretty(self):
        print("namespace ")
        for m in self.methods:
            m.pretty()

    def lower_ast(self):
        ret = []
        for p in self.methods:
            ret.append(p.lower_ast())
        return ret