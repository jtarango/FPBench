# * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
# * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
# * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# *
# * @file Makefile
# * @author Skynet
# * @brief Floating Point Core Benchmark Generator.
# * @see https://fpbench.org/
# * @benchmark: N/A
# * @researcher: Joseph David Tarango
# * @website http://www.cs.ucr.edu/~jtarango
# * @affiliation: University of California, Riverside
# * @date: JULY 18, 2020
# *
# To generate the everything, type "make all".
# To run other commands type "make <command>". I.E. "make turnin".
# To remove the executable and any extraneous files type "make clean" at
# the command prompt.
###############################################################################
# General extension information.
###############################################################################
# Linux extension    |Windows Equivalent    |Short description
#  ------------------------------------------------------------
#  .so, .o            | .dll                 | Object that can be loaded at runtime (Similar to DLL)
#  .a                 | .lib                 | Static library
#  [none], .elf(rare),| .exe, .com(rare)     | Linux executables
#    .bin(rare)       |                      |
#  .sh                | .bat                 | Shell script
#  .exe               | .exe                 | Mono application, Wine application
#  .deb               | .msi                 | Installer package for Debian/Ubuntu releases
#                     |                      |   (Though .deb is much more powerful with
#                     |                      |   native support for dependencies and repos). Note that .deb is actually a .ar archive with a special control file, a special file order, and a different extension.
#  .rpm               | .msi                 | Installer package for RedHat/CentOS releases.
#  .tar.gz, .tar, .gz | .zip                 | Compressed files that can contain a program or any other data, like images, documents, etc
#  .ko                | .sys                 | Drivers and kernel modules are loaded into the Linux kernel and have more hardware access than other programs.
#  .sh, .php, .py, etc| .bat, .cmd, .vbs,    | Linux is capable of running any file that it has an interpreter for.
#                     |   .js                |   A line at the top of the file called the shebang specifies what interpreter to run the file with.
#                     |                      |   Windows only supports .bat and .cmd files as Batch files, and .vbs (vbscript) and .js (JScript, not to be confused with JavaScript) via the Windows Script Host (WSH).
###############################################################################
# Intel Compiler information setup.
# INTEL_LIB_PATH = /opt/intel/system_studio_2020/lib/intel64_lin/
# CPATH = ${ISS_ROOT}/compilers_and_libraries/linux/include
# CPATH_MORE = ${ISS_ROOT}/compilers_and_libraries/linux/daal/include
# DAALROOT = ${ISS_ROOT}/compilers_and_libraries/linux/daal
# TBBROOT = ${ISS_ROOT}/compilers_and_libraries/linux/tbb
# IE_COMPILER = ${ISS_ROOT}/compilers_and_libraries/linux/bin/intel64/icc
# Environment Setup
#  source /opt/intel/system_studio_2020/bin/compilervars.sh -arch intel64 -platform linux
# GUI Location
#  sh /opt/intel/system_studio_2020/iss_ide_eclipse-launcher.sh
# Example Compile
# gcc main.o calcProgram.o -L $(INTEL_LIB_PATH) -lirc -lsvml -limf -o calcProgram
###############################################################################
help:
	@echo "Usage: make all"
	@echo "[compiler=compiler_name]"
	@echo "compiler_name    - intel or gnu"

# Variable to hold Revision number
REVNUM=1

# Variable to hold last name
LASTNAME=Tarango

# Variable to hold first first name
FIRSTNAME=Joseph

TAR_NAME_PREFIX=benchmark

# Variable of Tarball generated
TARNAME=$(TAR_NAME_PREFIX)_$(REVNUM)_$(FIRSTNAME)_$(LASTNAME).tgz

#Variable of Post Script generated
PSPRINT=$(TAR_NAME_PREFIX)_$(REVNUM)_$(FIRSTNAME)_$(LASTNAME).ps
PDFPRINT=$(TAR_NAME_PREFIX)_$(REVNUM)_$(FIRSTNAME)_$(LASTNAME).pdf

