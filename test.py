#!/usr/bin/python
import sys
import gtk
import pygtk
import gobject
import os
import re
import time
import threading
swap_device =  ""
disk_names = []
disk_space_names = []
standard = None
standard_names = ["FAST", "TOTAL", "USDD", "RGP", "BHI", "CRTO", "CGB", "TWO"]
## This function is to get the system time and set the calender###
def get_sys_time():
	print time.localtime()
	return time.localtime()


### This function is to excute the system command ###
def sys_cmd(cmd, arg):
		command = cmd + " " + arg
		print "The command is:", command
		os.system(command)

###   This function is to find the device names of the swap space  ###
def find_swap():
	global swap_device
	if(os.path.isfile("/etc/fstab")):
		result = []
		f=file("/etc/fstab", 'r')
		for line in f.readlines():
			if(line.lstrip()[0] == '#'):
				continue
			else:
				for new_line  in line.split():
					if(new_line == "swap"):
						swap_device = line.split()[0]
						print "The swap deviece is:"+swap_device
					else:
						continue
	else:
		print "can not find swap device"
		return 
	print "swap space is mounted on the device:"+swap_device


### This function is to generate the system information file(sys_info.txt)  in current dircetory  ###
def generate_sysinfo_file():
	os.system("rm -rf ./sys_info.txt")
#	os.system("echo operating system: >> sys_info.txt")
#	os.system("cat /etc/issue >> ./sys_info.txt")
	os.system("echo system infomation: >> ./sys_info.txt")
	os.system("lsb_release -a >> ./sys_info.txt")
#	os.system("echo desktop: $COLORTERM  >> ./sys_info.txt")
	os.system("echo disk infomation: >> ./sys_info.txt")
	os.system("ls  /dev/sd* /dev/hd*  >> ./sys_info.txt 2>/dev/null")
				


### This function is to find the disk names and the disk space names in the system ###
def find_disks_names():
	global disk_names 
	global disk_space_names 
	f = file("sys_info.txt", 'r')
	for line in f:
		if (line.startswith("/")):
			if(re.search("\d", line)):
				disk_space_names.append(line)
			else:
				disk_names.append(line)
		else:
			continue
	for i, line in enumerate(disk_names):
		disk_names[i] = line[:-1]
	for i, line in enumerate(disk_space_names):
		disk_space_names[i] = line[:-1]
	print disk_names
	print disk_space_names

		



### This class is the implementation of the software ###
class secure_delete:
	def on_filechooserbutton1_file_set(self, widget, data = None):
		print "one file was selected."
		self.filechooserdialog1.show()


### This function is to go to the main page of Bizsmooth when user click the linkbutton1 ###
	def go_to_mainpage(self, widget, data = None):
		print "Will go to the main page of the Bizsmooth..."

### This function is to quit the main window of the software ###
	def gtk_main_quit(self, widget, data = None):
		gtk.main_quit()
	
	


### This function is to hide the filechooser dialog when user click the cancel button ###
	def filechooserdialog1_quit(self, widget, data = None):
		self.filechooserdialog1.hide()


### This function is to delete the files or the directories you selected and clicked the confirm button ###
	def get_filepath(self, widget, data = None):
		global standard
		self.progressbar1.set_text("Hello")
		#self.progressbar1.show()
		#self.progressbar1.set_fraction(0.3)
		filenames = self.filechooserdialog1.get_filenames()
		print filenames
	        
		totoal_list = []	
		dir_list = []
		file_list = []
		for each_file_dir in filenames:
			totoal_list.append(each_file_dir)
			if os.path.isdir(each_file_dir):
				dir_list.append(each_file_dir)
			else:
				file_list.append(each_file_dir)		
		
		files_to_delete = " ".join(file_list)
		dirs_to_delete = " ".join(dir_list)
		totals_to_delete = "".join(totoal_list)
		
		#self.progressbar1.pulse()		
#		if(files_to_delete != ""):
#			print "The files you selected are:"+ files_to_delete
#			print "start to delete files..."+files_to_delete
			
#			n=30
#			while(n<=100):
#				self.progressbar1.set_fraction(n/100)
#				n=n+1
#				time.sleep(0.1)	
	
		#if(widget.get_active()):
		#	self.progressbar1.pulse()
		#	time.sleep(1)	
#		if(os.system("./srm -v %s"%files_to_delete) == 0):
#				self.progressbar1.set_fraction(1.0)
#				time.sleep(2)
#				self.progressbar1.hide()
#				print "ok, progressbar1 was hide success."
		
				
		if(totals_to_delete != ""):
			print "The directories you selected are:" + totals_to_delete
#	
			if(os.system("./xad_file -s %s %s" %(standard, totals_to_delete)) == 0):
				print "All files and dirs you selected delete success"
			else:
				print "All files and dircetories delete failed"

					
