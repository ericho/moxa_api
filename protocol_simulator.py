# -*- coding: utf-8 -*-
from struct import pack


class ProtocolSimulator(object):
    """ This class generates random data and stores it into a file
        in order to simulate a Wireless Sensor Network gathering device.
        This data will be used by moxa_api """

    def __init__(self, n_nodes, filename):
        self.data = None
        self.file = filename
        self.nodes = n_nodes
        self.fd = None
        self.HEADER = '\x55'
        self.FOOTER = '\xaa'
        self.ID = '\x01'
        self.CMD = '\x07'

    def write(self, stream):
        self.fd.write(stream)

    def open_file(self):
        try:
            self.fd = open(self.file, "wb")
        except:
            print 'Cannot open the file %s' % self.file

    def close_file(self):
        self.fd.close()

    def generate_data(self):
        pass

    def pack_data(self, data):
        frame_len = pack('B', len(data) + 6)
        packed_data = self.HEADER + '\x00' + frame_len + self.ID + self.CMD
        if len(data) > 0:
            packed_data += data
        packed_data += self.FOOTER
        return packed_data


if __name__ == "__main__":
    ps = ProtocolSimulator(1, "stream")
    ps.open_file()
    print ps.pack_data('Hola')
    s = ps.pack_data('Hola')
    ps.write(s)
    ps.close_file()
