import sys
from ROOT import gSystem
gSystem.Load("libMyRepo_MichelElectron")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing MichelElectron..."

sys.exit(0)

