#!/usr/bin/python3

##############################################################################
#
# Copyright (C) 2014 Visualink
#
# Authors: Adrien Maglo <adrien@visualink.io>
#
# This file is part of Pastec.
#
# Pastec is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Pastec is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Pastec.  If not, see <http://www.gnu.org/licenses/>.
#
##############################################################################

import urllib.request
import json


class PastecException(Exception):
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return repr(self.msg)


class PastecConnection:

    def __init__(self, pastecHost = "localhost", pastecPort = 4212):
        self.host = pastecHost
        self.port = pastecPort

    def request(self, path, method, data = None):
        req = urllib.request.Request(url="http://" + self.host + ":" + \
                                     str(self.port) + "/" + path, \
                                     data=data, method=method)
        f = urllib.request.urlopen(req)
        ret = f.read().decode()
        return json.loads(ret)

    def indexImageFile(self, index, imageId, filePath):
        return self.indexImageData(index, imageId, self.loadFileData(filePath))

    def indexImageData(self, index, imageId, imageData):
        ret = self.request("indexes/" + index + "/images/" + str(imageId), "PUT", imageData)
        self.raiseExceptionIfNeeded(ret["type"])
        return {"image_id" : ret["image_id"],
                "nb_features_extracted" : ret["nb_features_extracted"]}

    def removeImage(self, index, imageId):
        ret = self.request("indexes/" + index + "/images/" + str(imageId), "DELETE")
        self.raiseExceptionIfNeeded(ret["type"])

    def addTag(self, index, imageId, tag):
        ret = self.request("indexes/" + index + "/images/%s/tag" % str(imageId), "PUT",
            bytearray(tag, "UTF-8"))
        print(ret)
        self.raiseExceptionIfNeeded(ret["type"])

    def remvoveTag(self, index, imageId):
        ret = self.request("indexes/" + index + "/images/%s/tag" % str(imageId), "DELETE")
        self.raiseExceptionIfNeeded(ret["type"])

    def loadIndex(self, index, path = ""):
        s = json.dumps({"type" : "LOAD", "index_path" : path})
        ret = self.request("indexes/" + index + "/io", "POST", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def writeIndex(self, index, path = ""):
        s = json.dumps({"type" : "WRITE", "index_path" : path})
        ret = self.request("indexes/" + index + "/io", "POST", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def loadIndexTags(self, index, path = ""):
        s = json.dumps({"type" : "LOAD_TAGS", "index_tags_path" : path})
        ret = self.request("indexes/" + index + "/io", "POST", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def writeIndexTags(self, index, path = ""):
        s = json.dumps({"type" : "WRITE_TAGS", "index_tags_path" : path})
        ret = self.request("indexes/" + index + "/io", "POST", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def clearIndex(self, index):
        s = json.dumps({"type" : "CLEAR"})
        ret = self.request("indexes/" + index + "/io", "POST", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def getIndexImageIds(self, index):
        ret = self.request("indexes/" + index + "/imageIds", "GET")
        self.raiseExceptionIfNeeded(ret["type"])
        imageIds = ret["image_ids"]
        return imageIds

    def imageQueryFile(self, index, filePath):
        return self.imageQueryData(index, self.loadFileData(filePath))

    def imageQueryData(self, index, imageData):
        ret = self.request("indexes/" + index + "/searcher", "POST", imageData)
        self.raiseExceptionIfNeeded(ret["type"])
        imageIds = ret["image_ids"]
        tags = ret["tags"]
        res = []
        if len(imageIds) != len(tags):
            raise PastecException("Image ids and tags arrays have different sizes.")
        for i in range(len(imageIds)):
            res += [(imageIds[i], tags[i])]
        return res

    def ping(self):
        s = json.dumps({"type" : "PING"})
        ret = self.request("", "POST", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def addIndex(self, index):
        s = json.dumps({"index_name" : index})
        ret = self.request("indexes", "POST", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def removeIndex(self, index):
        s = json.dumps({"index_name" : index})
        ret = self.request("indexes", "DELETE", bytearray(s, "UTF-8"))
        self.raiseExceptionIfNeeded(ret["type"])

    def raiseExceptionIfNeeded(self, val):
        if val == "ERROR_GENERIC":
            raise PastecException("Generic error.")
        elif val == "MISFORMATTED_REQUEST":
            raise PastecException("Misformatted request.")
        elif val == "TOO_MANY_CLIENTS":
            raise PastecException("Too many clients connected to the server.")
        elif val == "IMAGE_DATA_TOO_BIG":
            raise PastecException("Image data size too big.")
        elif val == "IMAGE_NOT_INDEXED":
            raise PastecException("Image not indexed.")
        elif val == "IMAGE_NOT_DECODED":
            raise PastecException("The query image could not be decoded.")
        elif val == "IMAGE_SIZE_TOO_SMALL":
            raise PastecException("Image size too small.")
        elif val == "IMAGE_NOT_FOUND":
            raise PastecException("Image not found.")
        elif val == "IMAGE_TAG_NOT_FOUND":
            raise PastecException("Image tag not found.")

        elif val == "INDEX_NOT_FOUND":
            raise PastecException("Index not found.")
        elif val == "INDEX_TAGS_NOT_FOUND":
            raise PastecException("Index tags not found.")
        elif val == "INDEX_NOT_WRITTEN":
            raise PastecException("Index not written.")
        elif val == "INDEX_TAGS_NOT_WRITTEN":
            raise PastecException("Index not written.")
        elif val == "INDEX_UNKNOWN":
            raise PastecException("Index unknown.")
        elif val == "INDEX_ALREADY_EXISTING":
            raise PastecException("Index already existing.")
        elif val == "INDEX_NAME_TOO_LONG":
            raise PastecException("Index name too long.")

    def loadFileData(self, filePath):
        fd = open(filePath, "rb")
        imageData = b""
        while 1:
            buf = fd.read(1024)
            # End of file
            if not buf:
                break
            imageData += buf
        return imageData
