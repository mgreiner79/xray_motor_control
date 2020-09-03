# -*- coding: utf-8 -*-
"""
Created on Sat Oct 19 00:16:08 2019

@author: Mark
"""

import tkinter as tk
from tkinter import Label, Entry, Button, Tk, W, E, N, S, messagebox, IntVar, DoubleVar, StringVar, NO, NS, EW, Toplevel, Spinbox, END, Radiobutton
from tkinter.ttk import Combobox, Scrollbar
import serial
import pickle

import serial.tools.list_ports
import os
#os.chdir('C:\\Users\\Mark\\Documents\\Arduino')


class Window:
    def __init__(self):
        self.root = tk.Tk() # This makes a tkinter object instance
        self.root.protocol("WM_DELETE_WINDOW", self._delete_window)
        self.port = StringVar() # This initiates a variable to be bound to a tkinter variable
        self.baud = 9600
        Label(self.root, text = 'X axis').grid(column=0, row=0, padx=10, pady=10)
        Label(self.root, text = 'Y axis').grid(column=1, row=0, padx=10, pady=10)
        Label(self.root, text = 'Z axis').grid(column=2, row=0, padx=10, pady=10)
        infile = open('coordinates','rb')
        current_coordinates = pickle.load(infile)
        infile.close() 
        self.new_x = IntVar() # This is what is to be entered by the user
        self.new_y = IntVar()
        self.new_z = IntVar()
        self.new_x.set(current_coordinates['x']) # This sets the entry boxes to the current coordinates
        self.new_y.set(current_coordinates['y'])
        self.new_z.set(current_coordinates['z'])                      
        self.delta_positions = {'x':0,'y':0,'z':0} # how much the positions should be changed
        self.current_x = current_coordinates['x'] #int() # Where the current position is
        self.current_y = current_coordinates['y'] #int()
        self.current_z = current_coordinates['z'] #int()
        self.x_entry = Entry(self.root, textvariable = self.new_x)
        self.x_entry.grid(column=0, row=1, padx=10, pady=10)
        self.y_entry = Entry(self.root, textvariable = self.new_y)
        self.y_entry.grid(column=1, row=1, padx=10, pady=10)
        self.z_entry = Entry(self.root, textvariable = self.new_z)
        self.z_entry.grid(column=2, row=1, padx=10, pady=10)
        self.com_values = [comport.device for comport in serial.tools.list_ports.comports()]
        self.cbox = Combobox(self.root, width=12, textvariable=self.port)
        self.cbox['values'] = self.com_values
        self.cbox.grid(column=1, row=3)
        Button(self.root, text = 'Connect', command=self.connect).grid(column=2, row=3, padx=10,pady=10)
        Button(self.root, text = 'Send', command=self.writeSerial).grid(column=2, row=4, padx=10,pady=10)
              
    def connect(self):
        self.serial = serial.Serial(self.port.get(), self.baud, timeout=0, writeTimeout=0)
        print('connected')
        #self.root.after(500, self.readSerial)
        
    def writeSerial(self):
        self.delta_positions['x'] = self.new_x.get() - self.current_x
        self.delta_positions['y'] = self.new_y.get() - self.current_y
        self.delta_positions['z'] = self.new_z.get() - self.current_z
        self.current_x = self.new_x.get()
        self.current_y = self.new_y.get()
        self.current_z = self.new_z.get()
        message = ''
        for i in self.delta_positions:
            message += i + ':' + str(self.delta_positions[i]) + ','
            
        message = message.rstrip(',')
        self.serial.write(message.encode())
        
    def _delete_window(self):
        print('destroyed')
        current_coordinates = {'x':self.current_x,'y':self.current_y,'z':self.current_z}
        outfile = open('coordinates','wb')
        pickle.dump(current_coordinates,outfile)
        outfile.close()
        try:
            self.root.destroy()
        except:
            pass
    
window = Window()
window.root.mainloop()


#%%

print([comport.device for comport in serial.tools.list_ports.comports()])


#%%
data.close()

