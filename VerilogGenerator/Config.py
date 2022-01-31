

glob__instance: 'Config' = None


class Config:        
    
    def __init__(self) -> None:
        self.d = {}

        fp = open("../Simulator/config.txt")
        lines = fp.readlines()
        for line in lines:
            k = line.split()
            if len(k) > 2:
                key = k[1]
                value = k[3]
                self.d[key] = value
        fp.close()
        
        
        
    def get_size(self, name):
        if not name in self.d:
            print("cannot get the size of type " + name)
            return 10000
        return self.d[name]
    
    def getInstance() -> 'Config':
        global glob__instance
        if glob__instance == None:
            glob__instance = Config()            
        return glob__instance