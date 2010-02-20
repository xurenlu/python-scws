#coding:utf8
import _scws
print "hello"
_scws.scws_new()
_scws.scws_set_charset("UTF8")
_scws.scws_set_xdb("/etc/scws/dict.utf8.xdb")
_scws.scws_set_rule("/etc/scws/rules.utf8.ini")

#print s[0]
print "hid"
print "hi"
d=_scws.get_res("我们是很好很好的好朋友啊,列宁说")
for c in d:
    print c[0]
    print c[1]
    print c[2]
print "exit..."
print d