# File hiearachy is $(TAR_NAME_PREFIX)_$(REVNUM)_$(PART)$(SUBPART)$(TESTBENCH).$(EXTENSION)
# $(REVNUM)= Revision number
# $(PART)= if a lab part exists in order from 1 to infinity
# $(SUBPART)= if other components were used to make this component that were 
#            not included then put them here in order from a to z. If "a" 
#            requires sub parts then put "a"$(subsubpart) I.E. "a1" now 
#            numbering. Continue letter/number trend to infinity.
# $(TESTBENCH)= if this is a test bench file then put "tb" to indicate it 
#              is test bench for this part
#$(EXTENSION)= is the character file extension

# Files to be included in the PS/PDF print
FILES=Makefile *.c
# Note do not add CSV, build, include/lib unless you want big tar packages!
# README *.h *.hpp *.cpp *.csv

# Variable of to use GCC, G++, ICC, ICPC compilers.
ifndef compiler
    compiler = gnu
    # intel
endif

# Existing compilers
GCCCOMPILER=gcc
GPPCOMPILER=g++
INTEL_C_COMPILER=icc
INTEL_CPP_COMPILER=icpc

# User choice of compiler
ifeq ($(compiler),intel)
    COMPILER=$(INTEL_CPP_COMPILER)
else
ifeq ($(compiler),gnu)
    COMPILER=$(GPPCOMPILER)
    COPTS += $(if $(filter ia32, $(_IA)), -m32, -m64)
else
    $(info Error in compiler selection.)
    $(warning Please select valid compiler.)
    COMPILER=$(GPPCOMPILER)
    COPTS += $(if $(filter ia32, $(_IA)), -m32, -m64)
    # $(error Halting make with compiler=$(compiler). You shall not pass!)
endif
endif

# GNU Options
# -g Produce debug information, necessary for debugging. 
# -Wall 	Show all reasonable warnings...
# -Werror Make all warnings into errors.
# -ansi This flag tells the compiler to enforce ANSI C standards 
# -pedantic More pedantic ansi, warnings for stuff you probably didn't mean. 
# -O2 Optimize even more. GCC performs nearly all supported optimizations that
#  do not involve a space-speed tradeoff. As compared to -O, this option 
#	increases both compilation time and the performance of the generated code.
# -O3 Optimize yet more. -O3 turns on all optimizations specified by -O2 
#  and also turns on the -finline-functions, -funswitch-loops, 
#  -fpredictive-commoning, -fgcse-after-reload, -ftree-vectorize and 
#  -fipa-cp-clone options.
#  -ffloat-store Do not store floating point variables in registers,
#    and inhibit other options that might change whether a floating point value
#    is taken from a register or memory.
#  -fexcess-precision=standard his option allows further control over excess
#    precision on machines where floating-point registers have more precision
#    than the IEEE float and double types and the processor does not support
#    operations rounding to those types.
# Intel Options
#  -fp-model precise=Intel compiler option for floats

#Variable of GCC Compiler flags
# Verbose mode for compiler debug.
# GCC_COMPILE_FLAGS= -g -Wall -Werror -ansi -O0 -fsplit-stack -m64 -static -ffloat-store -v
# GCC Coompile with no optimizations, debug, warnings, stack split feature, static compilation, and FP restrictions for reproducible results.
GCC_COMPILE_FLAGS= -g -Wall -Werror -ansi -O0 -fsplit-stack -m64 -static -ffloat-store
# Intel compiler with float precise mode and static compilation.
ICC_COMPILE_FLAGS= -g -std=c99 -fp-model=precise -static-intel

ifeq ($(compiler),intel)
    COMPILEFLAGS=$(ICC_COMPILE_FLAGS)
else
    # ifeq ($(compiler),gnu)
    COMPILEFLAGS=$(GCC_COMPILE_FLAGS)
    # endif
endif
###############################################################################
path := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
cwd  := $(shell pwd)
CURRENT_PATH := $(subst $(lastword $(notdir $(MAKEFILE_LIST))),,$(subst $(space),\$(space),$(shell realpath '$(strip $(MAKEFILE_LIST))')))

