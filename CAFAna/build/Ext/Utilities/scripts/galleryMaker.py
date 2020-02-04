#! /usr/bin/env python

import urllib2
import urllib
import HTMLParser
import os, sys
import xml.parsers.expat as expat
from xml.dom import minidom
import datetime
import cgi
import optparse
import codecs
import tarfile
from collections import OrderedDict
# Set the umask so that all files are user and group writable
os.umask(002)

############# FUNCTIONS  #############
#   The first part of this file will define functions and create objects that
# will aid in web interface.
xmlOut = "&outformat=xml"  #string we add to URLs to get XML output
endl = "\n"  # newline character for less (or quicker) typing
hendl = "<br>"

#password manager object
pwMan = urllib2.HTTPBasicAuthHandler()


# Function to handle authentication, we need to add a password for every
# webpage that we're going to access.
def authenticate(urlStr):
  pwMan.add_password( realm   ="PasswordRequired",
                      uri     = urlStr,
                      user    = 'nova',
                      passwd  = pw)
  opener = urllib2.build_opener(pwMan)
  urllib2.install_opener(opener)

#   Pages on DocDB will be behind a password.  getPage() will add the password to the manager
# and return the webpage as a (web) file object.
def getPage(url):
  authenticate(url)
  req = urllib2.Request(url)
  page = urllib2.urlopen(url)
  return page

# FileNBE objects hold the full name, base and extension
class FileNBE():
  def __init__(self, name, base, ext, url, tarPath=None):
    self.name = name.decode("utf-8")
    self.base = base.decode("utf-8")
    self.ext = ext.decode("utf-8")
    self.url = url.decode("utf-8")
    self.tarPath = tarPath

# Returns a filename given a DOM for a docdb file xml object
def getFileNBE(fileDom):
  # Find out if there is a "description" (for renamed files)
  descriptions = fileDom.getElementsByTagName("description")
  if len(descriptions) > 0:
    fileName = descriptions[0].lastChild.nodeValue
  #Otherwise use the filename
  else:
    fileName = fileDom.getElementsByTagName("name")[0].lastChild.nodeValue

  url = fileDom.getAttribute("href")
  baseExt = os.path.splitext(fileName)
  return FileNBE(fileName, baseExt[0], baseExt[1], url)

def loadStatusCache(filePath):
  cacheFile = open(filePath, 'r')
  cache = OrderedDict()
  for line in cacheFile:
    splitLine = line.strip().split("@")
    cache[splitLine[0]] = splitLine[1]
  return cache

class SavedFigure:
  def __init__(self, topic, thumbWebPath, caption, formats, docURL, docTitle, wrapperWebPath, wrapperPath, fileBaseName):
    self.topic          = topic           # Name of docdb topic
    self.thumbWebPath   = thumbWebPath    # Location of thumbnail relative to web dir
    self.caption        = caption         # Caption from gallery
    self.formats        = formats         # list of FileNBEs
    self.docURL         = docURL          # URL of docdb entry
    self.docTitle       = docTitle        # Title of docdb entry
    self.wrapperWebPath = wrapperWebPath  # Location of wrapper relative to web dir
    self.wrapperPath    = wrapperPath  # Location of wrapper relative to root of filesystem
    self.fileBaseName   = fileBaseName  # Location of wrapper relative to root of filesystem

  def makeWrapper(self):
    wrapper = codecs.open(self.wrapperPath, 'w', 'utf-8')
    wrapper.write(heading1(self.docTitle))
    wrapper.write(heading2(self.fileBaseName))
    wrapper.write(self.formatsBar())
    wrapper.write(hrule())
    wrapper.write(hendl)
    wrapper.write(paragraph(self.caption, fullWidth))
    wrapper.write('<img src=../"' + self.thumbWebPath +  '" style="border-style: none" Title="'+fig.caption+'" />')
    wrapper.write(hendl)
    wrapper.write(self.formatsBar())
    wrapper.write(paragraph("Disclaimer: this .png version of the image is for browsing purposes only.  Please find the official versions of this plot through the links above.", fullWidth))


    wrapper.close()


  def formatsBar(self):
    bar = ""
    bar += ("[" + link("DocDB" , self.docURL) + "]  " )

    for format in self.formats:
        bar += ("[" + link(format.ext , format.url) + "]  " )
    return bar

