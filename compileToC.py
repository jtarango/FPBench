#!/usr/bin/python
# -*- coding: utf-8 -*-
# *****************************************************************************/
# * Authors: Joseph Tarango
# * Developed on Python 3.7
# *****************************************************************************/
from __future__ import absolute_import, division, print_function, \
    unicode_literals  # , nested_scopes, generators, generator_stop, with_statement, annotations
import os, subprocess, pprint, datetime, timer, traceback

def findAll(fileType='.fpcore', directoryTreeRootNode=None, debug=False):
    """
    Find all files of a type given a directory.
    Args:
        fileType: file extension to look for.
        directoryTreeRootNode: filesystem main root node
        debug: debug mode for adding functionality.

    Returns:

    """
    if directoryTreeRootNode is None or os.path.exists(directoryTreeRootNode):
        directoryTreeRootNode = (os.path.dirname(__file__) or '.')

    if debug is True:
        print('Directory Root Node: {0}'.format(directoryTreeRootNode))

    # Walk Entire local tree and save contents
    directoryTree = []
    fileTypeTree = []
    for root, dirs, files in os.walk(directoryTreeRootNode):
        try:
            for dirSelect in dirs:
                dirLocation = os.path.abspath(os.path.join(root, dirSelect))
                directoryTree.append(dirLocation)
                if debug is True:
                    print('Directory Node: {0}'.format(dirLocation))
            for file in files:
                if file.endswith(fileType):
                    fileLocation = os.path.abspath(os.path.join(root, file))
                    fileTypeTree.append(fileLocation)
                    if debug is True:
                        print('{0} File Node: {1}'.format(fileType, fileLocation))
                else:
                    otherFileLocation = os.path.abspath(os.path.join(root, file))
                    if debug is True:
                        print('Other {0} File Node: {1}'.format(file.endswith(fileType), otherFileLocation))
        except BaseException as ErrorContext:
            print('Exception {0}'.format(ErrorContext))
            pass
    return pyFileTree, directoryTree


