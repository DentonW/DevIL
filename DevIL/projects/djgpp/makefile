###################################
#    Djgpp Makefile for DevIL     #
#    Last Modified: 07/13/2002    #
###################################

# Uncomment any of the following lines to define them

# Halts on warnings and errors
#WERROR=1
# Turns on debugging options
#DEBUG=1


.PHONY = baddjgpp badpath ilbad
#.PRECIOUS = $(OBJ)/%.o


# check that the DJGPP environment variable is set
ifndef DJDIR
baddjgpp:
	@echo Your DJGPP environment variable is not set correctly! It should
	@echo point to the djgpp.env file: see the djgpp readme.1st for details.
endif

# check that djgpp/bin is pathed
ifeq ($(wildcard $(addsuffix /djasm.exe,$(subst ;, ,$(PATH)))),)
badpath:
	@echo Your PATH is not set correctly! This must include the
	@echo djgpp bin directory: see the djgpp readme.1st for details.
endif

ifeq ($(wildcard ../../src-IL/src/il_devil.c),)
ilbad:
	@echo Bad DevIL installation! You did not preserve the directory structure
	@echo while unzipping it: did you remember to use the -d flag with pkunzip?
endif


#ifneq ($(wildcard $(DJDIR)/bin/rm.exe),)
RM = $(DJDIR)/bin/rm.exe
#else
RM = del
#endif


#Enables debugging features
ifdef DEBUG
DFLAGS = -DDEBUG
else
DFLAGS =
endif


ILDIR=	./../../src-IL/src
ILODIR=	.\..\..\src-IL\src\obj
#ILLIB=	./../../lib/djgpp/libil.a
ILLIB=	libil.a
ILDEST=	$(DJDIR)/lib/libil.a
ILINC=	$(DJDIR)/include/il/il.h
ILOBJ=	$(ILODIR)/il_bits.o $(ILODIR)/il_bmp.o $(ILODIR)/il_convert.o $(ILODIR)/il_cut.o \
		$(ILODIR)/il_doom.o $(ILODIR)/il_endian.o $(ILODIR)/il_error.o \
		$(ILODIR)/il_header.o $(ILODIR)/il_stack.o $(ILODIR)/il_internal.o \
		$(ILODIR)/il_manip.o $(ILODIR)/il_main.o $(ILODIR)/il_devil.o \
		$(ILODIR)/il_pal.o $(ILODIR)/il_pcd.o $(ILODIR)/il_pcx.o $(ILODIR)/il_pic.o \
		$(ILODIR)/il_pnm.o $(ILODIR)/il_raw.o $(ILODIR)/il_register.o $(ILODIR)/il_rawdata.o \
		$(ILODIR)/il_rle.o $(ILODIR)/il_sgi.o $(ILODIR)/il_states.o \
		$(ILODIR)/il_targa.o $(ILODIR)/il_utility.o $(ILODIR)/il_io.o \
		$(ILODIR)/il_files.o $(ILODIR)/il_icon.o $(ILODIR)/il_wal.o \
		$(ILODIR)/il_mdl.o $(ILODIR)/il_quantizer.o $(ILODIR)/il_convbuff.o \
		$(ILODIR)/il_lif.o $(ILODIR)/il_alloc.o $(ILODIR)/il_dds.o $(ILODIR)/il_profiles.o \
		$(ILODIR)/il_dcx.o $(ILODIR)/il_psd.o $(ILODIR)/il_fastconv.o \
		$(ILODIR)/il_psp.o $(ILODIR)/il_dds-save.o $(ILODIR)/il_neuquant.o \
		$(ILODIR)/il_pix.o $(ILODIR)/il_pxr.o $(ILODIR)/il_xpm.o


