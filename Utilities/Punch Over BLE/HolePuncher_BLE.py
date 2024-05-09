from typing import List, Iterable, Union
from bleak import BleakClient, BleakScanner, BLEDevice, BleakGATTServiceCollection
from tqdm import tqdm
import mido
import time
import logging
import asyncio

MIDI_FILE_PATH = "d:\Domino\ミカヅキ.mid" # path to midi file
MCODE_SERVICE_UUID = "00003200-0000-1000-D441-30F244F2E354"
MCODE_CHAR_UUID = "00003201-0000-1000-D441-30F244F2E354"
PUNCHER_GATT_SERVICES = [
    MCODE_SERVICE_UUID
]

class HolePuncher():
    PITCH_TO_MBNUM = {93: 30, 91: 29, 89: 28, 88: 27, 87: 26, 86: 25, 85: 24, 84: 23,
                    83: 22, 82: 21, 81: 20, 80: 19, 79: 18, 78: 17, 77: 16, 76: 15,
                    75: 14, 74: 13, 73: 12, 72: 11, 71: 10, 70: 9, 69: 8, 67: 7,
                    65: 6, 64: 5, 62: 4, 60: 3, 55: 2, 53: 1}

    client: BleakClient = None
    services: BleakGATTServiceCollection = None
    noteList: List = []

    def __init__(self, device: Union[str, BLEDevice], services: Iterable[str]):
        self.client = BleakClient(device, services=services, timeout=5)
    
    async def begin(self):
        await self.client.connect()
        # await self.client.pair()
        self.services = self.client.services

    def _phraseMidi(self, midi, pitch=0) -> List:
        ticks_per_beat = midi.ticks_per_beat

        # read all tempo events
        tempoList = []
        tick = 0
        for msg in mido.merge_tracks(midi.tracks):
            tick += msg.time
            if msg.type == "set_tempo":
                m = (tick, msg.tempo)
                if m not in tempoList:
                    tempoList.append(m)

        # read all notes
        tmp_noteList = []
        tick = 0
        for msg in mido.merge_tracks(midi.tracks):
            tick += msg.time
            if msg.type == "note_on" and msg.velocity > 0 and msg.note+pitch in self.PITCH_TO_MBNUM.keys():
                note = (tick, self.PITCH_TO_MBNUM[msg.note+pitch])
                if note not in tmp_noteList:
                    tmp_noteList.append(note)
        
        del tick
        
        # adjust note time
        noteList = []
        real_tick = 0
        relative_tick = 0
        init_sec_per_beat = tempoList[0][1] if tempoList[0][0] == 0 else 500000 # Default 120 BPM (500000 microseconds per beat (quarter note).)
        tick_scale = 1
        for note in tmp_noteList:
            note_delta_tick = note[0] - real_tick

            while len(tempoList) > 1 and note[0] > tempoList[1][0]:
                tempoList.pop(0)
                new_tick_scale = tempoList[0][1] / init_sec_per_beat

                relative_tick += (tempoList[0][0] - real_tick) * tick_scale     # ticks before tempo change
                note_delta_tick -= tempoList[0][0] - real_tick                  # ticks after tempo change still not calculated
                real_tick = tempoList[0][0]

                tick_scale = new_tick_scale

            real_tick += note_delta_tick
            relative_tick += note_delta_tick * tick_scale
            note = (relative_tick / ticks_per_beat, note[1])
            noteList.append(note)

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

    async def punchMidi(self, midi, pitch=0, zoom=1):
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

        # 传输数据
        ba = bytearray("M80 Y96 P0".encode("ASCII"))
        await self.client.write_gatt_char(self.services.get_characteristic(MCODE_CHAR_UUID), ba)

        last_tick = 0
        for note in self.noteList:
            comm = f"M{note[1]} Y{int((note[0]-last_tick)*zoom*96)} P1".encode("ASCII")
            # print(comm)
            last_tick = note[0]

            ba = bytearray(comm)

            await self.client.write_gatt_char(self.services.get_characteristic(MCODE_CHAR_UUID), ba)
            pbar.update(1)
        
        ba = bytearray("M90 Y96 P0".encode("ASCII"))
        await self.client.write_gatt_char(self.services.get_characteristic(MCODE_CHAR_UUID), ba)
    
    async def end(self):
        await self.client.disconnect()


async def main():
    mifiFilePath = MIDI_FILE_PATH
    puncherService = PUNCHER_GATT_SERVICES   # e.g. COM3 in windows

    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s')
    midi = mido.MidiFile(mifiFilePath)

    logging.info("Scanning devices...")
    devices = await BleakScanner.discover()
    for i in range(len(devices)):
        logging.info(f"{i}. {devices[i]}")
    n = input("Select device:")
    if n.isdigit():
        n = int(n)
    else:
        logging.info("Quit")
        raise SystemExit

    puncher = HolePuncher(devices[n], puncherService)
    await puncher.begin()
    await puncher.punchMidi(midi, pitch=0, zoom=1.0)
    await puncher.end()


if __name__ == '__main__':
    l = asyncio.get_event_loop()
    l.run_until_complete(main())