def autogenAll(racketExe='racket', operationMode='export.rkt', baseDir='benchmarks', sourceType='.fpcore', destinationType='.c', outFolder='compiledFiles', debug=False):
    """ Method to generate language files from fpcores.
        Example Usage - racket export.rkt benchmarks/rump.fpcore rump.c
    Args:
        racketExe: racket key program work in path
        operationMode: racket mode of operation in this case export to c.
        baseDir: base directory containing fpcores.
        sourceType: source type .fpcore
        destinationType: destination type .c
        outFolder: output folder to save files.
        debug: debug flag for new development

    Returns: Good and Bad list for completion of subprocess

    """
    List_Pass = []
    List_Fail = []
    runCommandFPB = None
    runCommandFPBCout = None
    runCommandFPBErr = None
    timer = None

    baseDirPath = os.path.abspath(os.path.join(os.getcwd(), baseDir))
    if os.path.exists(baseDirPath):
        files, directoryTree = findAll(fileType=sourceType, directoryTreeRootNode=baseDirPath, debug=debug)
    else:
        files, directoryTree = findAll(fileType=sourceType, directoryTreeRootNode=None, debug=debug)

    if debug is True:
        pprint.print("files {0}\n directories {1}".format(files, directoryTree))

    # Spawn a subprocess to generate the Python C-Type parser library and wait for it to complete
    cStart = datetime.now()
    outFolderPath = os.path.abspath(os.path.join(os.getcwd(), outFolder))

    for fileSelect in files:
        cStartElement = datetime.now()
        head, tail = os.path.split(fileSelect)
        selectedfileName = os.path.splitext(tail)[0]
        destFileName = (selectedfileName + destinationType)
        destFileName = os.path.abspath(os.path.join(outFolderPath, destFileName))

        cmdInputs = 'export.rkt {0} {1}'.format(str(fileSelect), str(destFileName))
        print(racketExe + cmdInputs)

        try:
            if debug is True:  # Output all to command line!
                runCommandFPB = subprocess.Popen([sys.executable, racketExe,  operationMode , str(fileSelect), str(destFileName)], shell=ENABLE_SHELL, env=myenv)
            else:  # Output nada to command line!
                runCommandFPB = subprocess.Popen([sys.executable, racketExe,  operationMode , str(fileSelect), str(destFileName)],
                                                 stdin=subprocess.PIPE, stdout=output if output else subprocess.PIPE, shell=ENABLE_SHELL, env=myenv )
            timer = Timer(timeout, runCommandFPB.kill)
            timer.start()
            runCommandFPBCout, runCommandFPBErr = runCommandFPB.communicate()
            runCommandFPB.poll()
        except SystemExit as errorContext:
            print("Terminated Process ID " + str(selectedfileName))
            pprint.pprint("{0}\n{1}\n{2}".format(errorContext, runCommandFPBCout, runCommandFPBErr))
            procKiller(runCommandFPB, True)
        finally:
            print("Normal termination Process ID " + str(selectedfileName))
            procKiller(runCommandFPB, False)
            timer.cancel()

        if runCommandFPB.returncode != 0:
            List_Fail.append(str(fileSelect))
            print("%s: auto generation parser fail time %s seconds" % (str(fileSelect), str(datetime.now() - cStartElement)))
            print("Please execute command: ")
            print("python " + ctypePyFile + ' ' + ctypeAutoGenInputs)
        else:
            List_Pass.append(str(fileSelect))
            print("%s: auto generation parser pass time %s seconds" % (str(fileSelect), str(datetime.now() - cStartElement)))
    print(str(projectname) + ": auto generation parser total elapsed " + str(datetime.now() - cStart) + " seconds")

    # Pass list output construction
    if List_Pass is not None:
        ObjectListPassStr = ("[")
        isFirst = True
        for item in List_Pass:
            if isFirst:
                isFirst = False
                ObjectListPassStr = (ObjectListPassStr + item)
            else:
                ObjectListPassStr = (ObjectListPassStr + "," + item)
        ObjectListPassStr = (ObjectListPassStr + "]")
        print("PassList: " + ObjectListPassStr)
    else:
        print("PassList: Nil")

    # Fail list output construction
    if List_Fail is not None:
        ObjectListFailStr = ("[")
        isFirst = True
        for item in List_Fail:
            if isFirst:
                isFirst = False
                ObjectListFailStr = (ObjectListFailStr + item)
            else:
                ObjectListFailStr = (ObjectListFailStr + "," + item)
        ObjectListFailStr = (ObjectListFailStr + "]")
        print("FailList: " + ObjectListFailStr)
    else:
        print("FailList: Nil")
    return (List_Pass, List_Fail)

def main():
    ##############################################
    # Main function, Options
    ##############################################
    from optparse import OptionParser

    parser = OptionParser()
    parser.add_option("--debug", action='store_true', dest='debug', default=False, help='Debug mode.')
    parser.add_option("--verbose", action='store_true', dest='verbose', default=False, help='Verbose printing for debug use.')
    parser.add_option("--baseDir", action='store_true', dest='baseDir', default=None, help='Base directory that contains fpcore files.')
    parser.add_option("--sourceTypes", action='store_true', dest='sourceTypes', default=None, help='Debug mode.')
    parser.add_option("--destinationType", action='store_true', dest='destinationType', default=None, help='Debug mode.')
    parser.add_option("--outFolder", action='store_true', dest='outFolder', default=None, help='Debug mode.')
    (options, args) = parser.parse_args()

    ##############################################
    # Main
    ##############################################
    pprint.pprint("Options from User\n {0}".format(options))
    if (options.baseDir is None):
        options.baseDir = 'benchmarks'
    if (options.sourceType is None):
        options.sourceType = '.fpcore'
    if (options.destinationType is None):
        options.destinationType = '.c'
    if (options.outFolder is None):
        options.outFolder = 'compiledFiles'
    pprint.pprint("Options selected\n {0}".format(options))
    autogenAll(racketExe='racket', operationMode='export.rkt',
               baseDir=options.baseDir, sourceType=options.sourceType,
               destinationType=options.destinationType, outFolder=options.outFolder,
               debug=options.debug)
    return 0

if __name__ == '__main__':
    """Performs execution delta of the process."""
    p = datetime.datetime.now()
    try:
        main()
    except Exception as e:
        print("Fail End Process: ", e)
        traceback.print_exc()
    q = datetime.datetime.now()
    print("Execution time: " + str(q-p))
