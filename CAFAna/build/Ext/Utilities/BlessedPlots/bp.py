import config                   #/// User Configuration
import helpers                  #/// Custom helper functions
import log                      #/// Basic console logging

import json                     #/// write/read JSON
import os, subprocess, shutil   #/// Interact with filesystem/os
import sys                      #/// System commands
import re                       #/// RegEx capabilities
from libs import requests       #/// HTTP CRUD operations
import tarfile                  #/// Extract tarballs
import time                     #/// Used for sleep functions
import uuid                     #/// Unique uid generator
import zipfile                  #/// Extract zip files
import collections


#///////////////////////////////////////////////////////////////////////////////
# Class Definitions
class DocDBEntry():
    def __init__(self):
        self.id       = 0
        self.revision = 1
        self.title    = ""
        self.modified = ""
        self.url      = ""
        self.files    = []
        self.authors  = []
        self.topics   = []


#///////////////////////////////////////////////////////////////////////////////
# Basic Initialization
def Init():
    # Show a splash screen
    print ""
    print "/////////////////////////////////////////////////////////////"
    print "//  BLESSED PLOTS BACKEND                                  //"
    print "/////////////////////////////////////////////////////////////"
    print ""

    time.sleep(1)

    # Check for Python config file
    if not (os.path.isfile(config.WEB_PATH + config.JSON_FILENAME)):
        log.error('File not found: ' + config.WEB_PATH + config.JSON_FILENAME + '. Let\'s start fresh.')
        config.REGENERATE = True
        time.sleep(1)
        
    if not (os.path.isfile(config.BLESSED_PLOTS)):
        log.error('No blessed plots configuration found. I don\'t know which plots go where. Aborting...')
        sys.exit(200)

    return 0


#///////////////////////////////////////////////////////////////////////////////
# Poll DocDB for Document Information
def GetDocumentInfoFromDocDB():
    log.info("Grabbing plots list from DocDB")

    # 370 = "BlessedPlots:Official", 416 = "BlessedPlots:Proposed"
    # You can find these numbers by looking at the URL after clicking on any
    # entry in Topics in docdb
    topicId = '416' if config.PROPOSED else '370'

    _xml             = helpers.CallDocDB('Search', 'topics', topicId)['docdb']['document']
    _plotlistByTopic = helpers.BlessedPlotsList()
    
    _docIds = []
    for topic in _plotlistByTopic:
        for doc in topic['docs']:
            if doc not in _docIds:
                _docIds.append(doc)

    # DocDB does a silly thing if there's only one document returned, which is
    # to give it to you straight, instead of as a one-entry list. Fix that up.
    if '@href' in _xml: _xml = [_xml]
    
    # Check to see if there are any docs in DocDB that are not listed in file, 
    # add them as uncategorized.
    helpers.CombineDocLists(_xml, _docIds)
    
    log.success('Found %i officially blessed plots on DocDB' % len(_docIds))

    time.sleep(1)

    _entries = []
    idx = 0

    for doc in _docIds:
        idx += 1

        # Get document from DocDB and check that it actually exists
        res = helpers.GetDoc(doc)['docdb']
        if 'document' not in res:
            log.error('[%i/%i] ERROR: doc %i not found in DocDB' % (idx, len(_docIds), doc))
            continue
        
        item = res['document']
        
        # Construct the entry
        entry = DocDBEntry()
        entry.id       = int(item['@id'])
        entry.revision = int(item['docrevision']['@version'])
        entry.title    = item['docrevision']['title']
        entry.modified = item['docrevision']['@modified']
        entry.url      = item['docrevision']['@href']

        log.info('[%i/%i] Fetching metadata for doc %i v%i -- %s' % (idx, len(_docIds), entry.id, entry.revision, entry.title))

        # Build author list (data structure is different if
        # there are multiple authors)
        if type(item['docrevision']['author']) is list:
            for author in item['docrevision']['author']:
                entry.authors.append(
                    {
                        'firstname' : author['firstname'],
                        'lastname'  : author['lastname'],
                        'id'        : int(author['@id'])
                    }
                )
        else:
            author = item['docrevision']['author']
            entry.authors.append(
                {
                    'firstname' : author['firstname'],
                    'lastname'  : author['lastname'],
                    'id'        : int(author['@id'])
                }
            )

        # Get list of info for this entry
        docXml = helpers.CallDocDB('ShowDocument', 'docid', item['@id'])

        docID = docXml['docdb']['document']['docrevision']['@docid']
        rev   = docXml['docdb']['document']['docrevision']['@version']

        # Get topics for this doc
        entry.topics = helpers.GetTopicsByDocID(doc)

        _entries.append(entry.__dict__)

    return _entries


