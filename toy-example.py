import read_xcdf 
from pandas import DataFrame
import matplotlib.pyplot as plt
import matplotlib.ticker as tk
import numpy as np
import copy
import pylab

df = DataFrame(read_xcdf.read_xcdf("toy.xcd"))

fig = plt.figure(figsize=(8,6))
inset = fig.add_subplot(111)

inset.plot(df["x"],df["y"],".")

inset.set_xlabel("X - fortnights")
inset.set_ylabel("Y - furlongs")

plt.savefig('toy.png',bbox_inches='tight')