def panelRowOpen():
  return """
      <div class="row">
      <div class="col-sm-4">
        """
def panelRowClose():
  return """
  </div><!-- /.col-sm-4 -->
  </div>

  """

# HTML Helper functions
def title(str):
  return "<title>" + str + "</title>" + endl
def heading1(str):
  return "<h1>" + str + "</h1>" + endl
def heading2(str):
  return "<h2>" + str + "</h2>" + endl
def heading3(str):
  return "<h3>" + str + "</h3>"+ endl
def heading4(str):
  return "<h4>" + str + "</h4>" + endl
def link(text, url):
  return '<a href="'+ url + '">' + text + "</a>" + endl
def paragraph(str, width=None):
  p = ""
  if width: p += '<p style="width:'+width+'px;">'
  else: p += '<p>'

  p+= str + "</p>" + endl
  return p
def entry(str):
  return "<li>" + str + "</li>" + endl
def hrule():
  return '<hr width="100%" align="left" noshade color="#000000">' + endl

def hruleWide():
  return '<hr  align="left" noshade color="#000000">' + endl

def gridEntry(fig, thumbWidth):
  #creates table entry with image and text
  entry =  '<td valign="bottom" style="border:1px solid black" >'
  entry += link('<img src="' + fig.thumbWebPath +  '" width="'+ thumbWidth +'" style="border-style: none" Title="'+fig.caption+'" />', fig.wrapperWebPath)
  entry += '<center>'
  entry += '<p>'
  entry += fig.formatsBar()
  entry += '</p>'
  entry += '</center>'
  entry += '</td>'

  entry += endl
  return entry

def bootStrap():
  return """
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../../favicon.ico">



    <!-- Bootstrap core CSS -->
    <link href="static/bootstrap/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="static/bootstrap/css/theme.css" rel="stylesheet">



  """

def bodyOpen():
  return '<body role="document">'

def bodyClose():
  return   """
		    <!-- Bootstrap core JavaScript
    		================================================== -->
    		<!-- Placed at the end of the document so the pages load faster -->
    		<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
    		<script src="static/bootstrap/js/bootstrap.min.js"></script>

  			</body>'
			"""

def jumbo(content):
  t = ' <div class="jumbotron">' + endl
  t+= "    " +  content + endl
  t+= '</div>' + endl
  return t


def navBar(categories=None):
  t = """
    <!-- Fixed navbar -->
    <div class="navbar navbar-default navbar-fixed-top" role="navigation">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">Blessed Plots</a>
        </div>
        <div class="navbar-collapse collapse">
          <ul class="nav navbar-nav">
      """
  t+= '          <li>' + link("Grid View", "sortgrid.html") + '</li>' + endl
  t+= '          <li>' + link("Unsorted GridView", "grid.html") + '</li>' + endl
  t+= '          <li>' + link("Captioned View", "gallery.html") + '</li>' + endl
  if categories:
    t+= """
            <li class="dropdown">
              <a href="#" class="dropdown-toggle" data-toggle="dropdown">Categories <span class="caret"></span></a>
              <ul class="dropdown-menu" role="menu">

        """
    for cat in categories:
      t +='              <li><a href="#' + cat.strip().replace(" ", "").lower() + '">' + cat + '</a></li>' + endl
    t+="""
                  </ul>
            </li>
        """
  t+= """
          </ul>
        </div><!--/.nav-collapse -->
      </div>
    </div>
      """
  return t


