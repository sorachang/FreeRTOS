import pytest
import socket
import threading


import socket
from src.main import *
@pytest.mark.string
def test_a():
    assert a(1)+1==3

def test_b():
    assert b(3)==9
    with pytest.raises((TypeError,ValueError)) as exp:
        b('a')
    assert "rror" in str(exp.value)
    
def test_query(db_connection):
    # 使用db_connection進行測試
    client=socket.socket()
    client.connect(("172.16.1.113",8888))
    assert client.send(b"")==0

@pytest.mark.parametrize("test_input, expected", [(1,1), (2,2), (3,3), (None,5)])
def test_classapple(test_input,expected):
    if test_input is None:
        apple=APPLE()
    else:
        apple=APPLE(test_input)
    assert apple.name=='apple'
    assert apple.price==expected
    assert apple.num==10