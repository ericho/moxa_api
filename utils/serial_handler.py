import serial

class SerialHandler(object):
    """ Class to handle all the serial operations """

    def __init__(self):
        self.serial = None

    def open_device(self, port):
        """ Open the device the specified port """
        try:
            self.serial = serial.Serial(port, baudrate=38400)
        except Exception as e:
            print "Error: ", e
            return False

    def close_device(self):
        """ Close the port to the device """
        self.serial.close()

    def read_data(self):
        """ This function receives data until the mark of end of frame is received. 
            Returns a list with all the bytes received.
        """
        keep_running = True
        recv_data = list()
        while keep_running:
            try:
                recv_byte = self.serial.read()
                if recv_byte == '\xaa':
                    keep_running = False
                recv_data.append(recv_byte)
            except KeyboardInterrupt:
                keep_running = False
        return recv_data

    def write(self, stream):
        self.serial.write(stream)

    def write_data(self):
        for node in self.nodes:
            self.write(self.pack_data(node.stream))
