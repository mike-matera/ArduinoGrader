import re 

__histogram = {}

def runner(suite) :
    for test in suite : 
        testid = test.id()
        if testid not in __histogram : 
            __histogram[testid] = [0,0]
        desc = test.shortDescription()
        if desc is not None:
            print (desc, ' ... ', end='', flush=True )
        result = test.run()
        if result.wasSuccessful() : 
            __histogram[testid][0] += 1
            print ("PASS")
        else:
            if len(result.failures) == 1 :
                __histogram[testid][1] += 1
                print ("FAIL")
                traceback = result.failures[0][1]
                m = re.search("AssertionError:\s*(.*)", traceback)
                if m is None :
                    print ("There seemes to be an undefined error:")
                    print (traceback)
                else:
                    parts = m.group(1).split(':', maxsplit=1)
                    if len(parts) == 2 :
                        print ("  Oops:", parts[0].strip())
                        print ("    ", parts[1].strip())
                    else:
                        print ("  Oops:")
                        print ("    ", parts[0].strip())

            else:
                print ("ERROR")
                print (result.errors[0][1])
                __histogram[testid][1] += 1
                
def histogram() :
    print ("FAIL\tPASS\tTest")
    for test, pf in sorted(__histogram.items(), key=lambda k: k[1]) :
        print (pf[1], pf[0], test, sep="\t")
