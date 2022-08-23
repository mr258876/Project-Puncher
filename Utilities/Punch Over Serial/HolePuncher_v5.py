import struct
from typing import List
from mido import MidiFile
from serial import Serial
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

    # PITCH_TO_MBNUM = {93: 29, 91: 28, 89: 27, 88: 26, 87: 25, 86: 24, 85: 23, 84: 22,
    #                 83: 21, 82: 20, 81: 19, 80: 18, 79: 17, 78: 16, 77: 15, 76: 14,
    #                 75: 13, 74: 12, 73: 11, 72: 10, 71: 9, 70: 8, 69: 7, 67: 6,
    #                 65: 5, 64: 4, 62: 3, 60: 2, 55: 1, 53: 0}

    ser = None
    noteList: List = []

    def __init__(self, port: str, bps: int):
        self.ser = Serial(port, bps, timeout=5)

    def readSerial(self):
        while True:
            if self.ser.in_waiting:
                data = self.ser.read_all().decode("ASCII")
                print(data)
            time.sleep(0.5)

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

    def punchMidi(self, midi, pitch=0, zoom=1):
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

        logging.info("Sending Data...")
        pbar = tqdm(range(len(self.noteList)))
        pbar.set_description("Sending data...")

        self.ser.read_all()
        # 启用串口指令
        self.ser.write(
            bytearray([0xC0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76]))
        bresp = [0 for i in range(16)]
        self.ser.readinto(bresp)
        # 查询打孔机状态
        self.ser.write(
            bytearray([0xC0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8]))
        bresp = [0 for i in range(16)]
        self.ser.readinto(bresp)
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x10]:
            raise Exception("Client Busy. Response:%s" % str(bresp))

        # 进入传输模式
        self.ser.write(
            bytearray([0xC0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAF]))
        bresp = [0 for i in range(16)]
        self.ser.readinto(bresp)
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x01]:
            raise Exception("Client Not Ready. Response:%s" % str(bresp))

        # 传输数据
        for note in self.noteList:
            ba = bytearray()
            ba.append(0xD0)
            ba.append(note[1])
            ba = ba + \
                struct.pack("d", (note[0])/ticks_per_beat*8.0)
            for i in range(5):
                ba.append(0x00)
            ba.append(self.getCRC8(ba))

            self.ser.write(ba)
            pbar.update(1)

            bresp = [0 for i in range(16)]
            self.ser.readinto(bresp)
            if not self.checkCRC8(bresp):
                raise Exception("Transmission Error. Response:%s" % str(bresp))
            if bresp[0] != 0xE0:
                raise Exception(
                    "Client Response Error. Response:%s" % str(bresp))
            if bresp[1:3] != [0x01, 0x01]:
                raise Exception(
                    "Client Receive Fail. Response:%s" % str(bresp))

        # 结束传输模式
        self.ser.write(
            bytearray([0xC0, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A]))
        bresp = [0 for i in range(16)]
        self.ser.readinto(bresp)
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x10]:
            raise Exception("Client Not Ready. Response:%s" % str(bresp))

        # 启动打孔
        self.ser.write(
            bytearray([0xC0, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D]))
        bresp = [0 for i in range(16)]
        self.ser.readinto(bresp)
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x10, 0x11]:
            raise Exception("Client Unable to punch. Response:%s" % str(bresp))

        # 结束串口通信
        self.ser.write(
            bytearray([0xC0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C]))
        bresp = [0 for i in range(16)]
        self.ser.readinto(bresp)
        if not self.checkCRC8(bresp):
            raise Exception("Transmission Error. Response:%s" % str(bresp))
        if bresp[0] != 0xE0:
            raise Exception("Client Response Error. Response:%s" % str(bresp))
        if bresp[1:3] != [0x01, 0x01]:
            raise Exception("Not Success. Response:%s" % str(bresp))

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
    puncherSerial = "PUNCHER_SERIAL"   # e.g. COM3 in windows

    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s')
    midi = mido.MidiFile(mifiFilePath)
    puncher = HolePuncher(puncherSerial, 115200)
    puncher.punchMidi(midi, pitch=4, zoom=1.0)
    # puncher.readSerial()