###This function is designed to the normal delete ###
	def normal_delete(self, widget, data = None):
		print "Please select the file which you want to delete..."
		self.filechooserdialog1.show()
#		self.name = get_filename(self, widget, data = None)
#		print self.name


### This function is to erase the swap space in your system ####
	def swap_erase(self, widget, data = None):
		global standard
		global swap_device
		find_swap()
		print "swap_device"
		print "***Note:You must disable the swapspace before using this program!***"
		if(os.system("./xad_swap -s %s %s"%(standard, swap_device)) == 0):
			print "swap erase success"
		else:
			print "swap erase failed"

### This function is to erase the whole disk which you selected ####
	def disk_erase(self, widget, data = None):
		global disk_names
		print disk_names
		store = gtk.ListStore(gobject.TYPE_STRING)
		for disk_item  in disk_names:
 	       		store.append([disk_item])
	        self.combo1.set_model(store)
 		self.combo1.set_text_column(0)


### This function is to get the disk name when you selected from the comboboxtext1 ####		
	def get_disk(self, widget, data = None):
		selected_disk = self.combo1.get_active_text()
		self.disk_confirm_dialog.show()
		self.disk_confirm_dialog.format_secondary_text("The disk: %s will be removed" % selected_disk)
		return selected_disk

### This function is used to confirm to erase the selected disk space ###
	def confirm_erase_disk(self, widget, data = None):
		global standard
		if(self.get_disk(widget) == None):
			print "Please selcted the disk which you want to erase..."
		else:
			os.system("./xad_hd -s %s %s" %(standard, self.get_disk(widget)))
		self.disk_confirm_dialog.hide()
	
	def undo_erase_disk(self, widget, data = None):
		self.disk_confirm_dialog.hide()


### This function is to enable and initialize the combobox entry ###
	def disk_space_erase(self, widget, data = None):
		global disk_space_names
		print disk_space_names
		store = gtk.ListStore(gobject.TYPE_STRING)
		for disk_space_item in disk_space_names:
			store.append([disk_space_item])
		self.combo2.set_model(store)
		self.combo2.set_text_column(0)
		

### This function is to get the diskspace when you selected from the comboboxtext1 ####		
	def get_disk_space(self, widget, data = None):
		selected_disk_space = self.combo2.get_active_text()
		
		self.disk_space_confirm_dialog.show()
		self.disk_space_confirm_dialog.format_secondary_text("The disk space: %s will be removed."%selected_disk_space)
		return selected_disk_space	
### This function is used to confirm to erase the selected disk space ###
	def confirm_erase_disk_space(self, widget, data = None):
		global standard
		if(self.get_disk_space(widget) == None):
			print "Please selcted one disk space which you want to erase..."
		else:
			os.system("./xad_sector -s %s %s" %(standard, self.get_disk_space(widget)))
		self.disk_space_confirm_dialog.hide()

### This function is to undo the action and back to the combobox dialog wating for the user selcted a new disk space ###
	def undo_erase_disk_space(self, widget, data = None):
		self.disk_space_confirm_dialog.hide()
		

### This function is to enable select the standard###
	def enable_select_standard(self, widget, data = None):
 		global standard_names
		store = gtk.ListStore(gobject.TYPE_STRING)
		for standard in standard_names:
			store.append([standard])
		self.combo3.set_model(store)
		self.combo3.set_text_column(0)



### This function is to display the system infomation when the sys_info button clicked ###
	def sys_info(self, widget, data = None):
		generate_sysinfo_file()
		f = open("sys_info.txt", "r")
		sys_info = f.read()
		print sys_info
		f.close()
		self.sysinfo_dialog.format_secondary_text(sys_info)
		self.sysinfo_dialog.show()
		
### This function is to get the name of the standard which you selected in the combox and display the message box inform you to confirm the selection ###
	def get_standard_name(self, widget, data = None):
		selected_standard = self.combo3.get_active_text()
		self.standard_confirm_dialog.show()
		self.standard_confirm_dialog.format_secondary_text("The standard you selected is: %s" % selected_standard)
		return selected_standard
	
### This function used to confirm to get the correct standard name ###
	def confirm_selected_standard(self, widget, data = None):
		global standard;
		if(self.get_standard_name(widget) == None):			
			print "you have not selected the erase standard, default is \"FAST\""	
			standard = "FAST"
		else:
			standard = self.get_standard_name(widget)
		self.standard_confirm_dialog.hide()


### This function is to quit the system infomation dialog when user clicked the cancel button ###
	def sys_info_quit(self, widget, data = None):
		self.sysinfo_dialog.hide()

