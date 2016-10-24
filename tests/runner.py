import os 
import re 
import tempfile 
import zipfile 
import logging
import sys 

__histogram = {}

def runner(suites, logfile) :

    zipf = zipfile.ZipFile(logfile, 'w', zipfile.ZIP_DEFLATED)

    for suite in suites : 
        context = suite.get_context()
        logger = logging.getLogger(context['user'] + context['builder'].get_sketch())
        logger.setLevel(logging.DEBUG)
        fh = logging.FileHandler(os.path.join(context['logdir'], 'grader.log'))
        fh.setLevel(logging.DEBUG)
        ch = logging.StreamHandler(sys.stdout)
        ch.setLevel(logging.DEBUG)
        logger.addHandler(fh)
        logger.addHandler(ch)
    
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
                logger.info("[pass] %s: %s", context['builder'].get_sketch(), desc)
            else:
                if len(result.failures) == 1 :
                    __histogram[testid][1] += 1
                    traceback = result.failures[0][1]
                    m = re.search("AssertionError:\s*(.*)", traceback)
                    if m is None :
                        logger.error('[fail] %s: %s\nThere seemes to be an undefined error:\n%s', context['builder'].get_sketch(), desc, traceback)
                    else:
                        parts = m.group(1).split(':', maxsplit=1)
                        if len(parts) == 2 :
                            logger.error('[fail] %s: %s\n  Oops: %s\n    %s', context['builder'].get_sketch(), desc, parts[0].strip(), parts[1].strip())
                        else:
                            logger.error('[fail] %s: %s\n  Oops: %s', context['builder'].get_sketch(), desc, parts[0].strip())

                else:
                    print ("[error] %s: %s", context['builder'].get_sketch(), result.errors[0][1])
                    __histogram[testid][1] += 1

        logbase = context['builder'].get_sketch()
        logbase = re.sub('\.ino', '', logbase)
        for root, dirs, files in os.walk(context['logdir']):
            for f in files:
                zipf.write(os.path.join(root, f), arcname=logbase + '/' + f)
    zipf.close()


def histogram(logger=None) :
    message = "FAIL\tPASS\tTest\n"
    for test, pf in sorted(__histogram.items(), key=lambda k: k[1]) :
        message += str(pf[1]) + "\t" + str(pf[0]) + "\t" + str(test) + "\n"
    if logger is None :
        print (message)
    else :
        logger.info(message)
