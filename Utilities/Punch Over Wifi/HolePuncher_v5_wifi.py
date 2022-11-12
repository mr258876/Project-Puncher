import struct
from typing import List
from mido import MidiFile
from WifiConnection import WifiConnection
from tqdm import tqdm
import mido
import time
import logging
import crcmod

class HolePuncher():
    PITCH_TO_MBNUM = {93: 0, 91: 1, 89: 2, 88: 3, 87: 4, 86: 5, 85: 6, 84: 7,
                      83: 8, 82: 9, 81: 10, 80: 11, 79: 12, 78: 13, 77: 14, 76: 15,
                      75: 16, 74: 17, 73: 18, 72: 19, 71: 20, 70: 21, 69: 22, 67: 23,
                      65: 24, 64: 25, 62: 26, 60: 27, 55: 28, 53: 29}

    conn = None
    noteList: List = []

    def __init__(self, ip: str, port: int = 22576):
        self.conn = WifiConnection(ip, port)

    def _phraseMidi(self, midi, pitch=0) -> List:
        noteList = []

        def getNotes(track):
            notes = []
            tick = 0
            for msg in track:
                tick += msg.time
                if msg.type == "note_on" and msg.velocity > 0 and msg.note+pitch in self.PITCH_TO_MBNUM.keys():
                    notes.append((tick, self.PITCH_TO_MBNUM[msg.note+pitch]))
            return notes

        for track in midi.tracks:
            notes = getNotes(track)
            for i in notes:
                if i not in noteList:
                    noteList.append(i)
        noteList.sort(key=lambda x: x[0])
        return noteList

    def _optimizeNoteSequence(self, nL) -> List:
        # 优化同一拍内打孔顺序
        resultList: List = []
        tempList: List = []
        d = True
        for n in nL:
            if not tempList:
                tempList.append(n)
                continue

            if n[0] != tempList[0][0]:
                tempList.sort(key=lambda x: x[1] if d else -x[1])
                resultList = resultList + tempList

                d = not d
                tempList = [n]
            else:
                tempList.append(n)
        resultList = resultList + tempList
        return resultList

    def punchMidi(self, midi, pitch=0, zoom=1, cursor=1):
        ticks_per_beat = midi.ticks_per_beat/zoom

        logging.info("Punch mission started.")

        logging.info("Midi file processment started.")
        logging.info("Midi Processment [1/2]: Phrasing notes...")
        self.noteList = self._phraseMidi(midi, pitch)
        # print(len(self.noteList))
        logging.info("Midi Processment [2/2]: Optimizing notes sequence...")
        self.noteList = self._optimizeNoteSequence(self.noteList)
        # print(len(self.noteList))
        logging.info("Midi file processment complete.")

        logging.info("Connecting...")

        # 启用串口指令
        self.conn.sendData([0xC0, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93])
        bresp = self.conn.readData()
        # 查询打孔机状态
        self.conn.sendData([0xC0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8])
        bresp = self.conn.readData()
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x10] and bresp[1:3] != [0x10, 0x01]:
            raise Exception("Client Busy. Response:%s" % str(bresp))

        # 进入传输模式
        self.conn.sendData([0xC0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAF])
        bresp = self.conn.readData()
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x01]:
            raise Exception("Client Not Ready. Response:%s" % str(bresp))

        logging.info("Sending data...")
        pbar = tqdm(range(len(self.noteList)))
        pbar.set_description("Sending data...")

        # 传输数据
        for note in self.noteList:
            ba = bytearray()
            ba.append(0xD0)
            ba.append(note[1])
            ba = ba + struct.pack("d", (note[0])/ticks_per_beat*8.0)
            for i in range(5):
                ba.append(0x00)
            ba.append(self.getCRC8(ba))

            self.conn.sendData(ba)
            pbar.update(1)

            bresp = self.conn.readData()
            if not self.checkCRC8(bresp):
                raise Exception("Transmission Error. Response:%s" % str(bresp))
            if bresp[0] != 0xE0:
                raise Exception(
                    "Client Response Error. Response:%s" % str(bresp))
            if bresp[1:3] != [0x01, 0x01]:
                raise Exception(
                    "Client Receive Fail. Response:%s" % str(bresp))

        # 结束传输模式
        self.conn.sendData([0xC0, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A])
        bresp = self.conn.readData()
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x10]:
            raise Exception("Client Not Ready. Response:%s" % str(bresp))
        
        # 设置起始位置
        if cursor > 1:
            ba = bytearray()
            ba.append(0xC0)
            ba.append(0xB0)
            ba = ba + struct.pack("H", cursor)
            for i in range(11):
                ba.append(0x00)
            ba.append(self.getCRC8(ba))

            self.conn.sendData(ba)

            bresp = self.conn.readData()
            if not self.checkCRC8(bresp):
                raise Exception("Transmission Error. Response:%s" % str(bresp))
            if bresp[0] != 0xE0:
                raise Exception(
                    "Client Response Error. Response:%s" % str(bresp))
            if bresp[1:3] != [0x01, 0x01]:
                raise Exception(
                    "Client Receive Fail. Response:%s" % str(bresp))

        # 启动打孔
        self.conn.sendData([0xC0, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D])
        bresp = self.conn.readData()
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x11]:
            raise Exception("Client Not Ready. Response:%s" % str(bresp))

        # 结束串口通信
        self.conn.sendData([0xC0, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE9])
        bresp = self.conn.readData()
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x01, 0x01]:
            raise Exception("Client Not Ready. Response:%s" % str(bresp))

        self.conn.close()

    @staticmethod
    def getCRC8(bytes):
        crc8 = crcmod.predefined.mkCrcFun('crc-8')
        return crc8(bytes)

    @staticmethod
    def checkCRC8(bytes):
        crc8 = crcmod.predefined.mkCrcFun('crc-8')
        return bytes[15] == crc8(bytearray(bytes[0:15]))


if __name__ == '__main__':
    mifiFilePath = "MIDI_FILE_PATH"
    puncherIP = "IP_ADDRESS"

    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s')
    midi = mido.MidiFile(mifiFilePath)
    puncher = HolePuncher(puncherIP)
    puncher.punchMidi(midi, pitch=0, zoom=1.0, cursor=1)
