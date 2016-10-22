import os 
import re 
import tempfile 
import zipfile 

__histogram = {}

def runner(suite, logger, **kwargs) :

    for test in suite : 
        testid = test.id()
        if testid not in __histogram : 
            __histogram[testid] = [0,0]
        desc = test.shortDescription()
        if desc is None:
            desc = testid
        result = test.run()
        if result.wasSuccessful() : 
            __histogram[testid][0] += 1
            logger.info("[pass] %s", desc)
        else:
            if len(result.failures) == 1 :
                __histogram[testid][1] += 1
                traceback = result.failures[0][1]
                m = re.search("AssertionError:\s*(.*)", traceback)
                if m is None :
                    logger.error('[fail] %s\nThere seemes to be an undefined error:\n%s', desc, traceback)
                else:
                    parts = m.group(1).split(':', maxsplit=1)
                    if len(parts) == 2 :
                        logger.error('[fail] %s\n  Oops: %s\n    %s', desc, parts[0].strip(), parts[1].strip())
                    else:
                        logger.error('[fail] %s\n  Oops: %s', desc, parts[0].strip())

            else:
                print ("[error] %s", result.errors[0][1])
                __histogram[testid][1] += 1

def save_logs(outfile, *files) :
    zipf = zipfile.ZipFile(outfile, 'w', zipfile.ZIP_DEFLATED)
    for fpath in files : 
        for root, dirs, files in os.walk(fpath):
            for f in files:
                zipf.write(os.path.join(root, f), arcname=f)
    zipf.close()
    
                
def histogram() :
    print ("FAIL\tPASS\tTest")
    for test, pf in sorted(__histogram.items(), key=lambda k: k[1]) :
        print (pf[1], pf[0], test, sep="\t")
