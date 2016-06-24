AM_CPPFLAGS = -I$(top_srcdir)/include

SUBDIRS = blood cpu debug dos fpu gui hardware libs ints misc shell platform 

bin_PROGRAMS = bloodbox

if HAVE_WINDRES
ico_stuff = winres.rc
endif

.rc.o:
	$(WINDRES) -o $@ $<
.asm.o:
	nasm -felf64 -o $@ $<

bloodbox_SOURCES = dosbox.cpp boxmem.asm $(ico_stuff)
bloodbox_LDADD = cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui.a \
               ints/libints.a misc/libmisc.a shell/libshell.a hardware/serialport/libserial.a libs/gui_tk/libgui_tk.a \
               blood/libblood.a

EXTRA_DIST = winres.rc dosbox.ico
