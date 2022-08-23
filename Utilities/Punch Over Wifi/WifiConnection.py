import pickle
import socket
from typing import List, Optional, Union
from ConnectionAbst import ConnectionAbst


class WifiConnection(ConnectionAbst):
    s:socket.socket
    _lastFrame: List[int]

    def __init__(self, ip: str, port: int = 22576) -> None:
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((ip, port))

    def sendData(self, data: List[int]) -> None:
        _lastFrame = data
        return self.__sendData(data)

    def readData(self) -> List[int]:
        from_server = bytearray(16)
        self.s.recvfrom_into(from_server)
        return list(from_server)
    
    def close(self) -> None:
        self.s.close()

    def __sendData(self, data: Union[bytearray, List[int]]) -> None:
        to_server = None
        if type(data) == list:
            to_server = bytearray()
            for d in data:
                to_server.append(d)
        else:
            to_server = data
        self.s.send(to_server)
