#!/bin/python3
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
# * @file generator.py
# * @author Skynet
# * @brief Floating Point Core Benchmark Generator.
# * @see https://fpbench.org/
# * @benchmark: N/A
# * @researcher: Joseph David Tarango
# * @website http://www.cs.ucr.edu/~jtarango 
# * @affiliation: University of California, Riverside
# * @date: JULY 18, 2020
# *
# *****************************************************************************/
# * Authors: Dominic Doty, Joseph Tarango
# * Developed on Python 3.7
# * Takes all the CSV files and does analysis and produces graphs
# * Analysis results are printed to a folder "analysis"
# * All charts and numerical results will be in there.
# *****************************************************************************/
from __future__ import absolute_import, division, print_function, \
    unicode_literals  # , nested_scopes, generators, generator_stop, with_statement, annotations
import csv, os, re, datetime, pprint
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Create a dict of lists where each element is a list of csv's
# that make up one benchmark function
# {ex0:[ex0-0, ex0-1, ex0-2], ex1:[ex1-1, ex1-2, ex1-3]}
# Argument is path of folder to explore
def get_csvs_list(folder):
    directory = os.fsencode(folder)

    # Where the results are stored
    result = dict()

    # Put each filename into a list at the key corresponding to the function
    for file in os.listdir(directory):
        filename = os.fsdecode(file)
        
        # Check for good csv's
        if filename.endswith(".csv"):
            # Strip out the name of the function
            bench_func = re.sub('-\d+\.csv$', '', filename)

            # If it's already in the set, append, otherwise add new key
            if bench_func in result:
                result[bench_func].append(filename)
            else:
                result[bench_func] = [filename]
    
    # Sort the values into a nice order
    for key in result:
        result[key] = sorted(result[key])

    return result


# Get a list of folders to work on, and grab the csv's inside them
def get_folders_list(top_folder):
    # Where the results are stored
    result = dict()

    # Go through the folders in the folder (one for each bench set expected)
    for folder in os.listdir(top_folder):
        
        # Create the full folder path
        folder_path = top_folder + "/" + folder

        # Confirm it is a folder
        if os.path.isdir(folder_path):
            # Get the dict of csv's from inside it
            # Add to the result dict
            result[folder] = get_csvs_list(folder_path)

    return result

# Open csv's compute the difference between types
# Takes a benchmark name
# Types are column headers in the csv and golden type is
# the type which others are compared against
def csv_analyze(benchmark, func_dict, types, golden_type):
    # Data pandas dataframe
    data = pd.DataFrame()
    
    # Numpy read csv in as pandas dataframe
    for func in func_dict:
        for file in func_dict[func]:
            file_path = "comb_data/" + benchmark + "/" + file
            newdata = pd.read_csv(filepath_or_buffer=file_path, skipinitialspace=True, float_precision='round_trip')
            newdata = newdata.apply(pd.to_numeric, errors='coerce')
            data = pd.concat([data, newdata], ignore_index=True, sort=False)

    # Calculate the deltas from golden
    deltas = pd.DataFrame()
    types = [type for type in types if type != golden_type]
    for type in types:
        deltas[type] = data[type] - data[golden_type]
        deltas[type].dropna()

    # Analyze deltas
    analysis = dict()
    for type in types:
        analysis[type] = []
        # analysis[type].append(deltas[type].min())
        # analysis[type].append(deltas[type].max())
        analysis[type].append(deltas[type].mean())
        analysis[type].append(deltas[type].std())

    # Box plot the deltas
    fig, ax = plt.subplots()
    ax.set_title(benchmark)
    delta_series = [value for key, value in deltas.items()]
    ax.boxplot(delta_series, labels=['float', 'double', 'posit8', 'posit16', 'posit32'], autorange=True)
    fig.savefig("plots/" + benchmark + ".png", bbox_inches='tight')
    plt.close(fig)

    # Plot the deltas vs result size
    # fig, ax = plt.subplots()
    # ax.set_title(benchmark)
    
    # data_series = [np.absolute(value) for key,value in data.items() if key in types]

    # ax.plot(data_series,delta_series, linestyle="None", markersize=2)

    # ax.legend(('float', 'double', 'posit8', 'posit16', 'posit32'), loc="upper right")

    # fig.savefig("plots/line_" + benchmark + ".png", bbox_inches='tight')
    # plt.close(fig)

    return analysis
        
def main():
    ##############################################
    # Main function, Options
    ##############################################
    from optparse import OptionParser

    parser = OptionParser()
    parser.add_option("--debug", action='store_true', dest='debug', default=False, help='Debug mode.')
    parser.add_option("--verbose", action='store_true', dest='verbose', default=False, help='Verbose printing for debug use.')
    (options, args) = parser.parse_args()

    ##############################################
    # Main
    ##############################################
    pprint.pprint("Options from User {0}".format(options))
               
    types = ["float", "double", "longdouble", "posit8", "posit16", "posit32", "dynamicpt"]
    golden = "longdouble"

    file_structure = get_folders_list("comb_data")

    # Loop through all the results and analyze them
    for bench in file_structure:
        analysis = csv_analyze(bench, file_structure[bench], types, golden)

        for type in analysis:
            print(bench, type, analysis[type])
               
    return 0


if (__name__ == '__main__'):
    """Performs execution delta of the process."""
    p = datetime.datetime.now()
    try:
        main()
    except Exception as e:
        print("Fail End Process: ", e)
        traceback.print_exc()
    q = datetime.datetime.now()
    print("Execution time: " + str(q-p))        
