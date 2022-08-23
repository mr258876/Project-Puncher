from abc import ABCMeta, abstractmethod
from typing import List

class ConnectionAbst(metaclass=ABCMeta):
    _lastFrame: List[int] = []
    
    @abstractmethod
    def sendData(self, data: List[int]) -> None:
        pass
    
    @abstractmethod
    def readData(self) -> List[int]:
        pass

    @abstractmethod
    def close(self) -> None:
        pass