#///////////////////////////////////////////////////////////////////////////////
# Get Document Information from JSON File
def GetDocumentInfoFromDisk():
    if config.REGENERATE:
        return []
    else:
        return json.loads(open(config.WEB_PATH + config.JSON_FILENAME).read())


#///////////////////////////////////////////////////////////////////////////////
# Return differences between current list of documents and previous
def FindChanges(new, old):
    # This is where additions, modifications, and deletions will live
    changes = {'additions': [], 'modifications': [], 'deletions': []}
    
    if config.REGENERATE:
        revisions = {doc['id'] : doc['revision'] for doc in new}
        
        # Just add everything from DocDB into additions
        for docID in revisions:
            changes['additions'].append(docID)
        
    else:
        # Build a reduced list of just id/rev numbers as key-value pairs
        newRevs = {entry['id'] : entry['revision'] for entry in new}
        oldRevs = {entry['id'] : entry['revision'] for entry in old}
        
        # Let's check for some differences
        for docID in newRevs:
            # If the new ID is already contained within the old set...
            if docID in oldRevs:
                # if the revision number of the new is different than the old,
                # then we need to repropcess
                if newRevs[docID] != oldRevs[docID]:
                    changes['modifications'].append(docID)
                    
            # If the new ID is not contained within old set, then we have a new
            # document
            else:
                changes['additions'].append(docID)
                
        # We also have to check for deletions, so now we look at situations where
        # the new list is missing something that the old list has
        for docID in oldRevs:
            if docID in newRevs:
                continue # Don't need to do anything here...
                
            # But, if this docID is not included in the new rest, we need tok
            # remove it
            else:
                changes['deletions'].append(docID)

    log.debug('Detected %i additions, %i modifications, and %i deletions' % (len(changes['additions']), len(changes['modifications']), len(changes['deletions'])))

    return set(changes['additions']) | set(changes['modifications'])

#///////////////////////////////////////////////////////////////////////////////
# Fetch all the documents for a list of docdb IDs to local disk
def DownloadFiles(docs):
    tempDir = config.WEB_PATH + config.PLOT_SUBDIR + str(uuid.uuid4()) + '/'
    os.mkdir(tempDir, 0755)

    idx = 0
    for doc in docs:
        idx += 1

        log.info('[%i/%i] Downloading files from docdb %i' % (idx, len(docs), doc))

        tempDocDir = tempDir + str(doc) + '/'
        os.mkdir(tempDocDir, 0755)

        helpers.Download(config.DOCDB_URL + 'RetrieveArchive?docid=%d&type=tar.gz' % doc,
                         tempDocDir+'doc_archive.tar.gz')
        with tarfile.open(tempDocDir+'doc_archive.tar.gz', 'r') as tar:
            oldDir = os.getcwd()
            os.chdir(tempDocDir)
            try:
                tar.extractall()
            finally:
                os.chdir(oldDir)


        for root, directories, filenames in os.walk(tempDocDir):
            for filename in filenames:
                if filename == 'doc_archive.tar.gz': continue

                path = os.path.join(root, filename)

                if filename.endswith('.tar.gz') or filename.endswith('.tar') or filename.endswith('.tgz'):
                    log.debug('Extracting '+filename)
                    with tarfile.open(path, 'r') as tar:
                        oldDir = os.getcwd()
                        os.chdir(tempDocDir)
                        try:
                            tar.extractall()
                        finally:
                            os.chdir(oldDir)

                if filename.endswith('.zip'):
                    log.debug('Extracting '+filename)
                    with zipfile.ZipFile(path, 'r') as z:
                        oldDir = os.getcwd()
                        os.chdir(tempDocDir)
                        try:
                            z.extractall()
                        finally:
                            os.chdir(oldDir)

    return tempDir