# Build Dir
BDIR=$(CURRENT_PATH)builds

# Lib Dir
LIBDIR=$(BDIR)/libs

# Objects folder
ODIR=$(BDIR)/obj

#Include Dir
IDIR=$(CURRENT_PATH)/include
ILDIR=$(IDIR)/lib

# Files such as *.h
_DEPS = $(error none yet.)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
# Compile order -IprogramA -IprogramA -IprogramA
INCLUDES = -I$(DEPS)
LIBINCLUDES = -L$(LIBDIR) -L $(IDIR)/lib
INC = $(INCLUDES) $(LIBINCLUDES)
###############################################################################
# Linking Libs
LIBS=-lm

# Compile with no main.
NO_MAIN=-c

# Variable of Program name
PROGRAM=test
###############################################################################
# Exe files
###############################################################################
MD := /usr/bin/mkdir
RM := /usr/bin/rm
A2PS := /usr/bin/a2ps
ENSCRIPT:= /usr/bin/enscript
TR := /usr/bin/tar
# Valgrind
FPBENCH_GRIND := /usr/bin/valgrind
# Ubuntu-Linux unlimited heap and stack
UNLIMITED_POWER := ulimit -s unlimited &&
###############################################################################
# Target
.DEFAULT_GOAL := all

# Do all, except run and debugging...
all: clean create_dirs print printpdf turnin compile_all
	$(info Making all. Fin.)
    $(info MAKEFILE_LIST: $(MAKEFILE_LIST))
    $(info          path: $(path))
    $(info           cwd: $(cwd))
    $(info  current path: $(CURRENT_PATH))
.PHONY: all

# Create directories
create_dirs:
	$(MD) -p $(BDIR)
	$(MD) -p $(ODIR)
	$(MD) -p $(LIBDIR)
	$(MD) -p $(IDIR)
	$(MD) -p $(ILDIR)
.PHONY: create_dirs

# Remove unnecessary files
clean:
	$(RM) -rf *~ $(BDIR) $(PSPRINT) $(PDFPRINT) $(TARNAME) *.pdf *.ps
.PHONY: clean

# Print files to a .ps document
print:
	$(A2PS) -M letter --line-numbers=1 --pro=color --highlight-level=light --pretty-print -o $(PSPRINT) $(FILES)
.PHONY: print

printpdf:
	$(ENSCRIPT) -2 --fancy-header --line-numbers=1 --truncate-lines \
	--word-wrap --style=emacs --tabsize=3 --landscape $(FILES) \
	-o - | ps2pdfwr - $(PDFPRINT)
.PHONY: printpdf

# Create a tar ball for project
turnin:
	$(TR) -czvf $(TARNAME) $(PSPRINT) $(FILES)
.PHONY: turnin

compile_all: compile_Default compile_F compile_D compile_LD compile_DYP compile_PT compile_UN
	$(info Compiling all.)
.PHONY: compile_all

# Dependencies
#$(ODIR)/%.o: %.c $(DEPS)
#	$(CC) $(NO_MAIN) -o $@ $< $(COMPILEFLAGS)

# Compile C code
#  Static lib with main API, Static lib with no main.
compile_Default:
	$(info Compile Default add for sending data to file. 2>&1 | tee log)
	$(COMPILER)            $(COMPILEFLAGS)                           -o $(BDIR)/template_default.a    template.c $(LIBS)
	$(COMPILER) $(NO_MAIN) $(COMPILEFLAGS) -DLIBRARY_MODE            -o $(LIBDIR)/template_default.so template.c $(LIBS)
.PHONY: compile_Default

compile_F:
	$(info Compile F.)
	$(COMPILER)            $(COMPILEFLAGS) -DCONFIG_F                 -o $(BDIR)/template_f.a          template.c $(LIBS)
	$(COMPILER) $(NO_MAIN) $(COMPILEFLAGS) -DCONFIG_F -DLIBRARY_MODE  -o $(LIBDIR)/template_f.so       template.c $(LIBS)

.PHONY: compile_F

