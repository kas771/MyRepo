import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from larlite import larlite as fmwk
gSystem.Load('libMyRepo_MyProject.so')

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Set input root file(s) from the arguments to this python script in the command line
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("chi2michel_output_anafile.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
myfilter = fmwk.michelfilter()
mymodule = fmwk.chi2michel()
# This is a public function I implemented in LeslieSampleAna.h
my_proc.add_process(myfilter)
my_proc.add_process(mymodule)

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
# If you want to run over the first 5 events, you can do ".run(1,5)" for example
#my_proc.run(1,5)
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
