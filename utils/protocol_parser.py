from serial_handler import SerialHandler

class ProtocolParser(object):
    """ A RS232 to ZigBee protocol parser """

    def __init__(self):
        self.sh = SerialHandler()
        self.sh.open_device('/dev/pts/4')

    def send_command(self):
        pass

    def receive_command(self):
        recv = self.sh.read_data()
        
    def _is_valid_frame(self, data):
        """ Checks if data meets the basic struture to be considered
            a data frame in the protocol
        """
        if len(data) < 6:
            return False

        if data[0] == HEADER and data[len - 1] == FOOTER:
            return True
        return False

    def frame_to_object(self, data):
        """ Extracts the data from a data frame and create a object 
            of FrameCommand
        """
        pass

    def command_to_frame(self, command):
        """ Converts a FrameCommand instance into a serial data frame
            to be send through serial port
        """
        pass
        
    def terminate(self):
        self.sh.close_device()

if __name__ == "__main__":
    pp = ProtocolParser()
    pp.receive_command()
    pp.terminate()
