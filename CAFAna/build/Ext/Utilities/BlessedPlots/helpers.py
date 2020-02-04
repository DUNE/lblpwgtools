import config        #/// User configuration
import log           #/// Basic console logging
import json          #/// read/write JSON

from libs import requests          #/// HTML CRUD operations
from libs import xmltodict         #/// XML parsing


#///////////////////////////////////////////////////////////////////////////////
# Check blessed plots config
def BlessedPlotsList():
    return json.loads(open(config.BLESSED_PLOTS).read())
    

#///////////////////////////////////////////////////////////////////////////////
# Make HTTP request to DocDB
def CallDocDB(function, parameter, value):
    # Construct full url from parameter and value
    url = config.DOCDB_URL + function + '?' + parameter + '=' + value + '&outformat=xml'

    # Make HTTP request
    response = requests.get(url, auth=('nova', config.PWD))

    return xmltodict.parse(response.content)


#///////////////////////////////////////////////////////////////////////////////
# Get individual document from DocDB
def GetDoc(docID):
    # Construct full url from parameter and value
    url = config.DOCDB_URL + 'ShowDocument?docid=' + str(docID) + '&outformat=xml'

    # Make HTTP request
    response = requests.get(url, auth=('nova', config.PWD))
    return xmltodict.parse(response.content)
    
    
#///////////////////////////////////////////////////////////////////////////////
# Find all the topics to which a given plot belongs
def GetTopicsByDocID(docID):
    topics = []
    for topic in BlessedPlotsList():
        if docID in topic['docs']:
            topics.append(topic['category'])
    
    if len(topics) == 0:
        topics = [unicode('uncategorized')]
    
    return topics
    
    
#///////////////////////////////////////////////////////////////////////////////
# 
def CombineDocLists(_xml, _docIds):
    for item in _xml:
        docId = int(item['@id'])
        if docId not in _docIds:
            _docIds.append(docId)
    
    return 0


#///////////////////////////////////////////////////////////////////////////////
# Download a file
def Download(url, destination):
    r = requests.get(url, auth=('nova', config.PWD))
    with open(destination, "wb") as dest:
        dest.write(r.content)
