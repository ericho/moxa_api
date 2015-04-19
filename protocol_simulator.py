# -*- coding: utf-8 -*-
import serial
import config
import os
import threading
import time
import sys
import struct

HEADER = '\x55'
FOOTER = '\xaa'

class FrameCommand(object):
    """ Class to represents the command structure """

    def __init__(self):
        self.id_message = 0
        self.command = 0
        self.data_length = 0
        self.data = ''
        

    def to_serial(self):
        frame_len = struct.pack('B', self.data_length + 6)
        packed_data = HEADER + '\x00' + frame_len + struct.pack('B', self.id_message) + self.command
        if self.data_length > 0:
            packed_data += self.data
        packed_data += FOOTER
        return packed_data
        
    def __str__(self):
        return self.__repr__()

    def __repr__(self):
        msg = "ID Message : {0}\nCommand :" \
              " {1}\nData Length : {2}\nData : {3}\n".format(self.id_message,
                                                             self.command,
                                                             self.data_length,
                                                             self.data)
        return msg

class ProtocolMessages(object):
    """ To handle all the protocol messages and commands """

    def __init__(self):

        self.protocol_messages = {
            'COMMAND_RECEIVED': '\x00',
            'INCORRECT_FRAME_SIZE': '\x01',
            'WRONG_HEADER': '\x02',
            'WRONG_FOOTER': '\x03',
            'UNKNOWN_COMMAND': '\x04',
            'TIMEOUT_ERROR': '\x05'
        }

        self.protocol_command = {
            'DEVICE_STATUS': '\x01',
            'GET_NETWORK_STATUS': '\x02',
            'GET_CHILDREN_AMOUNT': '\x03',
            'BAD_PARAMETERS': '\x56',
            'DEVICE_UP': '\x57',
            'IN_NETWORK_STATUS': '\x58',
            'OUT_NETWORK_STATUS': '\x59',
            'ENABLE_DATA_RECEPTION': '\x5a',
            'DISABLE_DATA_RECEPTION': '\x5b',
            'DATA_RECEPTION_CHANGED': '\x5c',
            'SET_REQUEST_MODE': '\x5d',
            'SET_SLEEP_MODE': '\x5e',
            'SET_RF_MODE': '\x5f',
            'NODE_COMMAND_SENT': '\x60'
        }

        self.command_responses = {
            1 : self.device_status,
            2 : self.network_status,
            3 : self.children_amount,
            4 : self.children_list,
            5 : self.lqi_rssi,
            6 : self.send_data_node,
            7 : self.data_from_node
        }

    def device_status(self):
        print "Device status called"
        fc = FrameCommand()
        fc.command = self.protocol_command['DEVICE_UP']
        return fc

    def network_status(self):
        print "Get Network status"

    def children_amount(self):
        print "Get Children amount"

    def children_list(self):
        print "Get children list"

    def lqi_rssi(self):
        print "Get LQI RSSI"

    def send_data_node(self):
        print "Send data node"

    def data_from_node(self):
        print "Data from node"
        
    def incorrect_frame_size(self):
        pass

    def wrong_header(self):
        pass

    def wrong_footer(self):
        pass

    def unknown_command(self):
        pass

    def _pack_message(self, msg):
        pass

