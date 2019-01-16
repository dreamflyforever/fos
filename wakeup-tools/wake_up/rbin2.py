#!/usr/bin/python
# coding: utf-8

from argparse import ArgumentParser
from operator import pos
from struct import pack, unpack
import sys, os


__updated__ = "2015-07-10 15:37:26"

class rbin2Item:
    fn = ""
    pos = 0
    len = 0
    reverse = False
    loc = ""

class rbin2:
    file_list = {}

    def reverse(self, data):
        data = bytearray(data)
        for i in range(len(data)):
            data[i] = 255 - data[i]
        return data

    def write(self, fp, data, reverse=False):
        if reverse == True:
            data = bytearray(data)
            for ch in data:
                fp.write(chr(255 - ch))
        else:
            fp.write(data)

    def create_dir(self, dn):
        if os.path.exists(os.path.dirname(dn)) is False:
            self.create_dir(os.path.dirname(dn))
        os.mkdir(dn)

    def extract(self, fn, dn):
        if dn != "/" and dn[-1] == "/":
            dn = dn[:-1]

        with open(fn, "rb") as fp:
            cnt, = unpack("<I", fp.read(4))
            for _ in range(0, cnt):
                l, = unpack("<I", fp.read(4))
                name = str(self.reverse(fp.read(l)))
                pos, = unpack("<I", fp.read(4))
                data_bytes, = unpack("<I", fp.read(4))

                fObject = self.file_list[name] = rbin2Item()
                fObject.fn = name
                fObject.len = data_bytes
                fObject.pos = pos
                fObject.reverse = name.endswith(".r")
                fObject.loc = dn + "/" + fObject.fn[2:]

                print fObject.fn, fObject.loc, fObject.len, fObject.reverse

                if os.path.exists(os.path.dirname(fObject.loc)) is False:
                    self.create_dir(os.path.dirname(fObject.loc))

                fp3 = open(fn, "rb")
                with open(fObject.loc, "wb") as fp2:
                    if fObject.len != 0:
                        fp3.seek(fObject.pos)
                        self.write(fp2, fp3.read(fObject.len), fObject.reverse)
                fp3.close()

    def zip(self, dn, fn):
        r = None
        for root, dirs, files in os.walk(dn):
            if r is None:
                r = root
            d = root.replace(r, ".")
            for fName in files:
                loc = root + "/" + fName

                file_name = d + "/" + fName
                fObject = self.file_list[file_name] = rbin2Item()
                fObject.fn = file_name
                fObject.pos = 0
                fObject.len = os.path.getsize(loc)
                fObject.loc = loc

                if file_name.endswith(".r"):
                    fObject.reverse = True

                print fObject.fn, fObject.loc, fObject.len, fObject.reverse
            # print root, dirs, files


        with open(fn, "wb") as fp:
            fp.write(pack("<I", len(self.file_list)))
            pos = 4
            for name in self.file_list.keys():
                pos += 4 + len(name) + 4 + 4;
            for name, fObject in self.file_list.items():
                fObject.pos = pos
                pos += fObject.len
                fp.write(pack("<I", len(name)))
                self.write(fp, name, True)
                fp.write(pack("<II", fObject.pos, fObject.len))

            for name, fObject in self.file_list.items():
                if fObject.len > 0:
                    if fObject.reverse == True:
                        self.write(fp, open(fObject.loc, "rb").read(), True)
                    else:
                        self.write(fp, open(fObject.loc, "rb").read())

if __name__ == "__main__":
    p = ArgumentParser()
    p.add_argument("-z", default=False, action="store_true", help=u"打包目录为rbin2文件")
    p.add_argument("-x", default=False, action="store_true", help=u"解压rbin2文件为目录")
    p.add_argument("-d", type=str, help=u"目录名称")
    p.add_argument("-f", type=str, help=u"文件名称")
    args = p.parse_args()

    if args.x == args.z:
        p.print_usage()

    r = rbin2()
    if args.z:
        r.zip(args.d, args.f)
    elif args.x:
        r.extract(args.f, args.d)