compile_D:
	$(info Compile D.)
	$(COMPILER)            $(COMPILEFLAGS) -DCONFIG_D                 -o $(BDIR)/template_d.a          template.c $(LIBS)
	$(COMPILER) $(NO_MAIN) $(COMPILEFLAGS) -DCONFIG_D -DLIBRARY_MODE  -o $(LIBDIR)/template_d.so       template.c $(LIBS)
.PHONY: compile_D

compile_LD:
	$(info Compile LD.)
	$(COMPILER)            $(COMPILEFLAGS) -DCONFIG_LD                 -o $(BDIR)/template_ld.a        template.c $(LIBS)
	$(COMPILER) $(NO_MAIN) $(COMPILEFLAGS) -DCONFIG_LD -DLIBRARY_MODE  -o $(LIBDIR)/template_ld.so     template.c $(LIBS)
.PHONY: compile_LD

compile_DYP:
	$(info Compile DYP.)
	$(COMPILER)            $(COMPILEFLAGS) -DCONFIG_DYP                -o $(BDIR)/template_dyp.a       template.c $(LIBS)
	$(COMPILER) $(NO_MAIN) $(COMPILEFLAGS) -DCONFIG_DYP -DLIBRARY_MODE -o $(LIBDIR)/template_dyp.so    template.c $(LIBS)
.PHONY: compile_DYP

compile_PT:
	$(info Compile PT.)
	$(COMPILER)            $(COMPILEFLAGS) -DCONFIG_PT                 -o $(BDIR)/template_pt.a         template.c $(LIBS)
	$(COMPILER) $(NO_MAIN) $(COMPILEFLAGS) -DCONFIG_PT -DLIBRARY_MODE  -o $(LIBDIR)/template_pt.so      template.c $(LIBS)
.PHONY: compile_PT

compile_UN:
	$(info Compile UN.)
	$(COMPILER)            $(COMPILEFLAGS) -DCONFIG_UN                 -o $(BDIR)/template_un.a         template.c $(LIBS)
	$(COMPILER) $(NO_MAIN) $(COMPILEFLAGS) -DCONFIG_UN -DLIBRARY_MODE  -o $(LIBDIR)/template_un.so      template.c $(LIBS)
.PHONY: compile_UN

run: run_default run_f run_d run_ld run_dyp run_pt run_un
	$(info Running code.)
.PHONY: run

# For Ubuntu/Linux we need more stack and heap space so use ulimit -s unlimited
run_DEFAULT:
	$(UNLIMITED_POWER) .$(BDIR)/template_default.a
.PHONY: run_Default

run_F:
	$(UNLIMITED_POWER) .$(BDIR)/template_f.a
.PHONY: run_F

run_D:
	$(UNLIMITED_POWER) .$(BDIR)/template_d.a
.PHONY: run_D

run_LD:
	$(UNLIMITED_POWER) .$(BDIR)/template_ld.a
.PHONY: run_LD

run_DYP:
	$(UNLIMITED_POWER) .$(BDIR)/template_dyp.a
.PHONY: run_DYP

run_PT:
	$(UNLIMITED_POWER) .$(BDIR)/template_pt.a
.PHONY: run_PT

run_UN:
	$(UNLIMITED_POWER) .$(BDIR)/template_un.a
.PHONY: run_UN

# Valgrind and debug program if we encounter issues.
grinder:
	$(info Debugging program file is grinder_f.log)
	$(FPBENCH_GRIND) --undef-value-errors=no --main-stacksize=99999999 --max-stackframe=99999999 --valgrind-stacksize=10485760 --num-callers=500 --verbose --log-file=grinder_f.log .$(BDIR)/template_default.a
.PHONY: grinder

# Python Code helper for semi-automation. Skynet generator will be the future.
# pyCompileToC:
#    $(info python compileToC.py)
#    $(error Not Supported yet... You shall not pass!)
#.PHONY: pyCompileToC

# pyDataAnalysis:
#    $(info python compileToC.py)
#    $(error Not Supported yet... You shall not pass!)
# .PHONY: pyDataAnalysis