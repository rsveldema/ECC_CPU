
class Namespace:
    def __init__(self, methods):
        self.methods = methods

    def pretty(self):
        print("namespace ")
        for m in self.methods:
            m.pretty()

    def lower(self):
        ret = []
        for p in self.methods:
            ret.append(p.lower())
        return ret