"""
    <!-- Fixed navbar -->
    <div class="navbar navbar-default navbar-fixed-top" role="navigation">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">Project name</a>
        </div>
        <div class="navbar-collapse collapse">

          </ul>
        </div><!--/.nav-collapse -->
      </div>
    </div>
"""


############# CONFIGURABLE PARAMETERS #############
webPath = "/nusoft/app/web/htdoc/nova/blessedplots/"
officialURL = "http://nova-docdb.fnal.gov:8080/cgi-bin/Search?topics=370" + xmlOut
thumbsDir =  "thumbs/"  #name of directory for thumbnails
wrappersDir =  "wrappers/"  #name of directory for thumbnails
thumbsPath = webPath + thumbsDir
wrappersPath = webPath + wrappersDir
tempDir = "temp/"  #name of directory for thumbnails
tempPath   =  webPath + tempDir
thumbSize = str(225)
pageTitle = "NOvA Blessed Plots and Figures Gallery"
gridTitle = "NOvA Blessed Plots and Figures Grid"
fullWidth = str(1000)
#Get NOvA DocDB password from the hidden file, you can do it some other way
pw = open(os.environ['NOVADOCPWDFILE'], 'r').readlines()[0].strip()

############# MAIN EXECUTIING BODY #############

optParser = optparse.OptionParser()
optParser.add_option("--nothumbs", action="store_true",
                    help="Do not remake the thumbnails, runs more quickly.  This is pretty dangerous, could lead to borked HTML pages.  Only meant to debug HTML writing.")
optParser.add_option("--nocaps", action="store_true",
                    help="Do not download the captions, runs more quickly.  Only meant to debug HTML writing.")
optParser.add_option("--nocache", action="store_true",
                    help="Ignore the fact that the cached version is current, run anyway")


(options, args) = optParser.parse_args()
#Open the Topics page for "Official" blessed plots
# This is how we access the plots we want, they must be in the "Blessed Plots:Official" topic
officialPage = getPage(officialURL)

# Parse the xml output and find the document tags
officialDom = minidom.parse(officialPage)
documents = officialDom.getElementsByTagName("document")
# The DOM object will return all of the XML objects with a "document" tag.
# We can then loop over the documents.

# We will first have a loop to compare the state of the page to a chached version.
# This will allow us to decide whether or not we actually need to run the page again
statusPath = webPath + "status.txt"
cacheStatus = loadStatusCache(statusPath)
status = OrderedDict()
for doc in documents:
  id = doc.getAttribute("id")
  rev = doc.getElementsByTagName("docrevision")[0]
  mod = rev.getAttribute("modified")
  status[id] = mod

changed = False  # Assume page has not changed until we prove it has
for id in status.keys():
  if id in cacheStatus.keys():
    if not cacheStatus[id] == status[id]:
      changed = True # because the mod time has changed
  else:
    changed = True # because there is a new id in the status
# Last thing to check is whether or not we removed any documents
for id in cacheStatus.keys():
  if not id in status.keys():
    changed = True # because

if not changed and not options.nocache:
  print "The page has not changed, exiting"
  os._exit(0)
# write the status to a file
statusFile = open(statusPath, 'w')
for key in status.keys():
  statusFile.write(key + "@" + status[key] + endl)
statusFile.close()
# Our first task will be to generate a dictionary to map topics to documents
# The dictionary will map topic name strings to lists DOM(xml parsed) objects
topicMap = dict()
for doc in documents:
  # Get the link for the document
  docURL = doc.getAttribute("href") + xmlOut
  print "DOC ID : ", doc.getAttribute("id")
  # Load the xml page for that document and parse it to extract topics.
  docPage = getPage(docURL)
  docDom = minidom.parse(docPage)
  topics = docDom.getElementsByTagName("topic")

  topicNames = []
  #Get all the topic names for this document
  for topic in topics:
    topicNames.append(topic.getElementsByTagName("name")[0].lastChild.nodeValue)

  #skip this doc if it's not official, this is just a double check for sanity
  if not "Official" in topicNames:
    continue

  #Loop over topics to add urls to topicMap
  for topic in topics:
    # Access the name tag for each topic
    nameDom = topic.getElementsByTagName("name")
    # Read out the name
    name = nameDom[0].lastChild.nodeValue
    print name
    # Don't make entries in the map for "Blessed Plots" or "Official" topics.
    # Hopefully these are the only ones worth special casing
    if name == "Blessed Plots" or name == "Official":
      continue

    #Create an entry in the topic map if that topic isn't there yet
    if not name in topicMap.keys():
      topicMap[name] = []
    # append the document URL to the list for that topic
    topicMap[name].append(docDom.getElementsByTagName("document")[0])

