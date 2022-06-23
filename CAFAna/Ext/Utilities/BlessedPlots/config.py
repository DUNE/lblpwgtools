import os         #/// Interact with OS
import errno

#///////////////////////////////////////////////////////////////////////////////
# User Configuration
WEB_PATH      = '/nusoft/app/web/htdoc/nova/blessedplots/'
JSON_FILENAME = 'BlessedPlotsMeta.json'
PLOT_SUBDIR   = 'plots/'
BLESSED_PLOTS = './BlessedPlots.json'
DOCDB_URL     = 'http://nova-docdb.fnal.gov:8080/cgi-bin/'
REGENERATE    = False
PWD           = open(os.environ['NOVADOCPWDFILE'], 'r').readlines()[0].strip()

# Is this the proposed-only page or the official-only page?
PROPOSED      = False

EXTS          = ['.txt', '.png', '.jpg', '.jpeg', '.gif', '.pdf', '.eps', '.ps', '.C', '.tar.gz', '.zip']

# mkdir -p WEB_PATH/PLOT_SUBDIR
# Easier with python >= 3.2, but I don't have that
try:
  os.makedirs(WEB_PATH + PLOT_SUBDIR)
except OSError, exc:
  if exc.errno == errno.EEXIST and os.path.isdir(WEB_PATH + PLOT_SUBDIR):
    pass
  else:
    raise
