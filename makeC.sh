#!/bin/bash
# -*- coding: utf-8 -*-
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
# * @file
# * @author Skynet
# * @brief Floating Point Core Benchmark Code Generator.
# * @see https://fpbench.org/
# * @benchmark: N/A
# * @researcher: Joseph David Tarango
# * @website http://www.cs.ucr.edu/~jtarango 
# * @affiliation: University of California, Riverside
# * @date: JULY 18, 2020
# *
# Example of how to get and make
# git clone https://github.com/FPBench/FPBench.git
# cd FPBench
# git checkout aa7bacb06277f5e8e20ba33d36bd233a7244fb15
racket export.rkt benchmarks/fptaylor-extra.fpcore FPCBenchMarks/fptaylor-extra.c
racket export.rkt benchmarks/fptaylor-real2float.fpcore FPCBenchMarks/fptaylor-real2float.c
racket export.rkt benchmarks/fptaylor-tests.fpcore FPCBenchMarks/fptaylor-tests.c
racket export.rkt benchmarks/hamming-ch3.fpcore FPCBenchMarks/hamming-ch3.c
racket export.rkt benchmarks/herbie.fpcore FPCBenchMarks/herbie.c
racket export.rkt benchmarks/precimonious.fpcore FPCBenchMarks/precimonious.c
racket export.rkt benchmarks/rosa.fpcore FPCBenchMarks/rosa.c
racket export.rkt benchmarks/rump.fpcore FPCBenchMarks/rump.c
racket export.rkt benchmarks/salsa.fpcore FPCBenchMarks/salsa.c

