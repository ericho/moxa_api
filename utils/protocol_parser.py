import struct
from serial_handler import SerialHandler
from protocol_simulator import FrameCommand

HEADER = '\x55'
FOOTER = '\xaa'

class ProtocolParser(object):
    """ A RS232 to ZigBee protocol parser """

    def __init__(self, port):
        self.sh = SerialHandler()
        self.sh.open_device(port)

    def terminate(self):
        self.sh.close_device()

    def send_command(self, cmd):
        stream = self.command_to_frame(cmd)
        self.sh.write(stream)

    def receive_command(self):
        """ Reads from serial port until the FOOTER has arrived, then parses
            all the data and converts into a FrameCommand instance.
            This instance is returned.
        """
        recv = self.sh.read_data()
        if self._is_valid_frame(recv):
            return self.frame_to_object(recv)
        else:
            return None

    def frame_to_object(self, data):
        """ Extracts the data from a data frame and create an object
            of FrameCommand
        """
        if len(data) < 6:
            return None

        cmd = FrameCommand()
        cmd.id_message = struct.unpack('B', data[3])[0]
        cmd.command = struct.unpack('B', data[4])[0]
        cmd.data_length = struct.unpack('!H', data[1] + data[2])[0]
        if cmd.data_length > 0:
            cmd.data = data[5:len(data)-1]
        else:
            cmd.data = None

        return cmd

    def command_to_frame(self, cmd):
        """ Converts a FrameCommand instance into a serial data frame
            to be send through serial port
        """
        packed_data = HEADER
        packed_data += '\x00'
        packed_data += struct.pack('B', cmd.data_length + 6)
        packed_data += struct.pack('B', cmd.id_message)
        packed_data += struct.pack('B', cmd.command)
        if cmd.data_length > 0:
            for i in cmd.data:
                packed_data += cmd.data[i]
        packed_data += FOOTER
        return packed_data

    def _is_valid_frame(self, data):
        """ Checks if data meets the basic struture to be considered
            a data frame in the protocol
        """
        if len(data) < 6:
            return False

        if data[0] == HEADER and data[len(data) - 1] == FOOTER:
            return True
        return False


if __name__ == "__main__":
    pp = ProtocolParser('/dev/pts/4')
    r = pp.receive_command()
    print r
    r.command = struct.unpack('B', '\x57')[0]
    print r
    pp.send_command(r)
    pp.terminate()
