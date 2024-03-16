from build.sabre import Backend, Model

def test(): 
    c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96), (3,4,0.9),(4,3,0.9)]
    backend = Backend(c_list)
    model = Model(backend)


if __name__ == "__main__":
    test()