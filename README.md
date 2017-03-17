# xcdf-dataframe
A small project to parse xcdf (http://github.com/jbraun/xcdf) directly into a python dictionary for use with panda's DataFrame. 

It works with some limitations. It provides one function exported to python that parses an XCDF file into a python dict of 
string/ndarray. That being a format that the python pandas DataFrame constructor, you can make DataFrames right from xcdf files.

It has terrible error checking. And it won't do the "child" branches of xcdf because it's not clear how to wedge them into a 
DataFrame. 
