#! /usr/bin/python3

import sys 
import os
import re 
import zipfile
import os
import io
import importlib 
import unittest 

def zipwalk(vpath, zfilename):
    """Zip file tree generator.

    Thanks: http://code.activestate.com/recipes/425840-zip-walker-zip-file-tree-generator/
    For each file entry in a zip archive, this yields
    a two tuple of the zip information and the data
    of the file as a StringIO object.

    zipinfo, filedata

    zipinfo is an instance of zipfile.ZipInfo class
    which gives information of the file contained
    in the zip archive. filedata is a StringIO instance
    representing the actual file data.

    If the file again a zip file, the generator extracts
    the contents of the zip file and walks them.

    Inspired by os.walk .
    """

    if vpath is None : 
        vpath = []

    tempdir=os.environ.get('TEMP',os.environ.get('TMP',os.environ.get('TMPDIR','/tmp')))
    
    try:
        z=zipfile.ZipFile(zfilename,'r')
        for info in z.infolist():
            fname = info.filename
            data = z.read(fname)
            extn = (os.path.splitext(fname)[1]).lower()

            if extn=='.zip':
                checkz=False
                
                tmpfpath = os.path.join(tempdir,os.path.basename(fname))
                try:
                    open(tmpfpath,'w+b').write(data)
                except (IOError, OSError) as e:
                    print (e)

                if zipfile.is_zipfile(tmpfpath):
                    checkz=True

                if checkz:
                    try:
                        for x in zipwalk(vpath + [fname], tmpfpath):
                            yield x
                    except Exception as e:
                        raise
                    
                try:
                    os.remove(tmpfpath)
                except:
                    pass
            else:
                yield (info, data, vpath + [fname])
    except RuntimeError as e:
        print ('Runtime Error')
    except zipfile.error as e:
        raise
                        
if __name__=="__main__":
    import sys

    installdir = '/home/maximus/Arduino/ArduinoGrader'
    os.environ['PYTHONPATH'] = installdir

    if len(sys.argv) != 2 : 
        print ("usage: argv[0] <project> <zipfile>")
        exit(-1)

    test = None
    try :
        test = importlib.import_module("tests." + sys.argv[1])
    
    except Exception as e :
        print ("Failed to import test.\n", e)
        exit (2)
    

    #tempdir = tempfile.TemporaryDirectory()
    tempdir = './canvas-temp'
    rdir = '/home/maximus/Downloads/submissions (3).zip'

    suite = unittest.TestSuite()
    loader = unittest.TestLoader()
    
    for i, d, v in zipwalk(None, rdir):
        base = v[0]
        parts = base.split('_')
        user = parts.pop(0)
        late = False
        if parts[0] == 'late' :
            late = True
            parts.pop(0)
        
        while re.match('\d+', parts[0]) is not None :
            parts.pop(0)

        if len(v) > 1 : 
            zippath = i.filename.split('/')
            filename = zippath[-1]
            if filename == "" :
                continue;
            
        else:
            filename = '_'.join(parts)

        # Fix filename bugaboos 
        filename = filename.replace('.ino.ino', '.ino')
        
        # Fix numering hassle. 
        while re.search('-\d+\.\w+$', filename) is not None : 
            filename = re.sub('-\d+\.', '.', filename)

        if not os.path.isdir(os.path.join(tempdir, user)) :
            os.makedirs(os.path.join(tempdir, user))

        f = open (os.path.join(tempdir, user, filename), 'wb') 
        f.write(d)
        f.close()

        for pattern in test.files :
            m = re.search(pattern[0], ArduinoBuilder.get_sketch())
            if m is not None :
                for tc in pattern[1:] :
                    suite.addTests(loader.loadTestsFromTestCase(tc))

    unittest.TextTestRunner(verbosity=2).run(suite)
