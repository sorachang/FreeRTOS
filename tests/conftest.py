import pytest
import socket
import threading
@pytest.fixture
def db_connection():
    soc=socket.socket()
    soc.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    soc.bind(("172.16.1.113",8888))
    soc.listen()
    threading.Thread(target=soc.accept).start()
    yield soc