class ProtocolSimulator(object):
    """ This class generates random data and stores it into a file
        in order to simulate a Wireless Sensor Network gathering device.
        This data will be used by moxa_api """

    def __init__(self, n_nodes, filename):
        """ n_nodes : Number of nodes to be simulated
            filename : File name for pipe data """
        self.data = None
        self.file = filename
        self.n_nodes = n_nodes
        self.fd = None
        self.HEADER = '\x55'
        self.FOOTER = '\xaa'
        self.ID = '\x01'
        self.CMD = '\x07'
        self.nodes = []
        self.frame_recv = []
        self.frame_detected = False
        self.frame_length = 0
        # Getting initial configuration
        self.config = config.main_config
        config.config()
        #self.signal = signal.signal(signal.SIGINT, self.end_thread)
        self.pm = ProtocolMessages()

    def open_port(self):
        try:
            self.serial = serial.Serial(self.file, baudrate=38400)
        except Exception as e:
            print "Error", e
            return False

    def start_reading(self):
        try:
            print "Launching reading thread..."
            self.thread = threading.Thread(target=self.read)
            self.thread.start()
        except Exception as e:
            print "Error running read thread", e

    def end_thread(self, signal, frame):
        print "Signal received"
        self.thread.exit()

    def read(self):
        """ This function runs in a thread to continuosly read the port """
        keep_running = True
        while keep_running:
            try:
                read_byte = self.serial.read()
                if read_byte == '!':
                    keep_running = False
                self.detect_frame(read_byte)
            except KeyboardInterrupt:
                keep_running = False

    def detect_frame(self, data):
        """ This function receives a byte a stores into a list, detects if
            the frame is a valid frame in the protocol. When the frame is
            correct and complete notifies that there is a new frame available """
        self.frame_recv.append(data)
        if len(self.frame_recv) >= 3:
            # We need at leat three bytes to start analyzing the frame
            if not self.frame_detected:
                # There is not a frame detected, start to analyze the bytes
                # in the list.
                if self.frame_recv[0] == self.HEADER:
                    # Let's see if the length is valid:
                    _extract_bytes = self.frame_recv[1:3]
                    _frame_length = _extract_bytes[0]
                    _frame_length += _extract_bytes[1]
                    self.frame_length = struct.unpack('!H', _frame_length)[0]
                    print "The lenght is {0}".format(self.frame_length)
                    if self.frame_length >= 6 and self.frame_length <= 261:
                        # Sorry for the magic numbers above.
                        # The protocols says only frames between 6 and 261 are allowed.
                        # Is a Bitcloud limitation to every message sent into the network.
                        self.frame_detected = True
                    else:
                        # The frame has incorrect length
                        self._msg_incorrect_size()
                        self._clean_frame()

                else:
                    # Cleaning the list
                    self._msg_wrong_header()
                    self._clean_frame()
            else:
                # We are in the process of received a frame
                # We'll wait until the length in the frame is the same as the
                # length detected by the protocol.
                if self.frame_length == len(self.frame_recv):
                    print "Frame received!"
                    if self.frame_recv[self.frame_length - 1] == self.FOOTER:
                        print "Frame complete and valid"
                        data_command = FrameCommand()
                        data_command.id_message = struct.unpack('B', self.frame_recv[3])[0]
                        data_command.command = struct.unpack('B', self.frame_recv[4])[0]
                        data_command.data_length = len(self.frame_recv) - 6
                        for i in range(data_command.data_length):
                            data_command.data += self.frame_recv[i + 5]
                        print data_command
                        self.parse_command(data_command)
                        # TODO: Do something with this data_command
                        # Clean up an receive new frames
                        self._clean_frame()
                    else:
                        self._msg_wrong_footer()
                        self._clean_frame()

    def parse_command(self, data_command):
        """ Parses the command inside a FrameCommand object and call
            the appropiate function to respond. """
        print "Command recv {0}".format(data_command.command)
        callback = self.pm.command_responses[data_command.command]
        res = callback()
        self.write(res.to_serial())
        

    def _clean_frame(self):
        self.frame_length = 0
        self.frame_recv = []
        self.frame_detected = False

    def _msg_incorrect_size(self):
        self.write(self.pack_message(self.pm.protocol_messages['INCORRECT_FRAME_SIZE']))

    def _msg_wrong_header(self):
        self.write(self.pack_message(self.pm.protocol_messages['WRONG_HEADER']))

    def _msg_wrong_footer(self):
        self.write(self.pack_message(self.pm.protocol_messages['WRONG_FOOTER']))

    def _msg_unknown_command(self):
        self.write(self.pack_message(self.pm.protocol_messages['UNKNOWN_COMMAND']))

    def push_recv_byte(self, recv_byte):
        """ A queue handles the byte received to detect a valid frame """
        pass

    def write(self, stream):
        self.serial.write(stream)
        #self.fd.write(stream)

    def write_data(self):
        for node in self.nodes:
            self.write(self.pack_data(node.stream))

    def open_file(self):
        try:
            self.fd = open(self.file, "wb")
        except:
            print 'Cannot open the file %s' % self.file

    def close_file(self):
        self.serial.close()
        #self.fd.close()

    def create_nodes(self):
        for i in range(0, self.n_nodes):
            self.nodes.append(Node())

    def generate_data(self):
        for node in self.nodes:
            node.generate_data()
            node.serialize_data()

    def pack_data(self, data):
        frame_len = pack('B', len(data) + 6)
        packed_data = self.HEADER + '\x00' + frame_len + self.ID + self.CMD
        if len(data) > 0:
            packed_data += data
        packed_data += self.FOOTER
        return packed_data

    def pack_message(self, msg):
        # Messages are always 6 bytes long
        frame_len = struct.pack('B', 6)
        packed_msg = self.HEADER + '\x00' + frame_len + self.ID + msg + self.FOOTER
        return packed_msg

    def data_to_serial(self, fc):
        frame_len = struct.pack('B', fc.data_length + 6)
        packed_data = self.HEADER + '\x00' + frame_len + self.ID + struct.pack('B', fc.command)
        if fc.data_length > 0:
            packed_data += data
        packed_data += self.FOOTER
        return packed_data


if __name__ == "__main__":
    ps = ProtocolSimulator(1, "/dev/pts/23")
    ps.open_port()
    ps.start_reading()
    #ps.create_nodes()
    #ps.generate_data()
    #ps.write_data()
    #ps.close_file()