print "###########################################################"

topics = topicMap.keys()
topics.sort()


# Now we will begin to write the HTML.  As we do it, we will convert files to thumbnails.

gallery = codecs.open(webPath + "gallery_new.html", 'w', "utf-8")


if not options.nothumbs:
  # clear out the temp and tumbnail directory, but only if we plan on making them again
  os.system("rm -f " + tempPath + "*")
  os.system("rm -f " + thumbsPath + "*")

# First we write the page heading
gallery.write(bootStrap())
gallery.write(navBar(topics))
gallery.write(bodyOpen())
gallery.write("<title>" + pageTitle +  "</title>")
gallery.write(jumbo(heading1(pageTitle) + heading2("Captioned View" +endl)) )
gallery.write(paragraph("To find out how to add a plot to this page, read this " + link("guide", "howto.html") + "."))


gallery.write('<a name="toc"> </a>')
gallery.write(heading2("Categories"))
gallery.write("<ul>")
#Loop over topics  to make Table of Contents
for topic in topics:
  gallery.write(entry(link(topic + hendl, "gallery.html#" + topic.strip().replace(" ", "").lower())))
gallery.write("</ul>")

gallery.write(hrule())
savedFigs = [] # flat list of saved figures, used for grid view below
savedFigsByTopic = OrderedDict()
#Loop over topics  to make the actual body
for topic in topics:
  print topic
  savedFigsByTopic[topic] = []
  #Make an anchor and a header
  gallery.write('<a name="' + topic.strip().replace(" ", "").lower() + '"> </a>')
  gallery.write(heading2(hendl))
  gallery.write(hrule())
  gallery.write(heading2(topic) + hrule()  + endl)
  #Start a table
  gallery.write('<table border=0  class="fixed" cellpadding="20">     <col width="' + thumbSize + 'px" /> <col width="740px" />' + endl)
  ## Loop over docs in each topic
  for docDom in topicMap[topic]:
    #Get the files in each doc
    docId = docDom.getAttribute("id")
    docURL = docDom.getAttribute("href")
    docTitle = docDom.getElementsByTagName("title")[0].lastChild.nodeValue
    files = docDom.getElementsByTagName("file")

    ## Construct list of files from DOM objects
    nbes = [ getFileNBE(fileDom) for fileDom in files]


    # There can also be documents with all the files in one "Document Archive"
    for fileDom in files:
      fileNBE = getFileNBE(fileDom)
      if fileNBE.name == 'Document Archive':
        arcUrl = fileDom.getAttribute("href")
        tempName = tempPath + 'docArchive.tar.gz'
        open(tempName, 'w').write(getPage(arcUrl).read())
        tar = tarfile.open(tempName, 'r')
        for name in tar.getnames():
          base, ext = os.path.splitext(name)
          # Skip any strange files in the archive
          if "._" in base: continue

          # Direct links to the files still work, even though they're inside
          # the tarball.

          url = 'http://nova-docdb.fnal.gov:8080/cgi-bin/RetrieveFile?docid='+docId+'&filename='+name
          nbes.append(FileNBE(name, base, ext, url, tempName))


    ## loop over files
    for fileNBE in nbes:
      if fileNBE.ext == ".txt":
        foundOnceAlready = False # for now, will reset later
        print "Found CAPTION : ", fileNBE.name, fileNBE.base, docId
        #### THUMBNAIL BLOCK: We need to make a thumbnail for each
        shortName = "doc" + docId + "_" + fileNBE.base
        thumbName =   shortName + ".png"
        thumbPath    = thumbsPath + thumbName
        thumbWebPath = thumbsDir + thumbName  # location of thumbnail, relative to the page directory
        wrapperName  = shortName + ".html"
        wrapperPath  = wrappersPath + wrapperName
        wrapperWebPath = wrappersDir + wrapperName

        if not os.path.isfile(thumbPath) and not options.nothumbs:# Check to see if the thumbnail exists , or if we asked to not make thumbnails
          print "MAKING THUMBNAIL "
          for otherFileNBE in nbes:
            # Create the thumbnail based on the first file with the same base and not .txt extension
            if otherFileNBE.base == fileNBE.base and not otherFileNBE.ext == ".txt":
              # Then we make the caption by downloading the file to temp and converting to .png in thumb
              imgUrl = otherFileNBE.url
              tempName = tempPath + otherFileNBE.name # name of file used to produce thumbnail
              if otherFileNBE.tarPath:
                tar = tarfile.open(otherFileNBE.tarPath, 'r')
                tar.extract(otherFileNBE.name, tempPath)
              else:
                open(tempName, 'w').write(getPage(imgUrl).read())
              os.system("convert -trim -resize " + fullWidth + " " + tempName + " " + thumbPath)  # resize to something reasonable
              continue # Because we have the thumbnail now
        else:
          print "THUMBNAIL ALREADY EXISTS"
          foundOnceAlready = (True and not options.nothumbs)
        #Now we have all the tools to write the table

        gallery.write("<tr>") #opens table row
        gallery.write('<td>' + link('<img src="' + thumbWebPath +  '" width="'+ thumbSize +'" style="border-style: none"/ >', wrapperWebPath) + '</td>' ) #creates table entry with image
        gallery.write('<td valign="top">')
        gallery.write("<p>")
        gallery.write(heading3(docTitle)) # writes document title in cell
        gallery.write(heading4(fileNBE.base))
        gallery.write(link("[DocDB]", docURL) + "  " )
        formats = []
        for otherFileNBE in nbes:
          if otherFileNBE.base == fileNBE.base and not otherFileNBE.ext == ".txt":
            gallery.write("[" + link(otherFileNBE.ext , otherFileNBE.url) + "]  " )
            formats.append(otherFileNBE)

        gallery.write("</p>")
        if not options.nocaps:
          if fileNBE.tarPath:
            capFile = tarfile.open(fileNBE.tarPath, 'r')\
                                  .extractfile(fileNBE.name)
          else:
            capFile  = getPage(fileNBE.url)
          caption = cgi.escape(str(capFile.read()).decode(encoding="utf-8", errors='ignore')).replace("\n", "<br> \n").replace("\t", "&emsp;")  # cgi.escape() turns characters into HTML safe ones
        else:
          caption = "No caption option enabled. 	Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
        gallery.write("<p>")
        gallery.write(caption)  # write the caption
        gallery.write("</p>")
        gallery.write(' </td>' ) #closes table entry
        gallery.write("</tr>")

        # Make a saved figure descriptor with all information necessary later
        fig = SavedFigure(topic, thumbWebPath, caption, formats, docURL, docTitle, wrapperWebPath, wrapperPath, fileNBE.base)
        fig.makeWrapper()

        savedFigsByTopic[topic].append(fig) # Add plot to the saved topic map for sorted grid view
        if not foundOnceAlready:
          savedFigs.append(fig) # Add plot to the flat list for grid view


  gallery.write("</table>" + endl)
  gallery.write("</section>")
  gallery.write(link("Return to Table of Categories", "gallery.html#toc"))

