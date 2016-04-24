# -*- coding: utf-8 -*-
from struct import pack
import random

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
