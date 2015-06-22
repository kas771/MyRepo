import sys
import ROOT
from ROOT import *
from ROOT import larlite as fmwk
import signal
gSystem.Load('libMyRepo_MyProject.so')

if len(sys.argv) == 1:
    print "\n-------You forgot to include a source file!-------\n"
    print "Usage: python hitviewr.py /input/file/with/gaushits.root"
    quit()

my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)
my_proc.set_io_mode(fmwk.storage_manager.kREAD)
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

myfilter = fmwk.michelfilter()
my_proc.add_process(myfilter)

#Get access to the larlite data manager
mgr = fmwk.storage_manager()
mgr.set_io_mode(mgr.kREAD)

#Add input files
for x in xrange(len(sys.argv)-1):
    mgr.add_in_filename(sys.argv[x+1])
mgr.open()

#Make a canvas to draw the hits
canvas=TCanvas("HitViewer","HitViewer",600,500)
gStyle.SetOptStat(0)
canvas.SetGridx(1)
canvas.SetGridy(1)
#canvas.SetLogz()

#Make an instance of the HitViewer
algo = fmwk.HitViewer()


#Catch ctrl+C to close things
signal.signal(signal.SIGINT, signal.SIG_DFL)

processed_events=0
#Loop over events
while mgr.next_event():
    #Process this event, and if it returns "False"
    #that means it didn't pass the filter, so we'll skip the event
    my_proc.process_event()
    if not my_proc.get_ana_status(myfilter):
        continue
    
    #Get the hits from the data file
    ev_hit = mgr.get_data(fmwk.data.kHit,'gaushit')

    if not ev_hit:
        print "WTF your input file doesn't have hits in it."
        quit()

    plane_todraw = int(2)

    print "Event ID: %d, Plane: %d, Number of events drawn: %d" % (ev_hit.event_id(),plane_todraw, processed_events)

    algo.GenerateHisto(ev_hit,int(plane_todraw))

    canvas.cd()
    myHisto = algo.GetHitHisto()
    if not myHisto:
        print "Uh oh something went wrong... HitViewer returned a non-existant hit histogram. Quitting!"
        quit()

    myHisto.Draw("COLZ")
    canvas.Update()
    
    processed_events += 1

    print "Hit enter to go to the next event!\n"
    sys.stdin.readline()
        

        


