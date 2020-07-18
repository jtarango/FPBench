#!/usr/bin/python
# -*- coding: utf-8 -*-
# *****************************************************************************/
# * Authors: Joseph Tarango
# * Developed on Python 3.7
# * Application to compile .fpcores to .c programs and other supported types.
# *****************************************************************************/
from __future__ import absolute_import, division, print_function, \
    unicode_literals  # , nested_scopes, generators, generator_stop, with_statement, annotations
import os, subprocess, pprint, datetime, threading, traceback, psutil

def findAll(fileType='.fpcore', directoryTreeRootNode=None, debug=False, verbose=False):
    """
    Find all files of a type given a directory.
    Args:
        fileType: file extension to look for.
        directoryTreeRootNode: filesystem main root node
        debug: debug mode for adding functionality.
        verbose: Add more information to debug.

    Returns:

    """
    if directoryTreeRootNode is None or os.path.exists(directoryTreeRootNode) is False:
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
                if debug is True and verbose is True:
                    print('Directory Node: {0}'.format(dirLocation))
            for file in files:
                if file.endswith(fileType):
                    fileLocation = os.path.abspath(os.path.join(root, file))
                    fileTypeTree.append(fileLocation)
                    if debug is True:
                        print('{0} File Node: {1}'.format(fileType, fileLocation))
                else:
                    otherFileLocation = os.path.abspath(os.path.join(root, file))
                    if verbose is True and debug is True:
                        print('Other {0} File Node: {1}'.format(file.endswith(fileType), otherFileLocation))
        except BaseException as ErrorContext:
            print('Exception {0}'.format(ErrorContext))
            pass
    return fileTypeTree, directoryTree

def procKiller(autoGenBuild=None, killAllProcess=False):
    if autoGenBuild is None:
        me = psutil.Process(os.getpid())
    else:
        me = autoGenBuild
    if killAllProcess is True:
        for child in me.get_children():
            child.kill()

