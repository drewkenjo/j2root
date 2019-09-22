#!/usr/bin/groovy

import org.jlab.jroot.ROOTFile
import org.jlab.jroot.TNtuple

int ntimes = 1e6

def f1 = new ROOTFile('test.root')
def t1 = f1.makeNtuple('h22', 'title', 'x:y')

(0..<ntimes).each{
  t1.fill(it, it+Math.random())
}

t1.write()
f1.close()
