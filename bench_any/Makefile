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
# Intel Compiler information
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

# Variable to hold lastname
LASTNAME=Tarango

# Variable to hold first lastname
FIRSTNAME=Joseph

TAR_NAME_PREFIX=benchmark

# Variable of Tarball generated
TAR=$(TAR_NAME_PREFIX)_$(REVNUM)_$(FIRSTNAME)_$(LASTNAME).tgz

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

#Files to be included in the PS/PDF print
FILES=Makefile *.c
# README *.h *.hpp *.cpp

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
GCC_COMPILE_FLAGS= -g -Wall -Werror -ansi -O0 -fsplit-stack -m64 -static -ffloat-store -v
#GCC_COMPILE_FLAGS= -g -Wall -Werror -ansi -O0 -fsplit-stack -m64 -static -ffloat-store
ICC_COMPILE_FLAGS= -g -std=c99 -fp-model=precise -static-intel
ifeq ($(compiler),intel)
    COMPILEFLAGS=$(ICC_COMPILE_FLAGS)
else
    # ifeq ($(compiler),gnu)
    COMPILEFLAGS=$(GCC_COMPILE_FLAGS)
    # endif
endif

INCLUDES = -I/usr/local/include -Ia -Ib -Ic -I($(HOME)/include
LIBINCLUDES = -L/usr/local/lib -Lbuild/lib  -L $(HOME)/lib
INC = $(INCLUDES) $(LIBINCLUDES)

# Variable of Program name
PROGRAM=test

FPBENCH_GRIND = /usr/bin/valgrind

# Targets
# Compile all, specifically first clean, print,turnin, and compile in this order
# all: clean print turnin
.DEFAULT_GOAL := all

all: clean compile_Default compile_F compile_D compile_LD compile_DYP compile_PT compile_UN print printpdf turnin

grinder:
	$(info --leak-check=yes --tool=memcheck --track-origins=yes)
	$(FPBENCH_GRIND) --undef-value-errors=no --main-stacksize=99999999 --max-stackframe=99999999 --valgrind-stacksize=10485760 --num-callers=500 --verbose --log-file=grinder_f.log ./template_f.a

# Compile C code
compile_Default:
	$(info Compile Default 2>&1 | tee log)
	$(COMPILER)     $(COMPILEFLAGS) -o template_default.a  template.c -lm
	#$(COMPILER) -c $(COMPILEFLAGS) -o template_default.so template.c -lm

compile_F:
	$(info Compile F.)
	$(COMPILER)    $(COMPILEFLAGS) -DCONFIG_F -o template_f.a  template.c -lm
	$(COMPILER) -c $(COMPILEFLAGS) -DCONFIG_F -o template_f.so template.c -lm

compile_D:
	$(info Compile D.)
	$(COMPILER)    $(COMPILEFLAGS) -DCONFIG_D -o template_d.a  template.c -lm
	$(COMPILER) -c $(COMPILEFLAGS) -DCONFIG_D -o template_d.so template.c -lm

compile_LD:
	$(info Compile LD.)
	$(COMPILER)    $(COMPILEFLAGS) -DCONFIG_LD -o template_ld.a  template.c -lm
	$(COMPILER) -c $(COMPILEFLAGS) -DCONFIG_LD -o template_ld.so template.c -lm

compile_DYP:
	$(info Compile DYP.)
	$(COMPILER)    $(COMPILEFLAGS) -DCONFIG_DYP -o template_dyp.a  template.c -lm
	$(COMPILER) -c $(COMPILEFLAGS) -DCONFIG_DYP -o template_dyp.so template.c -lm

compile_PT:
	$(info Compile PT.)
	$(COMPILER)    $(COMPILEFLAGS) -DCONFIG_PT -o template_pt.a  template.c -lm
	$(COMPILER) -c $(COMPILEFLAGS) -DCONFIG_PT -o template_pt.so template.c -lm

compile_UN:
	$(info Compile UN.)
	$(COMPILER)    $(COMPILEFLAGS) -DCONFIG_UN -o template_un.a  template.c -lm
	$(COMPILER) -c $(COMPILEFLAGS) -DCONFIG_UN -o template_un.so template.c -lm

run: run_default run_f run_d run_ld run_dyp run_pt run_un
	$(info Run Libraries.)

run_default:
	ulimit -s unlimited && ./template_default.a

run_f:
	ulimit -s unlimited && ./template_f.a

run_d:
	ulimit -s unlimited && ./template_d.a

run_ld:
	ulimit -s unlimited && ./template_ld.a

run_dyp:
	ulimit -s unlimited && ./template_dyp.a

run_pt:
	ulimit -s unlimited && ./template_pt.a

run_un:
	ulimit -s unlimited && ./template_un.a

# Python Code
# pyRunner:
#    $(info python compileToC.py)
#    $(error Not Supported yet... You shall not pass!)

# Remove unnecessary files
clean:
	rm -rf *~ $(PSPRINT) $(PDFPRINT) $(TAR) *.o *.clib *.a *.so *.pdf *.ps

# Print files to a .ps document
print:
	a2ps -M letter --line-numbers=1 --pro=color --highlight-level=light --pretty-print -o $(PSPRINT) $(FILES)

printpdf:
	enscript -2 --fancy-header --line-numbers=1 --truncate-lines \
	--word-wrap --style=emacs --tabsize=3 --landscape $(FILES) \
	-o - | ps2pdfwr - $(PDFPRINT)

# Create a tar ball for project turn in
turnin:
	tar -czvf $(TAR) $(PSPRINT) $(FILES)

.PHONY: all clean compile_Default compile_F compile_D compile_LD compile_DYP compile_PT compile_UN print printpdf turnin run grinder