gallery.write(paragraph("Page generated at " + datetime.datetime.now().strftime("%Y/%m/%d at %H:%M")))
gallery.write(bodyClose())
gallery.close()

grid = codecs.open(webPath + "grid_new.html", 'w', "utf-8")
grid.write(bootStrap())
grid.write("<title>" + pageTitle +  "</title>"+endl)
grid.write(jumbo(heading1(pageTitle) + heading2("Unsorted Grid View" +endl)))
grid.write(hrule())

grid.write(navBar(topics))

grid.write(hrule())
grid.write(paragraph("To find out how to add a plot to this page, read this " + link("guide", "howto.html") + ".") + endl)

# Set the number of columns and column width
gridNumCols = 4
gridThumbWidth = str(240)

grid.write('<table style="border:1px solid black;border-collapse:collapse;" class="fixed" cellpadding="5">   ' + endl)
for i in range(0, gridNumCols):
  grid.write(' <col width="' + thumbSize + 'px" /> ' + endl)

grid.write("<tr>"+endl) #opens table row
currentCell = 1
for fig in savedFigs:
  grid.write(gridEntry(fig, gridThumbWidth)) #creates table entry with image

  if currentCell == gridNumCols:
    grid.write("</tr>"+endl) # closes current table row
    grid.write("<tr>"+endl) # opens new table row
    currentCell = 0
  currentCell += 1

