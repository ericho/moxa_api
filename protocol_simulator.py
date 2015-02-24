# -*- coding: utf-8 -*-
from struct import pack
import random
import serial
import config
import os
import threading
import time
import sys


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
        # Getting initial configuration
        self.config = config.main_config
        config.config()
        #self.signal = signal.signal(signal.SIGINT, self.end_thread)

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
                print type(read_byte), read_byte
            except KeyboardInterrupt:
                keep_running = False

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


class Node(object):

    def __init__(self):
        self.node_type = 0
        self.card_type = 0
        self.ext_addr = 0
        self.short_addr = 0
        self.lqi = 0
        self.rssi = 0
        self.acc_x = 0
        self.acc_y = 0
        self.irrometer = 0
        self.dist = 0
        self.bat = 0
        self.temp = 0
        # Creating address
        self.short_addr = random.randint(0, 65535)
        self.stream = ''

    def generate_data(self):
        self.lqi = random.randint(0, 255)
        self.rssi = random.randint(-128, 128)
        self.acc_x = random.uniform(-1000, 1000)
        self.acc_y = random.uniform(-1000, 1000)
        self.irrometer = random.uniform(0, 1000)
        self.dist = random.uniform(0, 200)
        self.bat = random.uniform(0, 13)
        self.temp = random.uniform(0, 100)

    def print_data(self):
        print 'Node type: %d' % self.node_type
        print 'Card type: %d' % self.card_type
        print 'Extended address: %x' % self.ext_addr
        print 'Short address: %x' % self.short_addr
        print 'LQI: %d' % self.lqi
        print 'RSSI: %d' % self.rssi
        print 'Acc X: %f' % self.acc_x
        print 'Acc Y: %d' % self.acc_y
        print 'Irrometer: %f' % self.irrometer
        print 'Dist: %f' % self.dist
        print 'Battery: %f' % self.bat
        print 'Temp: %f' % self.temp

    def serialize_data(self):
        self.stream = pack('!B', self.node_type)
        self.stream += pack('!B', self.card_type)
        self.stream += pack('!I', self.ext_addr)
        self.stream += pack('!H', self.short_addr)
        self.stream += pack('!B', self.lqi)
        self.stream += pack('!b', self.rssi)
        self.stream += pack('!f', self.acc_x)
        self.stream += pack('!f', self.acc_y)
        self.stream += pack('!f', self.irrometer)
        self.stream += pack('!f', self.dist)
        self.stream += pack('!f', self.bat)
        self.stream += pack('!f', self.temp)

if __name__ == "__main__":
    ps = ProtocolSimulator(1, "/dev/pts/4")
    ps.open_port()
    ps.start_reading()
    #ps.create_nodes()
    #ps.generate_data()
    #ps.write_data()
    #ps.close_file()