### This function is to enable and initialize the filechooserdialog2 ###
	def standard_erase(self, widget, data = None):
		print "Please select the disk space mount point which you want to erase with standard level."		
		self.filechooserdialog2.show()

	


### This function is to get the dir's names which you selected in filechooser dialog ###
	def get_file_system_dir(self, widget, data = None):
		self.name = self.filechooserdialog2.get_filename()
		filenames = self.filechooserdialog2.get_filenames()
		print filenames
	      	
		dir_list = []
		file_list = []
		for each_file_dir in filenames:
			if os.path.isdir(each_file_dir):
				dir_list.append(each_file_dir)
			else:
				file_list.append(each_file_dir)		
		
		files_to_delete = " ".join(file_list)
		dirs_to_delete = " ".join(dir_list)
		
		if(files_to_delete != ""):
			print "One or more files were be selected, Please check it out..."

		if(dirs_to_delete != ""):
			print "The directories you selected are:" + dirs_to_delete
			print "start to sfill directories:" + dirs_to_delete
			self.progressbar1.pulse()		
			self.progressbar1.set_text("erasing......")
			#new_thread = threading.Thread(target=sys_cmd,  args=("./xad_clear ", dirs_to_delete))
			#new_thread = threading.Thread(sys_cmd("./xad_clear ", dirs_to_delete))
			print "A new thread will be start."
			#new_thread.setDaemon(True)
			#new_thread.start()
			#print threading.enumerate()
			#new_thread.join()
			#if(os.system("./xad_clear  %s" %dirs_to_delete) == 0):
			#	print "The file systems you selected erase success with standard level"
			#else:
			#	print "file systems you specified fill failed"

###This function is to hide the filechooserdialog2 when you click on the "cancel" button ###
	def filechooserdialog2_quit(self, widget, data = None):
		self.filechooserdialog2.hide()
	

	def display_sys_info(self):
		os.system("lsb_release -a > system_distri.txt")
		f = open("system_distri.txt", 'r')
		sys_info = f.read()
		f.close()
		text_buffer = self.textview1.get_buffer()
		text_buffer.set_text(sys_info)
			
	def display_time(self):
		date = time.strftime("%Y-%m-%d", time.localtime())
		text_buffer = self.textview2.get_buffer()
		text_buffer.set_text(date)	
 	
	def display_bar(self, widget, data=None):			
		i=0.0;
		while(1):
			i=i+0.01
			print "i is:", i
			if(i>=1.0):
				i=0.0
			self.progressbar2.set_orientation(gtk.PROGRESS_RIGHT_TO_LEFT)
			self.progressbar2.set_text("test for progressbar")
			self.progressbar2.set_fraction(i)
			
			print "The fraction is set within the value:", self.progressbar2.get_fraction()
			
			time.sleep(0.5)
	def __init__(self):
		builder = gtk.Builder()
		builder.add_from_file("test.glade")
		self.window = builder.get_object("window1")
		self.filechooserdialog1 = builder.get_object("filechooserdialog1")
		self.filechooserdialog2 = builder.get_object("filechooserdialog2")
		self.sysinfo_dialog = builder.get_object("messagedialog1")
		self.disk_space_confirm_dialog = builder.get_object("messagedialog2")
		self.disk_confirm_dialog = builder.get_object("messagedialog3")
		self.standard_confirm_dialog = builder.get_object("messagedialog4")
		self.combo1 = builder.get_object("comboboxentry1")
		self.combo2 = builder.get_object("comboboxentry2")
		self.combo3 = builder.get_object("comboboxentry3")
		self.Calendar1 = builder.get_object("calendar1")
		self.progressbar1 = builder.get_object("progressbar1")
		self.progressbar2 = builder.get_object("progressbar2")
		self.textview1 = builder.get_object("textview1")
		self.textview2 = builder.get_object("textview2")
	#	self.progressbar1.hide()
	#	self.Calendar1.select_month(get_sys_time()[1] - 1, get_sys_time()[0])
	#	self.Calendar1.select_day(get_sys_time()[2])
		#self.progressbar1.show()
		#self.progressbar1.set_fraction(0.3)
		builder.connect_signals(self)
		

	
if __name__ == "__main__":
	editor = secure_delete()
	generate_sysinfo_file()
	find_disks_names()
	editor.display_sys_info()
	editor.display_time()
	editor.window.set_default_size(450, 350)
	editor.window.set_position(gtk.WIN_POS_CENTER)
	editor.window.set_title("Welcome to BizInfoshred!")
	editor.window.set_border_width(10)
	#editor.window.set_icon_from_file("/home/kevin/work/xad/Bizsmooth.png")
	editor.window.show()	
	#mythread = threading.Thread(target = sys_cmd, args = ("./xad_clear ", "/ext3"))
	#mythread.start()
	
	gtk.main()
