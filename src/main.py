

def a(i):
    return i+1
    
def b(y):
    try:
        return y*y
    except:
        raise TypeError("error")
    
    
class APPLE:
    def __init__(self,price=5,num=10):
        self.price=price
        self.name='apple'
        self.num=num
    def sell(num):
        self.num-=num
    def add(num):
        self.num+=num