import json
import vfinterface
import uuid
import ctypes
import os
import sys

__authors__ = ["Kevin Zeillmann"]
__author__ = __authors__[0]

__credits__ = __authors__

__version__ = "14.13"

__maintainer__ = __author__
__contact__ = __author__
__email__ = "kzeillmann@isis.vanderbilt.edu"

__copyright__ = "Copyright (C) 2012-2013, Vanderbilt University"
__url__ = 'https://svn.isis.vanderbilt.edu/META/trunk/src/Python27Packages/' + __name__
#__license__ = "MIT"

class LibraryManager:
    """
    Contains a JSON file as its data - performs actions on
     that file, including parsing, 
    """
    VF = vfinterface.VehicleForgeInterface('beta.vehicleforge.org')
    #The VF object acts as the interface to the VehicleForge servers.
    # The arguments are the IP address, the protocol, and the port number 
    
    filename = None
    version = 0
    data = None
    
    def __init__(self, in_filename=""):
        """
        Initializer - the user can specify the location of the JSON
         library, but it defaults to where it is installed  
         by the META tools suite
        """
        if in_filename is not "":
            self.filename = in_filename
        else:
            path = ctypes.c_wchar_p(chr(0x00) * 256)
    
            FOLDERID_Documents = ctypes.c_char_p(uuid.UUID("ED4824AF-DCE4-45A8-81E2-FC7965083634").bytes_le)
            
            ctypes.windll.shell32.SHGetKnownFolderPath(FOLDERID_Documents, 0, None, ctypes.byref(path))
            
            
            filename =  os.path.join(path.value, "META Documents", "MaterialLibrary", "material_library.json")
            
            self.filename = filename
            
        self._parseJSON()
    
    def _parseJSON(self):
        """
        A "private" method  called by the constructor. Not intended
        to be part of the public interface. This parses through
        the JSON file and loads the JSON data into the
        data object. Also finds the version number and loads
        that as well.
        """
        #print self.filename + "\n"
        tempMap = json.load(open(self.filename))
        self.version = tempMap["version"]
        self.data = tempMap["Material library"]

    def materialData(self, in_material):    
        """
        This function returns a dictionary of material properties and 
        their values. The input is the name of a material, and the 
        return value is the dictionary of the property names and
        their values.
        """
        return self.data[in_material]

    def listAllProperties(self):
        """
        This function lists all the properties a material can possess.
        Currently, it searches through all the materials and adds their
        properties to a set, before converting that to a list.
        """
        mySet = set()
        for material in self.data:
            #for every material, iterate through it
            # and add each of its properties to the set
            
            for prop in self.data[material]:
                #since sets are unique, we can add properties
                #to the set, and then convert it to a list
                mySet.add(prop)
        return list(mySet)
    
    def listAllMaterials(self):
        """
        This method returns a list of all the materials in
         the material library. 
        """
        myList = []
        for material in self.data:
            myList.append(material)
        return myList

    def checkVersion(self, in_username, in_password, in_url="http://beta.vehicleforge.org"):
        """
        Takes as input parameters a username and password for VehicleFORGE 
        and returns the version of the material library on the server
        as an integer. 
        """
        self.VF.base_url = in_url
        self.VF._login(in_username, in_password)
        info = self.VF.open("/rest/exchange/materials/current_version")
        versionJson = json.load(info)
        return versionJson["version"]

    def updateJSON(self, in_username, in_password, in_url="http://beta.vehicleforge.org"):
        """
        Takes as input parameters a username and password for VehicleFORGE 
        and updates the material library if a new version is available
        (if the version number on the server is > the local version).
        
        It downloads it and saves the new copy of the library over the 
        original copy.
        """
        currentVersion = self.checkVersion(in_username, in_password, in_url)
        if currentVersion > self.version:
            self.VF.base_url = in_url
            self.version = currentVersion
            #get the json data
            libData = self.VF.open("/rest/exchange/materials/")
            self.data = json.load(libData)
            #now that we've changed the data, we save it
            self._writeJSON(self.filename)
            
    
    def _writeJSON(self, in_writeLocation):
        """
        This function writes the current 
        material library to a JSON. This is invoked
        when updating to a new version of the library.
        """
        jsonFile = open(in_writeLocation, "w")
        
        finaldict = dict()
        finaldict["version"] = self.version
        finaldict["Material library"] = self.data
        
        jsonFile.write(json.dumps(finaldict, indent=4))
        jsonFile.close()    