#///////////////////////////////////////////////////////////////////////////////
# Look through all the downloaded files for captions and images, update documents_curr
def FindFiles(tempdir, documents_curr):

    files = collections.defaultdict(lambda: {}) # dict from id to dict from base to list of extensions

    # Find all the .txt files
    for root, directories, filenames in os.walk(tempdir):
        for filename in filenames:
            path = os.path.join(root, filename)[len(tempdir):]
            docid = int(path[:path.find('/')])
            if filename.endswith('.txt') and not filename.startswith('.'):
                cap = unicode(file(tempdir+'/'+path, 'r').read(), errors='ignore')
                path = path[len(str(docid))+1:] # drop the docid too for the rest
                base = path[:-4]
                files[docid][base] = {'base': base, 'caption': cap, 'exts': []}

    # Now find matching image files
    for root, directories, filenames in os.walk(tempdir):
        for filename in filenames:
            path = os.path.join(root, filename)[len(tempdir):]
            docid = int(path[:path.find('/')])
            path = path[len(str(docid))+1:] # drop the docid too for the rest
            if not filename.endswith('.txt') and not filename.startswith('.'):
                good = False
                for e in config.EXTS:
                    if filename.endswith(e): good = True
                if not good: continue

                stem = path[:path.rfind('.')]
                ext = path[path.rfind('.')+1:]
                if docid not in files or stem not in files[docid]:
                    log.debug('File with no .txt caption: '+path)
                else:
                    files[docid][stem]['exts'].append(ext)

    # Save this information into the actual documents
    for doc in documents_curr:
        doc['files'] = []
        docid = doc['id']
        for base in files[docid]:
            doc['files'].append(files[docid][base])


#///////////////////////////////////////////////////////////////////////////////
# Make thumnails from downloaded images
def ProcessImages(documents_to_process, documents, tempDir):
    if len(documents_to_process) == 0:
        return 0

    idx = 0
    for docID in documents_to_process:
        idx += 1
        log.info('[%i/%i] Processing images for docdb %i' % (idx, len(documents_to_process), docID))

        document = next((doc for doc in documents if doc['id'] == docID), None)

        tempDocDir = tempDir + str(document['id']) + '/'
        thumDir = tempDocDir + '/thumbs/'
        os.mkdir(thumDir)

        for aFile in document['files']:
            base = aFile['base']
            exts = aFile['exts']

            # Favoured versions to make a thumbnail version from
            srcs = ['png', 'jpg', 'jpeg', 'eps', 'pdf', 'ps']

            for src in srcs:
                if src in exts:
                    # Create thumbnail
                    opt = ''
                    opt2 = None
                    if src == 'pdf':
                        opt = ' -define pdf:use-cropbox=true -transparent-color white '
                        opt2 = ' -transparent-color white ' # sometimes the cropbox is trouble
                    cmd = 'convert ' + opt + tempDocDir + base + '.'+src + ' -resize 400 ' + thumDir + base + '_thumb.png'
                    cmd2 = None
                    if opt2: cmd2 = 'convert ' + opt2 + tempDocDir + base + '.'+src + ' -resize 400 ' + thumDir + base + '_thumb.png'

                    # In case of tarballs etc there can be subdirs required
                    # in the thumbs directory. Maybe we should have made
                    # them up-front?
                    try:
                        os.makedirs(os.path.dirname(thumDir+base))
                    except:
                        pass

                    if cmd2:
                        os.system(cmd + ' || ' + cmd2)
                    else:
                        os.system(cmd)
                    log.success('Created thumbnail from '+base+'.'+src+': '+thumDir + base + '_thumb.png')
                    break

    os.system('cp -rpf ' + tempDir + '* ' + config.WEB_PATH + config.PLOT_SUBDIR)
    shutil.rmtree(tempDir)

    return 0


#///////////////////////////////////////////////////////////////////////////////
# Write JSON file to disk
def WriteJSON(documents):
    os.system('cp -pf ' + config.BLESSED_PLOTS + ' ' + config.WEB_PATH )
    log.success('Copied ' + config.BLESSED_PLOTS + ' to ' + config.WEB_PATH)
    jsonSerialized = json.dumps(documents, sort_keys = True, indent = 2)

    jsonFile = open(config.WEB_PATH + config.JSON_FILENAME, 'w')
    jsonFile.write(jsonSerialized)
    jsonFile.close()

    log.success('Wrote ' + config.WEB_PATH + config.JSON_FILENAME)
