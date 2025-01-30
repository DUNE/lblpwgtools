#! /usr/bin/env python

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
# BLESSED PLOTS WEB BACKEND
# Maintained by: Justin Vasel <jvasel@indiana.edu>
#
# This script maintains the database of blessed plots for presentation on the
# web.
import config     #/// User configuration
import bp         #/// Blessed plots specific functions


# Start by setting some things up. This involves printing a splash screen
# and checking for the existence of a JSON file with potentially outdated
# document information in it
bp.Init()

# Get all the document information from DocDB.
documents_curr = bp.GetDocumentInfoFromDocDB()

# Get all the document information from the pre-existing JSON file. If there is
# no JSON file already, the REGENERATE flag will be set to True and an empty
# array will be returned here.
documents_prev = bp.GetDocumentInfoFromDisk()

# Take in the output from the previous two lines. This checks for discrepencies
# between the 'current' and 'previous' lists. Files that have been added or
# modified will be returned by this function to be processed.
documents_to_process = bp.FindChanges(documents_curr, documents_prev)

# Fetch files for the new documents, remember where we fetched them to
tmpdir = bp.DownloadFiles(documents_to_process)

# Look through the directory to associate image files with their captions
bp.FindFiles(tmpdir, documents_curr)

# Generate thumbnails in the web directory. The first argument is simply an
# array of DocDB IDs, so the second argument is needed to extract necessary
# info about those IDs, like filenames, etc.
bp.ProcessImages(documents_to_process, documents_curr, tmpdir)

# Save current document info to JSON file on disk for use by the website and
# this script later.
bp.WriteJSON(documents_curr)