grid.write("</tr>"+endl) # close last table row
grid.write("</table>"+endl) # close table

grid.write(paragraph("Page generated at " + datetime.datetime.now().strftime("%Y/%m/%d at %H:%M")))
grid.write(bodyClose())
grid.close()

sGrid = codecs.open(webPath + "sortgrid_new.html", 'w', "utf-8")
sGrid.write(bootStrap())
sGrid.write(bodyOpen())
sGrid.write("<title>" + pageTitle +  "</title>"+endl)
sGrid.write(jumbo(heading1(pageTitle) + heading2("Grid View") ) )
sGrid.write(hrule())
sGrid.write(navBar(topics))
sGrid.write(hrule())
sGrid.write(paragraph("To find out how to add a plot to this page, read this " + link("guide", "howto.html") + "."))

sGrid.write(heading2("Categories"))

sGrid.write("<ul>")
#Loop over topics  to make Table of Contents
for topic in topics:
  sGrid.write(entry(link(topic + hendl, "sortgrid.html#" + topic.strip().replace(" ", "").lower())))
sGrid.write("</ul>")

for topic in topics:
  #Make an anchor and a header
  sGrid.write('<a name="' + topic.strip().replace(" ", "").lower() + '"> </a>')
  sGrid.write(heading2(hendl))
  sGrid.write(hrule())
  sGrid.write(heading2(topic) + hrule()  + endl)
  #Start a table
  sGrid.write('<table style="border:1px solid black;border-collapse:collapse;" class="fixed" cellpadding="5">   ' + endl)
  for i in range(0, gridNumCols):
    sGrid.write(' <col width="' + thumbSize + 'px" /> ' + endl)

  sGrid.write("<tr>"+endl) #opens table row
  currentCell = 1
  for fig in savedFigsByTopic[topic]:
    sGrid.write(gridEntry(fig, gridThumbWidth)) #creates table entry with image

    if currentCell == gridNumCols:
      sGrid.write("</tr>"+endl) # closes current table row
      sGrid.write("<tr>"+endl) # opens new table row
      currentCell = 0
    currentCell += 1

  sGrid.write("</tr>"+endl) # close last table row
  sGrid.write("</table>"+endl) # close table



sGrid.write(paragraph("Page generated at " + datetime.datetime.now().strftime("%Y/%m/%d at %H:%M")))

sGrid.write(bodyClose())
sGrid.close()

os.system("mv -f " + webPath + "gallery_new.html " + webPath +  "gallery.html")
os.system("mv -f " + webPath + "grid_new.html " + webPath +  "grid.html")
os.system("mv -f " + webPath + "sortgrid_new.html " + webPath +  "sortgrid.html")