ILUDIR=  ./../../src-ILU/src
ILUODIR= .\..\..\src-ILU\src\obj
#ILULIB=	./../../lib/djgpp/libilu.a
ILULIB=	libilu.a
ILUDEST=	$(DJDIR)/lib/libilu.a
ILUINC=	$(DJDIR)/include/il/ilu.h
ILUOBJ=	$(ILUODIR)/ilu_error.o $(ILUODIR)/ilu_filter.o $(ILUODIR)/ilu_internal.o \
		$(ILUODIR)/ilu_manip.o $(ILUODIR)/ilu_mipmap.o $(ILUODIR)/ilu_filter_rcg.o \
		$(ILUODIR)/ilu_noise.o $(ILUODIR)/ilu_rotate.o $(ILUODIR)/ilu_scale.o \
		$(ILUODIR)/ilu_states.o $(ILUODIR)/ilu_utilities.o $(ILUODIR)/ilu_main.o \
		$(ILUODIR)/ilu_alloc.o $(ILUODIR)/ilu_scale2d.o $(ILUODIR)/ilu_scale3d.o \
		$(ILUODIR)/ilu_region.o


ILUTDIR=	./../../src-ILUT/src
ILUTODIR=	.\..\..\src-ILUT\src\obj
#ILUTLIB=	./../../lib/djgpp/libilut.a
ILUTLIB=	libilut.a
ILUTDEST=	$(DJDIR)/lib/libilut.a
ILUTINC=	$(DJDIR)/include/il/ilut.h
ILUTOBJ=	$(ILUTODIR)/ilut_allegro.o $(ILUTODIR)/ilut_internal.o $(ILUTODIR)/ilut_main.o \
			$(ILUTODIR)/ilut_opengl.o $(ILUTODIR)/ilut_states.o


WFLAGS=	-Wall -W -Wno-unused
ILFLAGS=	-I./../../src-IL/include
ILUFLAGS=	-I./../../src-ILU/include
ILUTFLAGS=	-I./../../src-ILUT/include


all : compile_msg $(ILODIR) $(ILLIB) $(ILDEST) $(ILUODIR) $(ILULIB) $(ILUDEST) $(ILUTODIR) $(ILUTLIB) $(ILUTDEST)
	@echo 
	@echo Done. 

compile_msg:
	@echo DevIL is compiling...may take awhile.

$(ILODIR)/%.o : $(ILDIR)/%.c
	gcc $(ILFLAGS) $(WFLAGS) -c $< -o $@ -I..\..\include

$(ILLIB): $(ILOBJ)
	ar rs $(ILLIB) $(ILOBJ)

# Need to find out how to make /include/il
$(ILDEST): $(ILLIB)
	copy libil.a $(subst /,\,$(ILDEST))
	copy ..\..\include\il\il.h $(subst /,\,$(ILINC))

$(ILUODIR)/%.o : $(ILUDIR)/%.c
	gcc $(ILUFLAGS) $(WFLAGS) -c $< -o $@ -I..\..\include

$(ILULIB): $(ILUOBJ)
	ar rs $(ILULIB) $(ILUOBJ)

$(ILUDEST): $(ILULIB)
	copy libilu.a $(subst /,\,$(ILUDEST))
	copy ..\..\include\il\ilu.h $(subst /,\,$(ILUINC))

$(ILUTODIR)/%.o : $(ILUTDIR)/%.c
	gcc $(ILUTFLAGS) $(WFLAGS) -c $< -o $@ -I..\..\include

$(ILUTLIB): $(ILUTOBJ)
	ar rs $(ILUTLIB) $(ILUTOBJ)

$(ILUTDEST): $(ILUTLIB)
	copy libilut.a $(subst /,\,$(ILUTDEST))
	copy ..\..\include\il\ilut.h $(subst /,\,$(ILUTINC))

clean:
#	RM -v $(ILODIR)/*.o
#	RM -v $(ILUODIR)/*.o
#	RM -v $(ILUTODIR)/*.o
	del $(ILODIR)\*.o
	del $(ILUODIR)\*.o
	del $(ILUTODIR)\*.o