def autoGenAll(racketExe='/usr/bin/racket', operationMode='export.rkt ', baseDir='benchmarks', sourceType='.fpcore',
               destinationType='.c', outFolder='compiledFiles', debug=False, verbose=False, shell=False, myenv=None,
               timeout=120):
    """ Method to generate language files from fpcores.
        Example Usage - racket export.rkt benchmarks/rump.fpcore rump.c
    Args:
        racketExe: racket key program work in path.
        operationMode: racket mode of operation in this case export to c.
        baseDir: base directory containing fpcores.
        sourceType: source type .fpcore
        destinationType: destination type .c
        outFolder: output folder to save files.
        debug: debug flag for new development
        verbose: Add more debug information.
        shell: Operate in shell mode for subprocess.
        myenv: Pass user environment to subprocess.
        timeout: Timeout for each execution.

    Returns: Good and Bad list for completion of subprocess

    """
    if myenv is None:
        myenv = os.environ.copy()
    List_Pass = []
    List_Fail = []
    runCommandFPB = None
    runCommandFPBCout = None
    runCommandFPBErr = None
    timer = None
    output = None

    baseDirPath = os.path.abspath(os.path.join(os.getcwd(), baseDir))
    basePathExists = os.path.exists(baseDirPath)
    if verbose or debug:
        print("Base path Exists {}".format(basePathExists))
    if basePathExists is True:
        files, directoryTree = findAll(fileType=sourceType, directoryTreeRootNode=baseDirPath, debug=debug, verbose=verbose)
    else:
        files, directoryTree = findAll(fileType=sourceType, directoryTreeRootNode=None, debug=debug, verbose=verbose)

    # Check Destination folder
    outFolder = os.path.abspath(os.path.join(os.getcwd(), outFolder))
    if os.path.exists(outFolder) is False:
        os.makedirs(outFolder)

    operationMode = os.path.abspath(operationMode)
    if os.path.exists(operationMode) is False:
        return

    if debug is True:
        pprint.pprint("files {0}\n directories {1}".format(files, directoryTree))

    # Spawn a subprocess to generate the Python C-Type parser library and wait for it to complete
    cStart = datetime.datetime.now()
    outFolderPath = os.path.abspath(os.path.join(os.getcwd(), outFolder))

    for fileSelect in files:
        cStartElement = datetime.datetime.now()
        head, tail = os.path.split(fileSelect)
        selectedfileName = os.path.splitext(tail)[0]
        destFileName = (selectedfileName + destinationType)
        destFileName = os.path.abspath(os.path.join(outFolderPath, destFileName))

        cmdInputs = ('{0} {1} {2}'.format(str(operationMode), str(fileSelect), str(destFileName)))

        firstCommand = ('{0} {1}'.format(racketExe, cmdInputs))
        secondCommand = 'exit'
        if debug is True and verbose is True:
            print('Command {0}'.format(racketExe))
            print('Command Inputs {}'.format(cmdInputs))
            print('Command Exit {0}'.format(secondCommand ))
            print('Full Commands {0}; {1}'.format(firstCommand, secondCommand))

        try:
            if debug is True:  # Output all to command line!
                runCommandFPB = subprocess.Popen(["{}; {}".format(firstCommand, secondCommand)], shell=shell, env=myenv, close_fds=True)
            else:
                runCommandFPB = subprocess.Popen(["{}; {}".format(firstCommand, secondCommand)], shell=shell, env=myenv, stdin=subprocess.PIPE,
                                                 stdout=output if output else subprocess.PIPE, close_fds=True)

            timer = threading.Timer(timeout, runCommandFPB.kill)
            timer.start()
            runCommandFPBCout, runCommandFPBErr = runCommandFPB.communicate()
            runCommandFPB.poll()
        except SystemExit as errorContext:
            print("Terminated Process ID " + str(selectedfileName))
            pprint.pprint("{0}\n{1}\n{2}".format(errorContext, runCommandFPBCout, runCommandFPBErr))
            procKiller(runCommandFPB, True)
        finally:
            if debug is True and verbose is True:
                print("Normal termination Process ID " + str(selectedfileName))
            procKiller(runCommandFPB, False)
            timer.cancel()

        if runCommandFPB.returncode != 0:
            List_Fail.append(str(fileSelect))
            print("%s:generation parser fail time %s seconds" % (str(fileSelect), str(datetime.datetime.now() - cStartElement)))
            print("Please execute command: ")
            print("python {0} {1} {2}".format(str(racketExe),  str(operationMode), str(cmdInputs)))
        else:
            List_Pass.append(str(fileSelect))
            print("%s: generation parser pass time %s seconds" % (str(fileSelect), str(datetime.datetime.now() - cStartElement)))
    print(str(racketExe) + ":generation parser total elapsed " + str(datetime.datetime.now() - cStart) + " seconds")

    # Pass list output construction
    if List_Pass is not None and List_Fail is not []:
        ObjectListPassStr = ("[")
        isFirst = True
        for item in List_Pass:
            if isFirst:
                isFirst = False
                ObjectListPassStr = (ObjectListPassStr + item)
            else:
                ObjectListPassStr = (ObjectListPassStr + "," + item)
        ObjectListPassStr = (ObjectListPassStr + "]")
        pprint.pprint("PassList: {}".format(ObjectListPassStr))
    else:
        print("PassList: Nil")

    # Fail list output construction
    if List_Fail is not None and List_Fail is not []:
        ObjectListFailStr = ("[")
        isFirst = True
        for item in List_Fail:
            if isFirst:
                isFirst = False
                ObjectListFailStr = (ObjectListFailStr + item)
            else:
                ObjectListFailStr = (ObjectListFailStr + "," + item)
        ObjectListFailStr = (ObjectListFailStr + "]")
        pprint.pprint("FailList: " + ObjectListFailStr)
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
    parser.add_option("--shell", action='store_true', dest='shell', default=True, help='Enter shell mode for independent execution.')
    parser.add_option("--baseDir", action='store_true', dest='baseDir', default=None, help='Base directory that contains fpcore files.')
    parser.add_option("--sourceTypes", action='store_true', dest='sourceTypes', default=None, help='Debug mode.')
    parser.add_option("--destinationType", action='store_true', dest='destinationType', default=None, help='Debug mode.')
    parser.add_option("--outFolder", action='store_true', dest='outFolder', default=None, help='Debug mode.')
    (options, args) = parser.parse_args()

    ##############################################
    # Main
    ##############################################
    pprint.pprint("Options from User {0}".format(options))
    if (options.baseDir is None):
        options.baseDir = 'benchmarks'
    if (options.sourceTypes is None):
        options.sourceTypes = '.fpcore'
    if (options.destinationType is None):
        options.destinationType = '.c'
    if (options.outFolder is None):
        options.outFolder = 'compiledFiles'
    pprint.pprint("Options selected {0}".format(options))
    myenv = os.environ.copy()
    autoGenAll(racketExe='/usr/bin/racket', operationMode='export.rkt',
               destinationType=options.destinationType, outFolder=options.outFolder,
               debug=options.debug, verbose=options.verbose, shell=options.shell, myenv=myenv)
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
