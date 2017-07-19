#! /usr/bin/python3

import sys 
import os
import re 
import zipfile
import os
import io
import importlib 
import unittest 
import tempfile 
import logging
from tests.builder import ArduinoBuilder
from tests.comments import Comments
from tests.runner import runner, histogram
from tests.base import GraderSuite

def unzip_walk(zipname, vpath) :
    if vpath is None :
        vpath = [] 

    z = zipfile.ZipFile(zipname, 'r')
    for info in z.infolist() :
        nextpath = vpath + [info.filename]
        data = z.read(info.filename)
        if (os.path.splitext(info.filename)[1]).lower() == '.zip' :
            # This is a zip... recurse. 
            with tempfile.NamedTemporaryFile() as tempzip : 
                tempzip.write(data)
                tempzip.flush()
                for x in unzip_walk(tempzip.name, nextpath) :
                    yield x 
        else:
            yield (nextpath, data)

    z.close()

def submissions(zipfile) :
    subs = {}
    for name, data in unzip_walk(zipfile, None) :
        parts = name[0].split('_')
        user = parts.pop(0)

        if user not in subs :
            subs[user] = {}

        if parts[0] == 'late' :
            subs[user]['late'] = True
            parts.pop(0)
        else:
            subs[user]['late'] = False

        while re.match('\d+', parts[0]) is not None :
            parts.pop(0)
            
        # Find the real filename
        if len(name) > 1 : 
            filename = name[-1]
        else:
            filename = '_'.join(parts)

        if filename[-1] == '/' : 
            continue 

        dirname = os.path.dirname(filename)
        filename = os.path.basename(filename)

        origname = filename;

        # Fix dirname bugaboos 
        dirname = dirname.replace('.ino/', '/')
        
        # Fix filename bugaboos 
        filename = filename.replace('.ino.ino', '.ino')

        # Fix numering hassle.         
        while re.search('-(\d+)\.\w+$', filename) is not None : 
            filename = re.sub('-\d+\.', '.', filename)

        if 'files' not in subs[user] : 
            subs[user]['files'] = {}

        subs[user]['files'][filename] = {}            
        subs[user]['files'][filename]['origname'] = origname
        subs[user]['files'][filename]['dirname'] = dirname
        subs[user]['files'][filename]['data'] = data         

    return subs

def main() :
    installdir = '/home/maximus/Arduino/ArduinoGrader'
    os.environ['PYTHONPATH'] = installdir

    if len(sys.argv) < 3 : 
        print ("usage:", sys.argv[0], "<project> <zipfile> [username]")
        exit(-1)

    userfilter = None
    if len(sys.argv) == 4 :
        userfilter = sys.argv[3]
        print ("Regrading user:", userfilter)

    test = None
    try :
        test = importlib.import_module("tests." + sys.argv[1])    
    except Exception as e :
        print ("Failed to import test.\n", e)
        exit (2)

    subs = submissions(sys.argv[2])
    gradedir = sys.argv[1] + '-grades'
    if not os.path.isdir(gradedir) :
        os.makedirs(gradedir)

    with tempfile.TemporaryDirectory(prefix='ArduinoGrader') as workdir :
        for user in sorted(subs) :
            if userfilter is not None and user != userfilter :
                continue
            graders = []
            print ("\n\n ****** Running test for user", user, " ******\n\n")
            if not os.path.isdir(os.path.join(workdir, user)) :
                os.makedirs(os.path.join(workdir, user))
            for filename in subs[user]['files'].keys() :
                dirname = subs[user]['files'][filename]['dirname']
                origname = subs[user]['files'][filename]['origname']
                if origname != filename :
                    print ("Renamed file:", os.path.join(dirname, origname), "->", filename)
                else:
                    print ("Found file:", os.path.join(dirname, filename))
                    
                if not os.path.isdir(os.path.join(workdir, user, dirname)) :
                    os.makedirs(os.path.join(workdir, user, dirname))

                extract = os.path.join(workdir, user, dirname, filename)
                with open (extract, 'wb') as f :
                    f.write(subs[user]['files'][filename]['data'])

                for pattern in test.files :
                    m = re.search(pattern[0], filename)
                    if m is not None :
                        context = {}
                        context['tempdir'] = os.path.join(workdir, user, 'temp')
                        context['program'] = extract
                        context['user'] = user
                        if len(pattern) > 2 :
                            context['extras'] = pattern[2]
                        else:
                            context['extras'] = []

                        suite = GraderSuite(context)
                        graders.append(suite)

                        if 'suite' not in subs[user] :
                            subs[user]['suite'] = []

                        subs[user]['suite'].append(suite)
                        for tc in ['.*\.ino', Comments, ArduinoBuilder] + pattern[1:] :
                            for name in unittest.defaultTestLoader.getTestCaseNames(tc) :
                                suite.addTest(tc(name, context))

            runner(graders, os.path.join(gradedir, user + '.zip'))

    print ("\n\n===== done =====\n\n")
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.DEBUG)
    fh = logging.FileHandler(os.path.join(gradedir, 'histogram.txt'))
    fh.setLevel(logging.DEBUG)
    ch = logging.StreamHandler(sys.stdout)
    ch.setLevel(logging.DEBUG)
    logger.addHandler(fh)
    logger.addHandler(ch)
    histogram(logger)

if __name__=="__main__":
    main()

