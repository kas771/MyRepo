import sys
from ROOT import gSystem
gSystem.Load("libMyRepo_MyProject")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing MyProject..."

sys.exit(0)

